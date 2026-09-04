#pragma once

#include "world_engine/game_adapter/ports.hpp"

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

namespace world_engine::game_adapter {

class GameAdapter final : public IGameOutputCapabilityPort {
public:
    GameAdapter(
        GameAdapterConfig config,
        IGameIngressPort& ingress,
        IGameAdapterStatusSink& status_sink,
        IRuntimeBridge& runtime_bridge);

    GameAdapter(const GameAdapter&) = delete;
    GameAdapter& operator=(const GameAdapter&) = delete;

    OperationResult start();
    OperationResult recover();
    OperationResult stop();
    OperationResult accept_observation(const RuntimeObservation& observation);

    [[nodiscard]] AdapterStatusEvidence status() const;
    [[nodiscard]] std::vector<CapabilityEvidence> capabilities() const override;
    OperationResult execute(const GameAction& action) override;

private:
    [[nodiscard]] OperationResult validate_observation_locked(const RuntimeObservation& observation) const;
    [[nodiscard]] OperationResult validate_action_locked(const GameAction& action) const;
    [[nodiscard]] CapabilityState capability_state_locked(CapabilityId id) const;
    [[nodiscard]] AdapterState operational_state_locked() const;
    [[nodiscard]] AdapterStatusEvidence status_locked(std::string reason_code = {}) const;
    void publish(AdapterStatusEvidence evidence);

    GameAdapterConfig config_;
    IGameIngressPort& ingress_;
    IGameAdapterStatusSink& status_sink_;
    IRuntimeBridge& runtime_bridge_;

    mutable std::mutex mutex_;
    AdapterState state_{AdapterState::created};
    std::uint64_t generation_{0};
    std::uint64_t last_observation_sequence_{0};
    GameProfile profile_;
};

}  // namespace world_engine::game_adapter
