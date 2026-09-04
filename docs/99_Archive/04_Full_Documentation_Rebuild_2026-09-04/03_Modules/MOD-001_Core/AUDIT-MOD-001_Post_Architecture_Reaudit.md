# AUDIT-MOD-001 — Post-Architecture Re-audit of MOD-001 Core

**Project:** World Engine  
**Audited artifact:** `MOD-001_Core_RU/DE_v1_1_Audited_Complete.docx`  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Baseline:** AUDIT-008/009/010 + ADR-008 correction model + corrected ARCH-004/005/007–012 sources  
**Status:** Complete — current MOD-001 v1.1 is not implementation-ready and requires v1.2 regeneration  
**Date:** 2026-09-01

## 1. Result summary

The current MOD-001 v1.1 was a major improvement over the first Core specification, but it was audited against an intermediate dependency interpretation that has since been superseded by the repository-wide architecture audit.

The document is therefore **historically valid as an audit-stage artifact but no longer correct as the final implementation contract**.

Primary reason: v1.1 overcorrected the old God-Core risk by restricting Core too aggressively to only Event Engine and Scheduler. The stabilized architecture now distinguishes concrete module dependencies from explicit application/use-case ports and inverted integration ports.

## 2. Severity summary

| Finding | Severity | Required action |
|---|---:|---|
| Core outbound dependencies limited to Event Engine + Scheduler only | Critical | Replace with explicit contract/port model including Save command, Dialogue/use-case and Game Output/Capability ports where Core owns orchestration. |
| `Core -> Save Manager` categorically forbidden | Critical | Permit narrow Save Manager command/use-case contract; keep storage/save semantics owned by Save Manager. |
| `Core -> Game Adapter` ban conflates concrete adapter with outbound port | Critical | Continue forbidding concrete Game Adapter dependency, but add Core/Application -> Game Output/Capability Port implemented by Game Adapter. |
| Core has no route to synchronous Dialogue workflow | Critical | Add explicit Dialogue Command/Use-Case Port; Event Engine must not be used as RPC. |
| Application lifecycle/readiness is attributed to Core | High | Move process-level construction/startup/shutdown/readiness aggregation to Application Host / Composition Root. |
| Persistence prohibition is too broad | High | Keep generic Repository/DB/CRUD forbidden; allow only a narrow Core-owned persistence/status port if a concrete Core responsibility requires it. |
| Core Degraded/Application Degraded scopes are mixed | High | Separate Core coordination state from application-level Ready/Degraded/Recovery outcomes. |
| Scheduler ambiguity remains recorded as open question | High | Close it: `Core -> Scheduler`; Scheduler publishes triggers through Event Engine and has no reverse Core dependency. |
| MOD-001 diagrams/acceptance checks encode old dependency graph | High | Regenerate Part 03/05/06/08/09 and any affected overview/requirements text. |
| Core generic AI dependency is forbidden | Correct | Keep. AI consumers use AI Adapter; Core receives scoped readiness/capability evidence unless a future explicit Core-owned AI use case is approved. |
| Service Locator/global registry prohibition | Correct | Keep unchanged. |

## 3. Current statements that must be superseded

The current v1.1 repeatedly states variants of:

- only Event Engine and Scheduler are Core's direct outbound dependencies;
- Core -> Save Manager/Repository/Database is absent/forbidden;
- Core -> Game Adapter is absent and there is no outbound game dependency;
- application persistence/Game/AI readiness participates only as external evidence;
- Core owns application lifecycle/readiness gates;
- Scheduler dependency ambiguity remains unresolved.

These statements are no longer sufficient or, in several cases, are now wrong under the corrected contract-ownership model.

## 4. Corrected Core contract model

### 4.1 Concrete implementation dependencies forbidden

Core must not depend on:

- concrete `Game Adapter` implementation or game SDK/API types;
- physical `Database` / SQLite / SQL APIs;
- arbitrary domain repositories or generic CRUD;
- concrete AI provider/model SDK;
- Plugin Detector internals;
- concrete simulation/domain module internals;
- global Service Locator/module lookup registry.

### 4.2 Explicit Core-consumed contracts permitted

Core may consume only explicit contracts required by Core-owned coordination/use cases, including:

- Event Engine public contract;
- Scheduler control contract;
- Dialogue Command/Use-Case Port for synchronous conversation orchestration;
- Save Manager command/use-case contract for explicit save/load/shutdown workflows;
- Game Output/Capability Port for approved game-facing reads/actions/capability requests;
- additional narrowly defined synchronous application use-case ports only when a documented Core orchestration use case requires an immediate result;
- a narrow Core-owned persistence/status contract only if a concrete Core responsibility proves it necessary.

Port consumption does not grant access to concrete implementation internals.

## 5. Game Adapter correction

Current v1.1 correctly forbids Core importing/calling the **concrete Game Adapter implementation**, but incorrectly treats that as "no outbound game dependency of any kind".

Correct model:

### Inbound

`Game Runtime -> Game Adapter -> Game Ingress Port -> Core/Application`

### Outbound

`Core/Application -> Game Output/Capability Port <- Game Adapter implementation -> Game Runtime`

Therefore:

- `Core -> concrete Game Adapter` remains forbidden;
- `Core/Application -> Game Output/Capability Port` is required for approved game-facing output;
- bidirectional runtime flow does not create a static concrete implementation cycle.

## 6. Dialogue/use-case correction

Conversation is a synchronous request/result workflow.

Canonical path:

`Game Adapter -> Game Ingress -> Core -> Dialogue Command Port -> Dialogue Engine`

and then:

`Dialogue result -> Core -> Game Output/Capability Port -> Game Adapter -> Game`

Consequences for MOD-001:

- Core requires a narrow Dialogue/use-case contract for this workflow;
- Event Engine may publish `ConversationStarted/Completed` semantic events, but is not the RPC transport;
- Core does not own Dialogue domain logic;
- Dialogue implementation remains replaceable behind the port.

This directly changes the old `ACOMP-001` rule that only Event Engine and Scheduler may appear as outbound Core edges.

## 7. Save System correction

The categorical v1.1 ban on `Core -> Save Manager` is superseded.

Correct rule:

`Core/Application Save/Load Use Case -> Save Manager Command Contract`

Core may:

- initiate explicit manual/application Save/Load workflows;
- receive typed Save/Load results;
- participate in shutdown save barrier coordination through the Save use-case contract.

Core may not:

- implement Save semantics;
- inspect SaveParticipant private state;
- access Save Repository/SQLite directly;
- own autosave retry/storage policy.

Autosave remains:

`Scheduler -> Event Engine : AutosaveDue -> Save Manager subscriber`

## 8. Persistence correction

Current MOD-001 correctly rejects physical DB access and generic repository usage, but its wording is too broad where it forbids every possible persistence/status port.

Final wording should be:

- no Core -> physical Database/SQLite/SQL;
- no Core -> generic domain repositories/CRUD;
- no Core ownership of Save persistence;
- a **narrow Core-owned persistence/status port** is architecturally permitted only if a specific Core responsibility cannot be satisfied by Host/readiness evidence or another owner contract.

Default implementation should prefer zero direct Core persistence dependency unless justified.

## 9. AI correction

Current MOD-001 direction remains mostly correct here.

Final rules:

- Core is not a generic AI consumer in v1.0;
- Dialogue is the primary AI consumer;
- approved consumers call AI Adapter / AI Capability Port;
- Core may receive scoped AI readiness/capability evidence;
- Core does not start/stop provider processes;
- provider lifecycle is Host-owned;
- any future Core-owned AI use case requires an explicit architecture/module decision.

The existing direct-provider/secret/AI-authority prohibitions should remain.

## 10. Application Host vs Core lifecycle

This is the second largest correction after dependency ports.

Application Host / Composition Root owns:

- component construction;
- dependency injection/binding;
- configuration/profile selection;
- Game Adapter construction;
- Local AI/provider process wiring;
- top-level startup order;
- top-level shutdown order;
- application readiness aggregation;
- process-level recovery decision.

Core owns:

- Core coordination lifecycle;
- Core-owned orchestration gates;
- lifecycle/use-case coordination through its explicit ports;
- Core state/invariants;
- Core diagnostics/readiness evidence.

MOD-001 Parts 01, 02, 05 and 06 currently assign too much application-level lifecycle/readiness policy to Core and must be rewritten accordingly.

## 11. Readiness/state correction

The following scopes must be independent:

- Core lifecycle state;
- Core readiness;
- Event Engine/Scheduler readiness;
- Game integration capability/readiness;
- AI capability/provider health;
- Persistence/Save readiness;
- application startup outcome.

`ApplicationDegraded` must not automatically be represented as `CoreDegraded`.

Recommended MOD-001 approach:

- keep Core lifecycle states focused on Core-owned coordination (`Created`, `Starting`, `Ready`, `Stopping`, `Stopped`, failure/recovery states as explicitly justified);
- use `Degraded` for Core only if a clearly defined optional **Core-owned** coordination capability can be unavailable while Core remains ready enough to serve its contract;
- application `Ready/Degraded/Startup Failed/Recovery Required` belongs to Host/application policy.

This should be decided explicitly during MOD-001 v1.2 regeneration instead of carrying the old mixed model forward.

## 12. Scheduler question is resolved

The former OQ about `Core -> Scheduler` vs `Scheduler -> Core timing contracts` is closed by the audited baseline.

Final static relation:

`Core -> Scheduler control contract`

Scheduler does not depend statically on Core.

Scheduled occurrences are emitted through explicit scheduler/event contracts, e.g.:

`Scheduler -> Event Engine : AutosaveDue`

MOD-001 must remove the open-question wording and corresponding uncertainty from requirements, testing and governance sections.

## 13. Parts requiring changes

| Part | Impact | Required correction |
|---|---:|---|
| Part 01 Overview | High | Move application lifecycle ownership to Host; expand contract model beyond Event/Scheduler; clarify concrete adapter vs outbound port. |
| Part 02 Requirements | Critical | Rewrite FR-CORE-002/003/005/008/013/014/019 and affected NFR/traceability. |
| Part 03 Dependencies & Interfaces | Critical | Replace old dependency table; add Dialogue Command Port, Save command contract, Game Output/Capability Port; nuance persistence port. |
| Part 04 Internal Components | Medium/High | Ensure orchestrators store only explicit injected ports; no pseudo-registry; adapt readiness evidence scope. |
| Part 05 Lifecycle | Critical | Separate Host application startup/shutdown from Core lifecycle; add explicit Save/Dialogue/Game-output use-case paths. |
| Part 06 Config/Health/Readiness | Critical | Host distributes typed config and aggregates application readiness; Core consumes Core view and emits Core evidence. |
| Part 07 Performance/Security/Concurrency | Medium | Mostly reusable; update timeout/cancellation owners for new use-case ports and Host boundaries. |
| Part 08 Testing/Acceptance | Critical | Replace architecture conformance assertions based on “only Event/Scheduler”; add port-binding and no-concrete-dependency tests. |
| Part 09 Diagrams/Governance | Critical | Replace direct dependency/sequence diagram requirements; remove old no-Core->Save/no-output-game rules; close Scheduler OQ. |

## 14. Revised architecture-conformance tests for MOD-001

The v1.2 test suite should prove at minimum:

1. Core implementation has no import/reference to concrete Game Adapter implementation or game SDK.
2. Game ingress reaches Core through the defined ingress contract.
3. Core can invoke Game Output/Capability Port without importing adapter implementation.
4. Core can coordinate conversation through Dialogue Command Port without importing Dialogue implementation internals.
5. Core can issue Save/Load command through Save Manager contract without accessing Repository/Database.
6. Autosave does not create Scheduler->Save Manager or Scheduler->Core static dependency.
7. Core does not become a generic AI consumer.
8. Host owns component construction/startup/readiness aggregation.
9. No Service Locator/global module registry exists.
10. Static dependency graph remains acyclic.
11. Event Engine is not used as RPC for synchronous dialogue/save query results.
12. Core/Application readiness scopes remain distinct.

## 15. Versioning recommendation

Do not overwrite the current v1.1 file silently.

Create:

- `MOD-001_Core_RU_v1_2_Architecture_Audited_Complete.docx`
- `MOD-001_Core_DE_v1_2_Architecture_Audited_Complete.docx`

The v1.2 change log should explicitly state that repository-wide architecture audit changed:

- dependency semantics;
- synchronous use-case port allowance;
- Game Adapter port ownership;
- Save Manager command boundary;
- Host/Core lifecycle scope;
- readiness scope;
- Scheduler cycle resolution.

## 16. Gate

MOD-001 v1.1 must not be treated as the final implementation contract.

The next safe module action is to regenerate MOD-001 v1.2 only after ARCH-007–012 audited artifacts are formally regenerated/QA-checked, then run a final cross-document parity and cycle validation.

MOD-002 remains blocked until MOD-001 v1.2 passes that gate.
