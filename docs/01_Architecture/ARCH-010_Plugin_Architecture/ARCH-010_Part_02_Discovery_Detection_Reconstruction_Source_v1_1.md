# ARCH-010 — Part 02 — Plugin Discovery, Detection & Change Tracking — Reconstructed Audited Source v1.1

**Status:** Reconstructed audit candidate — replaces invalid duplicated Part-02 body  
**Derivation basis:** ADR-006 Plugin Detection; ADR-006 clarification; ARCH-010 Parts 01 and 03–08; ARCH-004/005 audited model; ARCH-007 audited plugin-detection sequence  
**Date:** 2026-09-01

> This content is reconstructed, not recovered. The available legacy Part-02 file contains a duplicate of Part 01 and cannot serve as the source body.

## 1. Purpose

This part defines how World Engine discovers plugin/mod artifacts in approved game-installation sources, turns those observations into deterministic detection evidence, compares the evidence with the last valid baseline, and classifies confirmed environment changes before metadata extraction/validation continues in Part 03.

Discovery and Detection are integration-layer concerns. They do not execute plugin code and do not decide final gameplay, capability or Save compatibility policy.

## 2. Pipeline position

Canonical ARCH-010 pipeline:

`Discovery -> Detection -> Metadata Extraction -> Validation -> Registration/Baseline -> Event Publication`

Definitions:

- **Discovery** — observe candidate artifacts from configured/allowed sources.
- **Detection** — normalize artifact identity and determine the observed plugin-environment state.
- **Change Tracking** — compare a successfully validated detection candidate with the last known valid baseline.
- **Metadata Extraction** — parse structured plugin metadata after a candidate artifact has been discovered/detected.
- **Compatibility** — separate decision owned by the relevant consumer (for example Game Adapter capability mapping or Save Manager restore policy).

Detection is not execution and detection is not compatibility.

## 3. Lifecycle ownership

Application Host / Composition Root owns Plugin Detection lifecycle wiring:

- construction;
- injection of typed configuration and repository/baseline contracts;
- initial startup invocation;
- optional future scheduling/watcher wiring;
- shutdown ordering.

Core and MOD-012 Scheduler do not own Plugin Detector internals.

A scheduled or externally triggered rescan may invoke an explicit detection command/port, but this does not create a static `Scheduler -> Plugin Detection implementation` ownership relation unless a later module/API decision explicitly defines one.

## 4. Discovery sources

Plugin Discovery reads only approved sources derived from the active Game Profile and validated typed configuration.

Source classes may include:

- active game/plugin directories;
- game-managed plugin lists/load-order evidence when exposed through an approved integration contract;
- approved metadata manifests/registries;
- other explicitly configured read-only integration sources.

Rules:

1. No arbitrary filesystem crawling.
2. No scanning outside configured/allowed roots through path traversal or link tricks.
3. A discovered file is untrusted data.
4. Discovery grants no execute/load/injection permission.
5. Game-specific path and format knowledge remains in the integration boundary.
6. Raw filesystem details are normalized before they are exposed to ordinary consumers.

## 5. Discovery result contract

A discovery candidate should contain, at architecture level:

- source identifier/provenance;
- normalized artifact path/reference;
- observed artifact type when determinable without unsafe parsing;
- stable candidate identity inputs;
- basic filesystem/manifest observations needed for change comparison;
- operation/scan correlation identifier;
- discovery timestamp/observation context where required;
- discovery warnings/errors.

The exact programming-language type and field names are deferred to API/module specifications.

## 6. Identity normalization

Detection must not rely on nondeterministic filesystem ordering or presentation-only path differences.

Normalization rules include:

- canonical path/root-relative identity where supported;
- explicitly defined case handling for the target filesystem/game rules;
- normalization of separators/aliases where safe;
- duplicate observations of the same underlying artifact collapse to one normalized candidate or are reported as an explicit ambiguity;
- identity must retain provenance so diagnostics can explain where the artifact came from.

Normalization must not merge two distinct artifacts merely because their display names are equal.

## 7. Detection state

Detection answers: **What is observed in the current plugin environment?**

At minimum it distinguishes:

- observed candidate artifact;
- unavailable/unreadable source;
- malformed/ambiguous observation;
- candidate absent from the current successful scan;
- unknown/unsupported artifact type where evidence is insufficient.

An unavailable detection source must not be converted into a valid empty environment.

## 8. Fingerprint model

A Plugin/Profile fingerprint is a deterministic summary identity of compatibility-relevant detection evidence.

Rules inherited from ADR-006:

- fingerprint input order is deterministic;
- nondeterministic directory enumeration must not affect the result;
- compatibility-relevant environment changes alter the fingerprint;
- fingerprint is evidence for comparison/diagnostics/Save compatibility, not automatically a cryptographic security proof;
- exact hash/fingerprint algorithm is an implementation decision unless separately fixed by ADR.

Per-artifact fingerprints may be used for efficient change comparison, provided the profile-level result remains reproducible and explainable.

## 9. Initial detection cycle

A successful initial cycle:

1. receives validated detection configuration/Game Profile evidence;
2. scans approved discovery sources;
3. normalizes candidate identities;
4. rejects/records unsafe or unreadable observations according to policy;
5. constructs a candidate plugin environment/profile evidence set;
6. passes discovered candidates to metadata extraction/validation stages;
7. after downstream validation succeeds, establishes the first known-valid baseline through the Plugin Baseline persistence contract.

A partial/failed initial scan must not be presented as a valid empty baseline.

## 10. Subsequent detection / change tracking

After a known-valid baseline exists, a successful candidate scan is compared with that baseline.

Canonical change classes:

- **Added** — normalized artifact exists in the new valid candidate but not in the previous baseline.
- **Removed** — artifact existed in the previous baseline but is confirmed absent from the new complete/valid candidate scan.
- **Modified/Changed** — stable identity remains applicable but compatibility-relevant fingerprint/metadata evidence changed.
- **Unchanged** — relevant normalized evidence is equivalent.

`Removed` may only be asserted after a scan has enough evidence to distinguish true absence from source/read failure.

Repeated identical scans should converge to stable `Unchanged` results.

## 11. Partial scan and failure rules

Fail-safe rules:

- a partial scan does not overwrite the last known valid baseline;
- an unreadable directory/source does not mean all contained plugins were removed;
- a failed rescan keeps the previous valid profile/baseline available as historical evidence while current detection health is reported separately;
- repeated failures may move Plugin Detection capability to a scoped unavailable/degraded state according to application policy;
- retry behavior is bounded; no infinite rescan loop;
- errors are correlated to the scan/operation ID and affected source.

## 12. Baseline relationship

Part 02 produces candidate detection/change evidence. Part 04 owns the Plugin Registry/Baseline persistence model.

The new baseline is established only after the candidate passes required downstream metadata/validation checks and persistence completes successfully.

Therefore:

`scan completed` != `baseline committed`.

This prevents malformed or partially observed environments from replacing known-valid detection evidence.

## 13. Separation from metadata extraction

Discovery/Detection should perform only the minimum safe observations needed to identify candidates and route them into Part 03.

Deep parsing belongs to Metadata Extraction/Validation.

This separation supports:

- bounded scanning cost;
- smaller attack surface;
- better error classification;
- deterministic change tracking;
- ability to reject unsafe artifacts before expensive parsing.

## 14. Separation from compatibility decisions

Plugin Detection supplies evidence only.

- Game Adapter owns mapping plugin/profile evidence to integration capabilities.
- Save Manager owns restore compatibility decisions using Save Contract policy and stored fingerprint/profile evidence.
- Configuration owns user/system enablement policy.
- domain modules do not infer support by independently scanning plugin files.

Unknown plugin != supported plugin, and detected plugin != compatible plugin.

## 15. Events and consumers

Change events are published only from validated detection/profile evidence according to the event policy defined later in ARCH-010.

Possible semantic event families include:

- PluginProfileDetected;
- PluginProfileChanged;
- PluginArtifactAdded;
- PluginArtifactRemoved;
- PluginArtifactChanged;
- PluginDetectionUnavailable/Recovered.

Exact event catalog/names are owned by the Event Catalog stage and may differ. Event publication does not transfer compatibility ownership to Event Engine.

## 16. Performance and resource constraints

- scanning is centralized rather than repeated by every consumer;
- large environments must not trigger uncontrolled full rescans for every query;
- incremental comparison may use normalized fingerprints/caches/baselines;
- CPU, memory, I/O and file-count processing must be bounded;
- detection should avoid indefinitely blocking latency-critical gameplay paths;
- implementation may use background work, but candidate/baseline promotion remains deterministic and synchronized.

Exact scan intervals, watcher technology and resource budgets are deferred.

## 17. Security constraints

Discovery/Detection must not:

- execute plugin code;
- dynamically load untrusted binaries as a consequence of detection;
- follow unsafe paths outside allowed roots;
- grant AI additional permissions;
- expose arbitrary filesystem content to domain modules;
- treat external descriptions/text as trusted instructions;
- allow a malformed plugin to corrupt the last valid baseline.

## 18. Observability

Every detection cycle should expose/log safe diagnostics for:

- scan/operation ID;
- selected Game Profile;
- allowed source identifiers (without leaking sensitive unrelated paths where policy forbids it);
- counts of discovered/validated/rejected/changed artifacts;
- partial/failed source state;
- prior/new profile fingerprint when safe to expose;
- duration/resource warnings;
- baseline promotion outcome.

## 19. Acceptance criteria

Part 02 is accepted when tests demonstrate:

1. only configured/allowed sources are scanned;
2. path traversal/unsafe source access is rejected;
3. initial valid scan can establish candidate evidence for a baseline;
4. partial scan cannot replace the last known valid baseline;
5. added/removed/changed/unchanged classifications are deterministic;
6. unreadable source is not interpreted as mass removal;
7. repeated identical scans converge to unchanged;
8. profile fingerprint is deterministic for equivalent evidence;
9. discovery does not execute plugin content;
10. consumers can rely on normalized evidence without rescanning the game directories independently.

## 20. Deferred decisions

Not fixed by this reconstructed architecture part:

- concrete filesystem watcher technology;
- exact scan cadence;
- exact hash/fingerprint algorithm;
- exact Skyrim plugin binary parser;
- platform-specific file identity optimization;
- parallelism/threading implementation;
- event names beyond the architectural event families.
