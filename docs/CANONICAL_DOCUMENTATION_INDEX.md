# World Engine — Canonical Documentation Index

**Repository scope:** `main` after document-consolidation merge  
**Index date:** 2026-09-04  
**Status:** Final audited index — use only the artifacts selected below for implementation

## Selection rules

1. Files listed as canonical below are the implementation-facing source of truth.
2. `Architecture_Audited_Complete` is required for active Word artifacts unless this index explicitly says otherwise.
3. ADR-001–007 v2.0 are complete standalone decisions. They integrate the earlier v1 decision and its amendment or clarification; reading the archived pair is no longer required.
4. `docs/99_Archive/` contains preserved, superseded, noncanonical history. Archived files must not be used to define new behavior.
5. Audit reports and audited Markdown sources are traceability/regeneration evidence, not substitutes for the selected artifact.
6. ADR-008 remains the controlling cross-cutting interpretation for dependency semantics and contract ownership.

## Architecture artifacts

| ID | Canonical RU | Canonical DE | Historical location | Governing authority | Status |
|---|---|---|---|---|---|
| ARCH-001 | [v1.4 audited RU](01_Architecture/ARCH-001_Overall_System_Architecture/ARCH-001_World_Engine_Overall_System_Architecture_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-001_Overall_System_Architecture/ARCH-001_World_Engine_Overall_System_Architecture_DE_v1_4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-001_Overall_System_Architecture/` | AUDIT-022; ADR-008 | Current |
| ARCH-002 | [v1.4 audited RU](01_Architecture/ARCH-002_Module_Architecture/ARCH-002_World_Engine_Module_Architecture_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-002_Module_Architecture/ARCH-002_World_Engine_Module_Architecture_DE_v1_4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-002_Module_Architecture/` | AUDIT-022; ADR-008 | Current |
| ARCH-003 | [v1.4 audited RU](01_Architecture/ARCH-003_Data_Flow/ARCH-003_World_Engine_Data_Flow_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-003_Data_Flow/ARCH-003_World_Engine_Data_Flow_DE_v1_4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-003_Data_Flow/` | AUDIT-022; ADR-008 | Current |
| ARCH-004 | [v1.4 audited RU](01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/` | AUDIT-022; ADR-008 | Current dependency authority |
| ARCH-005 | [v1.4 audited RU](01_Architecture/ARCH-005_Module_Dependencies/ARCH-005_World_Engine_Module_Dependencies_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-005_Module_Dependencies/ARCH-005_World_Engine_Module_Dependencies_DE_v1_4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-005_Module_Dependencies/` | AUDIT-022; ADR-008 | Current static-dependency authority |
| ARCH-006 | [v1.2 audited language-neutral SVG](01_Architecture/ARCH-006_Component_Diagram/ARCH-006_Component_Diagram_v1_2_Architecture_Audited.svg) | Same language-neutral SVG | `99_Archive/01_Architecture/ARCH-006_Component_Diagram/` | AUDIT-014; ADR-008 | Current component diagram |
| ARCH-007 | [v4 audited RU](01_Architecture/ARCH-007_Sequence_Diagrams/ARCH-007_Sequence_Diagrams_RU_v4_Architecture_Audited_Complete.docx) | [v4 audited DE](01_Architecture/ARCH-007_Sequence_Diagrams/ARCH-007_Sequence_Diagrams_DE_v4_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-007_Sequence_Diagrams/` | AUDIT-023; ADR-005 v2.0; ADR-008 | Current sequence authority, including atomic restore activation |
| ARCH-008 | [v1.1 audited RU](01_Architecture/ARCH-008_Deployment_Architecture/ARCH-008_Deployment_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-008_Deployment_Architecture/ARCH-008_Deployment_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-008_Deployment_Architecture/` | AUDIT-024; ADR-008 | Current deployment authority |
| ARCH-009 | [v1.1 audited RU](01_Architecture/ARCH-009_Configuration_Architecture/ARCH-009_Configuration_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-009_Configuration_Architecture/ARCH-009_Configuration_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-009_Configuration_Architecture/` | AUDIT-024; ADR-008 | Current configuration authority |
| ARCH-010 | [v1.1 audited RU](01_Architecture/ARCH-010_Plugin_Architecture/ARCH-010_Plugin_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-010_Plugin_Architecture/ARCH-010_Plugin_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-010_Plugin_Architecture/` | AUDIT-025; ADR-006 v2.0; ADR-008 | Current plugin authority; Part 02 is an explicit reconstruction |
| ARCH-011 | [v1.1 audited RU](01_Architecture/ARCH-011_Save_System_Architecture/ARCH-011_Save_System_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-011_Save_System_Architecture/ARCH-011_Save_System_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-011_Save_System_Architecture/` | AUDIT-023; ADR-005 v2.0; ADR-008 | Current Save/Restore authority |
| ARCH-012 | [v1.1 audited RU](01_Architecture/ARCH-012_AI_Integration_Architecture/ARCH-012_AI_Integration_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-012_AI_Integration_Architecture/ARCH-012_AI_Integration_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | `99_Archive/01_Architecture/ARCH-012_AI_Integration_Architecture/` | AUDIT-023; ADR-004 v2.0; ADR-008 | Current AI integration authority |

## Architecture Decision Records

ADR-001–007 v2.0 replace the earlier two-file decision sets with one complete document per language. Their status is `Accepted — Architecture Audited`.

| ID | Canonical RU | Canonical DE | Historical location | Governing audit | Status |
|---|---|---|---|---|---|
| ADR-001 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-001_Core_Architecture/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-002 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-002_Event_System/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-003 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-003_Database/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-004 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-004_Local_AI/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-005 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-005_Save_System/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-006 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-006_Plugin_Detection/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-007 | [v2.0 audited RU](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_RU_v2_0_Architecture_Audited_Complete.docx) | [v2.0 audited DE](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_DE_v2_0_Architecture_Audited_Complete.docx) | `99_Archive/02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/` | AUDIT-029 | Accepted — Architecture Audited |
| ADR-008 | [v1.0 proposed audited RU](02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/ADR-008_Dependency_Semantics_Contract_Ownership_RU_v1_0_Proposed_Audited.docx) | [v1.0 proposed audited DE](02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/ADR-008_Dependency_Semantics_Contract_Ownership_DE_v1_0_Proposed_Audited.docx) | No historical binary | AUDIT-026 | Proposed — Architecture Audited; controlling interpretation |

## Module artifacts

| ID | Canonical RU | Canonical DE | Historical location | Governing authority | Status |
|---|---|---|---|---|---|
| MOD-001 Core | [v1.2 audited RU](03_Modules/MOD-001_Core/MOD-001_Core_RU_v1_2_Architecture_Audited_Complete.docx) | [v1.2 audited DE](03_Modules/MOD-001_Core/MOD-001_Core_DE_v1_2_Architecture_Audited_Complete.docx) | `99_Archive/03_Modules/MOD-001_Core/` | AUDIT-017, AUDIT-018, AUDIT-021; ADR-008 | Current Core implementation contract; C1–C6 incorporated |
| MOD-002+ | Not yet authorized | Not yet authorized | Any draft is non-authoritative | Repository audit gate | Requires an explicit implementation decision |

## Consolidation and archive result

- ADR-001–007: 14 new v2.0 DOCX, each a complete standalone RU or DE decision.
- Quality verification: 14/14 structural checks passed; 14/14 style checks passed; 14/14 heading and section checks passed; 14/14 accessibility checks reported no high, medium or low findings.
- Visual verification: 56/56 rendered pages inspected; no clipping, overlap, blank-page, broken-glyph or misplaced-heading defects found.
- Historical cleanup: 56 superseded files moved from active folders to `docs/99_Archive/` with their original bytes preserved.
- Semantic corrections: ownership, dependency direction, event/command/query distinctions, restore atomicity, local-AI lifecycle, plugin-detection responsibility and multi-game boundary rules are integrated directly in the v2.0 ADRs.
- Detailed evidence: [AUDIT-029](01_Architecture/AUDIT-029_Document_Consolidation_Visual_Semantic_and_Archive_QA.md).

The selected document set is internally consistent and suitable as the baseline for subsequent implementation planning.
