#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace world_engine::game_adapter {

inline constexpr std::uint32_t kContractSchemaVersion = 1;

enum class AdapterState {
    created,
    initializing,
    ready,
    degraded,
    unavailable,
    recovering,
    stopping,
    stopped,
    failed,
};

enum class CapabilityId {
    runtime_identity,
    session_lifecycle,
    world_time_observation,
    player_location_observation,
    actor_basic_observation,
    dialogue_context_observation,
    subtitle_output,
    voice_playback_trigger,
    safe_point_evidence,
};

enum class CapabilityState {
    unknown,
    available,
    degraded,
    unavailable,
    unsupported,
};

enum class OperationStatus {
    succeeded,
    accepted,
    rejected,
    invalid_input,
    unsupported,
    unavailable,
    timed_out,
    busy,
    failed,
};

struct CapabilityEvidence {
    CapabilityId id{};
    CapabilityState state{CapabilityState::unknown};
    std::string reason_code;
};

struct GameProfile {
    std::string game_id;
    std::string runtime_version;
    std::string distribution;
    std::string adapter_id;
    std::string adapter_version;
    std::string profile_version;
    std::vector<CapabilityEvidence> capabilities;
};

struct GameAdapterConfig {
    std::string expected_game_id{"skyrim-se-ae"};
    std::size_t max_payload_bytes{1024U * 1024U};
    std::vector<CapabilityId> required_capabilities;
};

struct RuntimeObservation {
    std::uint32_t schema_version{kContractSchemaVersion};
    std::string event_id;
    std::uint64_t sequence{};
    std::int64_t observed_at_unix_ms{};
    std::string game_id;
    std::string event_type;
    std::string payload_json;
};

struct IngressEnvelope {
    std::uint32_t schema_version{kContractSchemaVersion};
    std::string event_id;
    std::uint64_t sequence{};
    std::int64_t observed_at_unix_ms{};
    std::string source_game_id;
    std::string event_type;
    std::string normalized_payload_json;
};

struct GameAction {
    std::uint32_t schema_version{kContractSchemaVersion};
    std::string action_id;
    std::string correlation_id;
    CapabilityId capability{};
    std::string capability_token;
    std::string action_type;
    std::string payload_json;
};

struct OperationResult {
    OperationStatus status{OperationStatus::failed};
    std::string code;
    std::string detail;

    [[nodiscard]] bool ok() const noexcept {
        return status == OperationStatus::succeeded || status == OperationStatus::accepted;
    }

    static OperationResult succeeded(std::string code = "ok") {
        return {OperationStatus::succeeded, std::move(code), {}};
    }

    static OperationResult rejected(OperationStatus status, std::string code, std::string detail = {}) {
        return {status, std::move(code), std::move(detail)};
    }
};

struct AdapterStatusEvidence {
    AdapterState state{AdapterState::created};
    std::string generation;
    std::string reason_code;
    GameProfile profile;
};

[[nodiscard]] constexpr std::string_view to_string(AdapterState value) noexcept {
    switch (value) {
        case AdapterState::created: return "created";
        case AdapterState::initializing: return "initializing";
        case AdapterState::ready: return "ready";
        case AdapterState::degraded: return "degraded";
        case AdapterState::unavailable: return "unavailable";
        case AdapterState::recovering: return "recovering";
        case AdapterState::stopping: return "stopping";
        case AdapterState::stopped: return "stopped";
        case AdapterState::failed: return "failed";
    }
    return "unknown";
}

}  // namespace world_engine::game_adapter
