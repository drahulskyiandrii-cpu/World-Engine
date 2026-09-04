# ARCH-007 — Sequence Diagrams — Audited Source v3

**Status:** Audit branch source candidate  
**Baseline:** ADR-002, ADR-005, ADR-006, ADR-008 + amendments; ARCH-004/005 audited model  
**Date:** 2026-09-01

## 1. Sequence semantics

Unless explicitly labelled otherwise, arrows in these diagrams represent **runtime calls/messages**, not static implementation dependencies.

Tags:

- `[SYNC]` — synchronous command/query/use-case call;
- `[EVENT]` — publication/delivery through Event Engine;
- `[PERSIST]` — persistence operation through an approved persistence boundary;
- `[AI]` — AI request/response behind AI Adapter;
- `[EVIDENCE]` — readiness/capability/profile/status evidence;
- `[LIFECYCLE]` — Application Host / Composition Root wiring/startup/shutdown activity.

Static dependency direction remains defined by ARCH-005.

---

## SD-001 — NPC Conversation

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant EE as Event Engine
    participant D as Dialogue Engine
    participant M as Memory Engine
    participant CH as Character Engine
    participant R as Relationship Engine
    participant Q as Quest Engine
    participant AI as AI Adapter
    participant CM as Context Manager
    participant P as Provider Abstraction
    participant V as Voice Engine

    G->>GA: Player/NPC interaction
    GA->>C: Game Ingress Port — normalized conversation command [SYNC]
    C->>EE: ConversationStarted [EVENT]
    C->>D: Dialogue Command Port [SYNC]
    D->>M: approved memory read [SYNC]
    D->>CH: approved character read [SYNC]
    D->>R: approved relationship read [SYNC]
    D->>Q: optional quest-context read [SYNC]

    opt AI required
        D->>AI: AI request [AI]
        AI->>CM: build approved context [AI]
        CM->>M: Memory Read / Context Source [SYNC]
        M-->>CM: normalized context records
        AI->>P: provider-neutral inference request [AI]
        P-->>AI: provider result [AI]
        AI-->>D: normalized AI result [AI]
        D->>D: deterministic validation
    end

    opt significant new memory/fact
        D->>M: submit validated memory candidate [SYNC]
    end

    opt speech capability enabled
        D->>V: Speech Output Port [SYNC/optional media]
        V-->>D: speech result/status
    end

    D-->>C: final dialogue result [SYNC]
    C->>GA: Game Output/Capability Port — approved response [SYNC]
    GA->>G: render/speak/act in game
    C->>EE: ConversationCompleted / semantic follow-up events [EVENT]
```

### Audit corrections

- Event Engine no longer acts as RPC transport for the dialogue result.
- Game Adapter never calls Dialogue directly.
- Dialogue never calls concrete Game Adapter directly.
- AI always enters through AI Adapter; Context Manager remains behind that boundary.
- Voice is optional media output and does not own dialogue semantics.

---

## SD-002 — NPC Death

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant EE as Event Engine
    participant CH as Character Engine
    participant M as Memory Engine
    participant R as Relationship Engine
    participant Q as Quest Engine
    participant RP as Repository Ports

    G->>GA: NPC death observed
    GA->>C: normalized NPC death ingress [SYNC]
    C->>EE: NPC_Died [EVENT]
    EE-->>CH: NPC_Died [EVENT]
    EE-->>M: NPC_Died [EVENT]
    EE-->>R: NPC_Died [EVENT]
    EE-->>Q: NPC_Died [EVENT]

    CH->>CH: update owned character state
    M->>M: create/update owned memory records
    R->>R: update owned relationship state
    Q->>Q: update owned quest state if applicable

    opt normal durable domain persistence required
        CH->>RP: dedicated character persistence [PERSIST]
        M->>RP: dedicated memory persistence [PERSIST]
        R->>RP: dedicated relationship persistence [PERSIST]
        Q->>RP: dedicated quest persistence [PERSIST]
    end

    Note over C,Q: No Save Manager is required merely because a semantic event changed domain state.
```

### Audit corrections

- NPC death no longer routes ordinary persistence through Save Manager.
- Save Manager remains checkpoint/save-artifact coordinator, not generic domain persistence coordinator.
- Each subscriber updates only owned state.

---

## SD-003 — Plugin Detection

```mermaid
sequenceDiagram
    participant H as Application Host / Composition Root
    participant PD as Plugin Detection
    participant FS as Allowed Plugin Sources
    participant RP as Plugin Baseline Repository Port
    participant EE as Event Engine
    participant GA as Game Adapter
    participant SM as Save Manager

    H->>PD: construct + inject typed configuration [LIFECYCLE]
    H->>PD: start initial detection cycle [LIFECYCLE]
    PD->>FS: discover allowed plugin inputs
    FS-->>PD: discovered files/metadata
    PD->>PD: normalize + validate + fingerprint
    PD->>RP: read prior detection baseline [PERSIST]
    RP-->>PD: prior baseline
    PD->>PD: compute profile/diff
    PD->>RP: persist new detection evidence [PERSIST]
    PD->>EE: ProfileDetected / ProfileChanged [EVENT]
    EE-->>GA: plugin profile evidence [EVENT/EVIDENCE]
    EE-->>SM: relevant fingerprint/profile evidence [EVENT/EVIDENCE]
    GA->>GA: capability mapping decision
    SM->>SM: restore-compatibility policy remains Save-owned
```

### Audit corrections

- Core and Scheduler no longer own Plugin Detection lifecycle.
- Plugin Detection receives typed configuration through Host wiring instead of reading global Configuration directly at runtime.
- Detection evidence is distinct from Game Adapter capability mapping and Save Manager restore compatibility authority.
- No plugin code execution is implied.

---

## SD-004 — Save World

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant SM as Save Manager
    participant SP as SaveParticipants
    participant RP as Save Persistence Port / Repository
    participant DB as SQLite

    G->>GA: manual/application save request
    GA->>C: normalized save ingress [SYNC]
    C->>SM: Save command/use-case contract [SYNC]
    SM->>SP: collect coherent snapshots [SYNC]
    SP-->>SM: participant snapshots + validation metadata
    SM->>SM: stage candidate save artifact
    SM->>RP: begin controlled save persistence [PERSIST]
    RP->>DB: transaction/write [PERSIST]
    DB-->>RP: commit result
    RP-->>SM: persistence result
    SM->>SM: validate committed artifact
    SM->>SM: promote candidate to known-good checkpoint
    SM-->>C: save result [SYNC]
    C->>GA: approved save status via Game Output Port [SYNC]
    GA->>G: display/acknowledge result
```

### Audit corrections

- Save Manager uses injected SaveParticipant contracts rather than inspecting private module state.
- Save remains `Core/Application -> Save Manager`; storage remains behind Save Persistence/Repository boundary.

---

## SD-005 — Load World

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant SM as Save Manager
    participant RP as Save Persistence Port / Repository
    participant DB as SQLite
    participant SP as SaveParticipants

    G->>GA: load request
    GA->>C: normalized load ingress [SYNC]
    C->>SM: Load command/use-case contract [SYNC]
    SM->>RP: read selected artifact/metadata [PERSIST]
    RP->>DB: read snapshot [PERSIST]
    DB-->>RP: persisted package
    RP-->>SM: artifact + metadata
    SM->>SM: validate version/integrity/compatibility
    SM->>SP: stage + validate participant state [SYNC]
    SP-->>SM: validation result

    alt validation succeeds
        SM->>SP: apply via state-owner restore contracts [SYNC]
        SP-->>SM: post-apply validation
        SM-->>C: restore ready for application activation
        C->>GA: Game Output/Capability Port — resume/apply game-facing state [SYNC]
        GA->>G: resume with restored World Engine state
    else validation/apply fails
        SM->>SM: abort activation; retain known-good runtime/persisted state
        SM-->>C: controlled load failure
        C->>GA: failure status via Game Output Port [SYNC]
        GA->>G: display controlled failure
    end
```

### Audit corrections

- Restore is explicitly staged and validated before activation.
- Partial restore is never reported as success.
- Game Adapter does not own Save semantics.
- State-owner validation is not bypassed by Save Manager.

---

## SD-006 — Scheduled World Update / Autosave

```mermaid
sequenceDiagram
    participant T as System/Game Time
    participant S as Scheduler
    participant EE as Event Engine
    participant DM as Interested Domain Modules
    participant SM as Save Manager

    T->>S: due time reached
    S->>S: evaluate schedule/budget/enabled state
    S->>EE: ScheduledTaskTriggered [EVENT]
    EE-->>DM: deliver typed scheduled semantic event [EVENT]
    DM->>DM: update owned state according to domain policy
    DM->>EE: optional follow-up semantic events [EVENT]
    S->>S: record next-run state

    opt autosave schedule due
        S->>EE: AutosaveDue [EVENT]
        EE-->>SM: deliver AutosaveDue [EVENT]
        SM->>SM: evaluate Save-owned autosave policy/safe-point evidence
        Note over SM: If allowed, normal Save Contract sequence begins.
    end
```

### Audit corrections

- Removed `Scheduler -> Core` callback dependency.
- Removed direct `Scheduler -> Save Manager` dependency.
- Scheduler owns timing; domain modules own domain logic; Save Manager owns autosave/save semantics.

---

## SD-007 — Knowledge Acquisition

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant D as Dialogue Engine
    participant AI as AI Adapter
    participant M as Memory Engine
    participant RP as Memory Repository Port
    participant EE as Event Engine

    G->>GA: player shows/tells/interacts with knowledge-bearing object
    GA->>C: normalized interaction ingress [SYNC]
    C->>D: Dialogue/Knowledge use-case command [SYNC]
    D->>D: collect deterministic object/player/NPC context

    opt semantic interpretation requires AI
        D->>AI: interpretation request [AI]
        AI-->>D: normalized candidate interpretation [AI]
        D->>D: deterministic validation
    end

    D->>M: submit KnowledgeCandidate with provenance/source [SYNC]
    M->>M: validate type, provenance, duplication, retention policy

    alt accepted
        M->>RP: persist owned knowledge/memory state [PERSIST]
        M->>EE: KnowledgeAcquired / MemoryUpdated [EVENT]
        EE-->>D: optional reaction notification [EVENT]
    else rejected
        M-->>D: candidate rejected with reason [SYNC]
    end

    D-->>C: use-case response
    C->>GA: Game Output Port — dialogue/feedback result [SYNC]
    GA->>G: display NPC response/action
```

### Audit corrections

- Removed Game Adapter -> Dialogue bypass.
- Memory Engine owns memory/knowledge validation and persistence.
- AI supplies candidate interpretation only and does not write persistent memory directly.

---

## 2. Cross-scenario invariants

1. Game Adapter communicates with the application through ingress/output capability ports; it does not call domain modules directly.
2. Dialogue never calls the concrete Game Adapter.
3. Event Engine carries semantic events, not generic request/response RPC.
4. Scheduler owns time/triggers, not persistence or domain policy.
5. Save Manager owns Save Contract and checkpoint/restore semantics, not ordinary domain persistence.
6. Repository/SQLite access remains behind dedicated persistence ports.
7. AI requests enter through AI Adapter; AI output remains non-authoritative until deterministic validation.
8. Plugin Detection owns discovery/profile evidence only; capability and restore compatibility have separate owners.
9. Application Host / Composition Root owns top-level construction/lifecycle wiring.
10. Runtime arrows in this document must not be copied into ARCH-005 as static dependencies without explicit contract analysis.

## 3. Legacy issues resolved

This source explicitly resolves audit findings corresponding to:

- direct `Dialogue Engine -> Game Adapter`;
- `Game Adapter -> Core/Dialogue Engine` bypass;
- Event Engine used as dialogue RPC;
- `Core/Scheduler -> Plugin Detection Subsystem` ownership ambiguity;
- NPC-death persistence through Save Manager;
- `Scheduler -> Core -> Save Manager` autosave chain;
- load flow without a sufficiently explicit staging/activation barrier.
