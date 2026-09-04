# AUDIT-010 — Contract Ownership Freeze and Candidate QA

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Contract ownership frozen for regeneration; ARCH-001–005 v1.2 candidate QA completed  
**Date:** 2026-09-01

## 1. Purpose

This checkpoint closes the remaining contract-ownership questions from AUDIT-009 and records the render/semantic QA result of the first v1.2 regenerated ARCH-001–ARCH-005 candidates.

The purpose is to prevent the audit from repeatedly redesigning the same dependency model while also preventing visually clean but linguistically inconsistent draft documents from being promoted as canonical.

## 2. Contract ownership decisions frozen for the next regeneration

### 2.1 Dialogue / Voice

Canonical model:

- Dialogue owns the decision that optional speech output is required for a dialogue response.
- Dialogue depends on a narrow `Speech Output Port` contract.
- Voice Engine implements the `Speech Output Port`.
- Runtime direction may be `Dialogue -> Voice`, but Dialogue does not import concrete Voice implementation types.
- Voice does not depend on Dialogue internals and does not own dialogue semantics.

This removes the legacy ambiguity created by older `Voice -> Dialogue` dependency wording.

### 2.2 Context Manager / Memory

Canonical model:

- Context Manager may retrieve approved memory/knowledge only through a narrow `Memory Read / Context Source` contract.
- Memory Engine owns memory retrieval semantics and implements/provides that contract.
- Context Manager does not access Memory internals, Repository or SQLite directly.
- AI Adapter remains the consumer-facing AI entry boundary.

### 2.3 Save participation

Canonical model:

- Save Manager depends on a collection of explicit `SaveParticipant` abstractions.
- State-owning modules implement participation contracts for their own state.
- Application Host / Composition Root injects participant implementations explicitly.
- No runtime registry scanning, Service Locator or concrete SaveManager-to-every-domain-module dependency is allowed.
- Save Manager coordinates save/restore semantics but does not take ownership of participant domain state.

### 2.4 Configuration

Canonical model:

- Configuration subsystem loads/parses/validates/normalizes configuration.
- Application Host / Composition Root selects the effective profile and provides each component only its typed configuration view.
- Typed configuration views are immutable by default.
- No mutable global Config singleton is allowed.
- Controlled runtime reload, if enabled, uses explicit ARCH-009 contracts/events and does not turn Core into a configuration gateway.

## 3. Static graph result after contract freeze

The graph from AUDIT-009 remains acyclic after these four decisions. No new concrete implementation edge is required that introduces a cycle.

In particular:

- `Dialogue -> Speech Output Port <- Voice Engine` is a port binding, not a concrete Dialogue/Voice cycle.
- `Context Manager -> Memory Context Source Port <- Memory Engine` is a read-only port relation.
- `Save Manager -> SaveParticipant abstractions <- state owners` is dependency inversion with Composition Root wiring.
- typed configuration injection is lifecycle wiring, not a global module dependency.

## 4. ARCH-001–ARCH-005 v1.2 candidate QA result

The first regenerated v1.2 candidates are architecturally much closer to the resolved baseline and their page layout renders cleanly. However they are **not eligible for canonical promotion** for one important reason: language localization is incomplete.

### 4.1 Confirmed localization defect

RU candidates contain substantial English prose/headings/table labels beyond unavoidable technical identifiers. Examples include:

- `Overall System Architecture`, `Layers and components`, `Core role`;
- `Module catalog`, `Ownership rules`, `Communication rules`;
- almost all ARCH-003 flow headings and table content;
- ARCH-004/005 section titles and explanatory text.

DE candidates likewise retain an unnecessary amount of English explanatory text and table labels.

Technical identifiers such as `Core`, `Event Engine`, `Scheduler`, `Game Adapter`, port names and event names may remain English. Explanatory prose, headings and ordinary table labels must be localized.

### 4.2 QA classification

| Candidate | Architecture semantics | Layout | RU/DE localization | Promotion status |
|---|---|---|---|---|
| ARCH-001 v1.2 RU/DE | PASS with current baseline | PASS | FAIL | Regenerate |
| ARCH-002 v1.2 RU/DE | PASS with current baseline | PASS | FAIL | Regenerate |
| ARCH-003 v1.2 RU/DE | PASS with current baseline | PASS | FAIL | Regenerate |
| ARCH-004 v1.2 RU/DE | PASS after contract freeze | PASS | FAIL | Regenerate |
| ARCH-005 v1.2 RU/DE | PASS after contract freeze | PASS | FAIL | Regenerate |

The v1.2 candidates are therefore useful as audited semantic source material, but they are not final documentation artifacts.

## 5. Promotion requirements for regenerated ARCH-001–005

Before any replacement is uploaded as canonical/audited documentation, each RU/DE pair must pass all of the following:

1. semantic alignment with ADR-008 and accepted/proposed ADR amendments;
2. complete distinction of static dependency, runtime call, event relation, data flow, ownership and lifecycle wiring;
3. dependency graph cycle check;
4. contract owner identified for every port/dependency in ARCH-005;
5. no Service Locator/global registry wording;
6. RU explanatory text fully Russian except technical identifiers;
7. DE explanatory text fully German except technical identifiers;
8. RU/DE semantic parity review;
9. complete DOCX render and page-by-page visual QA;
10. traceability to audit findings and superseded legacy material.

## 6. Next correction step

The next safe action is to regenerate ARCH-001–ARCH-005 as fully localized audited candidates using this frozen contract model, render them, verify RU/DE parity, and only then add them to the audit branch as binary review artifacts.

ARCH-006 and later architecture corrections remain blocked from canonical regeneration until ARCH-004/ARCH-005 pass this promotion gate, because diagrams and sequences must consume the same dependency semantics.

**MOD-002 remains blocked.**
