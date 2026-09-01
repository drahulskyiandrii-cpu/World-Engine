# AUDIT-005 — Deep Review of ARCH-006 through ARCH-009

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Deep review complete; dependency-resolution proposal requires one amendment  
**Date:** 2026-09-01

## 1. Important new finding: synchronous application use-case ports

The ARCH-007 sequence review exposed a gap in the first AUDIT-002 dependency-resolution proposal.

A player conversation is a synchronous use case: Game Adapter submits an interaction request and the application must return a dialogue result. ADR-002 explicitly says commands/queries requiring an immediate result should use an approved direct contract rather than turning Event Engine into RPC.

Therefore the architecture cannot simultaneously require:

- `Game Adapter -> Core ingress`,
- no `Game Adapter -> Dialogue` bypass,
- Event Engine not used as generic RPC,
- and Core having no route to a Dialogue use-case contract.

### Revised rule

Core may consume **explicit application use-case ports** for synchronous workflows that it coordinates. The concrete domain/application module is injected behind the port at Composition Root.

Example:

`Game Adapter -> Core Ingress -> Dialogue Command Port <- Dialogue Engine implementation -> result -> Core -> Game Output Port <- Game Adapter implementation`

This is a **port binding / runtime call**, not a `Core -> concrete Dialogue implementation` dependency.

The same pattern can be used for other coordinated synchronous use cases where an immediate typed result is required. Every use-case port must be explicit, narrow, owned by a documented contracts boundary and covered by module/API specifications.

This finding means the prepared ADR-008/ARCH-004/ARCH-005/ARCH-001–003 files remain **candidates requiring amendment before binary commit**. They are not discarded; their “Core only Event/Scheduler/Save/GameOutput” wording must be generalized to allow approved use-case ports without allowing a God Core.

## 2. ARCH-006 Component Diagram

The current committed ARCH-006 image is structurally inconsistent with the audited architecture.

### A006-01 — Universal Event Engine statement — High

The note says: `All modules communicate through the Event Engine.`

ADR-002 explicitly rejects a universal event bus and allows approved direct contracts for commands/queries.

**Required correction:** replace with “Semantic events/notifications use Event Engine; synchronous commands/queries use approved direct contracts/use-case ports.”

### A006-02 — Core `services registry` — High

Core is labeled with `services registry`. This is unsafe because ARCH-004/005 and MOD-001 move toward explicit constructor/factory injection and no Service Locator.

**Required correction:** remove registry wording. Composition Root/Host owns wiring; Core may hold only the explicit ports/contracts it consumes.

### A006-03 — Wrong layer placement — High

The diagram places:

- MOD-013 Save Manager inside Simulation area;
- MOD-014 Voice inside Simulation area;
- Dialogue/Voice and AI components inconsistently with later AI architecture.

**Canonical target:** Dialogue in Simulation/Application; Voice in AI/Media Integration; Save Manager in Persistence.

### A006-04 — AI Adapter missing — Critical

The diagram shows LLM Service and Prompt & Context Manager but no explicit AI Adapter boundary, allowing readers to infer direct module/provider access.

ARCH-012 and ADR-004 require provider access through AI Adapter.

**Required correction:** render AI Adapter as the only provider boundary and place Local/External providers outside the application trust boundary.

### A006-05 — Unlabeled bidirectional arrows create apparent cycles — Critical

Memory/Character, Organization/Relationship, Economy/Politics and other arrows are drawn bidirectionally without relation type. This recreates the obsolete cycles from ARCH-002.

**Required correction:** new diagram must use separate legends for static contract dependency, event relation, runtime/data flow and port binding. No generic bidirectional arrow between concrete modules.

### A006-06 — Scheduler/persistence path ambiguity — High

The diagram visually connects Scheduler to persistence/Database. This can be read as a direct Scheduler -> Database/Save dependency.

**Required correction:** autosave is `Scheduler -> Event Engine : AutosaveDue`, then Save Manager applies policy. Physical storage remains behind Repository.

### A006-07 — Example flow bypasses Core semantics — Medium/High

Example shows Game Adapter -> Event Engine directly. Some ingress may eventually be allowed to publish events through a governed ingress adapter, but the current project architecture describes Core/application ingress coordination. The diagram cannot silently establish a different ingress architecture.

**Required correction:** show Game Adapter -> Core/Game Ingress, then classify command/event. For dialogue, use Dialogue Command Port; for semantic facts, publish through Event Engine.

### A006-08 — Identity mismatch — Low

Repository canonical document is ARCH-006 while the image uses CD-001. Keep CD-001 as diagram identifier only if metadata explicitly says it is the component diagram belonging to ARCH-006.

## 3. ARCH-007 Sequence Diagrams

ARCH-007 is valuable because it exposes runtime behavior, but several sequences violate or blur boundaries.

### A007-01 — Conversation uses Event Engine as RPC — Critical

SD-001 currently uses:

`Core -> Event Engine : ConversationStarted`

`Event Engine -> Dialogue Engine`

then Dialogue eventually produces a response.

A conversation response is an immediate command/result workflow. ADR-002 says Event Engine must not become generic RPC.

**Required correction:**

- Core invokes a narrow Dialogue Command/Use-Case Port and receives a typed result.
- `ConversationStarted` may still be published as a semantic/lifecycle event if independent subscribers need it, but it is not the request/response transport.

### A007-02 — Dialogue -> Game Adapter bypass — Critical

SD-001 directly returns final dialogue output from Dialogue Engine to Game Adapter.

**Required correction:** Dialogue returns result through its use-case port to Core/application coordinator; Core sends approved output through Game Output Port implemented by Game Adapter.

### A007-03 — Game Adapter -> Core/Dialogue bypass — Critical

SD-007 explicitly says `Game Adapter -> Core/Dialogue Engine`.

**Required correction:** Game Adapter has one application ingress boundary. It must not choose a domain module implementation directly.

### A007-04 — NPC death turns Save Manager into normal persistence coordinator — Critical

SD-002 sends a persistence-relevant change to Save Manager, then `Save Manager -> Repository -> Database` as if every domain mutation were persisted by Save Manager.

ADR-005 explicitly says Save System is **not** a replacement for normal Database persistence.

**Required correction:** domain owners persist durable state through their own approved repository contracts when required. Save Manager only owns coherent Save/Checkpoint artifact workflows. A later checkpoint may include the changed state; it is not the generic per-event write path.

### A007-05 — Plugin detection lifecycle ownership — High

SD-003 starts with `Core/Scheduler -> Plugin Detection Subsystem` and lets detector access Configuration and Repository-managed baseline directly.

**Required correction:** Host/Composition Root owns detector construction/lifecycle scheduling. A scheduled trigger may initiate detection through an explicit detection port or event. Detector may use its own approved configuration/profile and repository contracts, but Core/Scheduler do not own detector internals.

### A007-06 — Save result path incomplete — Medium

SD-004 ends with `Game Adapter -> Skyrim : save success` without a complete typed return chain from Save Manager/Core to Game Adapter.

**Required correction:** explicitly show Save Manager result -> Core/application command result -> Game Output/response boundary -> Game Adapter -> game.

### A007-07 — Load activation wording — High

SD-005 applies validated state fragments from Save Manager directly to Simulation Modules, then publishes LoadCompleted. ARCH-011 later defines staging, cross-module validation and controlled activation.

**Required correction:** distinguish snapshot reconstruction/staging from activation. State owner contracts participate in reconstruction; Host/Core coordinates activation gate; `LoadCompleted` occurs only after successful activation.

### A007-08 — Scheduler -> Core cycle and Scheduler/Core -> Save Manager — Critical

SD-006 currently:

`Scheduler -> Core -> Event Engine`

and later `Scheduler/Core -> Save Manager`.

**Required correction:** Scheduler publishes typed scheduled trigger through Event Engine. For autosave/checkpoint it publishes `AutosaveDue`; Save Manager owns policy. Core does not need a reverse Scheduler callback dependency.

### A007-09 — Sequence arrow semantics missing — High

ARCH-007 must state for every arrow whether it is runtime call, event publication/delivery, port call, data response or external integration. Sequence direction is not static dependency direction.

## 4. ARCH-008 Deployment Architecture

ARCH-008 is substantially stronger than early drafts, but several corrections are needed.

### A008-01 — World Engine process “owns” components — Medium

Text says the World Engine process “owns Core, Event Engine, Scheduler and simulation logic.” A process contains/hosts these components; architectural ownership belongs to the specialized component/module.

**Required correction:** use `hosts/contains` for process composition; reserve `owns` for state/semantics/lifecycle policy.

### A008-02 — ASCII diagram visually nests modules under Core — Medium

The process diagram can be read as Core containing Event Engine, Scheduler, Simulation, AI Adapter and Save Manager.

**Required correction:** show them as sibling components under World Engine Host/Process, not children of MOD-001 Core.

### A008-03 — Localhost HTTP is prematurely fixed — High

ARCH-008 states that v1 Local AI uses localhost HTTP. ARCH-012 states that localhost/IPC transport is conceptually allowed but the concrete transport mechanism is a later implementation decision.

**Required correction:** deployment architecture should say “loopback/IPC local transport behind AI Adapter; concrete protocol/port TBD by technical decision.” If HTTP is intentionally selected, create/approve a dedicated ADR rather than silently fixing it here.

### A008-04 — `World Engine -> SQLite` wording — Medium

Some deployment tables show World Engine directly connected to SQLite via Repository/local DB access.

**Required correction:** label the boundary as Repository/Database implementation. Ordinary application modules do not directly access SQLite.

### A008-05 — Startup/shutdown sequence scope — High

Startup order is valid as an application lifecycle, but can be misread as MOD-001 Core directly initializing Repository, Game Adapter and AI Adapter.

**Required correction:** explicitly assign construction/startup orchestration to World Engine Host/Composition Root. Core contributes its own lifecycle and coordination gate; it does not become static owner of every startup participant.

### A008-06 — Ready/Degraded scope — Medium

`Ready`, `Degraded`, `Recovery Required` are used at process/application and subsystem levels.

**Required correction:** every status must be scoped: `ApplicationReady`, `CoreReady`, `AICapabilityUnavailable`, etc., or the prose must explicitly identify the subject.

## 5. ARCH-009 Configuration Architecture

ARCH-009 is mostly compatible with the audited target and requires clarification rather than redesign.

### A009-01 — Configuration subsystem ownership — Medium

Some traceability text refers to “Configuration Architecture / Core governance” for schema structure.

**Required correction:** configuration subsystem owns parsing/schema/normalization semantics. Core/Host may consume validated typed configuration and participate in application startup policy, but Core does not own the YAML schema.

### A009-02 — Startup state scope — High

Part 04 defines:

- Ready — required components validated/available;
- Degraded — Core ready, optional capability unavailable;
- Startup Failed — critical config/dependency invalid.

This mixes application and Core state.

**Required correction:** these are application startup outcomes. Module/capability readiness is reported separately and aggregated by Host/application readiness policy.

### A009-03 — Effective configuration distribution — Low/Medium

The overall model is sound: modules receive only typed sections they need. Keep this rule, but document recipients as explicit typed configuration views rather than implying a global config object accessible through Core.

### A009-04 — Hot reload — Valid

Default immutable effective configuration after startup, controlled hot reload and explicit validation are compatible with the audited architecture.

### A009-05 — Secrets — Valid

Secret references, least exposure and no credentials in Git are consistent with AI/persistence boundaries.

## 6. Amendment required to AUDIT-002 / ADR-008 proposal

Before the prepared binary correction artifacts are uploaded, change the dependency model as follows:

### Old candidate wording

Core only consumes Event Engine, Scheduler, Save Manager command/control and Game Output port.

### Corrected rule

Core consumes:

- coordination/control ports required by Core lifecycle (Event Engine, Scheduler);
- Save/Load command port where Core owns cross-system orchestration;
- Game Output port;
- **explicit application use-case ports required by synchronous workflows** (for example Dialogue Command Port), with concrete modules injected behind those ports;
- no private domain state, no concrete module implementation lookup and no arbitrary repository/AI provider access.

This preserves ADR-001's legitimate coordination role without turning Core into a God Core or Event Engine into RPC.

ARCH-005 should distinguish:

1. concrete module implementation dependencies;
2. shared/owned contract package dependencies;
3. port bindings at Composition Root;
4. event relations.

## 7. Next correction sequence

1. Amend ADR-008 candidate and ARCH-004/005 candidates with explicit application use-case ports.
2. Amend ARCH-001–003 candidates accordingly (especially dialogue flow).
3. Define replacement specification for ARCH-006 diagram; do not reuse the current diagram as canonical.
4. Revise ARCH-007 sequences.
5. Produce small clarification revisions for ARCH-008/009.
6. Then repair ARCH-010 and reconcile ARCH-011/012.

MOD-001 must remain marked for re-audit because its current prohibition on all Core -> domain/use-case relations is now known to be too strict for synchronous coordinated workflows.
