# AUDIT-027 — ADR-005 v1.2 Binary Remediation QA

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Base remote HEAD:** `76bbeb4e385b31b6ae4640b637341940cdb846ae`  
**Gate recorded:** 2026-09-04  
**Status:** PASS — corrected ADR-005 v1.2 RU/DE binaries are eligible to replace the truncated audit-branch objects at the same paths

## 1. Defect and scope

The final remote inventory discovered that the two ADR-005 v1.2 files already present on the audit branch were not valid complete DOCX packages. Both remote objects were only 7,506 bytes and ZIP traversal reported truncated input; `python-docx` could not open either file.

| Remote path language | Defective Git blob | Defective bytes | Result |
|---|---|---:|---|
| RU | `74d6630d174f5a1f10a0346f7deab29cd2d5a097` | 7,506 | Truncated / invalid DOCX |
| DE | `bba64bfdb247fda3f0b6b0aaa23575a64e73f1db` | 7,506 | Truncated / invalid DOCX |

This gate rebuilds only the two ADR-005 v1.2 amendment binaries. The original ADR-005 RU/DE v1 Complete decision records remain unchanged historical artifacts. No source, audit report or earlier hash is treated as proof that the defective uploaded objects were complete.

## 2. Corrected artifact manifest

| Artifact | Pages | Paragraphs | Tables | Rows | Bytes | SHA-256 | Git blob |
|---|---:|---:|---:|---:|---:|---|---|
| `ADR-005_Save_System_Amendment_RU_v1_2_Proposed_Audited.docx` | 2 | 34 | 2 | 10 | 39,706 | `56b3443bd79e6dbc320e7e83cd596f5565eb85179f53ac7ac8144a75c40a4b95` | `83860b1da67eb30f474d6bc091046407059e7834` |
| `ADR-005_Save_System_Amendment_DE_v1_2_Proposed_Audited.docx` | 2 | 34 | 2 | 10 | 39,447 | `84cd3d9711d55d2ef4b19573103ab3651953606826377ac1346b31cc01bbd503` | `d5787f0c4ae884da866ef6cf2c09f49f7b79ac29` |

The RU and DE artifacts have equal paragraph, table, row and page counts. Metadata identifies ADR-005, amendment v1.2, the correct language, audit status and 2026-09-04 gate date.

## 3. Semantic contract verification

Both corrected files contain the same required Save/Restore model:

- Scheduler emits `AutosaveDue`; Event Engine carries the semantic event; Save Manager owns the save use case;
- a restore candidate is deserialized as **NON-LIVE** state;
- every participant completes `StageRestore` before activation;
- cross-participant validation precedes an **ATOMIC LOGICAL ACTIVATION BARRIER**;
- success uses `CommitActivation` for the full set;
- failure uses `AbortActivation` / `RollbackActivation`; rollback failure enters `RecoveryRequired` instead of exposing a partially activated live world;
- persistence access remains behind the repository contract.

These rules are consistent with the audited ARCH-007, ARCH-011, ADR-008 and MOD-001 v1.2 artifacts.

## 4. Structural and package QA

Both corrected packages passed:

- successful ZIP and `python-docx` traversal;
- exact expected document properties and language identity;
- one A4 portrait section;
- ten valid Heading 1 sections;
- one font family (Arial) under style lint;
- no comments, tracked changes, macros, content controls, text boxes or `customXml` parts;
- no accessibility findings at high, medium or low severity.

## 5. Render QA

The corrected DOCX files were exported through Microsoft Word after the final build and inspected page by page:

- pages inspected: 4 / 4;
- actual page counts: RU 2, DE 2;
- tagged PDF exports: 2 / 2;
- clipping, overlap, missing glyphs, broken tables, footer collision or page-order defects: 0.

## 6. Gate result

**ADR-005 binary remediation gate: PASS.** The manifest objects are approved to replace the defective objects at the same audit-branch paths. `AUDIT-013` remains semantic history; this report is the controlling binary-integrity evidence for ADR-005 v1.2.

This correction is limited to `audit/architecture-consistency-2026-09-01`. `main` remains unchanged at `33129be20cf1d924b4c79ecbcd6ba7e5174591e6`.
