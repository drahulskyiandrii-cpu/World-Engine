# AUDIT-015 — ARCH-011 / ARCH-012 Audited Binary Candidate QA

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Clean RU/DE DOCX candidates regenerated and QA-passed locally; binary publication pending  
**Date:** 2026-09-01

## 1. Scope

This gate regenerates clean source-based RU/DE DOCX candidates for:

- ARCH-011 Save System Architecture;
- ARCH-012 AI Integration Architecture.

The candidates are based on the audited sources and repository-wide correction baseline rather than patching legacy merged objects in place.

## 2. ARCH-011 Save System candidate

Canonical corrections represented in both languages:

- Save Manager owns Save/Restore semantics, not ordinary domain persistence;
- SaveParticipant contracts are explicitly injected by Composition Root;
- manual save is coordinated through Core/Application -> Save Manager command contract;
- autosave trigger is `Scheduler -> Event Engine : AutosaveDue -> Save Manager`;
- Scheduler has no direct Save Manager dependency and owns no save retry policy;
- Game Adapter provides evidence/capabilities but owns no autosave/restore policy;
- Save is an authoritative persisted/restorable checkpoint, not necessarily the newest live runtime state;
- restore is staged outside live state;
- all required participants are validated before activation;
- one logical all-or-nothing activation barrier is mandatory;
- partial restore never reports success;
- rollback/recovery semantics are explicit;
- `LoadCompleted` occurs only after successful activation/post-validation.

### Structural parity

| Metric | RU | DE |
|---|---:|---:|
| Rendered pages | 6 | 6 |
| Heading count | 19 | 19 |
| Table count | 8 | 8 |
| DOCX ZIP integrity | PASS | PASS |

A first render revealed an undesirable page split inside the Restore Phases code block. The heading was moved to a clean page boundary and both documents were re-rendered. The second full visual pass is clean.

### Binary verification

| File | Size | SHA-256 | Expected Git blob SHA |
|---|---:|---|---|
| `ARCH-011_Save_System_Architecture_RU_v1_1_Architecture_Audited_Complete.docx` | 43039 | `9afb21a496b48573f4722ce4ea80417eb954a5bcf596e302ba8eee8f703feb0d` | `26aeab1ac3891d8cf133e050632735703f4d10fd` |
| `ARCH-011_Save_System_Architecture_DE_v1_1_Architecture_Audited_Complete.docx` | 42485 | `48166a9fae74675699d701131d9f9921eecd333b3daca5eafcaf7b0242694ff0` | `a10b89a0c4291c2223f584c045274f00dad038c5` |

## 3. ARCH-012 AI Integration candidate

Canonical corrections represented in both languages:

- AI Adapter / AI Capability Port is the only approved application entry boundary for AI requests;
- Dialogue Engine is the primary v1.0 AI consumer;
- Core is not a generic AI consumer;
- Context Manager remains behind AI Adapter from the consumer perspective;
- Context Manager retrieves memory only through `Memory Read / Context Source Port`;
- no AI component receives direct SQLite/Save/Game/Secret authority;
- Local AI remains a separate optional process;
- local transport is loopback/IPC-neutral and concrete protocol/port is deferred;
- Application Host owns top-level AI lifecycle wiring;
- AI/provider/component/application readiness scopes remain distinct;
- bounded queues/backpressure/deadlines/cancellation are required;
- AI output remains a non-authoritative candidate until deterministic validation;
- game-facing output routes through owning use case -> Core/Application -> Game Output/Capability Port;
- Local -> External fallback re-evaluates privacy/data-classification boundaries.

### Structural parity

| Metric | RU | DE |
|---|---:|---:|
| Rendered pages | 6 | 6 |
| Heading count | 21 | 21 |
| Table count | 9 | 9 |
| DOCX ZIP integrity | PASS | PASS |

All pages of both language versions were visually inspected after rendering. No clipping, overlap, broken table, unreadable code block or page-order issue was found.

### Binary verification

| File | Size | SHA-256 | Expected Git blob SHA |
|---|---:|---|---|
| `ARCH-012_AI_Integration_Architecture_RU_v1_1_Architecture_Audited_Complete.docx` | 42554 | `058b372625289eff4a53a930fcc67d04530a61065b0b5db61d3323d0bbb3c912` | `a85b26a4c79e3a39503a69dbdab4e1a95e062ed8` |
| `ARCH-012_AI_Integration_Architecture_DE_v1_1_Architecture_Audited_Complete.docx` | 42193 | `710051eafe84e123985651599cf4deb1311acd2db5f179e1c89c82fb64c293da` | `d903093cba5516d07498e175c009a9023676db60` |

## 4. Cross-document consistency

The new binary candidates preserve the semantic PASS already recorded by AUDIT-013 and AUDIT-014:

- ARCH-011 restore semantics match ARCH-007 v4;
- ARCH-012 process/readiness semantics match ARCH-008/009 audited sources;
- no Scheduler->SaveManager or Scheduler->Core reverse dependency is introduced;
- no Core-wide AI ownership is introduced;
- no direct SQLite bypass is introduced;
- no Service Locator is introduced;
- the stabilized static dependency graph remains unchanged.

## 5. Gate result

**ARCH-011 clean RU/DE candidate QA: PASS.**  
**ARCH-012 clean RU/DE candidate QA: PASS.**

Remaining repository action is binary publication to the audit branch and verification of the Git blob SHAs above. Until that publication occurs, the legacy Complete DOCX files remain the physical binaries in GitHub and must not be mistaken for the final audited artifacts.
