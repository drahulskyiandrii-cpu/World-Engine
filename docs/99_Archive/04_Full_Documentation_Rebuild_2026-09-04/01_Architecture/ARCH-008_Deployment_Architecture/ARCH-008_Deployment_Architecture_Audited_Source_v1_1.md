# ARCH-008 — Deployment Architecture — Audited Source v1.1

**Status:** Audit branch source candidate  
**Baseline:** ADR-008 + ADR amendments + ARCH-004/005 audited dependency model  
**Date:** 2026-09-01

## 1. Scope retained from v1.0

World Engine v1.0 remains a Windows-first, single-machine deployment with:

- Skyrim / supported game runtime;
- Game Adapter integration boundary;
- one main World Engine application process;
- SQLite local persistence behind Repository/Database implementation;
- optional Local AI Runtime as a separate process;
- optional External AI Provider through a remote provider boundary;
- no mandatory dedicated server, cloud backend or distributed deployment.

The deployment correction changes ownership/transport semantics, not the product scope.

## 2. Process composition

The World Engine main process **hosts** sibling logical components. Physical co-location does not imply that MOD-001 Core semantically owns them.

```text
Windows User Machine
│
├── Game Runtime (Skyrim / supported game)
│     └── Game Adapter integration
│
├── World Engine Process
│     └── Application Host / Composition Root
│           ├── MOD-001 Core
│           ├── MOD-011 Event Engine
│           ├── MOD-012 Scheduler
│           ├── Simulation/Application modules
│           ├── MOD-005 Dialogue Engine
│           ├── AI Adapter + Context Manager
│           ├── MOD-013 Save Manager
│           ├── Repository/Database implementation
│           └── Logging/Diagnostics/Configuration runtime services
│
├── SQLite database files
│     └── accessible through Repository/Database implementation only
│
└── AI options
      ├── Local AI Runtime — separate optional process
      └── External AI Provider — remote optional service
```

Application Host / Composition Root is a construction/lifecycle mechanism, not a runtime Service Locator exposed to modules.

## 3. Application Host responsibilities

Host owns process-level:

- component construction;
- explicit dependency injection;
- selected configuration/profile binding;
- active Game Profile binding;
- startup ordering;
- application-level readiness aggregation;
- shutdown ordering;
- process-level recovery decisions;
- optional Local AI process lifecycle wiring if World Engine launches it.

Core owns Core coordination semantics and its own lifecycle only.

## 4. Local AI deployment boundary

Canonical deployment rule:

`World Engine -> AI Adapter -> local loopback/IPC transport -> Local AI Runtime`

The exact local protocol and port are **TBD by a dedicated technical decision**.

Permitted future implementations may include loopback HTTP, named pipes or another approved local IPC mechanism, provided that:

- provider details remain behind AI Adapter;
- the local transport is not exposed as a public network service by default;
- timeout/cancellation/error normalization is provider-boundary controlled;
- Local AI failure does not corrupt world state or persistence.

ARCH-008 no longer fixes localhost HTTP as the mandatory v1.0 protocol.

## 5. Persistence deployment boundary

Physical deployment may contain SQLite in the same installation/data root, but application modules do not gain direct SQLite access.

Canonical path:

`Dedicated persistence/repository port -> Repository implementation -> SQLite`

Save Manager uses save-oriented persistence contracts. Domain modules use dedicated repository contracts for owned data where required. AI Runtime, Game Adapter and Plugin Detection do not receive direct SQLite access.

## 6. Startup lifecycle

Startup is an **application lifecycle** coordinated by Host.

```text
1. Resolve installation/runtime roots.
2. Load and validate configuration through Configuration subsystem.
3. Construct components and inject explicit typed contracts/configuration views.
4. Validate required persistence/database capability.
5. Start Event Engine and Scheduler through public lifecycle contracts.
6. Start/validate Game Adapter integration and active Game Profile.
7. Initialize AI Adapter/provider mode.
8. If Local AI is enabled, validate the configured local runtime boundary.
9. Collect scoped readiness/health evidence.
10. Host determines application-level Ready / Degraded / Startup Failed / Recovery Required outcome.
```

No step implies that Core instantiates Repository, Game Adapter, Save Manager or AI Adapter.

## 7. Shutdown lifecycle

```text
1. Application shutdown request is accepted by Host.
2. Core stops accepting new Core-owned non-critical workflows.
3. Scheduler stops initiating new scheduled work.
4. Event Engine drains/cancels pending delivery according to policy.
5. Save Manager may execute a controlled shutdown checkpoint if policy requires it.
6. Persistence finishes/aborts controlled operations.
7. AI Adapter stops accepting new requests and handles in-flight work according to policy.
8. Optional Local AI Runtime may be stopped independently if Host owns its launched process.
9. Host confirms subsystem shutdown and releases process-level resources.
```

Each subsystem owns its own shutdown semantics; Host owns ordering.

## 8. Readiness and degraded operation

Deployment architecture uses application-scoped outcomes:

- **Application Ready** — all required baseline capabilities are ready according to policy;
- **Application Degraded** — baseline operation remains valid while one or more explicitly optional capabilities are unavailable;
- **Application Startup Failed** — required configuration/capability is invalid or unavailable and normal runtime must not activate;
- **Application Recovery Required** — startup detects a state that requires controlled recovery before normal activation.

Subsystems report scoped states such as `CoreReady`, `AICapabilityUnavailable`, `GameIntegrationUnavailable`, or equivalent evidence. These are not interchangeable with application outcome.

## 9. Failure isolation retained

- AI provider/runtime failure: optional AI becomes unavailable/degraded; no world-state corruption.
- SQLite/persistence unavailable: controlled critical startup/runtime failure because v1.0 persistence is required.
- Event Engine failure: critical coordination/runtime failure requiring controlled recovery/shutdown.
- Scheduler failure: scheduled capability unavailable; continuation depends on application policy.
- Game Adapter unavailable: application may support a controlled non-game/degraded mode only if explicitly allowed by product policy.
- invalid configuration: controlled startup failure or optional-capability degradation according to configuration criticality.

## 10. Save/checkpoint wording

A known-good Save is the **authoritative persisted/restorable checkpoint**.

It is not necessarily the newest live runtime state after additional post-save mutations.

## 11. Security rules retained

- Game-specific interaction only through Game Adapter boundaries.
- Provider/model access only through AI Adapter/provider abstraction.
- SQLite only through persistence boundaries.
- secret values are not logged.
- plugin/mod content grants no execution privilege.
- Local AI transport defaults to local-only exposure.
- optional-process compromise/failure must be contained where practical.

## 12. Acceptance corrections

Deployment acceptance must verify:

1. Host-coordinated application startup and shutdown.
2. Scoped readiness evidence and correct application-level outcome.
3. Repository/SQLite boundary.
4. Game integration boundary.
5. optional AI disabled/unavailable path.
6. transport-neutral Local AI configuration.
7. controlled Save/Load smoke test.
8. diagnostic/log path without secrets.
9. recovery from incomplete update/persistence problems.
10. no accidental Core ownership of sibling modules.

## 13. Deferred implementation decisions

Still TBD:

- installer format;
- exact installation/data directory layout;
- exact Local AI IPC/loopback protocol and port;
- whether World Engine launches Local AI automatically;
- Windows service vs ordinary user process;
- concrete update mechanism;
- detailed hardware/resource matrix;
- telemetry backend/format.

## 14. Superseded v1.0 interpretations

The following v1.0 readings are explicitly superseded by this audited source:

- World Engine process `owns` sibling module semantics;
- Event Engine/Scheduler/Simulation/Save/AI are children of MOD-001 Core;
- localhost HTTP is already a mandatory local-AI transport decision;
- Core performs application-wide startup/shutdown wiring;
- unqualified `Ready/Degraded` means Core state;
- a Save is always the continuously current live world state.
