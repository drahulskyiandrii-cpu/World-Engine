# MOD-001 — Core — Architecture Audited Source v1.2

**Project:** World Engine  
**Module ID:** MOD-001  
**Layer:** Coordination / Application  
**Status:** Architecture-audited source candidate for RU/DE DOCX regeneration  
**Date:** 2026-09-02  
**Baseline:** ADR-001 amendment; ADR-002 clarification; ADR-003 amendment; ADR-005 v1.2 amendment; ADR-007 amendment; ADR-008; ARCH-001–005 v1.3 audited model; ARCH-006 v1.2; ARCH-007 v4; ARCH-008/009/010/011/012 audited sources

---

## 1. Module Overview

MOD-001 Core is the central **coordination/application orchestration component** of World Engine.

Core coordinates explicit application use cases and publishes/consumes coordination contracts without owning domain semantics, persistence mechanics, game-integration implementation details, AI provider behavior or top-level process composition.

The module is intentionally game-agnostic and provider-agnostic.

### 1.1 Primary mission

Core exists to:

- receive normalized application/game ingress requests through explicit ingress contracts;
- coordinate synchronous use cases through narrow command/query/use-case ports;
- publish semantic events through Event Engine when a state fact/reaction relationship is appropriate;
- use Scheduler through a one-way control contract for Core-owned scheduling responsibilities;
- return approved game-facing results through Game Output/Capability Port;
- expose Core-owned lifecycle/readiness/diagnostic evidence;
- enforce Core coordination invariants without becoming a Service Locator or God object.

### 1.2 Architectural position

```text
Game Runtime
   |
Game Adapter implementation
   |
Game Ingress Port
   v
MOD-001 Core / Application Coordination
   |-- Event Engine contract
   |-- Scheduler control contract
   |-- Dialogue Command / Use-Case Port
   |-- Save Manager Command / Use-Case Contract
   |-- Game Output / Capability Port
   `-- other explicitly approved narrow use-case ports
```

Application Host / Composition Root is outside MOD-001 and owns process-level construction, dependency injection, startup/shutdown ordering and application readiness aggregation.

---

## 2. Responsibilities

Core owns the following responsibilities:

1. Core-owned lifecycle state and lifecycle invariants.
2. Normalized ingress acceptance and request validation at the Core boundary.
3. Explicit synchronous use-case orchestration where an immediate typed result is required.
4. Semantic-event publication for decoupled reactions/facts.
5. Core-owned Scheduler control/registration where required.
6. Coordination operation context, correlation and cancellation propagation.
7. Mapping use-case results to approved application/game output contracts.
8. Core readiness/health evidence generation.
9. Coordination failure normalization and propagation.
10. Architecture-boundary enforcement for Core consumers/contracts.

Core does **not** own:

- domain rules or domain state of Memory, Character, Dialogue, Relationship, Organization, Economy, Politics or Quest;
- Event Engine delivery/queue semantics;
- Scheduler timing engine internals;
- Save serialization, checkpoint, restore, retry or storage semantics;
- ordinary domain persistence or generic CRUD;
- Repository/SQLite implementation;
- Game Adapter implementation or game SDK/API types;
- AI provider/model lifecycle or AI request implementation;
- Plugin Detection discovery/validation internals;
- configuration parsing/schema/merge/secret resolution;
- process-level component construction or application readiness aggregation.

---

## 3. Functional Requirements

### FR-CORE-001 — Core lifecycle state
Core shall maintain a deterministic Core-owned lifecycle state machine and reject invalid transitions.

### FR-CORE-002 — Ingress acceptance
Core shall expose a normalized application/game ingress contract and reject malformed or unsupported ingress before dispatching owned workflows.

### FR-CORE-003 — Synchronous use-case orchestration
Core shall be able to invoke explicitly injected narrow synchronous use-case ports when the workflow requires an immediate result.

### FR-CORE-004 — Dialogue orchestration
Core shall support the conversation workflow through a Dialogue Command/Use-Case Port without importing concrete Dialogue Engine implementation internals.

### FR-CORE-005 — Save/Load orchestration
Core shall support explicit manual/application Save/Load workflows through Save Manager Command/Use-Case Contract without accessing Save Repository/Database directly.

### FR-CORE-006 — Game output
Core shall return approved game-facing results/actions/capability requests through Game Output/Capability Port without depending on concrete Game Adapter implementation.

### FR-CORE-007 — Event publication
Core shall publish semantic facts/events through Event Engine when decoupled reaction semantics are appropriate.

### FR-CORE-008 — Event Engine is not RPC
Core shall not use Event Engine as a generic request/response transport for synchronous workflow results.

### FR-CORE-009 — Scheduler control
Core may consume Scheduler control contract for Core-owned scheduling responsibilities. Scheduler shall not depend statically on Core.

### FR-CORE-010 — Autosave ownership exclusion
Core shall not create a Scheduler -> Core -> Save Manager autosave chain. Scheduled autosave remains Scheduler -> Event Engine : AutosaveDue -> Save Manager subscriber.

### FR-CORE-011 — Coordination context
Core shall propagate correlation ID, deadline/cancellation context and safe diagnostic context across synchronous use-case boundaries.

### FR-CORE-012 — Failure normalization
Core shall normalize downstream contract failures into stable coordination/application error categories without leaking implementation-specific exceptions across public boundaries.

### FR-CORE-013 — Core readiness evidence
Core shall publish Core-scoped readiness/health evidence without determining application-level Ready/Degraded outcome.

### FR-CORE-014 — Typed configuration consumption
Core shall receive only a typed Core-specific configuration view prepared by Configuration subsystem and distributed by Application Host / Composition Root.

### FR-CORE-015 — No raw configuration ownership
Core shall not parse raw YAML, own the configuration schema, resolve secrets or distribute the global configuration tree.

### FR-CORE-016 — No generic persistence
Core shall not use generic domain repositories, arbitrary CRUD, SQLite or physical Database APIs.

### FR-CORE-017 — Conditional Core-owned persistence/status port
A narrow Core-owned persistence/status port is permitted only if a concrete Core responsibility cannot be satisfied by Host/readiness evidence or another existing owner contract. Default v1.0 implementation should have no such dependency unless justified.

### FR-CORE-018 — No generic AI consumer role
Core shall not be a generic AI consumer in v1.0. It may consume scoped AI readiness/capability evidence only.

### FR-CORE-019 — No concrete integration dependency
Core shall not import concrete Game Adapter, Plugin Detector, AI provider, database driver or game SDK types.

### FR-CORE-020 — No Service Locator
Core shall not expose or consume a global runtime module registry/service locator for obtaining arbitrary dependencies.

### FR-CORE-021 — Explicit dependency injection
All required Core-consumed contracts shall be supplied explicitly through construction/factory wiring owned by Composition Root.

### FR-CORE-022 — Controlled shutdown behavior
When Host requests Core shutdown, Core shall stop accepting new Core-owned non-critical workflows, cancel/deadline active work according to policy, and expose bounded completion evidence.

### FR-CORE-023 — Recovery behavior
Core shall support controlled recovery of Core-owned coordination state/in-flight operation metadata without taking ownership of persistence-, save-, game- or AI-specific recovery semantics.

### FR-CORE-024 — Operation result routing
Core shall return typed workflow results only to the owning caller/output boundary and shall not bypass application ownership by directly mutating concrete integration/domain implementations.

---

## 4. Non-Functional Requirements

### NFR-CORE-001 — Determinism
Given equivalent validated inputs, configuration and dependency results, Core coordination decisions shall be deterministic except for explicitly modeled asynchronous/event ordering.

### NFR-CORE-002 — Low coupling
Core shall depend only on explicit public contracts required by documented Core-owned use cases.

### NFR-CORE-003 — Acyclic dependencies
Core participation shall preserve the static dependency graph as acyclic.

### NFR-CORE-004 — Replaceability
Concrete Game Adapter, Dialogue, Save Manager, Scheduler/Event Engine implementations and infrastructure shall remain replaceable behind approved contracts.

### NFR-CORE-005 — Testability
Core shall be fully testable with fake/stub contracts without launching Skyrim, SQLite, Local AI or external providers.

### NFR-CORE-006 — Bounded resource usage
Core shall not retain unbounded histories, event queues, request payloads or domain caches.

### NFR-CORE-007 — Observability
Lifecycle transitions, orchestration failures, timeout/cancellation and readiness changes shall be diagnosable through bounded structured telemetry.

### NFR-CORE-008 — Security
Core shall not widen file/network/database/game/AI privileges beyond the explicit injected contract surface.

### NFR-CORE-009 — Concurrency safety
Lifecycle transitions and Core-owned mutable coordination state shall have a single serialized transition authority or equivalent race-safe mechanism.

### NFR-CORE-010 — No lock across external await
Core shall not hold Core-internal locks while awaiting downstream module/provider/game/persistence calls.

### NFR-CORE-011 — Cancellation propagation
Deadlines/cancellation shall propagate through synchronous use-case ports where supported.

### NFR-CORE-012 — Failure containment
Failure of an optional downstream capability shall not corrupt Core state; required-contract failure shall produce controlled failure/recovery evidence.

### NFR-CORE-013 — Configuration integrity
Core shall operate only on accepted typed configuration snapshots and shall not silently diverge from the accepted effective configuration.

### NFR-CORE-014 — Compatibility
Core public contracts shall remain game-agnostic; game-specific capability differences are expressed through Game Profile/Capability contracts.

### NFR-CORE-015 — Performance policy
Core shall minimize coordination overhead on latency-sensitive synchronous workflows. Numeric SLOs remain TBD until implementation/runtime measurements exist.

---

## 5. Dependencies

### 5.1 Mandatory Core-consumed contracts

| Contract | Relation | Purpose | Ownership note |
|---|---|---|---|
| Event Engine public contract | static dependency | publish semantic events / subscribe where explicitly owned | Event Engine owns delivery semantics |
| Scheduler control contract | static dependency | Core-owned schedule/control operations | no Scheduler -> Core dependency |
| Dialogue Command/Use-Case Port | static dependency | synchronous conversation orchestration | Dialogue/domain logic remains outside Core |
| Save Manager Command/Use-Case Contract | static dependency | explicit Save/Load/shutdown-save workflow | Save Manager owns Save semantics |
| Game Output/Capability Port | static dependency on port | game-facing output/capability use | concrete Game Adapter implements port |

### 5.2 Inbound contract

`Game Adapter implementation -> Game Ingress Port -> Core/Application`

The concrete adapter depends on the ingress contract; Core does not import the adapter implementation.

### 5.3 Conditional dependencies

A future narrow Core-owned persistence/status port is allowed only by documented need and must not become generic CRUD/Repository access.

Additional synchronous application use-case ports require architecture/module traceability and must remain narrow.

### 5.4 Not Core dependencies

- concrete Game Adapter;
- SQLite/Database driver;
- generic Repository/domain CRUD;
- AI provider/model SDK;
- Context Manager as a generic Core collaborator;
- Plugin Detector internals;
- domain module private APIs/state;
- global Config/Service Locator.

---

## 6. Inputs and Outputs

### Inputs

- normalized Game/Application ingress envelope;
- explicit application command/query requests;
- typed Core configuration view;
- Core-required dependency readiness evidence;
- Scheduler/Event Engine callbacks only through defined contracts;
- cancellation/deadline/correlation context.

### Outputs

- typed use-case results;
- Game Output/Capability Port calls;
- semantic events via Event Engine;
- Scheduler control operations;
- Save/Dialogue command requests via explicit ports;
- Core-scoped readiness/health/diagnostic evidence;
- normalized coordination errors.

Core output never consists of raw SQL, provider-specific AI requests, game SDK calls or direct mutation of another module's private state.

---

## 7. Public Interface Contracts

The following are logical architecture contracts. Programming-language names/types are deferred to API/implementation design.

### 7.1 Core Ingress Contract

Responsibilities:

- accept normalized inbound request/event/command envelopes;
- validate operation type/version/correlation context;
- reject unsupported/malformed ingress;
- route to explicit Core-owned workflow handlers.

### 7.2 Core Lifecycle Contract

Logical operations:

- `Start(CoreStartContext)`;
- `Stop(CoreStopContext)`;
- `GetCoreState()`;
- `GetCoreReadinessEvidence()`.

This contract is called by Host/Composition Root. It does not allow arbitrary runtime service lookup.

### 7.3 Dialogue Command/Use-Case Port

Used for synchronous conversation/knowledge-related workflows requiring an immediate result.

### 7.4 Save Manager Command/Use-Case Contract

Used for explicit manual/application Save/Load and approved shutdown-save coordination.

### 7.5 Game Output/Capability Port

Used for approved outbound game-facing operations/capability queries. Game Adapter implementation satisfies this port.

### 7.6 Event Engine Contract

Used for semantic event publication/subscription where event semantics are appropriate.

### 7.7 Scheduler Control Contract

Used for Core-owned registration/cancellation/control. It does not make Scheduler a Core child or create reverse dependency.

---

## 8. Internal Components

### 8.1 Core Lifecycle Controller

Owns Core lifecycle state, valid transitions and lifecycle operation serialization.

### 8.2 Ingress Router

Validates normalized ingress and selects a documented Core-owned use-case orchestrator.

It is not a generic dynamic module dispatcher.

### 8.3 Use-Case Orchestrators

Small explicit coordinators for workflows such as:

- Conversation;
- Manual Save/Load;
- approved game-facing coordination;
- future architecture-approved synchronous workflows.

Each orchestrator receives only the contracts it needs.

### 8.4 Event Publication Gateway

Owns Core-side mapping of confirmed semantic facts to Event Engine contracts. It does not own Event Engine queues/delivery.

### 8.5 Scheduler Coordination Gateway

Owns Core-side Scheduler control usage. It does not own Scheduler lifecycle/timing engine.

### 8.6 Operation Context Manager

Creates/propagates bounded correlation, deadline and cancellation metadata.

### 8.7 Core State Store

Contains only Core-owned lifecycle/coordination metadata. It is not a domain-state cache or global runtime registry.

### 8.8 Core Readiness Evidence Publisher

Produces Core-scoped health/readiness evidence for Host/Diagnostics.

---

## 9. Events

Core may publish semantic events such as:

- application/game interaction accepted;
- ConversationStarted / ConversationCompleted;
- coordination workflow failed/aborted where event semantics are useful;
- Core lifecycle/readiness facts when Event Catalog later defines them.

Rules:

- exact names belong to Event Catalog stage;
- event publication follows ADR-002;
- events do not replace synchronous command/query results;
- producer/subscriber relation is not a direct static dependency by itself;
- events are published only after the represented fact actually occurred.

Autosave remains Scheduler-owned timing publication, not Core event publication.

---

## 10. Data Structures

Core-owned data is intentionally small and bounded.

Logical structures:

- `CoreLifecycleState`;
- `CoreReadinessEvidence`;
- `OperationContext`;
- `IngressEnvelope`;
- `UseCaseResult<T>`;
- `CoordinationError`;
- bounded in-flight operation descriptors.

Core shall not own:

- NPC/domain aggregates;
- Save artifact contents;
- plugin profiles as mutable source-of-truth state;
- AI context history;
- generic repository entities.

---

## 11. Core Lifecycle State Model

Recommended v1.2 Core-owned states:

```text
Created
  -> Starting
  -> Ready
  -> Stopping
  -> Stopped

Starting/Ready
  -> Failed or RecoveryRequired when a Core-owned invariant cannot be maintained safely
```

### 11.1 Degraded scope

`ApplicationDegraded` is an application-level outcome owned by Host policy.

Core shall not automatically enter `CoreDegraded` because AI, Game integration or another optional sibling capability is unavailable.

A distinct Core `Degraded` state may be introduced later only if a clearly defined optional **Core-owned coordination capability** can fail while Core still satisfies its public contract.

---

## 12. Startup Lifecycle

Application startup is Host-owned. Core startup is only one step.

Canonical application-level relationship:

```text
Configuration Subsystem -> validated effective configuration
Application Host / Composition Root
  -> construct/inject Core contracts
  -> start Event Engine/Scheduler/other required components according to policy
  -> call Core.Start(CoreStartContext)
  -> collect scoped readiness evidence
  -> determine application Ready/Degraded/StartupFailed/RecoveryRequired
```

Core.Start shall:

1. serialize the lifecycle transition;
2. validate Core-specific typed configuration;
3. validate presence/contract-version compatibility of mandatory injected ports;
4. initialize Core-owned state/operation infrastructure;
5. establish any Core-owned Scheduler registrations if policy requires them;
6. publish Core readiness evidence;
7. enter Ready only when Core-owned invariants are satisfied.

Core shall not instantiate Game Adapter, Save Manager, Repository, AI Adapter or provider processes.

---

## 13. Shutdown Lifecycle

Host owns top-level shutdown ordering.

When Host invokes Core.Stop:

1. transition Ready -> Stopping under serialized authority;
2. reject new Core-owned non-critical workflows;
3. propagate cancellation/deadlines to active Core-owned synchronous operations according to policy;
4. allow bounded completion where safe;
5. release/cancel Core-owned Scheduler registrations through Scheduler contract if applicable;
6. close Core-owned resources;
7. publish final Core readiness/lifecycle evidence;
8. transition to Stopped.

Core does not stop sibling components globally and does not own Local AI, Database or Game Adapter process shutdown.

A shutdown-save, when product policy requires it, is a separate Save use case coordinated through Save Manager Command Contract and application shutdown orchestration; Save semantics remain Save Manager-owned.

---

## 14. Recovery

Core recovery applies only to Core-owned coordination state and operation metadata.

Rules:

- invalid Core lifecycle transition is rejected deterministically;
- stale asynchronous result is discarded using operation/correlation context;
- partially completed synchronous use case returns controlled failure and delegates owner-specific rollback/recovery to the owning subsystem contract;
- Save restore recovery follows ARCH-007/ARCH-011 and is not implemented by Core;
- AI provider recovery is Host/AI Integration-owned;
- Game Adapter recovery is Integration/Host-owned;
- database/persistence recovery is Persistence owner-owned.

Core may participate in application recovery gating through explicit typed results/evidence without becoming the global recovery owner.

---

## 15. Configuration

Core consumes one immutable responsibility-scoped typed configuration view.

Possible logical categories:

- Core coordination feature flags;
- Core workflow timeout/deadline defaults;
- bounded concurrency settings;
- Core diagnostics policy;
- Core-owned scheduler-registration policy, if any.

Core does not consume raw YAML or global mutable configuration.

Configuration subsystem owns parse/schema/merge/normalization/secret-reference semantics; Host distributes typed views.

Hot reload of Core fields requires configuration-level candidate validation and Core acceptance through the controlled ARCH-009 transaction model.

---

## 16. Health, Readiness and Application Outcome

Keep four scopes distinct:

1. Core lifecycle state;
2. Core health/readiness evidence;
3. sibling capability health/readiness evidence;
4. application-level startup/runtime outcome.

Core readiness means Core can satisfy its own public coordination contract under the current injected contract state.

Application Host aggregates evidence and decides:

- Application Ready;
- Application Degraded;
- Application Startup Failed;
- Application Recovery Required.

Core does not transform every sibling optional failure into a Core state transition.

---

## 17. Error Handling

### 17.1 Error categories

Stable logical categories should include:

- invalid ingress;
- invalid lifecycle transition;
- required contract unavailable;
- downstream use-case failure;
- timeout/deadline exceeded;
- cancellation;
- stale result;
- Core invariant violation;
- configuration rejection;
- resource/backpressure rejection;
- recovery required.

### 17.2 Error rules

- do not leak raw provider/driver/game exceptions across Core public contracts;
- include correlation/operation context in diagnostics;
- do not include secrets or unnecessary sensitive context;
- failure owner remains the subsystem that owns the failed semantics;
- Core decides coordination outcome, not owner-specific internal recovery policy.

---

## 18. Failure Scenarios

| Scenario | Required Core behavior |
|---|---|
| Duplicate Start | idempotent success or deterministic already-started result; no second initialization |
| Stop during Starting | serialize transition; cancel/complete safely according to lifecycle rule |
| Invalid ingress | reject before domain/use-case dispatch |
| Event Engine required contract unavailable | Core cannot satisfy event-dependent contract; publish controlled failure evidence |
| Scheduler unavailable | fail only Core-owned scheduler-dependent functions; application policy decides wider outcome |
| Dialogue use-case timeout | cancel/deadline result; do not use Event Engine as fallback RPC |
| Save Manager unavailable for explicit Save/Load | return typed unavailable/failure; no Repository bypass |
| Game Output Port unavailable | return controlled integration failure; no concrete adapter lookup |
| AI unavailable | Core remains non-AI owner; use-case owner handles AI fallback; Core receives scoped result/evidence only |
| Stale async result | discard using operation context |
| Resource pressure | reject/defer according to bounded concurrency policy |

---

## 19. Performance Requirements

Core performance model is coordination-focused.

### Latency-sensitive

- normalized ingress validation;
- synchronous Dialogue/Save/game-output orchestration;
- cancellation/deadline propagation;
- lifecycle state reads.

### Asynchronous/background

- non-critical diagnostics;
- semantic event reactions outside immediate use-case result;
- readiness/telemetry publication.

Rules:

- no unbounded internal queue;
- no duplicate Event Engine/Scheduler queue implementation inside Core;
- no domain cache ownership;
- numerical latency/memory SLOs remain TBD until implementation/runtime measurement.

---

## 20. Concurrency and Backpressure

- lifecycle transitions have one serialized authority;
- read-only state snapshots may be thread-safe/lock-free/immutable as implementation permits;
- in-flight operation collection is bounded;
- locks are minimal and never held across downstream await;
- correlation/deadline/cancellation propagate through use-case ports;
- stale results cannot overwrite newer operation state;
- Event Engine owns event queues;
- Scheduler owns scheduling queues;
- Save Manager owns save/autosave queue/coalescing semantics;
- AI Integration owns AI queues/backpressure;
- Core shall not duplicate these owner queues.

---

## 21. Security and Constraints

Mandatory invariants:

- no direct game SDK/API types in Core;
- no raw SQL/SQLite/Database driver access;
- no provider/model SDK or secrets access;
- no arbitrary filesystem/process access introduced by Core coordination;
- no Plugin Detector code execution or plugin-content trust escalation;
- no Service Locator/global module registry;
- no runtime configuration mutation outside ARCH-009 controlled model;
- diagnostics redact secrets and avoid uncontrolled sensitive/high-cardinality data;
- AI remains non-authoritative;
- game-facing action occurs only through approved Game Output/Capability Port.

---

## 22. Metrics

Recommended Core metrics/events:

- lifecycle transition duration/outcome;
- Core readiness changes;
- ingress accepted/rejected counts by bounded operation category;
- synchronous use-case duration/outcome by bounded category;
- timeout/cancellation count;
- invalid transition count;
- backpressure/resource rejection count;
- downstream required-contract-unavailable count;
- stale-result discard count;
- recovery-required count.

Avoid IDs/user/content strings as high-cardinality metric labels.

---

## 23. Testing Strategy

### 23.1 Unit tests

- lifecycle transition table;
- ingress validation/routing;
- use-case orchestration with fake ports;
- error normalization;
- correlation/deadline/cancellation propagation;
- readiness evidence;
- stale-result rejection;
- bounded state/resource behavior.

### 23.2 Architecture-conformance tests

Must prove:

1. Core implementation does not import concrete Game Adapter/game SDK types.
2. Game ingress reaches Core through defined ingress contract.
3. Core can call Game Output/Capability Port without adapter implementation dependency.
4. Core coordinates conversation through Dialogue Command Port without importing Dialogue internals.
5. Core issues Save/Load through Save Manager contract without Repository/Database access.
6. no Scheduler -> Core static dependency exists.
7. no Scheduler -> Save Manager static dependency exists.
8. Event Engine is not used as RPC for synchronous results.
9. Core is not a generic AI consumer.
10. Host owns construction/startup/readiness aggregation.
11. no Service Locator/global module registry exists.
12. static dependency graph remains acyclic.
13. application/Core readiness scopes remain distinct.

### 23.3 Integration tests

- normalized Game ingress -> Core -> Dialogue -> Core -> Game Output;
- manual Save/Load coordination through Save Manager contract;
- Scheduler/Event Engine semantic trigger behavior without reverse Core dependency;
- failure of optional AI capability without incorrect Core ownership/state transition;
- Host startup/shutdown/readiness aggregation with Core evidence.

---

## 24. Acceptance Criteria

MOD-001 v1.2 is accepted when:

1. all Core-consumed contracts are explicit and injected;
2. concrete integration/infrastructure dependencies remain absent;
3. conversation synchronous path works without Event Engine RPC;
4. manual Save/Load works through Save Manager contract only;
5. game-facing output works through Game Output/Capability Port only;
6. Scheduler relationship is one-way Core -> Scheduler control contract;
7. autosave does not route through Core;
8. Core consumes typed configuration only;
9. Core and application readiness are separately modeled;
10. Core remains a non-generic AI consumer;
11. no Service Locator exists;
12. concurrency/cancellation/backpressure rules are verified;
13. architecture-conformance tests pass;
14. RU and DE final DOCX versions have semantic parity;
15. final dependency-cycle validation remains PASS.

---

## 25. Assumptions

- v1.0 remains a modular monolith in one main World Engine process with optional separate Local AI process.
- first supported game is Skyrim, but Core remains game-agnostic.
- SQLite remains v1.0 physical database behind persistence boundaries.
- AI remains optional/non-authoritative.
- Event Engine, Scheduler, Game Adapter, Dialogue and Save Manager module specifications will refine their own contracts without changing the frozen ownership model unless a new ADR is adopted.

---

## 26. Open Questions / Deferred Decisions

The old Scheduler/Core dependency ambiguity is **closed**.

Still deferred:

- programming language/framework/runtime;
- exact interface/type names;
- exact timeout/SLO values;
- exact thread/executor model;
- exact telemetry backend;
- whether a Core-owned persistence/status port is needed at all;
- exact Game Output capability IDs;
- exact Save/Dialogue contract method signatures;
- exact application Host implementation structure.

These decisions must not change the ownership/dependency semantics established by ADR-008 without a new architecture decision.

---

## 27. Future Improvements

Possible future evolution:

- more explicit application use-case coordinators;
- generated contract/version compatibility checks;
- richer operation tracing;
- formal state-machine verification;
- policy-driven workflow budgets;
- multi-session support after v1.0 if product scope changes.

Future work must not reintroduce Service Locator, concrete adapter cycles or generic persistence/AI ownership into Core.

---

## 28. Related ADRs

Primary:

- ADR-001 Core Architecture + audit amendment;
- ADR-002 Event System + clarification;
- ADR-003 Database + amendment;
- ADR-005 Save System + v1.2 amendment;
- ADR-007 Multi-Game Support + amendment;
- ADR-008 Dependency Semantics, Contract Ownership & Composition Root.

Contextual:

- ADR-004 Local AI + amendment;
- ADR-006 Plugin Detection + clarification.

---

## 29. Required Diagrams

Final MOD-001 RU/DE DOCX should include or reference:

1. Core context/ports diagram;
2. static dependency diagram consistent with ARCH-005;
3. Core lifecycle state diagram;
4. conversation use-case sequence reference to ARCH-007 SD-001;
5. Save/Load use-case sequence reference to ARCH-007 SD-004/SD-005;
6. Host vs Core lifecycle/readiness scope diagram.

Arrow semantics must distinguish static dependency, runtime call, event relation, data/evidence flow and lifecycle wiring.

---

## 30. Related Documents

- ARCH-001 Overall System Architecture v1.3 audited candidate;
- ARCH-002 Module Architecture v1.3 audited candidate;
- ARCH-003 Data Flow v1.3 audited candidate;
- ARCH-004 Architecture Principles & Dependency Rules v1.3 audited candidate;
- ARCH-005 Module Dependencies v1.3 audited candidate;
- ARCH-006 Component Diagram v1.2 audited;
- ARCH-007 Sequence Diagrams v4 audited;
- ARCH-008 Deployment Architecture v1.1 audited;
- ARCH-009 Configuration Architecture v1.1 audited;
- ARCH-010 Plugin Architecture v1.1 audited rebuild;
- ARCH-011 Save System Architecture v1.1 audited;
- ARCH-012 AI Integration Architecture v1.1 audited;
- DOC-TPL-001 Module Specification Template;
- AUDIT-MOD-001 Post-Architecture Re-audit.

---

## 31. Change Log

### v1.2 — Architecture audit regeneration

Supersedes the v1.1 implementation interpretation in the following areas:

- replaces the overly restrictive `Core -> Event Engine + Scheduler only` model;
- permits explicit narrow Core-owned synchronous use-case ports;
- adds Dialogue Command/Use-Case Port;
- permits narrow Save Manager Command/Use-Case Contract;
- adds Game Output/Capability Port while continuing to forbid concrete Game Adapter dependency;
- clarifies that Game Ingress is inbound through an explicit port;
- closes Scheduler/Core reverse-dependency ambiguity;
- moves top-level construction/startup/shutdown/readiness aggregation to Application Host / Composition Root;
- separates Core readiness from application-level Degraded/Recovery outcomes;
- retains no-generic-AI-consumer rule;
- retains no-Service-Locator and no-generic-persistence rules;
- aligns Save/Restore with atomic logical activation architecture.

### Gate

This source is the canonical basis for generating:

- `MOD-001_Core_RU_v1_2_Architecture_Audited_Complete.docx`
- `MOD-001_Core_DE_v1_2_Architecture_Audited_Complete.docx`

MOD-002 remains blocked until those binaries pass full render QA, RU/DE parity and final repository-wide dependency-cycle/integrity audit.
