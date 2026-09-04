# ARCH-011 — Save System Architecture — Audited Source v1.1

**Status:** Audit branch source candidate  
**Baseline:** ADR-005 + amendment; ADR-008; ARCH-004/005 audited model; ARCH-007 audited sequences  
**Date:** 2026-09-01

## 1. Scope retained

The v1.0 Save architecture remains valid in its major design:

- Save Manager owns Save/Restore lifecycle semantics;
- Save artifacts are versioned and validated;
- failed/incomplete writes never replace a known-good checkpoint;
- storage is behind Repository/persistence boundaries;
- restore is staged and validated before activation;
- migrations, backups and recovery are explicit;
- only one authoritative save commit is active per logical world/session;
- Save/Load conflict at the activation/commit boundary;
- Save System is not ordinary domain persistence.

The audit correction clarifies authority, trigger paths, participant contracts and restore activation.

## 2. Authority model

A committed Save is the **authoritative persisted/restorable checkpoint artifact** for the relevant slot/world/session.

It is not necessarily the newest live runtime truth after gameplay continues.

Authority is separated:

- state-owning modules own active runtime/domain semantics;
- Save Manager owns Save Contract, checkpoint artifact lifecycle and restore orchestration;
- Repository/Database owns storage mechanics;
- Game Adapter owns normalized game-integration evidence;
- Host/Application owns top-level activation/readiness coordination.

## 3. Persistence separation

Normal durable domain persistence and Save/Checkpoint orchestration are related but distinct.

A state-owning module may use a dedicated repository contract for normal durable persistence when its module specification requires it.

Save Manager does not become a generic per-event CRUD gateway for all modules.

Physical SQLite access remains behind Repository implementations.

## 4. Save participation contracts

Save Manager coordinates state owners through injected explicit contracts, not a runtime module registry/service locator.

Logical contract family:

```text
SaveParticipant
  - ParticipantId / StateContractVersion
  - PrepareSnapshot(context)
  - ValidateSnapshot(candidate)
  - StageRestore(fragment, context)
  - ValidateStagedRestore(context)
  - CommitActivation(context)
  - Abort/RollbackActivation(context) where required by implementation strategy
  - PostActivationValidate(context)
```

Exact API shape is deferred to MOD-013/API specifications. The architecture requirement is explicit ownership, deterministic ordering where needed, no private-state inspection and no concrete-module discovery by Save Manager.

Composition Root supplies the participant set.

## 5. Manual save path

Canonical synchronous use case:

`Game/User Request -> Game Adapter -> Game Ingress -> Core/Application Save Use-Case -> Save Manager Command Contract`

Save Manager then:

1. establishes one save operation/correlation context;
2. collects coherent snapshots from SaveParticipants;
3. validates required participant snapshot set;
4. serializes/stages candidate artifact;
5. writes through Save Persistence/Repository contract;
6. validates persisted candidate;
7. atomically promotes candidate as the new known-good persisted checkpoint;
8. emits/returns typed result;
9. publishes lifecycle event only after the corresponding state transition is real.

Core/Application receives a save result but does not own persistence implementation.

## 6. Autosave trigger path

Canonical v1.0 autosave trigger:

`Scheduler -> Event Engine : AutosaveDue -> Save Manager subscriber`

Rules:

- Scheduler owns timing/cadence only;
- Scheduler does not directly call Save Manager;
- Scheduler does not receive Save Manager result to implement persistence retry policy;
- Save Manager/application save policy decides coalescing, deferral, retry eligibility and safe execution;
- lifecycle/result events may be observed for diagnostics/telemetry without creating reverse static coupling.

## 7. Game integration evidence

Game Adapter may provide normalized evidence such as:

- game session attached/detached;
- safe/unsafe integration window;
- capability availability;
- game-side state synchronization prerequisites;
- active Game Profile/plugin capability evidence where relevant.

Game Adapter **does not own autosave policy**.

Save Manager or an explicit application save policy consumes the evidence and decides whether to execute/defer/reject a checkpoint operation.

## 8. Save concurrency

Preserve the v1.0 invariants:

- one authoritative save commit per logical world/session at a time;
- manual save has policy-defined priority over deferred autosave;
- duplicate autosave requests may coalesce;
- save/load/migration operations are mutually constrained at commit/activation boundaries;
- lock scope is minimal and must not wrap slow external/provider/game calls;
- cancellation is allowed only before the architecture-defined irreversible/promotion boundary.

## 9. Restore phases

Restore is not direct mutation from deserialized bytes into live state.

Canonical phases:

```text
Read Artifact
 -> Validate Artifact Metadata/Integrity/Version/Compatibility
 -> Deserialize into non-live candidate fragments
 -> Stage all SaveParticipants
 -> Cross-participant validation
 -> Atomic logical activation barrier
 -> Participant activation/commit
 -> Post-activation validation
 -> Application resume / LoadCompleted
```

## 10. Atomic logical activation barrier

The key audited requirement is **logical all-or-nothing activation** across required participants.

A restore must never report success if only part of the required world state became active.

Before activation:

- the existing live state remains authoritative;
- all required participant fragments are staged outside the active state or behind a reversible/transactional boundary;
- cross-participant references/invariants are validated;
- plugin/Game Profile compatibility policy has completed where required;
- conflicting Save/Migration operations are excluded.

At activation:

- activation occurs under one restore operation context;
- participant ordering/dependency constraints are explicit;
- implementation must support either atomic swap, shadow-state activation, reversible participant apply, transaction-like barrier, or another mechanism that provides equivalent logical all-or-nothing behavior.

After activation:

- post-activation validation executes before `LoadCompleted`/normal resume;
- failure triggers rollback/recovery according to the selected implementation strategy;
- partial success is never returned.

The exact mechanical strategy is deferred to MOD-013/implementation ADR if necessary.

## 11. Activation coordination ownership

- Save Manager owns artifact reconstruction, staging and restore orchestration.
- State-owning modules own validation/application of their state fragments.
- Application Host/Core coordination boundary owns the cross-system activation gate and application resume decision.
- Game Adapter only applies/receives game-facing integration actions through approved Game Output/Capability ports after the restore candidate is accepted for activation.

No direct Save Manager access to private module internals is permitted.

## 12. Restore compatibility

Save Manager owns restore compatibility decision using evidence from:

- Save Contract/schema/version;
- Plugin Profile/fingerprint evidence;
- active Game Profile/capability evidence;
- participant contract versions/migration availability;
- required configuration/environment constraints.

Plugin Detection supplies evidence; it does not decide restore compatibility.

Game Adapter supplies capability evidence; it does not decide Save compatibility.

## 13. Checkpoint lifecycle terms

Recommended terms:

- `Candidate` — staged but not accepted;
- `ValidatedCandidate` — passed required pre-commit checks;
- `CommittedCheckpoint` — persisted successfully;
- `KnownGoodCheckpoint` — committed and eligible as recovery/load source;
- `StagedRestore` — deserialized/validated but not active;
- `ActiveRuntimeState` — state currently owned by domain/runtime modules;
- `RecoveryRequired` — normal activation cannot safely complete.

Avoid using `authoritative state` without specifying persisted checkpoint vs live runtime state.

## 14. Save events

Events describe lifecycle facts only after the fact becomes true, for example conceptually:

- SaveStarted;
- SaveCommitted;
- SaveFailed;
- AutosaveDeferred;
- LoadStarted;
- RestoreStaged;
- LoadActivated;
- LoadFailed;
- RecoveryRequired.

Exact names belong to the Event Catalog stage.

`SaveCommitted` means the checkpoint has been committed/promoted; event publication is not part of storage ownership and cannot make an uncommitted candidate authoritative.

## 15. Recovery retained

Preserve:

- temporary artifacts never automatically become authoritative;
- startup detects incomplete operations;
- previous known-good checkpoints remain recoverable where policy permits;
- corruption/incompatible versions route to controlled recovery;
- failed migration preserves the original source artifact;
- backup/rotation policy is bounded and diagnosable.

## 16. Performance constraints

- snapshot collection is bounded and observable;
- serialization may use staging/background work where coherence is preserved;
- Save Manager must not create an unbounded save/autosave queue;
- autosave may defer under high load;
- persistence retry is bounded and Save-owned;
- restore validation performs only work capable of affecting activation decision on the critical path;
- operation IDs/correlation are mandatory for diagnosis.

## 17. Superseded v1.0 interpretations

The following readings are superseded:

- last valid Save is always the newest live world truth;
- ordinary domain persistence must flow through Save Manager;
- Scheduler directly invokes Save Manager for autosave;
- Scheduler owns Save retry policy from Save Manager result;
- Game Adapter policy decides whether autosave may run;
- Save Manager can directly mutate all live module state without participant-owned staging/activation contracts;
- post-activation rollback wording alone is sufficient without an explicit logical all-or-nothing activation barrier.

## 18. Acceptance criteria after audit

ARCH-011 is accepted when tests prove:

1. failed save never replaces known-good checkpoint;
2. autosave uses Event Engine trigger without Scheduler->SaveManager static dependency;
3. SaveParticipants are injected explicitly;
4. normal domain persistence does not require Save Manager;
5. partial/failed restore never reports success;
6. atomic logical activation/rollback strategy is demonstrated;
7. game safe-point evidence does not transfer policy ownership to Game Adapter;
8. restore compatibility ownership remains Save Manager;
9. SaveCommitted occurs only after real checkpoint commit/promotion;
10. dependency graph remains acyclic.
