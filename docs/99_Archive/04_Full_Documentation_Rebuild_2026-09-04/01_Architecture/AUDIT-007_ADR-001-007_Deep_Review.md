# AUDIT-007 — ADR-001–ADR-007 Deep Review

**Project:** World Engine  
**Audit baseline:** `main@33129be20cf1d924b4c79ecbcd6ba7e5174591e6`  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Complete — semantic ADR review; RU canonical content  
**Date:** 2026-09-01

## 1. Purpose

This review checks ADR-001 through ADR-007 against the architecture conflicts already identified in AUDIT-001 through AUDIT-006. The goal is to determine which decisions remain valid, which need explicit amendment/clarification, and which architecture corrections can now be treated as an upstream baseline.

The review does not silently rewrite ADR history. Existing ADRs remain historical decision records. Where wording is ambiguous or conflicts with the now-resolved dependency semantics, the correction must be introduced as an amendment/superseding clarification and reflected in downstream architecture documents.

## 2. Review result summary

| ADR | Result | Severity | Required action |
|---|---|---:|---|
| ADR-001 Core Architecture | Valid foundation, amendment required | High | Clarify Core vs application-host lifecycle, contract ownership, persistence/game/AI boundaries, and ambiguous final arrows. |
| ADR-002 Event System | Valid with minor clarification | Medium | Preserve hybrid event/direct-contract model; distinguish Event Engine dispatch scheduling from MOD-012 Scheduler and refine persisted-state wording. |
| ADR-003 Database | Valid foundation, amendment required | High | Narrow Core persistence visibility; distinguish logical persistence/status ports from generic Repository/domain CRUD. |
| ADR-004 Local AI | Valid foundation, amendment required | High | Canonicalize `Consumer -> AI Adapter -> Context Manager -> Provider`; remove Core as generic AI consumer and move provider lifecycle orchestration to Host/Composition Root. |
| ADR-005 Save System | Valid foundation, amendment required | High | Define save command/event trigger paths, remove implied Scheduler->Save Manager coupling, clarify restore activation and persisted-checkpoint authority. |
| ADR-006 Plugin Detection | Valid with clarification | Medium | Define Host/Composition Root lifecycle ownership and scope “single source of truth” to plugin-detection evidence. |
| ADR-007 Multi-Game Support | Valid foundation, amendment required | High | Define inbound/outbound port ownership, remove static-cycle ambiguity, and move adapter lifecycle orchestration to Host/Composition Root. |

No ADR requires wholesale rejection. The main problem is not the architectural intent; it is that static dependency, runtime call direction, lifecycle ownership and contract ownership were not separated rigorously enough.

## 3. ADR-001 — Core Architecture

### 3.1 What remains authoritative

ADR-001 correctly establishes that:

- Core is a coordination/orchestration component rather than a domain-logic owner;
- domain behavior remains in specialized modules;
- Event Engine is not a universal transport;
- approved synchronous contracts are allowed where immediate results are required;
- Save Manager owns save-specific orchestration;
- physical SQLite details remain outside Core;
- Game Adapter isolates game-specific runtime logic;
- AI providers remain behind AI integration boundaries;
- circular dependencies and hidden dependency gateways are not acceptable.

These principles remain valid.

### 3.2 Confirmed ambiguity

ADR-001 also contains several statements that are too broad when interpreted as static dependencies:

- `Core uses Repository/approved persistence contracts`;
- `AI requests go through AI Adapter`;
- Core may coordinate game-related operations;
- the final ASCII summary visually connects Core to Memory, Scheduler, Save Manager, Game Adapter, Plugin Architecture and AI Adapter.

Those arrows do not state whether they mean compile-time dependency, use-case call, event relationship or runtime participation. This is one of the root causes of AUD-001.

### 3.3 Required amendment

ADR-001 should be amended to state:

1. **Application Host / Composition Root** owns process composition, component construction, startup ordering and top-level shutdown wiring.
2. **Core owns its own coordination lifecycle**, but does not instantiate or globally own all subsystems.
3. Core may use **explicit use-case ports** only for workflows that genuinely belong to Core orchestration.
4. Core must not become a generic module lookup/service-locator gateway.
5. Repository access from Core, if any, is limited to a **narrow Core-owned persistence/status contract**; Core must not use generic domain repositories or domain CRUD.
6. Game integration is bidirectional at runtime but resolved through explicit **Game Ingress and Game Output/Capability ports**, not a static Core<->Game Adapter cycle.
7. Core is **not a generic AI consumer in v1.0**. AI requests originate from approved consumers such as Dialogue through AI Adapter. Core receives capability/readiness evidence unless a future explicit use case requires an AI contract.
8. Final diagrams must label arrow semantics.

## 4. ADR-002 — Event System

### 4.1 What remains authoritative

ADR-002 is internally strong and remains the canonical event decision:

- Event Engine handles semantic events and decoupled reactions;
- Event != Command != Query;
- Event Engine is not generic RPC;
- immediate-result workflows use approved direct contracts;
- event delivery/retry/backpressure remain bounded;
- Event Engine does not own domain state;
- event history is not automatically domain truth;
- full event sourcing and distributed brokers are outside v1.0.

This ADR resolves several ambiguities in older ARCH-003/006/007 rather than causing them.

### 4.2 Required clarification

Two wording corrections are required:

1. “Dispatch scheduling” inside Event Engine means **internal event-delivery scheduling**, not dependency on or ownership by MOD-012 Scheduler.
2. Save System should be described as the source of an **authoritative persisted/restorable checkpoint**, not necessarily the continuously current authoritative runtime world state after the save point.

No redesign of ADR-002 is required.

## 5. ADR-003 — Database

### 5.1 What remains authoritative

ADR-003 correctly defines:

- SQLite as v1.0 physical persistence technology;
- Repository as mandatory boundary hiding SQL/schema/connection details;
- domain modules own semantics;
- persistence layer owns storage mechanics;
- Save Manager owns save-specific persistence boundaries;
- AI, Plugin and Game Adapter have no direct SQLite visibility;
- raw SQL is not exposed to ordinary consumers.

### 5.2 Confirmed ambiguity

The document states that Core may see “high-level persistence contracts/status” and summarizes `Core / Domain Modules -> Logical Repository Contracts`. Taken literally, this can be read as permission for Core to become a generic persistence consumer.

That conflicts with the stricter ownership direction developed during the MOD-001 audit.

### 5.3 Required amendment

The corrected rule is:

- Domain/application owners may depend on **dedicated repository ports for their owned data/use cases**.
- Save Manager depends on **save-oriented persistence contracts**.
- Event Engine may use a dedicated event-history persistence contract.
- Core may depend only on a **narrow Core-owned persistence coordination/status port if required by a Core responsibility**.
- Core must not depend on generic domain repositories, arbitrary CRUD or physical Database contracts.
- Application readiness may be supplied by Host/Composition Root evidence rather than forcing a Core->Repository dependency.

The Repository boundary remains mandatory; the amendment narrows consumer scope rather than weakening it.

## 6. ADR-004 — Local AI

### 6.1 What remains authoritative

ADR-004 correctly establishes:

- Local AI is a replaceable provider outside the main World Engine process;
- AI Adapter/provider abstraction isolates consumers from model/provider APIs;
- AI has no direct SQLite/Save/Game authority;
- AI output is non-authoritative until deterministic validation;
- provider switching is policy controlled;
- AI is optional for v1.0 unless explicitly changed;
- context must be minimized and controlled;
- Dialogue is identified as the primary AI consumer.

### 6.2 Confirmed ambiguity

The final architecture summary uses `Core / Domain Modules -> AI Adapter`, while implementation text says consumer modules use AI Adapter and identifies Dialogue as primary consumer. Some later ARCH-012 diagrams also place Context Manager before AI Adapter.

This allows two incompatible readings: Core as generic AI consumer, or AI Adapter as the single application entry point.

### 6.3 Required amendment

Canonical AI call chain:

`Approved Consumer -> AI Adapter / AI Capability Port -> Context Manager -> Provider Abstraction -> Local/External Provider`

Rules:

- Context Manager is behind the AI Adapter boundary from the consumer perspective.
- Dialogue is the primary v1.0 AI consumer.
- Additional consumers require explicit use-case contracts.
- Core is not a generic AI consumer by default.
- Host/Composition Root owns provider process startup/shutdown wiring; Core receives AI readiness/capability evidence as needed.
- AI result becomes authoritative only after deterministic application/domain validation.

## 7. ADR-005 — Save System

### 7.1 What remains authoritative

ADR-005 correctly establishes:

- Save Manager is the mandatory coordinator of Save/Restore lifecycle;
- Save has an explicit versioned contract;
- Save Manager owns artifact lifecycle, validation, promotion, restore and recovery semantics;
- Database/Repository owns physical persistence mechanics;
- domain modules retain ownership of their state;
- restore must validate before activation;
- failed save must not replace a known-good save;
- autosave uses the same Save Contract as manual save.

### 7.2 Required clarification

The ADR describes Scheduler participation and autosave but does not define a canonical trigger path. Later ARCH documents turned that ambiguity into direct `Scheduler -> Save Manager` coupling.

The corrected v1.0 rule is:

- Manual/application save/load commands may enter through an explicit application/Core save use-case port and then invoke Save Manager’s command contract.
- Scheduled autosave uses `Scheduler -> Event Engine: AutosaveDue`, with Save Manager subscribing according to policy. MOD-012 therefore does not depend directly on Save Manager.
- Game Adapter may provide safe-point/restorable-state evidence but does not own autosave policy.
- Save Manager coordinates snapshot/restore contracts; state-owning modules apply/validate their state through approved participation ports.
- Application/Host resumes normal operation only after restore activation/validation succeeds.
- A committed Save is an **authoritative persisted/restorable checkpoint**, not necessarily the latest live runtime state after subsequent mutations.

## 8. ADR-006 — Plugin Detection

### 8.1 What remains authoritative

ADR-006 is consistent on core ownership rules:

- Plugin Detection owns discovery, normalization, validation and Plugin Profile construction;
- Game Adapter owns capability mapping;
- Save Manager owns restore compatibility decisions;
- Configuration owns user/system policy;
- consumers do not rescan plugin sources independently;
- plugin content is untrusted input and grants no execution privileges;
- Plugin Detection provides evidence, not domain authority.

### 8.2 Required clarification

Two points need explicit wording:

1. Application startup sequencing of Plugin Detection is owned by **Host/Composition Root**. Neither Core nor Scheduler becomes the owner of the detector subsystem simply because a sequence diagram shows a trigger.
2. “Single source of detection truth” means the authoritative source of **plugin discovery/profile evidence**, not all integration compatibility truth. Game Adapter still owns capability mapping and Save Manager still owns restore compatibility decisions.

For v1.0 the ADR may remain Skyrim-first; future games may implement equivalent extension-discovery capabilities under their Game Profile without making Skyrim plugin semantics universal.

## 9. ADR-007 — Multi-Game Support

### 9.1 What remains authoritative

ADR-007 correctly establishes:

- one shared game-agnostic Core/domain model;
- explicit Game Adapter + Game Profile per supported game;
- capability-based integration;
- game-specific APIs and translation remain inside adapter/integration boundary;
- one active Game Profile/session in v1.0;
- cross-game portability is selective and validated rather than automatic;
- Game Adapter failure must not corrupt Core/domain state.

### 9.2 Confirmed ambiguity

The document says Game Adapter exposes a stable contract that Core can use, while older dependency documents also express `Game Adapter -> Core`. Runtime flow is naturally bidirectional, but contract ownership is not defined, creating a static-cycle ambiguity.

### 9.3 Required amendment

Use explicit ports-and-adapters semantics:

- **Game Ingress Port** is owned by the application/Core boundary and invoked by Game Adapter for normalized observations/events/commands entering World Engine.
- **Game Output/Capability Port** is owned by the application/integration contract boundary and implemented by Game Adapter for approved reads/actions/capability queries.
- Runtime calls may be bidirectional while implementation dependencies remain acyclic.
- Domain modules do not import Game Adapter types or game SDK types.
- Capability checks belong to integration-aware application/coordination code, not generic domain logic.
- Host/Composition Root owns adapter construction, profile selection and lifecycle wiring.

ADR-007 therefore remains valid after contract-ownership clarification.

## 10. Architecture conflicts resolved by this ADR pass

| Prior audit item | Resolution |
|---|---|
| AUD-005 Core/Scheduler cycle | `Core -> Scheduler control contract`; Scheduler does not depend on Core. Scheduler publishes timing/domain-neutral events through Event Engine. |
| AUD-006 Core/Save ambiguity | Core may use a narrow Save Manager command/use-case contract for Core-owned save workflows. Scheduled autosave remains event-triggered; Save Manager owns semantics. |
| AUD-007 Core/AI ambiguity | Core is not a generic AI consumer in v1.0. Approved consumers, primarily Dialogue, use AI Adapter. |
| AUD-008 Game Adapter direction | Resolve with explicit ingress and output/capability ports; runtime bidirectionality does not imply static cycle. |
| AUD-015 Plugin Detection lifecycle | Host/Composition Root owns detector lifecycle wiring. |
| AUD-017 readiness/lifecycle scope | Component readiness and application startup outcome are distinct; Host aggregates application readiness, Core exposes Core coordination state. |
| AUD-021 Core/Repository ambiguity | Only narrow Core-owned persistence/status contracts are allowed; no generic repository/domain CRUD dependency from Core. |

## 11. ADR amendment strategy

Do not silently rewrite the historical v1.0 ADR files in place during the audit.

Preferred correction strategy:

1. Finalize **ADR-008 Dependency Semantics, Contract Ownership & Composition Root** as an accepted architectural clarification.
2. Add concise **v1.1 amendment/addendum** sections or amendment documents for ADR-001, ADR-003, ADR-004, ADR-005, ADR-006 and ADR-007 where the above semantics materially change interpretation.
3. ADR-002 needs only a minor clarification and traceability update.
4. Rebuild ARCH-001–ARCH-012 against the resolved ADR baseline.
5. Re-audit MOD-001 after the architecture documents stabilize.

## 12. Gate after ADR review

The upstream semantic audit of ADR-001–ADR-007 is complete enough to freeze a **provisional correction baseline**. Binary DOCX corrections prepared before this review must still be regenerated because they predate several decisions in this report, especially:

- synchronous application use-case ports;
- narrow Core persistence/status visibility;
- canonical AI entry path;
- Game Adapter ingress/output port ownership;
- Host/Composition Root lifecycle ownership.

**MOD-002 remains blocked until the corrected architecture set and revised MOD-001 pass final consistency review.**
