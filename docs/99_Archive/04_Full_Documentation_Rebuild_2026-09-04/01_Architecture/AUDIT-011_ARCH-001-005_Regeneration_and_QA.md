# AUDIT-011 — ARCH-001–ARCH-005 Regeneration and QA

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** ARCH-001–ARCH-005 v1.4 audited candidates regenerated and QA-passed locally; binary upload pending  
**Date:** 2026-09-01

## 1. Purpose

This checkpoint records the first architecture-foundation package that has been regenerated **after** the ADR review, contract-ownership freeze and localization QA findings.

The previous v1.1/v1.2/v1.3 candidates are superseded as audit work products. They must not be promoted as canonical files.

## 2. Regenerated candidates

The following RU/DE pairs were regenerated against AUDIT-008–AUDIT-010:

- ARCH-001 Overall System Architecture v1.4
- ARCH-002 Module Architecture v1.4
- ARCH-003 Data Flow v1.4
- ARCH-004 Architecture Principles & Dependency Rules v1.4
- ARCH-005 Module Dependencies v1.4

All ten files were rendered through the DOCX QA pipeline and visually reviewed. Layout is clean: no clipping, broken tables, overflow or missing glyphs were found.

The localization defect from AUDIT-010 was corrected. Technical identifiers/contract names remain in English where they are intended to be canonical implementation terms, while ordinary RU/DE explanatory prose, headings and table labels are localized.

## 3. Architecture decisions represented in v1.4

The v1.4 foundation consistently models:

- Application Host / Composition Root as process-level construction/lifecycle owner;
- distinction between static dependency, runtime call, event relation, data flow, ownership and lifecycle wiring;
- Core -> Event Engine;
- Core -> Scheduler control contract, with no Scheduler -> Core static dependency;
- Core -> narrow Save Manager command/use-case contract for explicit Core-owned save workflows;
- Scheduler -> Event Engine for scheduled semantic triggers, including `AutosaveDue`;
- Game Adapter bidirectionality through Game Ingress and Game Output/Capability ports rather than a concrete implementation cycle;
- synchronous application use-case ports such as Dialogue Command Port;
- Dialogue as primary v1.0 AI consumer;
- AI Adapter -> Context Manager -> Provider Abstraction as the consumer-facing AI path;
- Context Manager -> Memory Read / Context Source port, not Repository/SQLite;
- Dialogue -> Speech Output Port <- Voice Engine;
- Save Manager -> injected SaveParticipant abstractions rather than concrete domain-module lookup;
- dedicated repository ports for normal domain persistence;
- no global Service Locator or mutable global Config singleton.

## 4. Static dependency cycle validation

The concrete implementation graph was rechecked after the contract-ownership freeze.

**Result: PASS — no directed implementation dependency cycle detected.**

Port bindings, event producer/subscriber relations and Composition Root lifecycle wiring are deliberately not misclassified as concrete implementation edges.

## 5. Artifact manifest

| File | Git blob SHA if uploaded unchanged | SHA-256 | Size | QA |
|---|---|---|---:|---|
| `ARCH-001_World_Engine_Overall_System_Architecture_RU_v1_4_Audited_Candidate.docx` | `8c42821e3c123d2a6f1f23842d23615417cc8bb8` | `fd7f9a921748f7f9a60034bf24ba4a831c25fd27426399c082396cd1bc7c25b5` | 42118 | PASS — 3 pages |
| `ARCH-001_World_Engine_Overall_System_Architecture_DE_v1_4_Audited_Candidate.docx` | `f5df2f38100b79c27da6d41a3ecbd4ae8de8337a` | `1582efd6a463792cb8499a3e2a34758f4648e7fe35b0683d92ecda9492ccbe91` | 41578 | PASS — 3 pages |
| `ARCH-002_World_Engine_Module_Architecture_RU_v1_4_Audited_Candidate.docx` | `b077d91486b7cbda9371a552cc59f1bd8188cbc9` | `3ef9487efd2c89c6c48cc11400af468ee236f3278ee038e6c2a540d0eddc6617` | 41914 | PASS — 3 pages |
| `ARCH-002_World_Engine_Module_Architecture_DE_v1_4_Audited_Candidate.docx` | `bb86f9321d0ac4807e964d59aece0a50b43883fb` | `6021319a7101555ba733beadecebf560b0199f3e141eef0e90020b89e3bda673` | 41449 | PASS — 3 pages |
| `ARCH-003_World_Engine_Data_Flow_RU_v1_4_Audited_Candidate.docx` | `a9f2f872fb995d68d14e55ce3f26914c072790d3` | `a772d810fa15cd6be52fcf0fa6cea430eb54c907d267fcaadbbf6b02d302b504` | 42556 | PASS — 4 pages |
| `ARCH-003_World_Engine_Data_Flow_DE_v1_4_Audited_Candidate.docx` | `c8a9bc98429d18769fae36ac09c3de896ce0ce8b` | `da8938a34ecfe9d0915fa87c9cb315dd98ff77234eaa1ed865792af6372f5f5a` | 42075 | PASS — 4 pages |
| `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_4_Audited_Candidate.docx` | `f228f7b76ff9c4fa9b79ce22fbf39f355d3ef440` | `ed678c4fb34d221326f19f12c90edd41f7abc5af536061945871054cbb0999d6` | 43076 | PASS — 4 pages |
| `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_4_Audited_Candidate.docx` | `f82fdceaa5aa09831c9d07161d281fc47e7a1f97` | `ec26d3b2c29acd445f915823cbb32150a0161c0b7a14136d90070c01e6d97a82` | 42536 | PASS — 4 pages |
| `ARCH-005_World_Engine_Module_Dependencies_RU_v1_4_Audited_Candidate.docx` | `a0665d4d3ecbb38d7c5f03a90f01e9713c394870` | `4eb2b8cff41189751cc63e27797a091a7cb5d96e3ec9d43f8167f028ee895a72` | 42950 | PASS — 4 pages |
| `ARCH-005_World_Engine_Module_Dependencies_DE_v1_4_Audited_Candidate.docx` | `8cb2f73eb5d086348001966987e571ceb612dbca` | `6307c189be7c1bcbafa201fe52a1862b72ff2530795620499172e45212053a3d` | 42369 | PASS — 4 pages |

## 6. Promotion status

The v1.4 files are the current preferred **review candidates**, but they are not yet merged into `main` and should not replace the legacy canonical files until the downstream architecture pass (ARCH-006–ARCH-012) proves that no additional upstream change is required.

If later ARCH-006–ARCH-012 review reveals a new fundamental dependency conflict, the foundation package must be amended before promotion. Ordinary downstream wording changes do not require reopening the dependency model.

## 7. Next audit target

The next target is ARCH-006 Component Diagram and ARCH-007 Sequence Diagrams.

Required outputs:

1. a corrected component-diagram source/specification with explicit arrow types and corrected layer placement;
2. corrected sequence semantics for NPC Conversation, NPC Death, Plugin Detection, Save, Load, Scheduled Update and Knowledge Acquisition;
3. explicit elimination of direct Game Adapter -> domain-module and Dialogue -> concrete Game Adapter bypasses;
4. replacement of Scheduler -> Core -> Save Manager autosave routing with Scheduler -> Event Engine -> Save Manager;
5. distinction between runtime arrows and static dependencies in every sequence.

ARCH-008–ARCH-012 remain queued after this pass.

**MOD-002 remains blocked.**
