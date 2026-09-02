# AUDIT-MOD-001-v1.2 — Source Review After Architecture Final QA

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Reviewed source:** `MOD-001_Core_Architecture_Audited_Source_v1_2.md`  
**Baseline:** AUDIT-014 + ARCH-006 v1.2 + ARCH-007 v4 + ARCH-008/009/011/012 audited models + prior AUDIT-MOD-001 findings  
**Date:** 2026-09-02  
**Status:** PASS WITH REQUIRED CLARIFICATIONS before RU/DE binary generation

## 1. Summary

The v1.2 Core source is materially aligned with the stabilized architecture and fixes the major defects of MOD-001 v1.1:

- Core is no longer restricted to only Event Engine + Scheduler;
- explicit synchronous use-case ports are allowed;
- Save Manager command boundary is present;
- Game Output/Capability Port is present while concrete Game Adapter dependency remains forbidden;
- Host/Composition Root owns process-level construction/startup/shutdown/readiness aggregation;
- Scheduler reverse-dependency ambiguity is closed;
- Event Engine is not used as generic RPC;
- Core remains non-generic AI consumer;
- Service Locator/global registry remains forbidden;
- Save/Restore is aligned with the audited atomic-activation model.

The source is therefore structurally fit for final regeneration, but six clarifications are required before it becomes the final implementation contract.

## 2. Finding C1 — Outbound port ownership must be explicit

**Severity:** High

The source names `Dialogue Command/Use-Case Port` and `Game Output/Capability Port`, but the contract-ownership direction is not explicit enough.

To avoid reintroducing dependency-cycle ambiguity, the final specification must state:

- Core/Application **owns or consumes an application-facing outbound port abstraction** for synchronous Dialogue coordination;
- Dialogue implementation satisfies/implements that port according to the final language/framework binding;
- Core/Application owns the Game Output/Capability abstraction required by the use case;
- concrete Game Adapter implementation satisfies that outbound integration port;
- runtime call direction does not imply Core depends on concrete implementations.

Recommended canonical wording:

```text
Core/Application -> Dialogue Use-Case Port <- Dialogue implementation
Core/Application -> Game Output/Capability Port <- Game Adapter implementation
```

The exact source-code package ownership can be finalized in API/implementation design, but the architectural dependency inversion is fixed now.

## 3. Finding C2 — Requiredness classification of injected contracts

**Severity:** High

Section 5.1 currently labels all listed Core-consumed contracts as mandatory.

This is safe only if the active application mode requires every capability. The architecture already distinguishes application-level required vs optional/mode-dependent capabilities.

Final MOD-001 must classify contracts as one of:

- required for every Core activation;
- required for normal game-attached mode;
- mode-dependent;
- optional capability/evidence.

At minimum:

- Event Engine: required;
- Scheduler: required by current architecture baseline;
- Dialogue use-case port: required for normal gameplay/dialogue mode;
- Save Manager command contract: required for v1.0 normal application baseline unless product policy explicitly creates a no-save mode;
- Game Output/Capability Port: required for game-attached mode, but should not be described as universally required if a future controlled non-game/degraded mode is allowed;
- AI readiness evidence: optional/scoped evidence, not a mandatory Core contract by default.

Application Host owns application-level mode/requiredness policy. Core validates only the contract set required for its active Core mode/profile.

## 4. Finding C3 — Restore activation ownership needs sharper wording

**Severity:** High

The source correctly states that Save Manager owns restore semantics and that Application Host/Core coordination boundary may participate in activation/resume gating.

The final MOD-001 must not imply Core participates in the internal all-or-nothing participant commit barrier.

Required distinction:

- Save Manager owns restore orchestration and SaveParticipant staging/validation;
- Application Host / Activation Gate owns process/application activation coordination where required by ARCH-007;
- state owners own application of their own staged fragments;
- Core receives the typed Load outcome and coordinates approved application/game-facing continuation;
- Core does not call `CommitActivation` on SaveParticipants and does not own rollback mechanics.

This preserves ARCH-007/ARCH-011 ownership exactly.

## 5. Finding C4 — AI readiness evidence should remain opt-in for Core

**Severity:** Medium

The source allows Core to consume scoped AI readiness/capability evidence. This is architecturally permitted, but it should not become a default hidden dependency.

Final rule:

- Application Host owns application-level AI readiness aggregation;
- Dialogue/approved AI consumers own AI use-case behavior/fallback;
- Core receives AI capability evidence only when a documented Core-owned coordination use case genuinely requires it;
- absence of such a use case means Core has no AI readiness dependency.

This prevents a generic `Core -> AI status service` dependency from reappearing indirectly.

## 6. Finding C5 — Event subscription callback ownership must not create a reverse static dependency

**Severity:** Medium

The source allows Scheduler/Event Engine callbacks through defined contracts but does not explicitly state callback/handler inversion.

Final specification must state that:

- Event Engine may deliver events to a Core-owned handler/subscriber contract where Core is an approved subscriber;
- Event publication/subscription is an event relation, not automatically `Event Engine -> Core implementation` static coupling;
- callback interfaces/registration are wired by explicit contracts/composition;
- Core does not expose a generic dynamic dispatcher to Event Engine.

This keeps the static graph acyclic while allowing runtime delivery.

## 7. Finding C6 — Conditional Core persistence/status port requires a stronger gate

**Severity:** Medium

The conditional Core-owned persistence/status port is intentionally an escape hatch, but `documented need` alone is too weak.

Before such a dependency is added, require all of the following:

1. a concrete Core-owned responsibility cannot be satisfied through Host/readiness evidence or an existing owner contract;
2. the port is narrow, read/write scope is explicit and contains no generic CRUD;
3. ownership and migration semantics are defined;
4. ARCH-005 dependency graph is updated and cycle-checked;
5. MOD-001/API traceability is updated;
6. an ADR is created if the change alters an architecture boundary.

Default v1.0 state remains: **no Core persistence dependency**.

## 8. Source sections affected

| Source section | Required change |
|---|---|
| 3 Functional Requirements | Clarify AI evidence opt-in; strengthen conditional persistence gate. |
| 5 Dependencies | Add contract requiredness and explicit port ownership/inversion. |
| 6 Inputs/Outputs | Separate required inputs from optional/mode-dependent evidence. |
| 7 Public Interfaces | State consumer-owned/outbound port inversion and callback semantics. |
| 11 Lifecycle | Validate only active-mode required contract set. |
| 12 Startup | Distinguish Core readiness validation from Host mode/requiredness policy. |
| 14 Recovery | Clarify Core does not own SaveParticipant activation/rollback. |
| 16 Health/Readiness | AI evidence is optional unless Core use case requires it. |
| 23 Testing | Add contract ownership/requiredness/activation-gate tests. |
| 24 Acceptance | Add explicit no-Core-participant-activation and mode-aware dependency criteria. |

## 9. Additional conformance tests to add

The final v1.2 test set should additionally prove:

1. concrete Dialogue and Game Adapter implementations can be replaced without Core import changes;
2. missing mode-optional capability does not invalidate Core readiness unless active Core mode requires it;
3. Application Host, not Core, selects/validates application capability mode;
4. Core never invokes SaveParticipant stage/commit/rollback methods directly;
5. Core can run without an AI readiness dependency when no Core-owned AI-aware workflow is configured;
6. Event delivery to Core does not create a concrete EventEngine -> Core implementation dependency;
7. adding any future Core persistence/status port fails architecture review unless the six-point gate is satisfied.

## 10. Review result

**Semantic result:** PASS WITH REQUIRED CLARIFICATIONS.

No new fundamental architecture redesign is required.

The findings above are local specification-hardening changes and do not change the stabilized dependency graph.

### Next action

1. incorporate C1–C6 into the final MOD-001 regeneration source;
2. generate RU/DE `MOD-001 v1.2 Architecture Audited Complete` binaries;
3. perform RU/DE parity + full render QA;
4. publish corrected architecture binaries that are still pending on the audit branch;
5. rerun dependency-cycle and repository-integrity audit;
6. only then consider the PR to `main` and unblock MOD-002.
