# AUDIT-019 — Repository Publication and Supersession Plan

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Date:** 2026-09-02  
**Status:** Publication gate defined; `main` remains unchanged

## 1. Purpose

The semantic architecture audit is sufficiently stable that the next risk is no longer conceptual design. The next risk is publishing the **wrong binary generation**, leaving stale legacy files looking canonical, or merging an internally consistent audit source set without the matching RU/DE DOCX artifacts.

This document defines the repository publication, verification and supersession rules before the audit branch becomes reviewable.

## 2. Branch state at this checkpoint

At the start of this publication-planning pass:

- `main` remains at the pre-audit documentation baseline containing MOD-001 v1.1;
- audit branch is ahead of `main` and contains the audit sources/reports;
- audited source files do **not** by themselves make legacy binary DOCX files canonical;
- most corrected binary artifacts are still publication-pending;
- ARCH-006 audited SVG is already committed;
- ADR-005 Save System Amendment v1.2 RU/DE is already present on the audit branch.

No audit correction should be merged to `main` until the binary/canonicalization gates below pass.

## 3. Authority order for selecting publishable artifacts

When multiple audit reports describe different intermediate generations, use the latest applicable gate, not an older local candidate report.

### Architecture foundation

- ARCH-001–005: `AUDIT-011_ARCH-001-005_Regeneration_and_QA.md` identifies the current preferred **v1.4** RU/DE candidates and their verification hashes.
- `AUDIT-013_ARCH-001-005_and_ADR_Final_QA.md` confirms the model remains semantically valid and cycle-free.

### Component / process / configuration / AI

- ARCH-006 / ARCH-008 / ARCH-009 / ARCH-012: use `AUDIT-014_ARCH-006_008_009_012_Final_QA.md` as the latest selection/QA authority.
- Older `AUDIT-012_Final_QA_ARCH-006-008-009-012.md` is retained as audit history and must not override the later selected candidate generation.

### Sequence / Save

- ARCH-007 / ARCH-011: use `AUDIT-015_ARCH-007-011_Final_Artifact_QA.md`.

### Plugin Architecture

- ARCH-010: use `AUDIT-012_ARCH-010_Complete_Rebuild_and_QA.md` plus `ARCH-010_Audit_Rebuild_Map_v1_1.md`.
- The original v1 Complete artifacts are structurally invalid as current truth because they duplicate Part 01 and lack a genuine original Part 02 body.

### ADR package

- use `AUDIT-016_ADR_Correction_Package_Final_Visual_QA.md` for final visual gate;
- use ADR-005 **v1.2** amendment, not the earlier v1.1 amendment;
- use `AUDIT-010_ADR_Correction_Package.md` for filenames/hashes of ADR-001/002/003/004/006/007 + ADR-008 unless a later file-specific manifest supersedes them.

### MOD-001

- `MOD-001_Core_Architecture_Audited_Source_v1_2.md` is the main regeneration source;
- `AUDIT-018_MOD-001_v1_2_Final_Corrections_Freeze.md` is a mandatory overlay and supersedes the interpretation that the source-level PASS alone made MOD-001 implementation-final.

## 4. Candidate-generation discrepancy gate

The audit history contains multiple QA reports for ARCH-008/009/012 from different regeneration attempts.

For example, an older report records short source-based candidates, while the later `AUDIT-014` selects larger complete candidates and reports:

- ARCH-008: 37 pages RU / 37 pages DE;
- ARCH-009: 35 pages RU / 35 pages DE;
- ARCH-012: 6 pages RU / 6 pages DE.

Therefore binary publication for ARCH-008/009/012 must **not** rely on filename alone.

Before upload, record for each selected final file:

1. exact filename;
2. file size;
3. SHA-256;
4. expected Git blob SHA;
5. rendered page count;
6. RU/DE structural markers/tables as applicable;
7. the QA report that selected that exact generation.

If the local file no longer matches the latest selected generation, regenerate from the audited source rather than publishing an ambiguous candidate.

## 5. Architecture binary publication set

### Gate A — foundation

Publish as one logical pair per ARCH document:

- ARCH-001 v1.4 audited RU + DE;
- ARCH-002 v1.4 audited RU + DE;
- ARCH-003 v1.4 audited RU + DE;
- ARCH-004 v1.4 audited RU + DE;
- ARCH-005 v1.4 audited RU + DE.

Expected candidate filenames from the current manifest:

- `ARCH-001_World_Engine_Overall_System_Architecture_RU_v1_4_Audited_Candidate.docx`
- `ARCH-001_World_Engine_Overall_System_Architecture_DE_v1_4_Audited_Candidate.docx`
- `ARCH-002_World_Engine_Module_Architecture_RU_v1_4_Audited_Candidate.docx`
- `ARCH-002_World_Engine_Module_Architecture_DE_v1_4_Audited_Candidate.docx`
- `ARCH-003_World_Engine_Data_Flow_RU_v1_4_Audited_Candidate.docx`
- `ARCH-003_World_Engine_Data_Flow_DE_v1_4_Audited_Candidate.docx`
- `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_4_Audited_Candidate.docx`
- `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_4_Audited_Candidate.docx`
- `ARCH-005_World_Engine_Module_Dependencies_RU_v1_4_Audited_Candidate.docx`
- `ARCH-005_World_Engine_Module_Dependencies_DE_v1_4_Audited_Candidate.docx`

Before publication, consider renaming `Audited_Candidate` to `Architecture_Audited_Complete` only if no internal metadata/version field still calls the file a candidate. Do not rename a binary in a way that falsely upgrades its internal status.

### Gate B — component and downstream architecture

- ARCH-006 audited SVG — already committed; keep legacy JPG only as superseded/history until cleanup.
- ARCH-007 v4 audited RU + DE.
- ARCH-008 v1.1 latest selected audited RU + DE.
- ARCH-009 v1.1 latest selected audited RU + DE.
- ARCH-010 v1.1 audited rebuilt RU + DE.
- ARCH-011 v1.1 audited RU + DE.
- ARCH-012 v1.1 latest selected audited RU + DE.

Known verified candidates:

#### ARCH-007

- `ARCH-007_Sequence_Diagrams_RU_v4_Architecture_Audited_Complete.docx`
- `ARCH-007_Sequence_Diagrams_DE_v4_Architecture_Audited_Complete.docx`

#### ARCH-010

- `ARCH-010_Plugin_Architecture_RU_v1_1_Architecture_Audited_Complete.docx`
- `ARCH-010_Plugin_Architecture_DE_v1_1_Architecture_Audited_Complete.docx`

The ARCH-010 Git blob SHA after upload must equal:

- RU: `44aa7a6f3bb68d91b32c38be99a90fcc366b1f5e`
- DE: `e06c6cdc10279646c2930bdb601040dd1d32a45b`

if the QA-approved files are uploaded unchanged.

#### ARCH-011

- `ARCH-011_Save_System_Architecture_RU_v1_1_Architecture_Audited_Complete.docx`
- `ARCH-011_Save_System_Architecture_DE_v1_1_Architecture_Audited_Complete.docx`

For ARCH-008/009/012, record final hashes from the latest selected generation before publication because audit history contains more than one candidate generation.

## 6. ADR publication set

Do not overwrite historical ADR-001–007 Complete files silently. Publish amendments/clarifications alongside them.

Required:

- ADR-001 Core Architecture Amendment v1.1 RU + DE;
- ADR-002 Event System Clarification v1.1 RU + DE;
- ADR-003 Database Amendment v1.1 RU + DE;
- ADR-004 Local AI Amendment v1.1 RU + DE;
- ADR-005 Save System Amendment **v1.2** RU + DE — already present;
- ADR-006 Plugin Detection Clarification v1.1 RU + DE;
- ADR-007 Multi-Game Support Amendment v1.1 RU + DE;
- ADR-008 Dependency Semantics / Contract Ownership / Composition Root RU + DE.

Create/use:

`docs/02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/`

for ADR-008.

The original ADR-001–007 remain historical decision records; the amendments define the audited Version 1.x interpretation.

## 7. MOD-001 publication set

Do **not** publish a final MOD-001 v1.2 binary until C1–C6 from AUDIT-018 have been incorporated.

Target final filenames:

- `MOD-001_Core_RU_v1_2_Architecture_Audited_Complete.docx`
- `MOD-001_Core_DE_v1_2_Architecture_Audited_Complete.docx`

Required gates before publication:

- C1–C6 incorporated;
- RU/DE semantic parity;
- full render/page inspection;
- requirements and contract names translation spot-check;
- static dependency-cycle validation PASS;
- no regression to concrete Game Adapter / DB / generic AI / Service Locator dependencies.

## 8. Legacy/supersession policy

Do not leave two generations in the same canonical directory without an explicit status signal.

Preferred final policy:

1. publish and verify the audited replacement first;
2. only after verification, move superseded architecture binary artifacts to a structured archive, for example:
   `docs/99_Archive/Architecture_Pre_Audit/...`;
3. retain original ADR-001–007 in place because ADR history is intentional; amendments live beside them;
4. move or clearly supersede MOD-001 v1.1 only after v1.2 RU+DE is published and verified;
5. never delete the only copy of a historical artifact before archive/PR review confirms preservation.

ARCH-010 legacy v1 Complete is highest priority for supersession because it is structurally invalid as current architecture.

## 9. Canonical index requirement

Before merge, create a repository-readable documentation status/index file that identifies for each architecture/module artifact:

- canonical current version;
- language pair status;
- historical/superseded versions;
- relevant amendment/ADR;
- audit status;
- implementation-use status.

This prevents a developer from accidentally opening an old `v1_Complete.docx` and treating it as current truth.

## 10. Binary commit strategy

Use separate logical commits. Recommended order:

1. `docs: publish audited ARCH-001 architecture`
2. `docs: publish audited ARCH-002 module architecture`
3. `docs: publish audited ARCH-003 data flow`
4. `docs: publish audited ARCH-004 dependency principles`
5. `docs: publish audited ARCH-005 module dependencies`
6. `docs: publish audited ARCH-007 sequence diagrams`
7. `docs: publish audited ARCH-008 deployment architecture`
8. `docs: publish audited ARCH-009 configuration architecture`
9. `docs: publish audited ARCH-010 plugin architecture`
10. `docs: publish audited ARCH-011 save architecture`
11. `docs: publish audited ARCH-012 AI architecture`
12. ADR amendment/ADR-008 commits, one logical ADR per commit/pair;
13. `docs: publish MOD-001 Core v1.2` only after its final gate;
14. `docs: archive superseded pre-audit architecture artifacts` after all replacements are verified;
15. `docs: add architecture canonical documentation index`.

ARCH-006 audited SVG is already part of the branch and does not need a duplicate binary publication commit.

## 11. Verification after every binary commit

For each pair:

1. fetch latest audit-branch HEAD;
2. verify target path and no unintended overwrite;
3. create/upload exact binary blob;
4. verify Git blob SHA against expected hash when known;
5. create commit using latest parent;
6. fetch directory/tree again;
7. verify RU + DE are both present;
8. verify legacy file was not accidentally overwritten before supersession step.

Do not batch unrelated architecture documents into one opaque binary commit.

## 12. Final repository integrity gate

Before PR creation, verify:

- audited RU/DE pairs are present for every corrected ARCH document;
- ARCH-006 audited diagram is present;
- ADR amendment/clarification set is complete;
- ADR-008 RU/DE is present;
- MOD-001 v1.2 RU/DE is present and v1.1 is clearly superseded;
- no Part files are presented as the canonical Complete artifact unless explicitly intended as reconstruction evidence;
- ARCH-010 legacy invalid Complete is not presented as current;
- no accidental duplicate `Final/Complete/Candidate` generation appears current simultaneously;
- documentation index points to one canonical artifact per current document/language;
- static dependency graph still has zero directed implementation cycles;
- audit branch is not behind `main` or has been rebased/reconciled before PR;
- PR diff contains only intended documentation/audit/archive changes.

## 13. Merge gate

Do not merge to `main` until all of the following are true:

1. corrected binaries published and verified;
2. C1–C6 incorporated into MOD-001 v1.2 final binaries;
3. RU/DE parity PASS;
4. render QA PASS;
5. dependency-cycle check PASS;
6. supersession/index policy applied;
7. final repository integrity PASS;
8. reviewable PR opened and inspected.

**MOD-002 remains blocked until this merge gate is satisfied or a later explicit decision changes the gate.**
