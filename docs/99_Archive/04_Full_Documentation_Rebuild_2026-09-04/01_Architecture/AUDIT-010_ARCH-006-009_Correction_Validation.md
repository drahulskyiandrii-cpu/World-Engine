# AUDIT-010 — ARCH-006–ARCH-009 Correction Validation

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Correction validation complete for ARCH-006/007; correction specification frozen for ARCH-008/009  
**Date:** 2026-09-01

## 1. Scope

This pass validates the audited replacement sources for ARCH-006 and ARCH-007 against the resolved dependency baseline, then freezes the exact corrections required for ARCH-008 Deployment Architecture and ARCH-009 Configuration Architecture.

The baseline remains ADR-008 semantics + ADR amendments + audited ARCH-004/005 dependency model. No new fundamental dependency edge is introduced by this pass.

## 2. ARCH-006 validation

ARCH-006 audited source v1.2 is accepted as the current diagram source candidate with the following interpretation:

- port binding is not a concrete reverse implementation dependency;
- Host/Composition Root lifecycle wiring is not an ordinary runtime module dependency;
- event relations are separate from the static dependency graph;
- provider implementations remain behind the AI provider abstraction;
- repository implementations remain behind dedicated persistence ports;
- Game Adapter runtime bidirectionality is represented through Game Ingress and Game Output/Capability contracts, not a Core<->GameAdapter implementation cycle.

The legacy statements `All modules communicate through Event Engine` and Core `services registry` must not reappear in regenerated diagram artifacts.

## 3. ARCH-007 validation

ARCH-007 audited source v3 is accepted as the current runtime-sequence source candidate, except for one additional Save/Restore requirement:

### A007-R1 — Atomic restore activation barrier — High

The load sequence must explicitly distinguish:

1. artifact read and validation;
2. participant staging into non-live candidate state;
3. cross-participant validation;
4. atomic/controlled activation barrier;
5. post-activation validation;
6. rollback/recovery if activation cannot complete coherently.

A participant failure must not leave the application reporting a successful restore with only part of the live world updated. Exact implementation may use transactional snapshots, reversible participant apply, shadow state, or another mechanism defined later, but the architecture contract is atomic logical activation.

This refinement must be synchronized with ARCH-011.

## 4. ARCH-008 correction decisions

ARCH-008 remains a valid deployment foundation but requires the following normative corrections.

### A008-C1 — Process containment vs architectural ownership

Use `hosts/contains` for process composition. The World Engine process does not semantically own Core, Event Engine, Scheduler, Simulation, Save Manager or AI Adapter simply because they execute in the same process.

### A008-C2 — Application Host / Composition Root

Introduce Application Host / Composition Root as the process-level owner of:

- component construction;
- dependency injection;
- configuration/profile selection;
- startup ordering;
- application readiness aggregation;
- top-level shutdown ordering;
- process-level recovery decisions.

Core owns Core coordination semantics and its own lifecycle only.

### A008-C3 — Process diagram

All main-process modules are siblings hosted by the World Engine process/Host. Event Engine, Scheduler, Simulation, Dialogue, AI Adapter and Save Manager must not appear nested under MOD-001 Core.

### A008-C4 — Local AI transport

Replace fixed `localhost HTTP API` wording with:

`loopback/IPC local transport behind AI Adapter; concrete protocol/port TBD by dedicated technical decision`.

HTTP over loopback remains a possible implementation, not an architecture commitment in ARCH-008.

### A008-C5 — Startup lifecycle scope

Startup is an application lifecycle coordinated by Host/Composition Root. Required sequence remains conceptually valid, but wording must not imply Core directly instantiates Repository, Game Adapter or AI Adapter.

### A008-C6 — Shutdown lifecycle scope

Host/Composition Root coordinates shutdown ordering. Core may stop accepting new Core-owned non-critical workflows; Scheduler, Event Engine, Save Manager, AI Adapter and persistence each execute their own public shutdown contracts.

### A008-C7 — Readiness scope

`Ready`, `Degraded`, `Startup Failed` and `Recovery Required` in deployment architecture are **application-level outcomes** unless explicitly qualified otherwise. Component states use scoped names/evidence.

### A008-C8 — Persistence boundary

Deployment diagrams may show SQLite physically inside the installation, but normal modules access it only through Repository/persistence implementations. `World Engine -> SQLite` must not imply arbitrary module-level database access.

### A008-C9 — Save authority wording

Replace `last valid save is authoritative state` with `last valid save is the authoritative persisted/restorable checkpoint`. Live runtime state may advance after a checkpoint.

## 5. ARCH-009 correction decisions

ARCH-009 requires clarification, not redesign.

### A009-C1 — Configuration ownership

Configuration subsystem owns parsing, source hierarchy, merge, schema validation, normalization, secret-reference resolution and effective-configuration construction.

Core does not own YAML schema or global configuration distribution.

### A009-C2 — Typed distribution through composition

Canonical startup path:

`Configuration sources -> Configuration subsystem -> validated effective configuration -> Application Host/Composition Root -> typed configuration views -> components`.

No component receives unrestricted access to the full raw YAML tree.

### A009-C3 — Startup outcome scope

The configuration document may contribute validation evidence to application startup, but `Ready / Degraded / Startup Failed / Recovery Required` are application startup outcomes aggregated by Host, not MOD-001 Core states.

### A009-C4 — Runtime API consumers

Configuration health/schema/profile diagnostics belong primarily to Configuration subsystem + Host/Diagnostics. Core may receive only the typed values or narrow status evidence required by a documented Core responsibility.

### A009-C5 — Local AI endpoint examples

Examples must not prematurely fix localhost HTTP as the mandatory local transport. Use a transport-neutral local-runtime endpoint/reference example and defer concrete protocol/port to the deployment/technical decision.

### A009-C6 — Hot reload

Existing conservative hot-reload model is retained. Candidate effective configuration is validated before controlled atomic apply; failure reverts to the previous accepted effective runtime state.

### A009-C7 — Secrets

Existing secrets-by-reference and least-exposure model is retained. Secret values must not be exposed through diagnostics or broad configuration objects.

## 6. Cross-document consistency result

After the corrections above:

- no new static dependency cycles are required;
- Host/Composition Root remains the only top-level lifecycle wiring owner;
- Core does not become a service locator;
- deployment status and module status are scoped separately;
- local AI transport remains replaceable;
- Configuration remains an independent subsystem rather than a Core-owned facility.

## 7. Next gate

1. Regenerate ARCH-008 RU+DE audited candidates.
2. Regenerate ARCH-009 RU+DE audited candidates.
3. Rebuild malformed ARCH-010 RU+DE from Parts 01–08 with the real Part 02.
4. Apply Plugin Detection lifecycle clarification to rebuilt ARCH-010.
5. Reconcile ARCH-011 with atomic restore activation and event-triggered autosave.
6. Reconcile ARCH-012 with the canonical AI entry path.

MOD-001 remains blocked for re-audit until ARCH-008–012 are stabilized.
