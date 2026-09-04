# AUDIT-010 — Cross-Document Validation of ARCH-007 through ARCH-012

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Baseline:** ADR-008 + ADR-001–007 audit amendments + ARCH-004/005 audited dependency model  
**Status:** Complete — semantic cross-document validation; regeneration still pending for several DOCX artifacts  
**Date:** 2026-09-01

## 1. Purpose

This pass validates the corrected downstream architecture sources as one system. The objective is to confirm that sequence behavior, deployment, configuration, plugin detection, Save/Restore and AI integration now use the same ownership/contract semantics and do not reintroduce the dependency cycles and boundary bypasses found in the legacy documents.

The pass distinguishes:

- static implementation dependency;
- port/contract binding;
- runtime call direction;
- event relation;
- persistence relation;
- evidence flow;
- lifecycle wiring.

A runtime arrow is not copied into ARCH-005 unless it creates an actual static dependency.

## 2. Validation summary

| Document | Semantic result | Remaining action |
|---|---|---|
| ARCH-007 Sequence Diagrams | PASS with one required refinement | Add explicit all-or-nothing restore activation barrier to SD-005 before final DOCX regeneration. |
| ARCH-008 Deployment Architecture | PASS after audited correction source | Regenerate/QA RU+DE final audited DOCX; process hosts siblings through Application Host/Composition Root. |
| ARCH-009 Configuration Architecture | PASS after audited correction source | Regenerate/QA RU+DE final audited DOCX; typed distribution is Host-owned, not Core-owned. |
| ARCH-010 Plugin Architecture | FAIL legacy Complete artifact; reconstruction approved | Rebuild Part 02 from reconstruction source, then rebuild Complete RU+DE and run full parity/visual QA. |
| ARCH-011 Save System Architecture | PASS audited source | Regenerate RU+DE final audited DOCX and align ARCH-007 SD-005 to the same activation barrier. |
| ARCH-012 AI Integration Architecture | PASS audited source | Regenerate RU+DE final audited DOCX and verify Dialogue/MOD-001 contracts against final ARCH-005. |

No new dependency-model redesign is required by this pass.

## 3. ARCH-007 ↔ ARCH-005 validation

### 3.1 Conversation

Corrected runtime path:

`Game Adapter -> Game Ingress -> Core/Application -> Dialogue Command Port -> Dialogue Engine`

Response path:

`Dialogue result -> Core/Application -> Game Output/Capability Port -> Game Adapter -> Game Runtime`

This is compatible with the audited static model because:

- Game Adapter does not call Dialogue implementation directly;
- Core/Application depends on an explicit Dialogue use-case port, not the concrete Dialogue implementation;
- Game Adapter implements the outbound Game Output/Capability Port;
- Event Engine may receive `ConversationStarted/Completed` semantic events but is not used as request/response RPC.

**Result:** PASS.

### 3.2 Voice

Runtime `Dialogue -> Voice` is represented through `Speech Output Port`.

- Dialogue consumes the port;
- Voice Engine implements the optional port;
- no concrete Dialogue<->Voice implementation cycle is required.

**Result:** PASS.

### 3.3 Scheduler/autosave

Corrected path:

`Scheduler -> Event Engine : AutosaveDue -> Save Manager subscriber`

This preserves:

- `Core -> Scheduler` where Core needs Scheduler control contract;
- `Scheduler -> Event Engine` for scheduled semantic triggers;
- no `Scheduler -> Core` callback dependency;
- no direct `Scheduler -> Save Manager` dependency.

**Result:** PASS.

### 3.4 Plugin Detection

Application Host/Composition Root owns detector construction and startup wiring. Plugin Detection publishes normalized profile/fingerprint evidence. Game Adapter owns capability mapping; Save Manager owns restore compatibility.

**Result:** PASS.

### 3.5 Load/Restore

ARCH-007 v3 correctly stages/validates before activation but is still less explicit than ARCH-011 v1.1 regarding the activation barrier.

Required final SD-005 invariant:

`Read -> Validate -> Deserialize non-live -> Stage all participants -> Cross-validate -> Atomic logical activation barrier -> Activate/commit -> Post-validate -> Application resume`

Before the barrier, existing live state remains authoritative. If one required participant cannot activate, the load must not report success and rollback/recovery semantics apply.

**Result:** PASS WITH REQUIRED REFINEMENT.

## 4. ARCH-008 ↔ ARCH-004/005/009/012 validation

The audited deployment model is consistent when interpreted as:

- World Engine Process **hosts** components;
- Application Host / Composition Root constructs and wires components;
- Core does not instantiate/own Event Engine, Scheduler, Game Adapter, Save Manager, Repository or AI provider lifecycle;
- Local AI is a separate optional process;
- local AI transport is loopback/IPC behind AI Adapter, concrete protocol/port TBD by technical decision;
- SQLite access remains behind Repository/Database implementation;
- process/application readiness is aggregated by Host.

The following legacy readings remain superseded:

- Core contains/owns all runtime modules;
- `localhost HTTP` is already a mandatory architecture decision;
- AI unavailability automatically means Core itself is Degraded;
- process startup equals MOD-001 Core startup.

**Result:** PASS for audited source; binary regeneration pending.

## 5. ARCH-009 ↔ ARCH-004/008/MOD-001 validation

Corrected configuration ownership:

`Defaults/Base/Profile/Overrides -> Configuration Subsystem -> Validated Effective Configuration -> Application Host -> typed component views`

Validation confirms:

- schema/parsing/normalization belong to Configuration subsystem;
- Host owns typed distribution and application startup use of effective config;
- components do not retrieve arbitrary global configuration through Core;
- effective configuration is immutable by default after startup;
- hot reload is controlled and explicit;
- secrets remain references and are not exposed to ordinary modules;
- `Application Ready/Degraded/Startup Failed` is distinct from `CoreReady` and component health.

**Result:** PASS for audited source; binary regeneration pending.

## 6. ARCH-010 structural and semantic validation

### 6.1 Legacy artifact integrity

The committed legacy Complete RU+DE artifacts are not valid canonical files because their merge contains duplicated Part 01 and no genuine Part 02 body.

The separately named legacy Part 02 files also contain Part 01 content, so the missing source cannot be recovered from the repository as an original document.

**Status:** legacy Complete artifact FAIL.

### 6.2 Reconstruction rule

The audit reconstruction is explicitly marked **reconstructed, not recovered** and is derived only from sources that support the missing behavior:

- ADR-006 Plugin Detection;
- ARCH-010 Part 01 and Parts 03–08;
- corrected ARCH-007 Plugin Detection sequence;
- audited ARCH-004/005 dependency/lifecycle rules.

The reconstructed Part 02 covers:

- allowed discovery sources;
- deterministic identity normalization;
- discovery vs detection semantics;
- initial scan;
- subsequent change tracking;
- Added/Removed/Changed/Unchanged classification;
- partial/unreadable scan handling;
- deterministic fingerprint evidence;
- baseline-promotion boundary;
- no plugin code execution;
- separation of detection evidence from Game Adapter capability mapping and Save compatibility.

### 6.3 Critical invariant

`scan completed != baseline committed`

A partial or unreadable scan must never overwrite the last known valid baseline or be interpreted as mass plugin removal.

**Result:** reconstruction APPROVED; Complete RU+DE rebuild remains required.

## 7. ARCH-011 ↔ ADR-005/ARCH-007/ARCH-005 validation

The audited Save architecture is consistent with the baseline:

- Save Manager owns Save/Restore semantics;
- ordinary domain persistence does not have to flow through Save Manager;
- SaveParticipants are explicit injected contracts, not discovered through a Service Locator;
- Save persistence remains behind Repository/Database;
- scheduled autosave enters through Event Engine;
- Game Adapter provides safe-point/capability evidence only;
- restore compatibility remains Save Manager-owned;
- committed Save is an authoritative persisted/restorable checkpoint, not necessarily the newest live runtime state.

### 7.1 Restore atomicity

The final architecture requires logical all-or-nothing activation across required SaveParticipants.

Allowed implementation strategies include atomic swap, shadow-state activation, reversible participant apply, transaction-like activation barrier, or another mechanism with equivalent observable semantics.

The architecture does not prescribe the mechanical strategy yet; MOD-013/implementation ADR may do so.

**Result:** PASS; binary regeneration pending and ARCH-007 SD-005 must be aligned.

## 8. ARCH-012 ↔ ADR-004/ARCH-005/008/009 validation

Canonical AI path:

`Approved Consumer -> AI Adapter / AI Capability Port -> Context Manager -> Provider Abstraction -> Provider`

Validated invariants:

- AI Adapter is the consumer-facing application entry boundary;
- Context Manager is behind that boundary;
- Dialogue is the primary v1.0 AI consumer;
- Core is not a generic AI consumer by default;
- Context Manager reads Memory only through Memory Read/Context Source Port;
- no AI component accesses SQLite/Save/Game/Secrets directly;
- provider lifecycle is Host/Composition Root-owned;
- Local AI transport remains protocol-neutral at architecture level;
- provider output is non-authoritative until deterministic owning-use-case validation;
- game-facing action routes through Core/Application + Game Output/Capability Port, not directly from AI to Game Adapter.

**Result:** PASS; binary regeneration pending.

## 9. Cross-document consistency invariants now satisfied

The audited source set ARCH-007–012 now agrees on the following rules:

1. Host/Composition Root owns top-level construction/lifecycle wiring.
2. Core owns coordination/use-case orchestration, not the whole application object graph.
3. Event Engine is semantic-event infrastructure, not universal RPC.
4. Scheduler owns timing, not persistence policy.
5. Save Manager owns Save/Restore semantics, not all domain persistence.
6. Game Adapter owns game translation/capability implementation, not domain policy.
7. Plugin Detection owns discovery/profile evidence, not final compatibility policy.
8. AI Adapter is the AI entry boundary; AI remains non-authoritative.
9. Configuration subsystem owns schema/normalization; Host distributes typed views.
10. Static dependency, runtime call, event, evidence and lifecycle wiring are explicitly different relations.
11. No Service Locator/global runtime module registry is required.
12. The audited static dependency graph remains acyclic.

## 10. Remaining downstream blockers

Before the architecture stage can be declared fully audited:

1. regenerate ARCH-007 with the explicit restore activation barrier;
2. complete render/visual QA of ARCH-008/009 audited DOCX candidates;
3. create RU+DE reconstructed ARCH-010 Part 02 DOCX;
4. rebuild ARCH-010 Complete RU+DE using the reconstruction and Parts 03–08;
5. regenerate ARCH-011/012 RU+DE audited DOCX;
6. run RU/DE semantic parity checks for ARCH-007–012;
7. re-audit and revise MOD-001 against the stabilized architecture;
8. run final dependency-cycle and repository integrity audit.

## 11. Gate

The semantic architecture model is stable enough that the next work should be **formal artifact regeneration and MOD-001 impact correction**, not further redesign of the dependency foundation unless new evidence creates a direct contradiction.

MOD-002 remains blocked until those steps complete.
