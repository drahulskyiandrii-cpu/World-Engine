# ARCH-007 — SD-005 Restore Activation Amendment v4

**Project:** World Engine  
**Status:** Audit correction source for final ARCH-007 regeneration  
**Baseline:** ARCH-011 audited Save System v1.1 + ADR-005 amendment + ADR-008  
**Date:** 2026-09-01

## Purpose

ARCH-007 audited source v3 already removed the legacy Game Adapter/Dialogue bypasses and corrected Save/Load ownership, but SD-005 still needs a stronger all-or-nothing activation barrier so it cannot be implemented as partial live-state mutation.

This amendment replaces the restore portion of SD-005 in the final audited ARCH-007.

## Corrected SD-005 — Load World

```mermaid
sequenceDiagram
    participant G as Game Runtime
    participant GA as Game Adapter
    participant C as Core/Application
    participant SM as Save Manager
    participant RP as Save Persistence Port / Repository
    participant DB as SQLite
    participant SP as SaveParticipants
    participant H as Application Host / Activation Gate

    G->>GA: load request
    GA->>C: normalized load ingress [SYNC]
    C->>SM: Load command/use-case contract [SYNC]

    SM->>RP: read selected artifact/metadata [PERSIST]
    RP->>DB: read persisted checkpoint [PERSIST]
    DB-->>RP: persisted package
    RP-->>SM: artifact + metadata

    SM->>SM: validate integrity/schema/version/compatibility
    SM->>SM: deserialize into non-live candidate fragments
    SM->>SP: StageRestore for all required participants [SYNC]
    SP-->>SM: staged fragments + validation evidence
    SM->>SP: ValidateStagedRestore / cross-participant invariants [SYNC]
    SP-->>SM: pre-activation validation result

    alt all required validation succeeds
        SM->>H: request atomic logical activation barrier [SYNC]
        H->>SP: CommitActivation under one restore operation context [SYNC]
        SP-->>H: activation result

        alt all required participants activated
            H->>SP: PostActivationValidate [SYNC]
            SP-->>H: post-validation success
            H-->>SM: activation committed
            SM-->>C: load activated successfully
            C->>GA: Game Output/Capability Port — resume/apply game-facing state [SYNC]
            GA->>G: resume with restored World Engine state
            SM->>SM: publish LoadActivated/LoadCompleted fact [EVENT after success]
        else activation or post-validation fails
            H->>SP: Abort/RollbackActivation according to implementation strategy [SYNC]
            SP-->>H: rollback/recovery evidence
            H-->>SM: activation failed / recovery required
            SM-->>C: controlled load failure or RecoveryRequired
            C->>GA: failure/recovery status via Game Output Port [SYNC]
            GA->>G: display controlled failure / do not report success
        end
    else validation fails
        SM->>SP: discard staged candidate [SYNC]
        SM-->>C: controlled load validation failure
        C->>GA: failure status via Game Output Port [SYNC]
        GA->>G: display controlled failure
    end
```

## Mandatory invariants

1. Before activation, the existing live runtime state remains authoritative.
2. Deserialization never writes directly into active participant state.
3. Every required SaveParticipant stages state before any participant becomes authoritative.
4. Cross-participant references/invariants are validated before activation.
5. Activation is one logical all-or-nothing operation, even if the mechanical implementation uses multiple steps.
6. If one required participant cannot activate, the load cannot report success.
7. Rollback/recovery strategy must restore an acceptable pre-load or recovery state.
8. `LoadCompleted`/normal application resume occurs only after post-activation validation succeeds.
9. Game Adapter does not own Save semantics or participant activation.
10. Exact atomic mechanism is deferred to MOD-013/implementation ADR; observable semantics are fixed here.

## Allowed implementation strategies

The architecture permits any implementation that provides equivalent logical all-or-nothing behavior, for example:

- shadow-state construction + atomic pointer/state swap;
- transaction-like participant barrier;
- reversible participant apply with guaranteed rollback;
- immutable snapshot replacement;
- another explicitly tested mechanism.

The final MOD-013 specification must select/define the concrete strategy before implementation freeze.
