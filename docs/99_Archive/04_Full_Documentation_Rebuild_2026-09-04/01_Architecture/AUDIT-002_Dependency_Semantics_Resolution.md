# AUDIT-002 — Dependency Semantics Resolution

**Project:** World Engine  
**Baseline:** `main@33129be20cf1d924b4c79ecbcd6ba7e5174591e6`  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Proposed resolution validated against current ADR set  
**Date:** 2026-09-01

## 1. Purpose

This audit note resolves the root ambiguity identified in AUDIT-001: existing documents use the same arrow notation for static dependencies, runtime calls, event relations, data flow and ownership. Those meanings must be separated before revising ARCH-001–012 or MOD-001.

This note is an audit artifact. The corresponding formal architecture decision is ADR-008 (Dependency Semantics, Contract Ownership & Composition Root), currently prepared as a proposed document and not yet merged to `main`.

## 2. Source decisions checked

The proposed resolution was checked against the current repository decisions and specialized architecture documents, especially:

- ADR-001 Core Architecture
- ADR-002 Event System
- ADR-003 Database
- ADR-004 Local AI
- ADR-005 Save System
- ADR-006 Plugin Detection
- ADR-007 Multi-Game Support
- ARCH-011 Save System Architecture
- ARCH-012 AI Integration Architecture
- ARCH-005 / legacy DEPS-001
- MOD-001 Core

Where these documents disagree, this note records the conflict explicitly rather than treating the newest wording as automatically correct.

## 3. Canonical relation taxonomy

Every architecture relation must be classified as one of the following:

1. **Static dependency** — a module/package depends on a public contract owned by another architectural boundary.
2. **Port binding** — an implementation is injected behind a port/interface without reversing domain ownership.
3. **Runtime call** — caller invokes callee during execution; this does not by itself define static dependency direction.
4. **Event relation** — producer publishes a typed semantic event and subscribers react through Event Engine.
5. **Data flow** — information moves from source to destination; this does not define ownership.
6. **Ownership** — component owns state, policy, lifecycle or semantics; ownership is not derived from arrow direction.
7. **Context/readiness relation** — host supplies read-only health/capability evidence; this is not a module dependency unless a public contract is explicitly consumed.

Future diagrams and sequence documents must label relation type or use a legend that makes it unambiguous.

## 4. Contract ownership rule

Every public contract must have an explicit owner.

- Inner application/domain modules may own ports implemented by outer integration/provider layers.
- Shared contract packages may contain interfaces, immutable DTO/envelopes, enums and validation metadata, but no runtime state or service-locator behavior.
- Composition Root / World Engine Host creates implementations and wires them to ports through constructor/factory injection.
- Runtime lookup of arbitrary services is forbidden unless a later ADR explicitly approves it.

This allows dependency inversion without creating static implementation cycles.

## 5. Resolved Core boundaries

### 5.1 Core and Event Engine

**Allowed static dependency:** `Core -> Event Engine public contract`.

ADR-002 already establishes Event Engine as a dedicated subsystem for semantic events/notifications while allowing approved direct contracts for synchronous operations. Event Engine is not a universal RPC/message bus and does not own domain state.

### 5.2 Core and Scheduler

**Allowed static dependency:** `Core -> Scheduler control contract`.

The reverse static relation `Scheduler -> Core` is removed. Scheduler may publish typed scheduling events through Event Engine (for example `AutosaveDue`) but must not depend on Core timing internals. This resolves the cycle ambiguity in legacy ARCH-005/DEPS-001.

### 5.3 Core and Save Manager

**Allowed static dependency:** `Core -> Save Manager command/control contract` for explicit Save/Load/Shutdown orchestration.

Save Manager remains the owner of save semantics, snapshot orchestration, validation, artifact lifecycle and restore rules. The relation does **not** permit Core to access Save Manager internals or storage mechanics.

This reconciles ADR-001, ADR-005 and ARCH-011, which all describe Core participating in save/load orchestration while keeping Save Manager as the specialized owner.

### 5.4 Core and Repository / Database

**Forbidden by default:** `Core -> Repository/Database CRUD` and direct SQL.

ADR-003 currently contains broad wording that Core/application code may access persistence through Repository. For the revised architecture, that statement must be narrowed: domain modules may use dedicated approved repository contracts when their specifications require durable state, while Core receives persistence/readiness evidence and delegates save semantics to Save Manager. Core does not become a generic persistence consumer.

This is an explicit clarification of ADR-003 behavior and must be documented in the ADR traceability/amendment phase.

### 5.5 Core and Game Adapter

Bidirectional runtime interaction is required, but **no static implementation cycle** is allowed.

Inbound:

`Game Runtime -> concrete Game Adapter -> Core/Game Ingress contract`

Outbound:

`Core/application -> Game Output/Command Port <- concrete Game Adapter implementation -> Game Runtime`

Game-specific SDK/types remain inside Game Adapter. ADR-007's requirement that Core can use a stable adapter contract is therefore preserved through port ownership rather than a direct `Core -> concrete Game Adapter` implementation dependency.

### 5.6 Core and AI Adapter

**Default rule:** Core is not an AI consumer.

AI Adapter remains the only application boundary to AI providers, but primary AI consumers are modules with actual semantic AI use cases, especially Dialogue Engine. Core may receive AI capability/readiness evidence through host-level operational aggregation but does not call AI Adapter merely because AI exists.

ARCH-012 and ADR-001 currently contain broad `Core -> AI Adapter` wording. That wording must be revised/clarified to distinguish “application boundary available to Core/modules” from an actual required Core static dependency.

## 6. Scheduler and autosave

Legacy documents sometimes show `Scheduler -> Save Manager` directly. The revised model is:

`Scheduler -> Event Engine : AutosaveDue`

`Save Manager <- Event Engine subscription`

Save Manager applies save policy and decides whether a save is safe/allowed. Scheduler owns timing, not save semantics.

A direct Scheduler -> Save Manager dependency is forbidden unless a later module specification proves a synchronous command contract is necessary and architecture governance approves it.

## 7. Plugin Detection boundary

Plugin Detector/Analyzer remain integration components, not top-level domain modules and not Core internals.

- Plugin Detection owns discovery/normalization/validation evidence.
- Game Adapter owns capability mapping.
- Save Manager owns restore compatibility decisions.
- Core may consume normalized operational evidence/events, not raw plugin files or detector internals.

Composition Root owns construction and lifecycle wiring for the detector subsystem.

## 8. Canonical layer placement

| Layer | Components |
|---|---|
| Integration | MOD-002 Game Adapter; Plugin Detector/Analyzer as internal integration components |
| Coordination | MOD-001 Core; MOD-011 Event Engine; MOD-012 Scheduler |
| Simulation / Application | MOD-003 Memory; MOD-004 Character; MOD-005 Dialogue; MOD-006 Organization; MOD-007 Relationship; MOD-008 Economy; MOD-009 Politics; MOD-010 Quest |
| AI / Media Integration | AI Adapter; Prompt & Context Manager; MOD-014 Voice; Local/External AI provider boundaries |
| Persistence | MOD-013 Save Manager; Repository; Database; Migrations; Backups |

The layer map describes responsibility, not an “all dependencies must point down” rule.

## 9. Acyclic target graph

The canonical implementation dependency graph must remain acyclic. Simplified target relations:

- Game Adapter implementation -> Game/Core ingress contracts; implements Game Output port.
- Core -> Event Engine, Scheduler, Save Manager command/control contracts and Game Output port.
- Scheduler -> Event Engine publishing contract only.
- Character -> Memory public contract.
- Relationship -> Character public contract.
- Organization -> Relationship public contract.
- Economy -> Organization public contract.
- Politics -> Organization + Economy public contracts.
- Quest -> Memory + Relationship + Politics public contracts.
- Dialogue -> Memory + Character + Relationship + Quest + AI Adapter contracts.
- Voice -> Dialogue output contract (optional).
- Save Manager -> Repository + snapshot contracts supplied by state-owning modules.
- Domain modules -> dedicated repository contracts only where their own module specifications explicitly require persistent durable state.
- AI Adapter -> provider abstraction only; no direct world-state/database ownership.

## 10. Findings resolved or narrowed

| AUDIT-001 finding | Resolution in this phase |
|---|---|
| AUD-001 | Relation taxonomy defined; future diagrams must label arrow semantics. |
| AUD-005 | Scheduler no longer depends on Core; Core -> Scheduler only. |
| AUD-006 | Core -> Save Manager command/control contract explicitly allowed; Core -> Repository CRUD forbidden. |
| AUD-007 | Core is not an AI consumer by default; AI Adapter remains application AI boundary. |
| AUD-008 | Game runtime bidirectionality modeled with ingress/output ports; no static implementation cycle. |
| AUD-014 | Autosave uses Scheduler -> Event Engine -> Save Manager event relation; explicit Core Save/Load command contract remains allowed. |
| AUD-015 | Plugin Detector/Analyzer lifecycle belongs to Host/Composition Root; Core does not own detector internals. |
| AUD-020 | Canonical layer placement defined. |
| AUD-021 | Repository access narrowed: Core does not perform Repository CRUD; persistence health/readiness is contextual evidence. |

## 11. Documents that must change because of this resolution

High priority:

- ARCH-004 — replace obsolete layer-direction rule with relation/ownership rules.
- ARCH-005 — rebuild canonical dependency/event/persistence matrices.
- ARCH-001 — remove obsolete dependency table and generic “Core sends event to modules / modules save directly to Database” model.
- ARCH-002 — remove legacy cycles and correct Dialogue/Voice/Save Manager layer assignments.
- ARCH-003 — distinguish event flow, commands, data flow and persistence; correct save/load/autosave scenarios.
- ARCH-006 — replace misleading universal Event Engine and services-registry diagram.
- ARCH-007 — remove Game Adapter -> Dialogue bypass and clarify runtime-call arrow semantics.
- ARCH-011 — align autosave and Core/Save Manager contract semantics.
- ARCH-012 — remove implication that Core is a required AI consumer.
- MOD-001 — re-audit after upstream architecture is stable; allow narrow Core -> Save Manager command contract and Game Output port while retaining no Repository CRUD and no default AI dependency.

ADR traceability phase:

- ADR-001 — clarify final summary arrows and Core/Repository/AI/Game relations.
- ADR-003 — narrow broad Core Repository wording.
- ADR-005 — clarify autosave trigger relation.
- ADR-007 — express adapter contract through port ownership.

## 12. Gate

This resolution is sufficient to begin revising ARCH-004/ARCH-005 and then ARCH-001–003 on the audit branch. It is **not** yet permission to create MOD-002. Downstream module specifications remain blocked until the architecture audit is completed and the corrected dependency graph is revalidated.
