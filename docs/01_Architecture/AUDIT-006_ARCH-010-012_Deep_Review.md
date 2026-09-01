# AUDIT-006 — Deep Review of ARCH-010 through ARCH-012

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Deep review complete; ARCH-010 structurally invalid, ARCH-011/012 require targeted reconciliation  
**Date:** 2026-09-01

## 1. Repository integrity

The local RU copies used for semantic review were verified against current `main` Git blobs:

- ARCH-010 RU: `b59d7bfbbc2795cf7b898f1d330247d110e448d8`
- ARCH-011 RU: `85c9e7df72b3f6df29306578d146403f5a0cfa24`
- ARCH-012 RU: `a03a9212ec79ae37a82be98485bb7a42141a5c7c`

The corresponding DE files are present in the same canonical directories and are included in the later parity/repair pass.

## 2. ARCH-010 — Plugin Architecture

### 2.1 Structural state

ARCH-010 is **not a valid Complete document in either RU or DE**. The committed Complete files contain Part 01 twice and omit Part 02. The separate correct Part 02 source exists for both languages, so repair is deterministic and does not require reconstructing content from memory.

This confirms AUD-018/AUD-019.

### 2.2 Durable architecture that should be preserved

The following rules are consistent with ADR-006 and the audited target:

- plugin artifacts are untrusted input;
- Discovery -> Detection -> Metadata Extraction -> Validation -> Registration -> Event Publication is the v1 pipeline;
- Plugin Detector/Analyzer are internal integration components, not top-level MOD modules;
- detection is not code execution;
- Game Adapter remains the game-specific integration/capability-mapping boundary;
- registry/baseline state is conceptually separate from gameplay state;
- persistence goes through Repository, never arbitrary direct SQLite writes;
- significant plugin lifecycle changes may be published through Event Engine;
- plugin content cannot gain filesystem/database/process/network privileges merely by being discovered;
- AI-assisted interpretation remains non-authoritative and cannot bypass security/validation.

### 2.3 Required corrections beyond the merge repair

#### A010-01 — Detector lifecycle trigger semantics — High

Current text says Game Adapter may emit signals that trigger scans and sequence documents previously show Core/Scheduler starting Plugin Detection. Construction and lifecycle ownership are still not explicit.

**Correction:** World Engine Host / Composition Root owns construction and lifecycle wiring of the Plugin Detection subsystem. Runtime triggers arrive through an explicit Detection Scan port or typed scheduled/integration event. Core/Scheduler/Game Adapter do not gain ownership of detector internals.

#### A010-02 — Registry transaction vs event publication — Valid with wording refinement

ARCH-010 correctly requires that a `PluginAdded/Modified` state not be published as committed if required registry persistence failed. Preserve this rule, but label the persistence relation as Registry-owned Repository contract and the publication as an Event Engine relation, not one static dependency arrow.

#### A010-03 — Compatibility ownership must remain split — High

Plugin Architecture can determine detection/dependency/metadata compatibility evidence, but final game support capability and save-restore compatibility decisions belong to different owners:

- Game Adapter: game capability mapping;
- Save Manager: restore compatibility decision;
- Plugin subsystem: detection/metadata/dependency evidence.

The repaired document must not collapse these into one generic `compatible` truth.

#### A010-04 — “Event Engine is the approved cross-module event boundary” — Valid

This final rule is compatible with ADR-002 as long as it is explicitly limited to **events**. It must not be rewritten as “all communication uses Event Engine.” Commands/queries still use approved contracts/use-case ports.

#### A010-05 — Security sandbox terminology — Medium

ARCH-010 uses “sandboxing concept” but v1 does not yet select a concrete OS/container sandbox implementation. Preserve sandboxing as a security objective/containment model and mark the concrete mechanism TBD unless a later implementation ADR selects it.

### 2.4 Decision

ARCH-010 must be rebuilt RU+DE from Parts 01–08 in the correct order, then receive a small v1.1 audit clarification for lifecycle trigger/ownership and compatibility evidence. The malformed v1 Complete files cannot remain canonical after audit merge.

## 3. ARCH-011 — Save System Architecture

ARCH-011 is generally strong and contains the correct major ideas: staging, validation before activation, atomic commit, versioning/migration, recovery and Save Manager ownership. It needs targeted corrections rather than redesign.

### A011-01 — “Authoritative save” vs live runtime truth — High

Part 01 says the last confirmed valid save is the “authoritative user world state.” Once gameplay continues after a save, live in-memory domain state may be newer than the persisted save.

**Correction:** call it the **last authoritative persisted save/checkpoint artifact**, not the authoritative current world truth. Runtime domain owners remain authoritative for active live state.

### A011-02 — Generic module persistence wording — Medium

“Modules do not write persistence directly” is directionally correct but too broad after the Repository clarification.

**Correction:** modules do not write physical storage/SQLite directly. A state-owning domain module may use its own approved repository contract for normal durable persistence where its MOD specification requires it. Save Manager remains the owner of coherent Save/Restore artifacts and is not a replacement for normal persistence.

### A011-03 — Runtime integration diagram conflates Core/Simulation — High

Part 07 shows `Core / Simulation -> Save Manager` and also `Scheduler -> Save Manager`.

**Correction:**

- explicit manual/load orchestration: `Core/application coordinator -> Save Manager command port`;
- autosave timing: `Scheduler -> Event Engine : AutosaveDue -> Save Manager subscription/policy`;
- state-owning modules expose read-only snapshot provider contracts to Save Manager when a coherent checkpoint is taken;
- no generic Simulation -> Save Manager command relation.

### A011-04 — Scheduler receives Save Manager result for retry — High

Current text says Save Manager returns explicit result/state so Scheduler can plan retry. That couples Scheduler to persistence policy.

**Correction:** Save Manager owns save retry/deferral policy for a triggered autosave operation. Scheduler owns future timing cadence only. Save lifecycle/result can be observed via typed events/metrics, but Scheduler does not become Save Manager's command caller/result consumer by default.

### A011-05 — Game Adapter policy forbids/delays autosave — High

Current text says `Game Adapter policy` may forbid/defer autosave during unsafe runtime integration state.

**Correction:** Game Adapter reports normalized game-runtime safe-point/capability evidence. Save Manager (or an explicit application save policy) owns the decision to defer/reject. Integration adapter does not own save policy.

### A011-06 — Load activation ownership — High

Current load lifecycle says loaded state becomes active after validation and later Part 06 says Save Manager builds staging state and then “asks activation.” The exact activation owner is not consistently modeled.

**Correction:** Save Manager owns reconstruction/staging/validation of the save candidate; each state-owning module owns validation/application of its state fragment through an activation/snapshot contract; World Engine Host/Core coordinates the cross-system activation gate. `LoadCompleted` is emitted only after successful activation and post-activation checks.

### A011-07 — Event history and normal persistence remain separate — Valid

Preserve the existing separation: Save events describe lifecycle; Save System is not the general database persistence mechanism and must not silently become Event History storage.

### A011-08 — Final diagram publication order — Low/Medium

The final summary visually places Event Engine after `Valid Active State/Recovery Source`, which can imply Event Engine is part of storage completion. Clarify that event publication is lifecycle notification after owner-defined state transitions, not part of the storage ownership chain.

### 3.1 Decision

ARCH-011 should be revised to v1.1 Audited with the above ownership/trigger corrections while preserving its transaction, staging, migration, backup and recovery model.

## 4. ARCH-012 — AI Integration Architecture

ARCH-012 is also mostly strong: provider isolation, least context, non-authoritative output, no direct DB/save/secrets access, fallback/privacy rules and separate Local AI process are sound. Several diagrams and Core references need reconciliation.

### A012-01 — `Core / Modules -> AI Adapter` is too broad — Critical

Parts 01/02 state that Core and domain modules use AI capability contracts. This can be read as Core being a normal AI consumer.

**Correction:** only **approved AI use-case consumers** receive an AI capability contract. Dialogue is the primary v1 consumer; other consumers (Memory/Knowledge analysis, Plugin semantic analysis, future modules) require explicit use-case contracts. Core receives AI capability/readiness evidence but does not call AI for generic coordination by default.

### A012-02 — AI Adapter entry-point ordering contradicts Context pipeline — Critical

ARCH-012 says AI Adapter is the **only application entry point**, and Part 02 lifecycle correctly says:

`consumer -> AI Adapter validates -> Context Manager builds context -> provider abstraction`

But Part 03 and the final summary draw:

`consumer/Core/Modules -> Context Manager -> AI Adapter -> Provider`

That bypasses the stated only-entry boundary.

**Correction:** canonical runtime path is:

`approved consumer -> AI Adapter / AI Capability Port -> Context Manager (internal collaborator) -> Provider Abstraction -> Provider -> response validation -> consumer`.

Context Manager may call Memory/Knowledge retrieval contracts, but consumers do not bypass AI Adapter to reach it as the provider request entry point.

### A012-03 — Core Ready wording scope — Medium

“Core can be Ready while Local AI unavailable” mixes Core/module readiness with application capability state.

**Correction:** `CoreReady` and `AICapabilityUnavailable` are separate scoped statuses; application may be `ReadyDegraded` (or equivalent application outcome) when optional AI is unavailable. Final naming should align ARCH-009/readiness governance.

### A012-04 — Provider replacement mentions “Plugin module” — Low/Medium

Plugin Detector/Analyzer are not top-level modules. Wording such as “Dialogue/Memory/Plugin module contracts” should become “Dialogue/Memory contracts and Plugin Detection integration contracts/use cases.”

### A012-05 — `Core-first scheduling` — Medium

Operational section says AI scheduling should not compete with “Core workload.” The protected resource is the **latency-sensitive application/simulation/game-control path**, not the Core module specifically.

**Correction:** phrase resource priority in application/workload terms, not Core ownership.

### A012-06 — AI subsystem startup ownership — High

Part 07 startup lists loading AI config, initializing AI Adapter/provider, starting/checking Local AI Runtime. This is a valid application startup sequence but should not imply Core owns it.

**Correction:** World Engine Host/Composition Root owns AI subsystem construction/startup/shutdown wiring; AI Adapter owns its provider lifecycle contract; Core only consumes scoped readiness evidence where necessary.

### A012-07 — Final deterministic decision -> Game Adapter ambiguity — Critical

The final summary lists Game Adapter next to Dialogue/Memory/Character/Quest as a destination of deterministic AI application decisions. That can imply AI/application validation directly chooses an integration adapter operation.

**Correction:** AI result is returned to the owning application/domain use case. Any game action then uses the approved Game Output port implemented by Game Adapter. Game Adapter never becomes an AI decision owner.

### A012-08 — AI memory candidate persistence — Valid

Preserve: AI Adapter returns a result/candidate; Memory Engine validates and decides whether to mutate/persist knowledge through its own repository contract. AI never writes Memory Repository directly.

### A012-09 — Local transport decision — Valid in ARCH-012, conflicting ARCH-008

ARCH-012 correctly leaves concrete `IPC/localhost mechanism` to a separate implementation decision. ARCH-008 should be changed, not ARCH-012, unless the project intentionally adopts HTTP via a new ADR.

### A012-10 — Tool boundary — Valid with command-port clarification

The existing rule that AI only proposes tool calls and deterministic handlers validate/execute them is sound. Revised architecture should say those handlers invoke explicit application/domain command ports; AI cannot access concrete modules, Repository, Game Adapter implementation or process/file APIs directly.

### 4.1 Decision

ARCH-012 should be revised to v1.1 Audited, primarily correcting consumer scope, Adapter/Context ordering, startup/readiness scope and game-output routing. Its core security/provider abstraction model should be retained.

## 5. Impact on the architecture foundation candidate

This review does **not** introduce another fundamental reversal like AUDIT-005. It strengthens the current target:

- explicit synchronous use-case ports remain necessary;
- Core is a coordinator and may consume narrow application use-case ports, but is not a generic AI consumer;
- Save Manager command port is valid for explicit save/load orchestration;
- autosave remains event-triggered from Scheduler;
- domain normal persistence and Save/Checkpoint artifacts are separate concerns;
- AI Adapter is the only application AI entry point;
- Game Adapter implements integration ports and never owns domain/save/AI policy.

The stale first DOCX candidate set still requires regeneration according to AUDIT-005, but no additional new dependency category is required after AUDIT-006.

## 6. Next action

1. Re-audit ADR-001–007 specifically against AUDIT-002/AUDIT-005/AUDIT-006.
2. Resolve which ADRs need clarification/addendum vs full superseding decision.
3. Regenerate ADR-008 and ARCH-001–005 candidates once the ADR pass confirms no additional foundation change.
4. Rebuild ARCH-010 correctly and prepare targeted v1.1 revisions for ARCH-008/009/011/012.
5. Redesign ARCH-006 and revise ARCH-007 sequences only after the textual dependency foundation is stable.
6. Re-audit MOD-001 last.

MOD-002 remains blocked.
