# AUDIT-001 — Architecture Consistency Audit

**Project:** World Engine  
**Audit baseline:** `main@33129be20cf1d924b4c79ecbcd6ba7e5174591e6`  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** In progress — Phase 1 findings  
**Date:** 2026-09-01

## 1. Purpose

This audit checks the current World Engine architecture documentation as one system rather than as independent files. The goal is to detect contradictory ownership rules, dependency directions, runtime-flow ambiguities, stale drafts, malformed merged documents, layer inconsistencies and downstream conflicts with `MOD-001 Core` before more module specifications are created.

No architecture document is considered correct merely because it is newer or already committed. Conflicts are recorded first and resolved through explicit architecture changes.

## 2. Scope

Current audit scope:

- `ARCH-001` Overall System Architecture
- `ARCH-002` Module Architecture
- `ARCH-003` Data Flow
- `ARCH-004` Architecture Principles & Dependency Rules
- `ARCH-005` Module Dependencies
- `ARCH-006` Component Diagram
- `ARCH-007` Sequence Diagrams
- `ARCH-008` Deployment Architecture
- `ARCH-009` Configuration Architecture
- `ARCH-010` Plugin Architecture
- `ARCH-011` Save System Architecture
- `ARCH-012` AI Integration Architecture
- `ADR-001` through `ADR-007`
- `MOD-001 Core`
- Module Specification Template where it constrains dependency documentation

RU and DE versions are included in structural/parity checks. Semantic audit is performed primarily against the current RU canonical content, with DE parity checked separately.

## 3. Audit Method

The audit distinguishes five concepts that existing documents sometimes mix together:

1. **Static dependency** — compile-time/module-level dependency on a public contract.
2. **Runtime call direction** — who invokes whom during a sequence.
3. **Event dependency** — producer/subscriber relation through Event Engine.
4. **Data flow** — where information moves, independent of ownership.
5. **Ownership** — which subsystem owns state, semantics, lifecycle or policy.

An arrow in a sequence/data-flow diagram must not automatically be interpreted as a static module dependency. Future corrected documents must label arrow semantics explicitly.

### 3.1 Resolution order used during the audit

This is an audit methodology, not a retroactive rewrite of project history:

1. Explicit architecture decision records for the concern being decided.
2. Specialized v1.0 architecture documents for that concern.
3. Module dependency rules and module specifications after conflict resolution.
4. Earlier v0.1 Draft overview documents as historical design input.
5. Diagrams only when their arrow semantics are explicitly defined and consistent with textual rules.

If two authoritative/current documents disagree, the audit does **not** silently choose one. It records an upstream architecture conflict that must be resolved explicitly.

## 4. Repository Integrity Check

The audited local source copies were checked against Git blob SHA values for the current repository files. The checked copies of ARCH-001–005, ARCH-007–012, ADR-001–007, the module template and MOD-001 match the committed repository blobs used by this audit.

`ARCH-006` was also checked against the current committed image.

## 5. Severity Model

- **Critical** — creates contradictory implementation contracts or permits incompatible implementations.
- **High** — architecture boundary/dependency error that must be corrected before dependent module specifications are frozen.
- **Medium** — misleading/inconsistent wording, metadata or model that can cause implementation drift.
- **Low** — naming/editorial/traceability issue without immediate implementation ambiguity.

## 6. Confirmed Findings — Phase 1

| ID | Severity | Documents | Finding | Status |
|---|---|---|---|---|
| AUD-001 | Critical | ARCH-004, ARCH-005, ARCH-006, ARCH-007, ARCH-011, ADR-001, ADR-007, MOD-001 | Static dependency direction, runtime call direction, event flow and data flow are not consistently distinguished. Several arrows are therefore interpreted differently by different documents. | Open |
| AUD-002 | Critical | ARCH-001 vs ARCH-005/ADRs | ARCH-001 v0.1 lists obsolete direct dependencies: `Core -> Game Adapter/Event Bus`, direct `Memory -> Database`, `Organization/Economy -> Database/Event Bus`, etc. These conflict with later repository/event boundaries. | Confirmed |
| AUD-003 | Critical | ARCH-002 vs ARCH-005 | ARCH-002 module catalog contains an obsolete dependency graph and actual cycles, including Economy/Politics and Organization/Relationship relations that differ from ARCH-005. | Confirmed |
| AUD-004 | High | ARCH-004 | Rule “dependencies always point down architecture layers” is incompatible with actual approved same-layer dependencies and cross-layer consumer relationships. The document also carries internal `Document ID ARCH-001` although its canonical repository ID is ARCH-004. | Confirmed |
| AUD-005 | Critical | ARCH-005 | `Core -> Scheduler` is declared while Scheduler is also documented as depending on `Core timing contracts`, producing a dependency-cycle ambiguity despite cycles being forbidden. Contract ownership is not defined. | Confirmed |
| AUD-006 | Critical | ARCH-005 vs ARCH-011/ADR-001/ADR-003 | ARCH-005 restricts Core direct dependencies to Event Engine and Scheduler, while ARCH-011 explicitly shows `Core -> Save Manager` save/load requests and ADR-001/ADR-003 describe Core use of approved persistence/Repository contracts. The intended static dependency model is unresolved. | Open architecture decision |
| AUD-007 | Critical | ARCH-005/MOD-001 vs ARCH-012/ADR-001 | Current MOD-001 forbids direct `Core -> AI Adapter`, but ARCH-012 explicitly defines `Core -> AI Adapter` as an internal trusted boundary and ADR-001 says AI requests go through AI Adapter. | Open architecture decision |
| AUD-008 | Critical | ARCH-005/MOD-001 vs ADR-007/ARCH-007 | Current dependency model treats Game Adapter primarily as `Game Adapter -> Core`, while ADR-007 states that Game Adapter exposes a stable contract Core can use. Sequence diagrams include bidirectional runtime behavior. Static port ownership is not specified. | Open architecture decision |
| AUD-009 | High | ARCH-006 | Current component diagram states “All modules communicate through the Event Engine”, contradicting ADR-001/ADR-002 hybrid direct-contract + event model. Diagram also contains a `services registry` in Core, conflicting with explicit-DI/no-service-locator direction. | Confirmed |
| AUD-010 | High | ARCH-006 | Component diagram places Save Manager inside the Simulation area while ARCH-002/004/005 classify it as Persistence. Dialogue/Voice layer placement also differs across architecture documents. | Confirmed |
| AUD-011 | High | ARCH-006 vs ARCH-005 | Diagram visually connects Scheduler and Database/persistence paths in a way that can be read as a forbidden direct dependency. Arrow semantics are not sufficient to disambiguate this. | Confirmed ambiguity |
| AUD-012 | Critical | ARCH-007 | SD-001 directly shows `Dialogue Engine -> Game Adapter`, although ARCH-005 forbids Dialogue direct Game API/integration dependency. | Confirmed |
| AUD-013 | Critical | ARCH-007 | SD-007 includes `Game Adapter -> Core/Dialogue Engine`, allowing Game Adapter to bypass Core and call Dialogue directly. | Confirmed |
| AUD-014 | High | ARCH-007, ARCH-011, ARCH-005 | Save/load sequences use `Core -> Save Manager`; autosave architecture also uses `Scheduler -> Save Manager`. Neither relation is represented consistently in the canonical dependency matrix. | Confirmed inconsistency |
| AUD-015 | High | ARCH-007, ARCH-010 | Plugin detection sequence starts via `Core/Scheduler -> Plugin Detection Subsystem`, while Plugin Detector/Analyzer are defined as internal components rather than top-level modules. Host/composition ownership for this subsystem is not defined. | Open |
| AUD-016 | Medium | ARCH-008 | ASCII process diagram visually nests Event Engine, Scheduler, Simulation, Dialogue, AI Adapter and Save Manager/Repository under “Core”, while surrounding text says they are logical components of the World Engine process. This can incorrectly imply Core ownership. | Confirmed ambiguity |
| AUD-017 | Medium | ARCH-009, ARCH-008, MOD-001 | `Ready`, `Degraded`, `Startup Failed`, subsystem readiness and application readiness are used at different scopes. Some text says Core is ready while application is degraded; other text models Ready/Degraded as mutually exclusive Core states. Scope must be explicit. | Open |
| AUD-018 | Critical | ARCH-010 RU+DE | Complete ARCH-010 files are malformed: Part 01 is included twice and Part 02 is missing. Both RU and DE have the same merge defect. | Confirmed |
| AUD-019 | High | ARCH-010 | The actual Part 02 source exists separately, so ARCH-010 must be rebuilt before it can remain a canonical Complete document. | Confirmed |
| AUD-020 | High | ARCH-002/004/005/006/012 | Layer assignment is inconsistent. Dialogue/Voice are classified as AI in older layer docs, while the current component diagram places them in Simulation and ARCH-012 treats Dialogue primarily as an AI consumer behind AI Adapter. | Open architecture decision |
| AUD-021 | High | ADR-001, ADR-003, ARCH-005, MOD-001 | Repository/persistence relationship to Core is inconsistent: ADR-001 says Core uses approved Repository/persistence contracts; ADR-003 exposes high-level persistence contracts/status to Core; MOD-001 currently forbids Core -> Repository entirely. | Open architecture decision |
| AUD-022 | Medium | ARCH-001–006 | Canonical repository IDs and internal document IDs are inconsistent (`ARCH-004` contains `ARCH-001`; ARCH-002 contains `WE-ARC-005`; ARCH-003 uses an older WE-DF identifier; ARCH-006 image uses `CD-001`). | Confirmed |
| AUD-023 | High | ARCH-001–005 vs later ADR/ARCH | Early v0.1 Draft documents remain in canonical locations without a clear “superseded/revised by” marker even where later authoritative decisions contradict them. | Confirmed |
| AUD-024 | High | MOD-001 | MOD-001 v1.1 was audited against an incomplete interpretation of dependency rules. Its prohibitions on Core -> Save Manager, Core -> AI Adapter and outbound Game Adapter relation must be revalidated after upstream architecture conflicts are resolved. | Confirmed re-audit required |

## 7. Detailed Notes on Highest-Risk Conflicts

### 7.1 Dependency semantics are the root problem

Current documents frequently use one arrow notation for multiple meanings. This is the primary reason the same architecture can appear to both allow and forbid a connection.

Example:

- A sequence may show Core invoking a Game Adapter contract at runtime.
- The static dependency may still point from the adapter implementation toward a port/interface owned by Core or a neutral Contracts package.
- Without specifying **contract ownership**, both statements look contradictory.

The corrected architecture must explicitly document:

- contract/interface owner;
- compile-time dependency direction;
- runtime caller/callee direction;
- event producer/subscriber relation;
- data ownership.

### 7.2 Core / Save Manager requires an explicit decision

Current sources support two incompatible readings:

- ARCH-005 and current MOD-001: Core direct module dependencies are Event Engine and Scheduler only.
- ARCH-007/011: Core directly initiates Save Manager save/load operations.
- ADR-001: Save remains outside Core, but Core participates in orchestration and approved persistence contracts.

A likely clean target is a narrow `Core -> Save Manager command/orchestration contract` while Save Manager retains all save semantics and storage ownership. This is a **proposal**, not yet an adopted correction.

### 7.3 Core / AI Adapter requires an explicit decision

ARCH-012 explicitly defines `Core -> AI Adapter` as an internal trusted boundary, while current MOD-001 forbids that dependency.

The audit must decide whether:

- Core is allowed a narrow AI capability contract; or
- only selected domain/application modules may call AI Adapter and ARCH-012 must be changed.

No silent choice will be made.

### 7.4 Game Adapter requires port ownership clarification

ADR-007 states that Core can use a stable Game Adapter contract while ARCH-005 expresses `Game Adapter -> Core coordination interfaces`.

This may be resolved through dependency inversion/ports-and-adapters, but the current documents do not define where the game port contract is owned. The audit will make this explicit before changing arrows.

### 7.5 ARCH-010 is not structurally valid as a Complete document

Both current language versions contain:

- Part 01
- Part 01 again
- Part 03
- Part 04
- Part 05
- Part 06
- Part 07
- Part 08

The separate Part 02 source exists and should replace the duplicated Part 01 during repair.

## 8. Correction Order

Corrections should be performed in this order to avoid repeatedly rewriting downstream files:

1. **Define dependency/contract semantics** in ARCH-004/ARCH-005.
2. **Resolve the open Core boundaries**: Game Adapter, Save Manager/Repository, AI Adapter, Scheduler contract ownership.
3. **Revise ARCH-004 and ARCH-005 first** so they become the single dependency model.
4. **Revise ARCH-001, ARCH-002 and ARCH-003** to match the resolved model and mark/remove stale v0.1 rules.
5. **Replace ARCH-006 diagram** with a diagram using explicit arrow types and corrected layers.
6. **Revise ARCH-007 sequences** to distinguish runtime calls from static dependencies and remove actual boundary bypasses.
7. **Clarify ARCH-008/009 lifecycle/readiness scope** and process-vs-Core ownership.
8. **Rebuild ARCH-010 RU+DE** with the real Part 02.
9. **Reconcile ARCH-011 and ARCH-012** with the final dependency model.
10. **Re-audit ADR-001–007** against the corrected architecture; ADR changes require explicit amendment/superseding language rather than silent edits to decisions.
11. **Revise MOD-001** only after upstream architecture is stable.
12. Run **RU/DE parity audit**, DOCX render QA and repository tree audit.
13. Merge corrections through reviewable commits/PR rather than editing `main` directly during the audit.

## 9. Commit Strategy for the Audit Branch

Planned logical commits:

1. `docs: add architecture consistency audit baseline`
2. `docs: clarify architecture dependency semantics`
3. `docs: align core architecture dependency model`
4. `docs: revise architecture overview and module model`
5. `docs: revise data flow and sequence contracts`
6. `docs: replace component diagram after architecture audit`
7. `docs: repair ARCH-010 plugin architecture merge`
8. `docs: align save and AI architecture boundaries`
9. `docs: amend affected ADR traceability and clarifications`
10. `docs: revise MOD-001 after architecture audit`
11. `docs: complete architecture audit and parity review`

Commit boundaries may be split further if one document requires an independent review.

## 10. Current Audit Gate

**Do not create MOD-002 or later module specifications yet.**

The next safe action is to resolve AUD-001, AUD-005, AUD-006, AUD-007, AUD-008 and AUD-021 and then revise ARCH-004/ARCH-005. All other corrections depend on that foundation.
