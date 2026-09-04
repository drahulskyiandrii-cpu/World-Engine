# AUDIT-011 — ARCH-004 / ARCH-005 Regeneration

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Baseline:** ADR-008 proposed + ADR-001–ADR-007 v1.1 proposed addenda + AUDIT-008  
**Status:** regenerated DOCX candidates prepared and visually QA-checked; binary upload pending  
**Date:** 2026-09-01

## 1. Purpose

This step regenerates the architecture dependency foundation after the ADR correction package. The previous v1.1 correction drafts were discarded because they predated the completed ADR review and did not yet model synchronous use-case ports, Game ingress/output ownership, Host/Composition Root, Save participation or the final AI/context boundary precisely enough.

## 2. ARCH-004 v1.2 audited draft

Prepared and rendered/visually inspected:

- `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_RU_v1_2_Audited_Draft.docx`
  - 5 pages
  - SHA-256: `96888efc21577ae24f4796670ab5717a3639feeeaaaafe11512e4ef2c15f6aef`
  - Git blob SHA if uploaded unchanged: `e8fc16713225cfeb2dfe399716cc9bf566a6709f`
- `ARCH-004_World_Engine_Architecture_Principles_and_Dependency_Rules_DE_v1_2_Audited_Draft.docx`
  - 5 pages
  - SHA-256: `102134bf57e42e90583d65d172c1e6e8931eaa7035b641d3de1098fee7bc210d`
  - Git blob SHA if uploaded unchanged: `fdf0a03aaf343a7fae1d48873b3d974ade06b28c`

Key corrections in this candidate:

- removes the obsolete rule that dependencies always point “down” architecture layers;
- defines six distinct relation types: static dependency, runtime call, event relation, data flow, ownership and lifecycle wiring;
- makes Application Host / Composition Root the owner of construction, wiring, process startup/shutdown and application readiness aggregation;
- allows narrow synchronous Core use-case ports only for Core-owned orchestration;
- formalizes Game Ingress + Game Output/Capability ports;
- keeps Core away from generic Repository/Database CRUD and generic AI consumption;
- formalizes Scheduler autosave through Event Engine rather than direct Save Manager dependency;
- formalizes explicit SaveParticipant injection instead of registry scanning;
- formalizes Context Manager -> Memory public read/context-source contract;
- formalizes Dialogue -> Speech Output Port <- Voice Engine rather than a concrete Dialogue/Voice cycle;
- defines forbidden patterns and architecture-governance rules.

## 3. ARCH-005 v1.2 audited draft

Prepared and rendered/visually inspected:

- `ARCH-005_World_Engine_Module_Dependencies_RU_v1_2_Audited_Draft.docx`
  - 4 pages
  - SHA-256: `8aaa814d309a674a5b6b0567af4ee3422d64143d6e466e04124b4ae1b91470e1`
  - Git blob SHA if uploaded unchanged: `bd99a99ee9db2a673867a80be34a5ba5bc748829`
- `ARCH-005_World_Engine_Module_Dependencies_DE_v1_2_Audited_Draft.docx`
  - 4 pages
  - SHA-256: `7a1cf41d4afb69b12874a2456e233e20e2b5f098882a2e00de34030f6f485011`
  - Git blob SHA if uploaded unchanged: `1c9404419fc610cd942aa5078841d1be62b33cea`

The new dependency model separates:

1. static module dependencies;
2. inner-owned ports and adapter implementations;
3. event producer/subscriber relations;
4. persistence ports;
5. lifecycle/configuration wiring that belongs to Host rather than ordinary module dependency.

## 4. Resolved dependency questions

### Voice / Dialogue

Canonical model:

`Dialogue -> Speech Output Port <- Voice Engine implementation`

Runtime Dialogue -> Voice behavior remains possible without a concrete implementation cycle. Voice remains optional.

### Context Manager / Memory

Context Manager may retrieve approved memory context only through the public Memory read/context-source contract. It may not access Memory internals or Repository/Database directly.

### Save participants

Save Manager receives a collection of `SaveParticipant` abstractions through explicit Composition Root wiring. State-owning modules implement participation for their own state. No Service Locator or registry scan is permitted.

### Configuration

Modules receive typed immutable configuration views from Host/Composition Root. A mutable global Config singleton is not a module dependency.

## 5. Mechanical graph validation

A directed graph was built from the v1.2 static/port model, including Core, Scheduler, Event Engine, Save Manager, Dialogue, Memory, Character, Relationship, Organization, Economy, Politics, Quest, AI Adapter, Context Manager, Voice-related port abstractions and persistence boundaries.

Result:

**PASS — no directed dependency cycles detected.**

The validated model removes the previously identified Core/Scheduler, Economy/Politics and Organization/Relationship cycle interpretations.

## 6. Important modeling note

`Core -> Dialogue Command Port` is intentionally represented as a narrow synchronous use-case dependency for a conversation workflow requiring an immediate result. This does **not** authorize generic Core -> domain internals coupling. Additional synchronous Core use-case ports must be justified individually in the owning module specification/API.

Likewise, Game Adapter runtime bidirectionality is modeled through Game Ingress and Game Output/Capability contracts and therefore does not imply a concrete `Core <-> Game Adapter` implementation cycle.

## 7. Next safe correction step

With ARCH-004/005 regenerated and the graph passing the cycle check, the next documents to regenerate are:

1. ARCH-001 Overall System Architecture;
2. ARCH-002 Module Architecture;
3. ARCH-003 Data Flow;
4. then replacement ARCH-006 Component Diagram and corrected ARCH-007 sequences.

The previous local ARCH-001–003 v1.1 audited candidates are stale because they predate this v1.2 dependency foundation and must not be uploaded unchanged.

`MOD-002` remains blocked. `MOD-001` remains pending final re-audit after upstream corrections.
