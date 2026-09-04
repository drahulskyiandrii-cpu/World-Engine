# AUDIT-010 — ADR Correction Package

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Baseline:** AUDIT-007 + AUDIT-008 + AUDIT-009  
**Status:** ADR correction package prepared; DOCX render/visual QA passed; binary upload/merge still pending  
**Date:** 2026-09-01

## 1. Purpose

This package converts the resolved audit baseline into explicit decision artifacts without silently rewriting historical ADR-001–ADR-007 files. Existing v1.0 ADRs remain historical decision records. The new artifacts are proposed amendments/clarifications that define the Version 1.x interpretation after ADR-008.

## 2. Prepared decision artifacts

### ADR-008 — Dependency Semantics, Contract Ownership & Composition Root

Prepared and visually QA-checked:

- `ADR-008_Dependency_Semantics_Contract_Ownership_RU_v1_0_Proposed_Audited.docx`
  - pages: 5
  - SHA-256: `7876f4ed82b05da5d20a4095f5be1844dccce8f45d94fca52b0f319debb7a28d`
  - Git blob SHA if uploaded unchanged: `8da266eb6480dceded27510fce61c3df7f08e4b0`
- `ADR-008_Dependency_Semantics_Contract_Ownership_DE_v1_0_Proposed_Audited.docx`
  - pages: 5
  - SHA-256: `7fa41a851eca6259cb61ed42098b3e47a2250bd1641eb4bf04adff2d0d0de53f`
  - Git blob SHA if uploaded unchanged: `238fb3945721b9b9e9d99a411fdac38b78dea1b3`

ADR-008 now explicitly includes:

- static dependency vs runtime call vs event relation vs data flow vs ownership vs lifecycle wiring;
- Application Host / Composition Root ownership;
- Core use of narrow synchronous use-case ports when immediate results are genuinely required;
- Core -> Save Manager command/use-case contract;
- Game Ingress and Game Output/Capability port ownership;
- Core prohibition on generic Repository/Database CRUD;
- Core not being a generic AI consumer;
- Scheduler -> Event Engine autosave trigger semantics;
- explicit save-participant wiring without Service Locator;
- canonical layer placement.

## 3. ADR-001–ADR-007 proposed amendments/clarifications

All files below were rendered and visually inspected. Each is two pages and keeps the corresponding v1.0 ADR as historical context.

| Parent ADR | Proposed artifact | SHA-256 | Git blob SHA if unchanged | Result |
|---|---|---|---|---|
| ADR-001 Core Architecture | `ADR-001_Core_Architecture_Amendment_RU_v1_1_Proposed.docx` | `9a84f9917f7e15e961722a4f91febdfc05f73469d358b021df866d1094f2d777` | `9500ef8f7cb8e0f765886e9ea6c35fa32e99e342` | PASS |
| ADR-001 Core Architecture | `ADR-001_Core_Architecture_Amendment_DE_v1_1_Proposed.docx` | `0c2e7599363ed2a1942e9ada2a1c5636ed76b39b75ca177c82c8164a0cce5df3` | `474d0eb11baa944047e95175e36fc90d117906b7` | PASS |
| ADR-002 Event System | `ADR-002_Event_System_Clarification_RU_v1_1_Proposed.docx` | `882ccc5bd8248fca76a7a9f8e1ddc9c67429e2fdcef157b1563495bc0d918bf6` | `e72bf93b7fd26de3bf856a5fadf6493fb318d4f3` | PASS |
| ADR-002 Event System | `ADR-002_Event_System_Clarification_DE_v1_1_Proposed.docx` | `67488af59cb822e03e80090c2ba3468d23c6fc4099c54ec05104eb5526a3e933` | `1399ab2e13c26d08b2b1fadde0501b173fbd02ee` | PASS |
| ADR-003 Database | `ADR-003_Database_Amendment_RU_v1_1_Proposed.docx` | `c64c3de87512a1fe2026fef4b7a704b2b9112ca38046af519dc6d185ba398150` | `e752a9770a0848ae74ea3d2616b45b2d56a9b5ee` | PASS |
| ADR-003 Database | `ADR-003_Database_Amendment_DE_v1_1_Proposed.docx` | `2f330f0ee9e032da8c17b0a693d913a88fe3afffa5e01df0f3e59c37f11eefa5` | `4bf994d7fcdbab1559bc9d6f2734f2e9e67fa042` | PASS |
| ADR-004 Local AI | `ADR-004_Local_AI_Amendment_RU_v1_1_Proposed.docx` | `89fcad622fd6c152600428cf47b36d3a4f8c6f92dd8f5c52cd72bd12eba2ff4b` | `de8b1f46b74d2b35c9744985b4232ffa93ba4838` | PASS |
| ADR-004 Local AI | `ADR-004_Local_AI_Amendment_DE_v1_1_Proposed.docx` | `7785d953a7505b581603b7b9d6e15a4052200b588dca91b9f6a192252e7fa559` | `47cabdabbe244456e49f8af3344ac03852876520` | PASS |
| ADR-005 Save System | `ADR-005_Save_System_Amendment_RU_v1_1_Proposed.docx` | `b3c4aed1db6d01c4e79cdcf9bdf1c7d8aed9b8eb1c5be394be95bd344ac372c0` | `9d201fa8fd9d89a9aab0fb3ec8760427c685cd9e` | PASS |
| ADR-005 Save System | `ADR-005_Save_System_Amendment_DE_v1_1_Proposed.docx` | `edc846442de65a8177508f01f3ecfee38bbfff7167ec716244ef187b3c86d1f6` | `1e13617ce23ff5e28bf3565fd7142a3b0b7326c5` | PASS |
| ADR-006 Plugin Detection | `ADR-006_Plugin_Detection_Clarification_RU_v1_1_Proposed.docx` | `b0fabbfd38523371820a63a0369d0aeb20cd417d73e5aea031e43b8869e6f4e6` | `0160fb7a7d0acba5cfbef5fac93b64c689efead2` | PASS |
| ADR-006 Plugin Detection | `ADR-006_Plugin_Detection_Clarification_DE_v1_1_Proposed.docx` | `d5e8b97cf98acfee0bdf4c173e688feeb7c76943e87c388debb538b64769fbda` | `104256a04e799460b8eb3013a0780c7f55723813` | PASS |
| ADR-007 Multi-Game Support | `ADR-007_Multi_Game_Support_Amendment_RU_v1_1_Proposed.docx` | `148840436db28e623b2064d15f8c2767ba9408a7354c9f9fa64a7c9252f4e074` | `501e2248c76101b00896048916cc718d7694e2e5` | PASS |
| ADR-007 Multi-Game Support | `ADR-007_Multi_Game_Support_Amendment_DE_v1_1_Proposed.docx` | `eff319ff86c20a7016d2981a3207a182a8a7d28b00e54097b11107db46a72dfe` | `bce5e735428c31f8c4938164723ce8de9da5b797` | PASS |

## 4. Semantic outcome of the package

After these proposed amendments are accepted, the ADR layer expresses one consistent model:

1. **Host / Composition Root** owns construction, dependency wiring, process startup/shutdown and application readiness aggregation.
2. **Core** owns coordination semantics and may use explicit narrow use-case ports, but is not a Service Locator, database CRUD client or generic AI consumer.
3. **Event Engine** remains semantic-event infrastructure, not generic RPC; Event != Command != Query.
4. **Scheduler** does not depend on Core and does not depend directly on Save Manager. Autosave timing publishes `AutosaveDue` through Event Engine.
5. **Persistence** remains behind dedicated logical repository ports; Core may see only a justified narrow Core-owned status/persistence port.
6. **Save Manager** owns Save Contract and restore semantics; a committed Save is an authoritative persisted/restorable checkpoint, not necessarily the latest live runtime state.
7. **AI** consumer path is `Approved Consumer -> AI Adapter -> Context Manager -> Provider Abstraction -> Provider`; Dialogue is the primary v1.0 consumer.
8. **Plugin Detection** owns discovery/profile evidence only; capability mapping and restore compatibility retain separate owners.
9. **Game integration** uses Game Ingress plus Game Output/Capability ports so runtime bidirectionality does not create a concrete implementation cycle.

## 5. Repository placement after binary upload

- ADR-008 RU+DE: `docs/02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/`
- ADR-001 amendment RU+DE: existing `ADR-001_Core_Architecture/`
- ADR-002 clarification RU+DE: existing `ADR-002_Event_System/`
- ADR-003 amendment RU+DE: existing `ADR-003_Database/`
- ADR-004 amendment RU+DE: existing `ADR-004_Local_AI/`
- ADR-005 amendment RU+DE: existing `ADR-005_Save_System/`
- ADR-006 clarification RU+DE: existing `ADR-006_Plugin_Detection/`
- ADR-007 amendment RU+DE: existing `ADR-007_Multi_Game_Support/`

The original v1.0 Complete files must remain alongside the addenda until the final publication/versioning policy is decided. Do not overwrite historical ADR files silently.

## 6. Next correction stage

The next safe architecture work is now:

1. regenerate **ARCH-004 Architecture Principles & Dependency Rules** against ADR-008 + addenda;
2. regenerate **ARCH-005 Module Dependencies** with separate static-dependency, port, event and persistence views;
3. mechanically re-check the static graph for cycles;
4. only then regenerate ARCH-001, ARCH-002 and ARCH-003.

`MOD-002` remains blocked. `MOD-001` remains subject to re-audit after the upstream architecture set is corrected.
