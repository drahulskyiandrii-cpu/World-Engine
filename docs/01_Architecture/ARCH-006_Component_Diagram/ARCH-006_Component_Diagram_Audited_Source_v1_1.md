# ARCH-006 — Component Diagram — Audited Source v1.1

**Status:** Audit branch source candidate  
**Baseline:** ADR-008 + ADR amendments + ARCH-004/005 audited candidate model  
**Date:** 2026-09-01

## Diagram semantics

This source replaces the legacy diagram that mixed static dependencies, runtime calls, event flow and data flow in one arrow style.

The diagram below is primarily a **static component/contract view**.

- solid arrow `A --> B`: A statically depends on B's public contract;
- port node: inner-owned contract used for dependency inversion;
- dotted arrow: lifecycle wiring/evidence, not ordinary module dependency;
- event relations are shown separately and do not imply producer-to-subscriber static dependency;
- runtime sequences belong to ARCH-007.

```mermaid
flowchart TB
    subgraph EXT[External Systems]
        GAME[Skyrim / Supported Game]
        LOCALAI[Local AI Runtime]
        EXTAI[External LLM]
    end

    subgraph HOST[Application Host / Composition Root]
        H[Construction • DI • Config/Profile Selection • Startup/Shutdown • App Readiness]
    end

    subgraph INT[Integration]
        GA[MOD-002 Game Adapter]
        PD[Plugin Detection / Analyzer]
    end

    GIN[[Game Ingress Port]]
    GOUT[[Game Output / Capability Port]]

    subgraph COORD[Coordination / Application]
        CORE[MOD-001 Core]
        EV[MOD-011 Event Engine]
        SCH[MOD-012 Scheduler]
    end

    DLGPORT[[Dialogue Command Port]]

    subgraph SIM[Simulation / Domain Application]
        MEM[MOD-003 Memory Engine]
        CHAR[MOD-004 Character Engine]
        DLG[MOD-005 Dialogue Engine]
        ORG[MOD-006 Organization Engine]
        REL[MOD-007 Relationship Engine]
        ECO[MOD-008 Economy Engine]
        POL[MOD-009 Politics Engine]
        QUEST[MOD-010 Quest Engine]
    end

    SPEECH[[Speech Output Port]]
    MEMCTX[[Memory Read / Context Source Port]]

    subgraph AIM[AI / Media]
        AIA[AI Adapter]
        CTX[Context Manager]
        PROV[Provider Abstraction]
        VOICE[MOD-014 Voice Engine]
    end

    subgraph PERSIST[Persistence]
        SAVE[MOD-013 Save Manager]
        SAVEPORT[[Save Persistence Port]]
        PART[[SaveParticipant Contracts]]
        REP[Repository Implementations]
        DB[(SQLite Database)]
        HIST[[Event History Persistence Port]]
    end

    GAME --> GA
    GA --> GIN
    GIN --> CORE

    CORE --> GOUT
    GA --> GOUT
    GOUT --> GA
    GA --> GAME

    CORE --> EV
    CORE --> SCH
    CORE --> SAVE
    CORE --> DLGPORT
    DLG --> DLGPORT

    SCH --> EV

    CHAR --> MEM
    REL --> CHAR
    ORG --> REL
    ECO --> ORG
    POL --> ORG
    POL --> ECO
    QUEST --> MEM
    QUEST --> REL
    QUEST --> POL

    DLG --> MEM
    DLG --> CHAR
    DLG --> REL
    DLG --> QUEST
    DLG --> AIA
    DLG --> SPEECH
    VOICE --> SPEECH

    AIA --> CTX
    AIA --> PROV
    CTX --> MEMCTX
    MEM --> MEMCTX
    PROV --> LOCALAI
    PROV --> EXTAI

    MEM --> REP
    SAVE --> SAVEPORT
    SAVEPORT --> REP
    SAVE --> PART
    EV --> HIST
    HIST --> REP
    REP --> DB

    PD -. plugin profile evidence .-> GA
    PD -. fingerprint evidence .-> SAVE

    H -. lifecycle wiring .-> GA
    H -. lifecycle wiring .-> PD
    H -. lifecycle wiring .-> CORE
    H -. lifecycle wiring .-> EV
    H -. lifecycle wiring .-> SCH
    H -. lifecycle wiring .-> DLG
    H -. lifecycle wiring .-> AIA
    H -. lifecycle wiring .-> SAVE
    H -. typed config / readiness .-> INT
    H -. typed config / readiness .-> COORD
    H -. typed config / readiness .-> SIM
    H -. typed config / readiness .-> AIM
    H -. typed config / readiness .-> PERSIST
```

## Important interpretation rules

1. The diagram does **not** mean that every simulation module must communicate through Event Engine.
2. Event Engine coexists with approved direct command/query/use-case contracts.
3. Game Adapter never imports or calls Dialogue/Memory/Quest implementations directly.
4. Core does not import the concrete Game Adapter implementation; outbound game operations use Game Output/Capability Port.
5. Dialogue does not call the concrete Game Adapter; it returns the use-case result to Core/Application.
6. Scheduler does not depend directly on Save Manager. Autosave is an event relation shown below.
7. Save Manager does not discover concrete simulation modules through a registry. SaveParticipant implementations are injected by Composition Root.
8. Context Manager does not access Repository/SQLite directly.
9. Voice Engine implements optional speech capability and does not own Dialogue semantics.
10. Host/Composition Root lifecycle arrows are not ordinary module dependencies and Host is not a runtime Service Locator.

## Event relation view

```mermaid
flowchart LR
    SCH[Scheduler] -->|AutosaveDue| EV[Event Engine]
    EV -->|deliver| SAVE[Save Manager]

    CORE[Core/Application] -->|semantic game/world events| EV
    EV -->|deliver| MEM[Memory]
    EV -->|deliver| CHAR[Character]
    EV -->|deliver| REL[Relationship]
    EV -->|deliver| ORG[Organization]
    EV -->|deliver| ECO[Economy]
    EV -->|deliver| POL[Politics]
    EV -->|deliver| QUEST[Quest]

    PD[Plugin Detection] -->|ProfileDetected / ProfileChanged| EV
    EV -->|evidence notification| GA[Game Adapter]
    EV -->|evidence notification| SAVE2[Save compatibility consumer]
```

Event arrows above represent producer/subscriber relations only. They do not change the static dependency graph in ARCH-005.

## Legacy corrections captured

This source removes the following legacy ARCH-006 problems:

- `All modules communicate through Event Engine` statement;
- Core `services registry` / Service Locator implication;
- Save Manager shown as Simulation component;
- Voice shown in the wrong ownership layer;
- missing AI Adapter boundary;
- Scheduler-to-Database/persistence visual coupling;
- ambiguous bidirectional module arrows with no contract semantics.
