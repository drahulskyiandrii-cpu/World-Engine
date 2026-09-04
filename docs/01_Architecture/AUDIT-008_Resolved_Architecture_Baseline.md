# AUDIT-008 — Resolved Architecture Baseline

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Provisional correction baseline after ADR-001–ADR-007 review  
**Date:** 2026-09-01

## 1. Purpose

This document freezes the architecture semantics that downstream corrections must follow. It is not a replacement for formal ADRs; it is the audit baseline used to regenerate ADR-008, ARCH-001–ARCH-012 and MOD-001 consistently.

Any later correction that conflicts with this baseline must first update the relevant ADR decision rather than silently changing a diagram or module specification.

## 2. Fundamental semantic rule

Every architecture relation must state its type. The following are distinct:

1. **Static dependency** — compile-time/module dependency on a public contract.
2. **Runtime call** — caller invokes callee during execution.
3. **Event relation** — producer/subscriber relation through Event Engine.
4. **Data flow** — information movement independent of ownership.
5. **Ownership** — subsystem owning state, policy, lifecycle or semantics.
6. **Lifecycle wiring** — Host/Composition Root construction/startup/shutdown relationship.

A runtime arrow does not automatically define a static dependency.

## 3. Composition Root / Host

World Engine v1.0 uses a single-process modular-monolith architecture for the main application. A dedicated **Application Host / Composition Root** is responsible for:

- constructing components;
- resolving explicit dependencies;
- selecting configuration/profile;
- selecting the active Game Profile;
- wiring adapters and ports;
- startup ordering;
- top-level readiness aggregation;
- shutdown ordering;
- process-level recovery decisions.

The Host is not a business/domain module and must not become a Service Locator exposed to ordinary runtime code.

Core owns Core coordination semantics, not construction of the whole application graph.

## 4. Canonical layer placement

| Layer | Components |
|---|---|
| Integration | Game Adapter; Plugin Detection/Analyzer and other game-specific integration components |
| Coordination / Application | Core; Event Engine; Scheduler |
| Simulation / Domain Application | Memory; Character; Dialogue; Relationship; Organization; Economy; Politics; Quest |
| AI / Media | AI Adapter; Context Manager; provider abstraction; Local/External AI providers; Voice Engine |
| Persistence | Save Manager; Repository contracts/implementations; Database; Migrations; Backups |

Layer placement does not by itself determine dependency direction. Dependencies follow ownership and explicit contracts.

## 5. Core dependency model

### 5.1 Allowed Core relationships

Core may depend on:

- Event Engine public contract;
- Scheduler control contract;
- explicit application/use-case ports required for Core-owned orchestration;
- Save Manager command/use-case contract for explicit Core-owned save/load/shutdown workflows;
- Game Output/Capability Port for approved integration operations;
- narrow Core-owned persistence/status contract only when a Core responsibility genuinely requires it.

### 5.2 Core restrictions

Core must not depend on:

- physical Database/SQLite contracts;
- arbitrary domain repositories or generic CRUD;
- concrete Game Adapter implementation or game SDK types;
- concrete AI provider/model SDK;
- Plugin Detector internals;
- domain-module internals;
- a global Service Locator/module lookup registry.

Core must not own domain semantics merely because it coordinates a workflow.

## 6. Scheduler model

Canonical static relation:

`Core -> Scheduler control contract`

Scheduler does not statically depend on Core.

Scheduled occurrences are represented through explicit scheduler contracts/events. Example autosave trigger:

`Scheduler -> Event Engine: AutosaveDue`

Save Manager subscribes according to autosave policy.

Therefore:

- no `Scheduler -> Core` dependency;
- no direct `Scheduler -> Save Manager` dependency;
- no scheduler ownership of persistence retry policy.

Event Engine internal dispatch scheduling is unrelated to MOD-012 Scheduler.

## 7. Event Engine model

Event Engine is the dedicated semantic-event subsystem.

Use Event Engine for:

- state-change events;
- decoupled reactions;
- lifecycle notifications where appropriate;
- selected durable history/recovery use cases.

Do not use Event Engine as generic RPC.

Commands/queries requiring immediate results use approved direct use-case contracts.

Event Engine owns event-delivery lifecycle, not domain state or domain transactions.

## 8. Game Adapter port model

Runtime communication is bidirectional without a static dependency cycle.

### 8.1 Inbound

`Game Runtime -> Game Adapter -> Game Ingress Port -> Core/Application`

Game Ingress carries validated, normalized observations/events/commands into World Engine.

### 8.2 Outbound

`Core/Application -> Game Output/Capability Port <- Game Adapter -> Game Runtime`

Game Adapter implements the game-specific side of approved reads/actions/capability queries.

### 8.3 Rules

- Domain modules do not import Game Adapter or game SDK types.
- Core/application code asks for capabilities/approved operations, never Skyrim APIs.
- Game Adapter owns translation and integration capability status.
- Host/Composition Root owns adapter construction, Game Profile selection and lifecycle wiring.
- One active Game Profile/session is the v1.0 default.

## 9. Persistence model

Physical path:

`Dedicated Repository Port -> Repository Implementation -> SQLite`

Rules:

- physical SQL/schema details stay inside persistence layer;
- domain/application owners use dedicated logical repository contracts for their owned data/use cases;
- Save Manager uses save-oriented persistence contracts;
- Event Engine may use a dedicated history persistence contract;
- Core may see only a narrow Core-owned persistence/status contract when justified;
- Core does not perform generic domain CRUD;
- AI, Plugin Detection and Game Adapter have no direct SQLite access.

Normal domain persistence and Save System are related but distinct concerns.

## 10. Save System model

Save Manager owns:

- Save Contract;
- save snapshot orchestration;
- artifact lifecycle;
- validation/promotion;
- restore orchestration;
- save-specific recovery.

Database/Repository owns storage mechanics.

Domain/state-owning modules own their state semantics and expose explicit save/restore participation contracts.

Canonical trigger paths:

- manual/application save/load: application/Core use-case command -> Save Manager;
- scheduled autosave: Scheduler -> Event Engine `AutosaveDue` -> Save Manager subscriber;
- Game Adapter may provide safe-point/restorable-state evidence but does not own autosave policy.

A committed Save is an **authoritative persisted/restorable checkpoint**, not necessarily the newest live runtime state after later mutations.

Restore follows:

`Validate Artifact -> Stage -> Validate Participants -> Apply via state-owner contracts -> Post-validate -> Application activation/resume`

Partial restore must never be reported as successful.

## 11. AI model

Canonical consumer-facing path:

`Approved Consumer -> AI Adapter / AI Capability Port -> Context Manager -> Provider Abstraction -> Local/External Provider`

Rules:

- AI Adapter is the application entry boundary for AI requests;
- Context Manager is behind that boundary from the consumer perspective;
- Dialogue is the primary AI consumer for v1.0;
- additional consumers require explicit use-case contracts;
- Core is not a generic AI consumer by default;
- Local AI runs as a separate optional process;
- Host/Composition Root owns provider process lifecycle wiring;
- AI output is non-authoritative until deterministic application/domain validation;
- AI has no direct DB/Save/Game authority.

## 12. Plugin Detection model

Plugin Detection owns:

- discovery;
- normalization;
- validation;
- Plugin Profile/fingerprint evidence.

Game Adapter owns capability mapping.

Save Manager owns restore compatibility decisions.

Configuration owns user/system policy.

Host/Composition Root owns detector lifecycle wiring.

“Single source of detection truth” means plugin discovery/profile evidence only; it does not override Game Adapter capability or Save compatibility ownership.

## 13. Readiness and lifecycle scopes

The following must not be conflated:

- component lifecycle state;
- component readiness;
- Core coordination state;
- provider/adapter health;
- application startup outcome.

The Host aggregates application readiness and determines application-level `Ready / Degraded / Startup Failed / Recovery Required` outcomes.

Core exposes its own coordination lifecycle/readiness evidence but does not implicitly own every component lifecycle.

## 14. Synchronous use-case ports

A synchronous workflow requiring an immediate result may use a direct, narrow use-case contract. Example:

`Game Adapter -> Game Ingress -> Core -> Dialogue Command Port -> Dialogue Engine`

Response returns through the orchestration boundary and then through Game Output Port.

This does not permit arbitrary direct module coupling. Each use-case port must have:

- explicit owner;
- explicit input/output contract;
- reason for synchronous behavior;
- no concrete implementation leakage;
- no cyclic implementation dependency.

## 15. Canonical dependency invariants

1. No implementation dependency cycles.
2. No direct domain/integration access to SQLite.
3. No direct domain module access to concrete game API.
4. No direct consumer access to model/provider SDK.
5. Event Engine is not generic RPC.
6. Scheduler is not persistence coordinator.
7. Save Manager owns Save semantics, not all domain persistence.
8. Game Adapter owns translation, not shared domain state.
9. AI remains non-authoritative.
10. Plugin Detection supplies evidence, not compatibility policy.
11. Host/Composition Root wires lifecycle; Core does not become a Service Locator.
12. Arrow semantics must be explicit in diagrams.

## 16. Required document regeneration order

The following order is mandatory to avoid downstream churn:

1. Regenerate/finalize ADR-008 around this baseline.
2. Amend ADR-001/003/004/005/006/007 and clarify ADR-002.
3. Regenerate ARCH-004 dependency principles.
4. Regenerate ARCH-005 dependency model and mechanically verify acyclicity.
5. Regenerate ARCH-001 overall architecture.
6. Regenerate ARCH-002 module architecture.
7. Regenerate ARCH-003 data flow with explicit arrow semantics.
8. Replace ARCH-006 component diagram.
9. Regenerate ARCH-007 sequence diagrams.
10. Clarify ARCH-008 deployment ownership.
11. Clarify ARCH-009 readiness/configuration scopes.
12. Rebuild malformed ARCH-010 and align lifecycle ownership.
13. Align ARCH-011 Save architecture.
14. Align ARCH-012 AI architecture.
15. Re-audit and revise MOD-001.
16. Run RU/DE semantic parity, DOCX visual QA, dependency-graph validation and final repository audit.

## 17. Current gate

This baseline is **provisional but stable enough to begin formal correction generation**. Any correction artifact prepared before AUDIT-007/AUDIT-008 must be considered stale until regenerated against this baseline.

MOD-002 and later module specifications remain blocked.
