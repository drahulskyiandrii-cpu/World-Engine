# World Engine — Canonical Documentation Index

**Branch scope:** `audit/architecture-consistency-2026-09-01`  
**Index date:** 2026-09-04  
**Status:** Final audit index — use this file to select implementation-facing documentation on the audit branch

## Selection rules

1. A filename containing `Final` or `Complete` is not sufficient evidence that an artifact is current. Use the exact files selected below.
2. `Architecture_Audited_Complete` artifacts supersede older architecture binaries for implementation use while preserving the older files as history.
3. ADR-001–007 are historical decision records and are not rewritten. Their audited amendments/clarifications are additive; implementation must read the original decision together with the listed addendum and ADR-008.
4. Audit reports and audited Markdown sources are traceability/regeneration evidence, not substitutes for the selected implementation-facing artifact.
5. Until this audit branch passes final review and is merged, `main` remains the pre-audit baseline. MOD-002 remains blocked.

## Architecture artifacts

| ID | Canonical RU | Canonical DE | Historical / superseded artifact | Governing authority | Implementation-use status |
|---|---|---|---|---|---|
| ARCH-001 | [v1.4 audited RU](01_Architecture/ARCH-001_Overall_System_Architecture/ARCH-001_World_Engine_Overall_System_Architecture_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-001_Overall_System_Architecture/ARCH-001_World_Engine_Overall_System_Architecture_DE_v1_4_Architecture_Audited_Complete.docx) | `*_RU_v1.docx`, `*_DE_v1.docx` | AUDIT-022; ADR-008 | Current on audit branch; use for implementation. |
| ARCH-002 | [v1.4 audited RU](01_Architecture/ARCH-002_Module_Architecture/ARCH-002_World_Engine_Module_Architecture_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-002_Module_Architecture/ARCH-002_World_Engine_Module_Architecture_DE_v1_4_Architecture_Audited_Complete.docx) | `*_RU_v1.docx`, `*_DE_v1.docx` | AUDIT-022; ADR-008 | Current on audit branch; use for implementation. |
| ARCH-003 | [v1.4 audited RU](01_Architecture/ARCH-003_Data_Flow/ARCH-003_World_Engine_Data_Flow_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-003_Data_Flow/ARCH-003_World_Engine_Data_Flow_DE_v1_4_Architecture_Audited_Complete.docx) | `*_RU_v1.docx`, `*_DE_v1.docx` | AUDIT-022; ADR-008 | Current on audit branch; use for implementation. |
| ARCH-004 | [v1.4 audited RU](01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_4_Architecture_Audited_Complete.docx) | `*_RU_v1.docx`, `*_DE_v1.docx` | AUDIT-022; ADR-008 | Current dependency-rule authority. |
| ARCH-005 | [v1.4 audited RU](01_Architecture/ARCH-005_Module_Dependencies/ARCH-005_World_Engine_Module_Dependencies_RU_v1_4_Architecture_Audited_Complete.docx) | [v1.4 audited DE](01_Architecture/ARCH-005_Module_Dependencies/ARCH-005_World_Engine_Module_Dependencies_DE_v1_4_Architecture_Audited_Complete.docx) | `*_RU_v1.docx`, `*_DE_v1.docx` | AUDIT-022; ADR-008 | Current static-dependency authority. |
| ARCH-006 | [v1.2 audited language-neutral SVG](01_Architecture/ARCH-006_Component_Diagram/ARCH-006_Component_Diagram_v1_2_Architecture_Audited.svg) | Same language-neutral SVG | `ARCH-006_World_Engine_Component_Diagram_v1_0.jpg` | AUDIT-014; ADR-008 | Current component diagram; legacy JPG is not authoritative. |
| ARCH-007 | [v4 audited RU](01_Architecture/ARCH-007_Sequence_Diagrams/ARCH-007_Sequence_Diagrams_RU_v4_Architecture_Audited_Complete.docx) | [v4 audited DE](01_Architecture/ARCH-007_Sequence_Diagrams/ARCH-007_Sequence_Diagrams_DE_v4_Architecture_Audited_Complete.docx) | `*_RU_v2_Final.docx`, `*_DE_v2_Final.docx` | AUDIT-023; ADR-005 v1.2; ADR-008 | Current sequence authority, including atomic restore activation. |
| ARCH-008 | [v1.1 audited RU](01_Architecture/ARCH-008_Deployment_Architecture/ARCH-008_Deployment_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-008_Deployment_Architecture/ARCH-008_Deployment_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | RU/DE v1 Complete files | AUDIT-024; ADR-008 | Current deployment authority. |
| ARCH-009 | [v1.1 audited RU](01_Architecture/ARCH-009_Configuration_Architecture/ARCH-009_Configuration_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-009_Configuration_Architecture/ARCH-009_Configuration_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | RU/DE v1 Complete files | AUDIT-024; ADR-008 | Current configuration authority. |
| ARCH-010 | [v1.1 audited RU](01_Architecture/ARCH-010_Plugin_Architecture/ARCH-010_Plugin_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-010_Plugin_Architecture/ARCH-010_Plugin_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | RU/DE v1 Complete files are structurally invalid: duplicate Part 01, missing authentic Part 02 | AUDIT-025; ADR-006 clarification; ADR-008 | Current plugin authority. Part 02 is explicitly reconstructed, not claimed as recovered. |
| ARCH-011 | [v1.1 audited RU](01_Architecture/ARCH-011_Save_System_Architecture/ARCH-011_Save_System_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-011_Save_System_Architecture/ARCH-011_Save_System_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | RU/DE v1 Complete files | AUDIT-023; ADR-005 v1.2; ADR-008 | Current Save/Restore authority. |
| ARCH-012 | [v1.1 audited RU](01_Architecture/ARCH-012_AI_Integration_Architecture/ARCH-012_AI_Integration_Architecture_RU_v1_1_Architecture_Audited_Complete.docx) | [v1.1 audited DE](01_Architecture/ARCH-012_AI_Integration_Architecture/ARCH-012_AI_Integration_Architecture_DE_v1_1_Architecture_Audited_Complete.docx) | RU/DE v1 Complete files | AUDIT-023; ADR-004 amendment; ADR-008 | Current AI integration authority. |

## Architecture Decision Records

The word **current** below means the decision set to use on the audit branch. It does not silently change a `Proposed` ADR to `Accepted`.

| ID | Current RU decision set | Current DE decision set | Historical disposition | Audit status | Implementation-use status |
|---|---|---|---|---|---|
| ADR-001 | [v1 historical basis](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_RU_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_Amendment_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_DE_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-001_Core_Architecture/ADR-001_Core_Architecture_Amendment_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply amendment together with ADR-008. |
| ADR-002 | [v1 historical basis](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_RU_v1_Complete.docx) + [v1.1 clarification](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_Clarification_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_DE_v1_Complete.docx) + [v1.1 clarification](02_Architecture_Decision_Records/ADR-002_Event_System/ADR-002_Event_System_Clarification_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply clarification together with ADR-008. |
| ADR-003 | [v1 historical basis](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_RU_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_Amendment_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_DE_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-003_Database/ADR-003_Database_Amendment_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply amendment together with ADR-008. |
| ADR-004 | [v1 historical basis](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_RU_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_Amendment_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_DE_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-004_Local_AI/ADR-004_Local_AI_Amendment_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply amendment together with ADR-008. |
| ADR-005 | [v1 historical basis](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_RU_v1_Complete.docx) + [v1.2 audited amendment](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_Amendment_RU_v1_2_Proposed_Audited.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_DE_v1_Complete.docx) + [v1.2 audited amendment](02_Architecture_Decision_Records/ADR-005_Save_System/ADR-005_Save_System_Amendment_DE_v1_2_Proposed_Audited.docx) | Original retained unchanged; v1.1 transient amendment superseded by v1.2. | Proposed — Architecture Audited; AUDIT-013 + AUDIT-027 PASS | Apply v1.2 amendment together with ADR-008. |
| ADR-006 | [v1 historical basis](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_RU_v1_Complete.docx) + [v1.1 clarification](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_Clarification_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_DE_v1_Complete.docx) + [v1.1 clarification](02_Architecture_Decision_Records/ADR-006_Plugin_Detection/ADR-006_Plugin_Detection_Clarification_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply clarification together with ADR-008. |
| ADR-007 | [v1 historical basis](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_RU_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_Amendment_RU_v1_1_Proposed.docx) | [v1 historical basis](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_DE_v1_Complete.docx) + [v1.1 amendment](02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/ADR-007_Multi_Game_Support_Amendment_DE_v1_1_Proposed.docx) | Original retained unchanged. | Proposed — Architecture Audited; AUDIT-026 PASS | Apply amendment together with ADR-008. |
| ADR-008 | [v1.0 proposed audited RU](02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/ADR-008_Dependency_Semantics_Contract_Ownership_RU_v1_0_Proposed_Audited.docx) | [v1.0 proposed audited DE](02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/ADR-008_Dependency_Semantics_Contract_Ownership_DE_v1_0_Proposed_Audited.docx) | New decision; no historical binary. | Proposed — Architecture Audited; AUDIT-026 PASS | Controlling interpretation for dependency semantics and contract ownership on the audit branch. |

## Module artifacts

| ID | Canonical RU | Canonical DE | Historical / superseded artifact | Governing authority | Implementation-use status |
|---|---|---|---|---|---|
| MOD-001 Core | [v1.2 audited RU](03_Modules/MOD-001_Core/MOD-001_Core_RU_v1_2_Architecture_Audited_Complete.docx) | [v1.2 audited DE](03_Modules/MOD-001_Core/MOD-001_Core_DE_v1_2_Architecture_Audited_Complete.docx) | `MOD-001_Core_RU_v1_1_Audited_Complete.docx`, `MOD-001_Core_DE_v1_1_Audited_Complete.docx` | AUDIT-017, AUDIT-018, AUDIT-021; ADR-008 | Current Core implementation contract on the audit branch. C1–C6 are incorporated. |
| MOD-002+ | Not yet authorized | Not yet authorized | Any draft is non-authoritative | Repository audit merge gate | Blocked until the audit branch is reviewed and merged or an explicit later decision changes the gate. |

## Supersession policy applied by this index

- Legacy architecture binaries remain in place for preservation and review, but they are explicitly noncanonical for implementation.
- The ARCH-010 v1 Complete RU/DE files are additionally marked structurally invalid as current truth because they duplicate Part 01 and lack an authentic Part 02.
- Original ADR-001–007 remain historical decision records; they are not labelled erroneous or deleted. Their additive amendments/clarifications control the Version 1.x interpretation.
- MOD-001 v1.1 remains historical and must not be used to implement new Core behavior.
- Physical archive moves may be performed only after review confirms that this index is sufficient and historical links will remain stable.

## Current merge gate

AUDIT-028 completed the final repository-level gate with PASS:

- all 41 selected remote artifacts exist and match their QA-approved Git objects;
- all 20 expected RU/DE pairs are complete;
- the 18-node, 24-edge static implementation-dependency graph is acyclic;
- ownership, readiness, Save/Restore, AI and plugin rules are mutually consistent;
- the audit diff contains documentation only, plus this index pointer in the root README;
- the audit branch is zero commits behind `main`.

The audit branch is **merge-ready for review**. No merge to `main` is performed or authorized by this index alone; MOD-002 remains blocked until review and merge or a later explicit decision.
