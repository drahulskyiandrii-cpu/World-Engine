# AUDIT-009 — Static Dependency Graph Validation

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Provisional graph validated as acyclic  
**Date:** 2026-09-01

## 1. Purpose

This check translates the provisional correction baseline from AUDIT-008 into a concrete module-level static dependency graph and verifies that the proposed implementation dependency model contains no cycles.

Runtime calls through inverted ports, event producer/subscriber relations and data-flow arrows are deliberately excluded when they do not create a direct implementation dependency.

## 2. Provisional implementation dependency edges

The following graph is the current correction target:

```text
Core -> EventEngine
Core -> Scheduler
Core -> SaveManager

Scheduler -> EventEngine

Memory -> Repository
Character -> Memory
Relationship -> Character
Organization -> Relationship
Economy -> Organization
Politics -> Organization
Politics -> Economy
Quest -> Memory
Quest -> Relationship
Quest -> Politics
Dialogue -> Memory
Dialogue -> Character
Dialogue -> Relationship
Dialogue -> Quest
Dialogue -> AIAdapter
Voice -> Dialogue

SaveManager -> Repository
Repository -> Database

AIAdapter -> ContextManager
AIAdapter -> ProviderAbstraction
```

Game Adapter relationships are represented through application/integration ports rather than a direct implementation cycle:

```text
GameAdapter -> GameIngressContract <- Core/Application
Core/Application -> GameOutputCapabilityContract <- GameAdapter
```

Likewise, synchronous application use-case ports may be owned by an application/contracts package and implemented by a domain/application module without adding a reverse concrete dependency from Core to that implementation.

## 3. Cycle validation result

A directed-cycle check was performed against the provisional module graph above.

**Result: PASS — no directed implementation dependency cycles found.**

This resolves the previously ambiguous `Core <-> Scheduler` model by keeping only:

```text
Core -> Scheduler
Scheduler -> EventEngine
```

Scheduler timing/events do not require a static dependency back to Core.

## 4. Important exclusions from the graph

The following relations are not represented as direct implementation edges:

- Game Adapter runtime ingress/outbound calls — resolved through ports.
- Event producer/subscriber relations — represented through Event Engine contracts.
- Save participation of domain modules — resolved through Save participation contracts rather than Save Manager importing every concrete module.
- Plugin Detection lifecycle wiring — owned by Host/Composition Root.
- Application readiness aggregation — owned by Host/Composition Root.
- AI provider process lifecycle — owned by Host/Composition Root.

These exclusions are intentional and must be documented explicitly in ARCH-004/005 so they are not mistaken for missing dependencies.

## 5. Remaining graph questions before ARCH-005 is frozen

The graph is acyclic, but four contract-ownership details still need to be made explicit during ARCH-005 regeneration:

### 5.1 Voice / Dialogue

Current legacy documents disagree on runtime direction. ARCH-005 historically states `Voice -> Dialogue`, while sequence flows often show Dialogue requesting synthesis from Voice.

Recommended static model:

- Dialogue emits/provides a speech-output contract or invokes a media-output port.
- Voice implements/subscribes to that contract.
- Runtime `Dialogue -> Voice` must not automatically become a concrete reverse dependency if it would contradict the chosen port ownership.

This must be explicitly resolved in the corrected ARCH-005 and later MOD-005/MOD-014.

### 5.2 Context Manager / Memory retrieval

ARCH-012 requires approved memory retrieval for AI context, but contract ownership is not yet explicit.

Recommended model:

- Context Manager depends on a narrow `MemoryRead/ContextSource` port, not Memory internals or Repository.
- Memory Engine implements/provides the approved retrieval behavior.
- AI Adapter remains the consumer-facing entry boundary.

### 5.3 Save participation contracts

Save Manager must coordinate state-owning modules without importing every concrete module implementation.

Recommended model:

- Save Manager depends on a collection of explicit `SaveParticipant` contracts supplied by Composition Root.
- participant implementations remain owned by their modules;
- no runtime service locator is introduced.

### 5.4 Configuration

Configuration is a host/subsystem concern rather than a generic dependency on Core.

Recommended model:

- Host builds typed effective configuration;
- each component receives only its typed configuration view through construction;
- no global mutable Config singleton;
- runtime reload uses controlled configuration contracts/events defined by ARCH-009.

## 6. Freeze criteria for corrected ARCH-005

Corrected ARCH-005 may be marked architecture-audited only when:

1. every direct dependency has a named contract owner;
2. runtime-only/event/data-flow relations are separated from static edges;
3. the final graph passes cycle detection;
4. Game Adapter bidirectionality is represented through ports;
5. Save participation does not create a SaveManager<->domain cycle;
6. Voice/Dialogue direction is explicitly resolved;
7. Context Manager memory access uses a narrow read/context port;
8. no Service Locator/global registry is required.

## 7. Result

The core dependency model defined by AUDIT-008 is structurally viable and can be implemented without directed module cycles. The remaining questions are contract-ownership details, not evidence that the overall model must be redesigned again.
