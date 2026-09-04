#pragma once

#include "world_engine/game_adapter/types.hpp"

#include <vector>

namespace world_engine::game_adapter {

// Application/Core-owned inbound contract. MOD-002 consumes this abstraction
// and never reaches Core implementation details.
class IGameIngressPort {
public:
    virtual ~IGameIngressPort() = default;
    virtual OperationResult ingest(const IngressEnvelope& envelope) = 0;
};

// Host-owned evidence sink. The Application Host aggregates application
// readiness and does not delegate that policy to MOD-002.
class IGameAdapterStatusSink {
public:
    virtual ~IGameAdapterStatusSink() = default;
    virtual void publish(const AdapterStatusEvidence& evidence) = 0;
};

// MOD-002 internal gateway. Concrete implementations contain every Skyrim SDK,
// SKSE and transport-specific type.
class IRuntimeBridge {
public:
    virtual ~IRuntimeBridge() = default;
    virtual OperationResult connect() = 0;
    virtual void disconnect() noexcept = 0;
    virtual GameProfile discover_profile() = 0;
    virtual std::vector<CapabilityEvidence> discover_capabilities() = 0;
    virtual OperationResult execute(const GameAction& action) = 0;
};

// Application integration contract implemented by MOD-002. Callers depend on
// this abstraction, never on the concrete Skyrim adapter.
class IGameOutputCapabilityPort {
public:
    virtual ~IGameOutputCapabilityPort() = default;
    virtual std::vector<CapabilityEvidence> capabilities() const = 0;
    virtual OperationResult execute(const GameAction& action) = 0;
};

}  // namespace world_engine::game_adapter
