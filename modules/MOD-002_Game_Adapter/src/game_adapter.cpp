#include "world_engine/game_adapter/game_adapter.hpp"

#include <algorithm>
#include <exception>
#include <utility>

namespace world_engine::game_adapter {
namespace {

bool is_operational(AdapterState state) {
    return state == AdapterState::ready || state == AdapterState::degraded;
}

bool is_available(CapabilityState state) {
    return state == CapabilityState::available || state == CapabilityState::degraded;
}

}  // namespace

GameAdapter::GameAdapter(
    GameAdapterConfig config,
    IGameIngressPort& ingress,
    IGameAdapterStatusSink& status_sink,
    IRuntimeBridge& runtime_bridge)
    : config_(std::move(config)),
      ingress_(ingress),
      status_sink_(status_sink),
      runtime_bridge_(runtime_bridge) {}

OperationResult GameAdapter::start() {
    AdapterStatusEvidence initializing;
    {
        std::scoped_lock lock(mutex_);
        if (state_ != AdapterState::created && state_ != AdapterState::stopped) {
            return OperationResult::rejected(OperationStatus::rejected, "adapter.start.invalid_state");
        }
        ++generation_;
        last_observation_sequence_ = 0;
        state_ = AdapterState::initializing;
        initializing = status_locked("adapter.starting");
    }
    publish(std::move(initializing));

    try {
        auto connected = runtime_bridge_.connect();
        if (!connected.ok()) {
            AdapterStatusEvidence unavailable;
            {
                std::scoped_lock lock(mutex_);
                state_ = AdapterState::unavailable;
                unavailable = status_locked(connected.code);
            }
            publish(std::move(unavailable));
            return connected;
        }

        auto candidate_profile = runtime_bridge_.discover_profile();
        candidate_profile.capabilities = runtime_bridge_.discover_capabilities();

        AdapterStatusEvidence operational;
        {
            std::scoped_lock lock(mutex_);
            if (candidate_profile.game_id != config_.expected_game_id) {
                state_ = AdapterState::unavailable;
                profile_ = std::move(candidate_profile);
                operational = status_locked("adapter.profile.game_id_mismatch");
            } else {
                profile_ = std::move(candidate_profile);
                state_ = operational_state_locked();
                operational = status_locked(
                    state_ == AdapterState::ready ? "adapter.ready" :
                    state_ == AdapterState::degraded ? "adapter.degraded" :
                                                        "adapter.required_capability_unavailable");
            }
        }
        publish(std::move(operational));

        const auto final_state = status().state;
        if (is_operational(final_state)) {
            return OperationResult::succeeded(final_state == AdapterState::ready ? "adapter.ready" : "adapter.degraded");
        }
        return OperationResult::rejected(OperationStatus::unavailable, "adapter.required_capability_unavailable");
    } catch (const std::exception& error) {
        AdapterStatusEvidence failed;
        {
            std::scoped_lock lock(mutex_);
            state_ = AdapterState::failed;
            failed = status_locked("adapter.start.exception");
        }
        publish(std::move(failed));
        return OperationResult::rejected(OperationStatus::failed, "adapter.start.exception", error.what());
    }
}

OperationResult GameAdapter::recover() {
    AdapterStatusEvidence recovering;
    {
        std::scoped_lock lock(mutex_);
        if (state_ != AdapterState::degraded && state_ != AdapterState::unavailable && state_ != AdapterState::failed) {
            return OperationResult::rejected(OperationStatus::rejected, "adapter.recover.invalid_state");
        }
        state_ = AdapterState::recovering;
        ++generation_;
        last_observation_sequence_ = 0;
        recovering = status_locked("adapter.recovering");
    }
    publish(std::move(recovering));

    runtime_bridge_.disconnect();

    AdapterStatusEvidence reset;
    {
        std::scoped_lock lock(mutex_);
        state_ = AdapterState::created;
        reset = status_locked("adapter.recovery_reinitializing");
    }
    publish(std::move(reset));
    return start();
}

OperationResult GameAdapter::stop() {
    AdapterStatusEvidence stopping;
    {
        std::scoped_lock lock(mutex_);
        if (state_ == AdapterState::stopped) {
            return OperationResult::succeeded("adapter.already_stopped");
        }
        if (state_ == AdapterState::stopping) {
            return OperationResult::rejected(OperationStatus::busy, "adapter.stop.in_progress");
        }
        state_ = AdapterState::stopping;
        stopping = status_locked("adapter.stopping");
    }
    publish(std::move(stopping));

    runtime_bridge_.disconnect();

    AdapterStatusEvidence stopped;
    {
        std::scoped_lock lock(mutex_);
        state_ = AdapterState::stopped;
        stopped = status_locked("adapter.stopped");
    }
    publish(std::move(stopped));
    return OperationResult::succeeded("adapter.stopped");
}

OperationResult GameAdapter::accept_observation(const RuntimeObservation& observation) {
    IngressEnvelope envelope;
    {
        std::scoped_lock lock(mutex_);
        auto validation = validate_observation_locked(observation);
        if (!validation.ok()) {
            return validation;
        }

        // Reserve the sequence before leaving the lock. Failed downstream work
        // is not replayed implicitly, preventing duplicate side effects.
        last_observation_sequence_ = observation.sequence;
        envelope = {
            observation.schema_version,
            observation.event_id,
            observation.sequence,
            observation.observed_at_unix_ms,
            observation.game_id,
            observation.event_type,
            observation.payload_json,
        };
    }
    return ingress_.ingest(envelope);
}

AdapterStatusEvidence GameAdapter::status() const {
    std::scoped_lock lock(mutex_);
    return status_locked();
}

std::vector<CapabilityEvidence> GameAdapter::capabilities() const {
    std::scoped_lock lock(mutex_);
    return profile_.capabilities;
}

OperationResult GameAdapter::execute(const GameAction& action) {
    {
        std::scoped_lock lock(mutex_);
        auto validation = validate_action_locked(action);
        if (!validation.ok()) {
            return validation;
        }
    }

    try {
        return runtime_bridge_.execute(action);
    } catch (const std::exception& error) {
        return OperationResult::rejected(OperationStatus::failed, "adapter.action.exception", error.what());
    }
}

OperationResult GameAdapter::validate_observation_locked(const RuntimeObservation& observation) const {
    if (!is_operational(state_)) {
        return OperationResult::rejected(OperationStatus::unavailable, "adapter.ingress.not_operational");
    }
    if (observation.schema_version != kContractSchemaVersion) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.ingress.schema_unsupported");
    }
    if (observation.event_id.empty() || observation.event_type.empty()) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.ingress.identity_missing");
    }
    if (observation.game_id != profile_.game_id) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.ingress.game_id_mismatch");
    }
    if (observation.payload_json.size() > config_.max_payload_bytes) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.ingress.payload_too_large");
    }
    if (observation.sequence == 0 || observation.sequence <= last_observation_sequence_) {
        return OperationResult::rejected(OperationStatus::rejected, "adapter.ingress.duplicate_or_out_of_order");
    }
    return {OperationStatus::accepted, "adapter.ingress.valid", {}};
}

OperationResult GameAdapter::validate_action_locked(const GameAction& action) const {
    if (!is_operational(state_)) {
        return OperationResult::rejected(OperationStatus::unavailable, "adapter.action.not_operational");
    }
    if (action.schema_version != kContractSchemaVersion) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.action.schema_unsupported");
    }
    if (action.action_id.empty() || action.action_type.empty() || action.capability_token.empty()) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.action.authorization_context_missing");
    }
    if (action.payload_json.size() > config_.max_payload_bytes) {
        return OperationResult::rejected(OperationStatus::invalid_input, "adapter.action.payload_too_large");
    }

    const auto capability = capability_state_locked(action.capability);
    if (capability == CapabilityState::unsupported) {
        return OperationResult::rejected(OperationStatus::unsupported, "adapter.action.capability_unsupported");
    }
    if (capability == CapabilityState::unknown || capability == CapabilityState::unavailable) {
        return OperationResult::rejected(OperationStatus::unavailable, "adapter.action.capability_unavailable");
    }
    return OperationResult::succeeded("adapter.action.valid");
}

CapabilityState GameAdapter::capability_state_locked(CapabilityId id) const {
    const auto match = std::find_if(
        profile_.capabilities.begin(),
        profile_.capabilities.end(),
        [id](const CapabilityEvidence& evidence) { return evidence.id == id; });
    return match == profile_.capabilities.end() ? CapabilityState::unknown : match->state;
}

AdapterState GameAdapter::operational_state_locked() const {
    for (const auto required : config_.required_capabilities) {
        if (!is_available(capability_state_locked(required))) {
            return AdapterState::unavailable;
        }
    }

    const auto optional_limitation = std::any_of(
        profile_.capabilities.begin(),
        profile_.capabilities.end(),
        [](const CapabilityEvidence& evidence) {
            return evidence.state != CapabilityState::available;
        });
    return optional_limitation ? AdapterState::degraded : AdapterState::ready;
}

AdapterStatusEvidence GameAdapter::status_locked(std::string reason_code) const {
    return {
        state_,
        std::to_string(generation_),
        std::move(reason_code),
        profile_,
    };
}

void GameAdapter::publish(AdapterStatusEvidence evidence) {
    status_sink_.publish(evidence);
}

}  // namespace world_engine::game_adapter
