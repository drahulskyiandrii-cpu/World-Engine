# AUDIT-003 — Deep Review of ARCH-001, ARCH-002 and ARCH-003

**Project:** World Engine  
**Baseline files:** current `main` versions of ARCH-001, ARCH-002, ARCH-003  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Deep review complete; correction required  
**Date:** 2026-09-01

## 1. Scope and repository integrity

The reviewed RU source files were verified by Git blob SHA against the current repository:

- ARCH-001 RU blob: `798dbd58566ed9d9694ef850cfac89532a9a429a`
- ARCH-002 RU blob: `ac3d084301348dafdf37ba32560e9787a67c7d9c`
- ARCH-003 RU blob: `bec82c40587e9e4797cd1edf69398e6a7bcd4731`

The DE versions remain part of the parity phase; the semantic defects below originate in the shared architecture model and therefore affect both languages unless noted otherwise.

## 2. ARCH-001 — Overall System Architecture

### 2.1 Current role

ARCH-001 is an early v0.1 Draft overview. It correctly captures several durable project goals:

- game-independent Core;
- modular architecture;
- separation of simulation, memory, dialogue and game integration;
- adapter-based future game support;
- LLM as a service rather than the sole source of logic;
- SQLite as an acceptable local v1 persistence technology.

Those high-level goals remain valid.

### 2.2 Critical defects

#### A001-01 — Obsolete dependency table

The current table declares:

- `Core -> Game Adapter, Event Bus`
- `Memory Engine -> Database`
- `Organization Engine -> Database, Event Bus`
- `Economy Engine -> Database, Event Bus`
- `Politics Engine -> Organization Engine, Event Bus`

This table predates the governed Event Engine, Repository boundary, Save Manager architecture and final module dependency rules. It must not remain a canonical dependency model.

**Correction:** ARCH-001 v1.1 should stop presenting a detailed dependency matrix. It should reference ARCH-005 as the canonical dependency source and show only architectural boundaries/layers.

#### A001-02 — Core routes events directly to modules

Current processing text says:

`Game Adapter -> Core -> corresponding modules`

The revised architecture requires semantic events to go through Event Engine and synchronous operations to use explicitly named direct contracts. “Core sends the event to the modules” is too broad and can create a God-Core dispatcher.

**Correction:** describe ingress as `Game Adapter -> Core ingress / application command` followed by either Event Engine publication or an approved direct contract depending on message semantics.

#### A001-03 — Modules save changes directly to Database

Current text says modules update world state and save changes in Database. This collapses domain ownership, Repository abstraction and Save System semantics into one line.

**Correction:** state-owning modules own semantics; durable state uses approved repository contracts where required; Save Manager owns coherent Save/Restore artifact lifecycle; Database is physical storage behind Repository.

#### A001-04 — “AI modules” language is too broad

The overview groups AI behavior as a generic layer without separating Dialogue application logic, AI Adapter/provider abstraction and Voice/media integration.

**Correction:** use the canonical layer model from AUDIT-002 / revised ARCH-004.

#### A001-05 — Status and traceability

The file is a v0.1 Draft but occupies the canonical ARCH-001 location without a visible superseded/revised marker.

**Correction:** replace with ARCH-001 v1.1 Audited, preserve v0.1 in history/archive if desired, and reference ADR-001, ADR-002, ADR-003, ADR-007, ADR-008 and ARCH-004/005.

### 2.3 Required v1.1 content

ARCH-001 v1.1 should contain:

1. System purpose and architectural goals.
2. Canonical five-layer responsibility map.
3. World Engine Host / Composition Root distinction from MOD-001 Core.
4. Game integration boundary using ingress/output ports.
5. Coordination boundary: Core, Event Engine, Scheduler.
6. Simulation/application modules and ownership.
7. AI Adapter/provider boundary and non-authoritative AI rule.
8. Persistence boundary: Save Manager, Repository, Database.
9. Explicit relation legend: static dependency, runtime call, event relation, data flow, ownership.
10. High-level startup/deployment reference to ARCH-008/009 rather than embedding subsystem ownership in Core.
11. Links to ARCH-005 for dependency truth and ARCH-007 for runtime sequences.

## 3. ARCH-002 — Module Architecture

### 3.1 Valid durable content

The following principles remain valid:

- one main responsibility per module;
- low coupling;
- explicit events/contracts;
- Core coordinates but does not contain domain logic;
- Game Adapter contains no simulation logic;
- Event Engine routes events but owns no domain state;
- modules have explicit data ownership.

### 3.2 Critical defects

#### A002-01 — Actual circular dependencies

The current catalog contains:

- Organization -> Relationship
- Relationship -> Organization

and:

- Economy -> Politics
- Politics -> Economy

These are direct cycles and conflict with the document's own low-coupling goal and the later acyclic dependency rules.

**Correction target:**

- Character -> Memory
- Relationship -> Character
- Organization -> Relationship
- Economy -> Organization
- Politics -> Organization + Economy
- Quest -> Memory + Relationship + Politics

No reverse Organization -> Relationship cycle and no Economy <-> Politics cycle.

#### A002-02 — Legacy Core/Event dependencies

Current catalog says:

- Memory -> Core, Event Engine
- Event Engine -> Core
- Scheduler -> Core

This makes infrastructure ownership unclear and creates the same Scheduler/Core cycle later seen in DEPS-001.

**Correction:** static dependencies must follow revised ARCH-005. Event subscription is documented separately from static dependency.

#### A002-03 — Event Engine described as “single bus for the whole system”

The phrase “Единая шина событий для всей системы” can be read as a universal message bus. ADR-002 explicitly rejects that model.

**Correction:** Event Engine is the dedicated typed semantic event/lifecycle subsystem; commands/queries may use approved direct contracts.

#### A002-04 — Dialogue/Voice/Save layer placement

Current catalog places Dialogue and Voice in AI Layer and Save Manager in Persistence. Later diagrams/documents move these inconsistently.

**Canonical correction:**

- Dialogue -> Simulation/Application
- Voice -> AI / Media Integration
- Save Manager -> Persistence

Dialogue may consume AI capability but remains an application/domain-facing module that owns dialogue orchestration and fallback logic.

#### A002-05 — Generic module lifecycle is invalid

The current document gives all modules the same lifecycle:

`Constructed -> Initialized -> Running -> Paused -> Saving -> Destroyed`

This is not valid for every module. `Saving` is not a universal module lifecycle state, and later architecture uses specialized lifecycle/readiness models.

**Correction:** define only common lifecycle obligations (constructed/wired/initialized/readiness/shutdown) and delegate module-specific state machines to each MOD specification. Save state belongs to Save Manager/Save workflow, not every module.

#### A002-06 — Dialogue statement conflicts with Memory ownership evolution

The current text says Dialogue reads memory/character only for response formation and “does not modify memory directly.” That is generally sound, but sequence documents later show optional memory updates around dialogue. The correct rule is ownership-based: Dialogue can request/emit a semantic memory candidate through an approved Memory contract/event, but cannot mutate Memory internals.

#### A002-07 — Metadata identity

Internal ID is `WE-ARC-005` while repository canonical ID is ARCH-002.

**Correction:** v1.1 uses `ARCH-002` and records the legacy ID in change history if needed.

### 3.3 Required v1.1 content

ARCH-002 v1.1 should include:

- canonical module catalog MOD-001–014;
- responsibility and state ownership per module;
- canonical layer assignment;
- static dependency summary by reference to ARCH-005;
- event participation separated from direct dependency;
- module boundary rules;
- composition/DI rule;
- common lifecycle obligations without false universal Saving state;
- extension governance and module-spec requirement.

## 4. ARCH-003 — Data Flow

### 4.1 Valid durable content

The following remain useful:

- significant semantic events use Event Engine;
- event priorities exist;
- critical immediate operations may require synchronous handling;
- background work may be asynchronous;
- AI failure requires dialogue fallback;
- event failure should not crash the whole system;
- performance/backpressure matter.

### 4.2 Critical defects

#### A003-01 — One universal processing pipeline

Current canonical table defines:

`Game -> Game Adapter -> Core -> Event Engine -> Simulation Modules -> AI Modules -> Persistence Layer -> Game Adapter`

This is not a valid universal pipeline. Not every event goes through AI, not every event immediately persists, and persistence is not a stage that must follow simulation for each event.

**Correction:** ARCH-003 must define multiple flow classes:

1. Game ingress command/event flow.
2. Semantic event publication/reaction flow.
3. Synchronous command/query flow.
4. Background persistence flow.
5. Save/Load flow.
6. AI request/response flow.
7. Game output/action flow.

#### A003-02 — Core appears to dispatch every event

Current text implies Core forwards the event downstream. This conflicts with Event Engine ownership and can create a central dispatcher.

**Correction:** Core coordinates application workflows and may publish semantic events; Event Engine owns event delivery to subscribers.

#### A003-03 — “Memory Engine and Database can work asynchronously”

This sentence mixes a domain module with a physical persistence technology and can be read as direct Memory -> Database coupling.

**Correction:** Memory processing may be asynchronous when semantics permit; persistence behind Memory's approved repository contract may also be asynchronous only when consistency rules permit. Database is not addressed as a peer module in application flow.

#### A003-04 — JoinOrganization initiated by Dialogue/Quest

The current scenario says `Quest Engine or Dialogue Engine initiates JoinOrganization`. That can be valid only through an Organization-owned command contract/policy. The current wording allows domain ownership bypass.

**Correction:** Dialogue/Quest may request membership change through Organization's public command contract or produce an event that Organization handles; Organization owns validation and mutation.

#### A003-05 — Rumor creation by Event Engine

Current scenario says `Memory Engine or Event Engine creates RumorSpread`. Event Engine must not originate domain semantics on its own merely because it transports events.

**Correction:** a domain/application producer creates `RumorSpread`; Event Engine transports it.

#### A003-06 — Dragon attack started by “Scheduler and World State”

`World State` is not a defined top-level module. Scheduler owns timing, not domain attack semantics.

**Correction:** Scheduler emits a scheduled trigger; a domain/world simulation owner validates and produces `DragonAttackStarted` (exact owner deferred to module design if no module currently owns this domain).

#### A003-07 — Save flow bypasses Save Manager ownership details

Current scenario says Game Adapter sends SaveGame and Save Manager gathers states and Database receives snapshot. This is too imprecise and conflicts with ARCH-011 transaction/snapshot rules.

**Correction:** Save request enters through an approved ingress/command boundary; Core may issue Save Manager command; Save Manager establishes snapshot boundary, obtains read-only snapshot contracts, validates, persists through Repository, commits/promotes artifact and publishes lifecycle events.

#### A003-08 — Load flow says Core reinitializes modules and Event Engine restores flow

This is overly broad. ARCH-011 defines staging, validation and controlled activation. Event Engine does not “restore the current event flow” as a generic operation.

**Correction:** Save Manager stages and validates loaded state; Core/Host coordinates activation of validated runtime state; specialized owners activate their state; durable event recovery occurs only according to explicit Event System persistence policy.

#### A003-09 — Database unavailable -> temporary buffer

Current error rule says critical data should go to a temporary buffer/recovery journal if Database is unavailable. Later deployment architecture treats persistence unavailability as potentially critical, and no generic durable temporary buffer has been architected.

**Correction:** do not invent a buffer. Persistence failure is normalized; operations requiring durable commit fail or enter controlled degraded/recovery behavior according to owner policy. Any recovery journal requires an explicit architecture decision.

#### A003-10 — Distributed future improvement conflicts with current scope

“Distributed processing for heavy scenarios” is acceptable only as a future idea, but should be clearly marked outside v1 and not imply a current architectural trajectory.

### 4.3 Required v1.1 content

ARCH-003 v1.1 should define relation/flow classes rather than one universal pipeline, and each scenario must explicitly state:

- producer/initiator;
- relation type (command, query, event, data flow);
- owner of validation/mutation;
- optional asynchronous boundary;
- persistence owner when relevant;
- output path when relevant.

## 5. Cross-document correction matrix

| Problem | ARCH-001 | ARCH-002 | ARCH-003 | Upstream rule |
|---|---|---|---|---|
| Static/runtime/event arrow ambiguity | revise | revise | revise | AUDIT-002 / ADR-008 proposal |
| Core as universal dispatcher | revise | clarify | revise | ADR-001 + ADR-002 |
| Direct domain -> Database model | remove | remove/clarify | remove | ADR-003 + Save boundaries |
| Cyclic module graph | n/a | remove | avoid scenario bypasses | revised ARCH-005 |
| Universal Event Bus interpretation | avoid | remove wording | distinguish commands/events | ADR-002 |
| Game Adapter bidirectional interaction | port model | port model | ingress/output flows | ADR-007 + AUDIT-002 |
| Save/load semantics | high-level only | ownership only | align with ARCH-011 | ADR-005 / ARCH-011 |
| AI boundary | high-level capability | Dialogue consumer | dedicated AI flow | ADR-004/ARCH-012 clarification |
| Layer inconsistency | canonical map | canonical map | use same terms | revised ARCH-004 |

## 6. Decision

ARCH-001, ARCH-002 and ARCH-003 are **not safe to keep as current canonical implementation guidance in their present v0.1 form**. Their original high-level intent is valuable, but the dependency tables, universal pipeline and several scenarios are superseded by later decisions.

They should be replaced by audited v1.1 documents on the audit branch after ARCH-004/005 and the dependency-semantics decision are stabilized.

## 7. Next action

1. Finalize proposed ADR-008 and audited ARCH-004/005 artifacts.
2. Build ARCH-001 v1.1 Audited RU+DE.
3. Build ARCH-002 v1.1 Audited RU+DE.
4. Build ARCH-003 v1.1 Audited RU+DE.
5. Render and visually inspect every page.
6. Re-run dependency graph and scenario boundary checks before moving to ARCH-006/007.
