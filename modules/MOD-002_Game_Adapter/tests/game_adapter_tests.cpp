#include "world_engine/game_adapter/game_adapter.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace world_engine::game_adapter;

namespace {

class FakeIngress final : public IGameIngressPort {
public:
    OperationResult ingest(const IngressEnvelope& envelope) override {
        received.push_back(envelope);
        return {OperationStatus::accepted, "ingress.accepted", {}};
    }

    std::vector<IngressEnvelope> received;
};

class FakeStatusSink final : public IGameAdapterStatusSink {
public:
    void publish(const AdapterStatusEvidence& evidence) override {
        published.push_back(evidence);
    }

    std::vector<AdapterStatusEvidence> published;
};

class FakeRuntimeBridge final : public IRuntimeBridge {
public:
    OperationResult connect() override {
        ++connect_count;
        return connect_result;
    }

    void disconnect() noexcept override {
        ++disconnect_count;
    }

    GameProfile discover_profile() override {
        return profile;
    }

    std::vector<CapabilityEvidence> discover_capabilities() override {
        return capability_evidence;
    }

    OperationResult execute(const GameAction& action) override {
        executed.push_back(action);
        return execute_result;
    }

    int connect_count{};
    int disconnect_count{};
    OperationResult connect_result{OperationResult::succeeded("bridge.connected")};
    OperationResult execute_result{OperationResult::succeeded("bridge.executed")};
    GameProfile profile{
        "skyrim-se-ae", "1.7.104", "steam", "world-engine.skyrim", "0.1.0", "1", {}};
    std::vector<CapabilityEvidence> capability_evidence;
    std::vector<GameAction> executed;
};

void require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

GameAdapterConfig baseline_config() {
    return {
        "skyrim-se-ae",
        1024,
        {CapabilityId::runtime_identity, CapabilityId::session_lifecycle},
    };
}

std::vector<CapabilityEvidence> ready_capabilities() {
    return {
        {CapabilityId::runtime_identity, CapabilityState::available, "runtime.detected"},
        {CapabilityId::session_lifecycle, CapabilityState::available, "session.events.registered"},
        {CapabilityId::subtitle_output, CapabilityState::available, "ui.available"},
    };
}

void start_reaches_ready() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = ready_capabilities();
    GameAdapter adapter(baseline_config(), ingress, status, bridge);

    const auto result = adapter.start();

    require(result.ok(), "start should succeed");
    require(adapter.status().state == AdapterState::ready, "adapter should be ready");
    require(bridge.connect_count == 1, "runtime bridge should connect once");
    require(status.published.size() == 2, "initializing and ready evidence should be published");
}

void optional_capability_limitation_reaches_degraded() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = ready_capabilities();
    bridge.capability_evidence.push_back(
        {CapabilityId::voice_playback_trigger, CapabilityState::unsupported, "voice.not_in_mvp"});
    GameAdapter adapter(baseline_config(), ingress, status, bridge);

    const auto result = adapter.start();

    require(result.ok(), "degraded start should still be operational");
    require(adapter.status().state == AdapterState::degraded, "optional limitation should degrade adapter");
}

void missing_required_capability_is_unavailable() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = {
        {CapabilityId::runtime_identity, CapabilityState::available, "runtime.detected"},
        {CapabilityId::session_lifecycle, CapabilityState::unsupported, "events.missing"},
    };
    GameAdapter adapter(baseline_config(), ingress, status, bridge);

    const auto result = adapter.start();

    require(!result.ok(), "start should fail when a required capability is missing");
    require(adapter.status().state == AdapterState::unavailable, "adapter should be unavailable");
}

void ingress_is_normalized_and_deduplicated() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = ready_capabilities();
    GameAdapter adapter(baseline_config(), ingress, status, bridge);
    require(adapter.start().ok(), "adapter must start for ingress test");

    const RuntimeObservation observation{
        kContractSchemaVersion,
        "evt-1",
        1,
        1'788'480'000'000,
        "skyrim-se-ae",
        "game.session.loaded",
        R"({"save":"slot-1"})",
    };

    require(adapter.accept_observation(observation).ok(), "first observation should be accepted");
    require(ingress.received.size() == 1, "normalized envelope should reach ingress");
    require(
        adapter.accept_observation(observation).code == "adapter.ingress.duplicate_or_out_of_order",
        "duplicate observation should be rejected");
    require(ingress.received.size() == 1, "duplicate must not reach ingress");
}

void outbound_actions_require_capability_and_token() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = ready_capabilities();
    bridge.capability_evidence.push_back(
        {CapabilityId::voice_playback_trigger, CapabilityState::unsupported, "voice.not_in_mvp"});
    GameAdapter adapter(baseline_config(), ingress, status, bridge);
    require(adapter.start().ok(), "adapter must start for action test");

    const GameAction supported{
        kContractSchemaVersion,
        "action-1",
        "corr-1",
        CapabilityId::subtitle_output,
        "cap-token",
        "game.ui.subtitle.show",
        R"({"text":"Greetings, traveler."})",
    };
    require(adapter.execute(supported).ok(), "supported action should execute");
    require(bridge.executed.size() == 1, "supported action should reach runtime bridge");

    auto unsupported = supported;
    unsupported.action_id = "action-2";
    unsupported.capability = CapabilityId::voice_playback_trigger;
    require(
        adapter.execute(unsupported).status == OperationStatus::unsupported,
        "unsupported capability should return a typed outcome");
    require(bridge.executed.size() == 1, "unsupported action must not reach runtime bridge");

    auto unauthorized = supported;
    unauthorized.action_id = "action-3";
    unauthorized.capability_token.clear();
    require(
        adapter.execute(unauthorized).status == OperationStatus::invalid_input,
        "missing capability token should be rejected");
}

void recovery_revalidates_profile() {
    FakeIngress ingress;
    FakeStatusSink status;
    FakeRuntimeBridge bridge;
    bridge.capability_evidence = {
        {CapabilityId::runtime_identity, CapabilityState::available, "runtime.detected"},
        {CapabilityId::session_lifecycle, CapabilityState::unsupported, "events.missing"},
    };
    GameAdapter adapter(baseline_config(), ingress, status, bridge);
    require(!adapter.start().ok(), "initial start should be unavailable");

    bridge.capability_evidence = ready_capabilities();
    require(adapter.recover().ok(), "recovery should reconnect and revalidate");
    require(adapter.status().state == AdapterState::ready, "adapter should recover to ready");
    require(bridge.disconnect_count == 1, "recovery should quiesce the old bridge");
    require(bridge.connect_count == 2, "recovery should reconnect");
}

}  // namespace

int main() {
    start_reaches_ready();
    optional_capability_limitation_reaches_degraded();
    missing_required_capability_is_unavailable();
    ingress_is_normalized_and_deduplicated();
    outbound_actions_require_capability_and_token();
    recovery_revalidates_profile();
    std::cout << "MOD-002 Game Adapter tests passed\n";
    return EXIT_SUCCESS;
}
