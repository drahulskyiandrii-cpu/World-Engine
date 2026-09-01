# AUDIT-004 — Correction Artifacts Manifest

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Superseded candidate set — visual QA passed, semantic amendment required before upload  
**Date:** 2026-09-01

## 1. Purpose

This manifest identifies the first DOCX correction candidates produced during the architecture consistency audit. Every listed file rendered successfully and passed visual page review. However, the later deep review of ARCH-006/007 exposed an additional architecture requirement: Core must be able to invoke explicit synchronous **application use-case ports** (for example a Dialogue Command Port) without depending on concrete domain-module implementations or abusing Event Engine as RPC.

Therefore **none of the binary files below should be uploaded to the audit branch in their current form**. They are retained as QA-verified candidate history and must be regenerated after the AUDIT-005 amendment is applied.

## 2. Candidate foundation set — DO NOT UPLOAD YET

| File | Previous Git blob SHA candidate | Visual QA | Semantic status |
|---|---|---|---|
| `ADR-008_Dependency_Semantics_Contract_Ownership_RU_v1_0_Proposed.docx` | `d85f7f69e3dd29bae9b3b55649c55a1fe8ca094c` | PASS — 6 pages | REVISE: add explicit application use-case ports |
| `ADR-008_Dependency_Semantics_Contract_Ownership_DE_v1_0_Proposed.docx` | `21a6cf1c53968e9576ba041fe74cbd3e44249b82` | PASS — 6 pages | REVISE: add explicit application use-case ports |
| `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_1_Audited.docx` | `62d832297b79cac76882e9ee40e187b48e30ffdf` | PASS — 5 pages | REVISE: use-case-port rule |
| `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_1_Audited.docx` | `6b903b57a3c2806126103fbc9052b1d399d2518f` | PASS — 5 pages | REVISE: use-case-port rule |
| `ARCH-005_World_Engine_Module_Dependencies_RU_v1_1_Audited.docx` | `1cb4bde8246aac4096c175662548d2f0640f0cc0` | PASS — 6 pages | REVISE: separate concrete dependencies, contract dependencies, port bindings and event relations |
| `ARCH-005_World_Engine_Module_Dependencies_DE_v1_1_Audited.docx` | `723fe0f94bd5fa5d49a3da79e93f786a9bba0beb` | PASS — 6 pages | REVISE: separate concrete dependencies, contract dependencies, port bindings and event relations |

## 3. Candidate ARCH-001–003 replacements — DO NOT UPLOAD YET

| File | Previous Git blob SHA candidate | Visual QA | Semantic status |
|---|---|---|---|
| `ARCH-001_World_Engine_Overall_System_Architecture_RU_v1_1_Audited.docx` | `224356e62bb24aace845cf1ed33338ffae9e9594` | PASS — 4 pages | REVISE: synchronous use-case-port boundary |
| `ARCH-001_World_Engine_Overall_System_Architecture_DE_v1_1_Audited.docx` | `92ff1c6646a553cdd93f41597c74821f904d95a7` | PASS — 4 pages | REVISE: synchronous use-case-port boundary |
| `ARCH-002_World_Engine_Module_Architecture_RU_v1_1_Audited.docx` | `197e37064f0a7785497b19dd9201c882dcc25fe3` | PASS — 4 pages | REVISE: Core/application port bindings must be represented explicitly |
| `ARCH-002_World_Engine_Module_Architecture_DE_v1_1_Audited.docx` | `8ad62192d00e5a2b39f34426042ca46813d4f0bd` | PASS — 4 pages | REVISE: Core/application port bindings must be represented explicitly |
| `ARCH-003_World_Engine_Data_Flow_RU_v1_1_Audited.docx` | `e337e01027b31c893fe189d9a73333875bf01eda` | PASS — 4 pages | REVISE: dialogue request/response must use Dialogue use-case port, not Event Engine RPC |
| `ARCH-003_World_Engine_Data_Flow_DE_v1_1_Audited.docx` | `30df897a7d0ac6bfe736bbf80af67fe47ceb304a` | PASS — 4 pages | REVISE: dialogue request/response must use Dialogue use-case port, not Event Engine RPC |

## 4. Why the candidate set changed after QA

Visual QA validates rendering and layout, not architecture semantics. ARCH-007 sequence analysis revealed a synchronous request/response requirement that the first candidate dependency model did not fully represent.

The corrected target must distinguish:

1. concrete module implementation dependencies;
2. public/shared contract package dependencies;
3. Composition Root port bindings;
4. runtime calls;
5. event relations;
6. data flow;
7. ownership.

Core may coordinate a synchronous workflow through an explicit application use-case port implemented by a domain/application module. This does not permit Core to access private module state or resolve arbitrary implementations dynamically.

## 5. Required regeneration order

1. Regenerate ADR-008 RU+DE with the AUDIT-005 amendment.
2. Regenerate ARCH-004 RU+DE.
3. Regenerate ARCH-005 RU+DE and re-run the acyclic graph check.
4. Regenerate ARCH-001 RU+DE.
5. Regenerate ARCH-002 RU+DE.
6. Regenerate ARCH-003 RU+DE.
7. Render and visually inspect all regenerated files again.
8. Only then create binary commits on the audit branch.

## 6. Next audit targets after regeneration

- replacement specification for ARCH-006 Component Diagram;
- corrected ARCH-007 Sequence Diagrams;
- clarification revisions for ARCH-008 Deployment and ARCH-009 Configuration;
- ARCH-010 structural repair;
- ARCH-011 Save System reconciliation;
- ARCH-012 AI Integration reconciliation;
- ADR-001–007 amendment/traceability pass;
- MOD-001 re-audit.

No MOD-002 work is permitted before the corrected architecture foundation is stable.
