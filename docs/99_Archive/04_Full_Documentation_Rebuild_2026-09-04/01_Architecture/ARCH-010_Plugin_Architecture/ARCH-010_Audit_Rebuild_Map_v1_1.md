# ARCH-010 — Audit Rebuild Map v1.1

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Rebuild plan frozen; reconstructed Part 02 DOCX locally QA-passed; Complete RU+DE rebuild pending  
**Date:** 2026-09-01

## 1. Why ARCH-010 must be rebuilt

The legacy Complete RU+DE artifacts cannot remain canonical because:

1. the Complete merge contains Part 01 twice;
2. a genuine original Part 02 body is absent;
3. the separately named legacy Part 02 files also contain Part 01 body content;
4. several Parts 01/03–08 contain pre-audit wording that must be aligned with ADR-006, ADR-008 and the corrected Host/Game/Save/Event boundaries.

The missing Part 02 is therefore **reconstructed, not recovered**.

## 2. QA-passed reconstructed Part 02 binaries

The following local candidates were generated from the audit reconstruction source and rendered page-by-page with the DOCX QA workflow:

| File | Pages | Size | SHA-256 | Expected Git blob SHA | QA |
|---|---:|---:|---|---|---|
| `ARCH-010_Plugin_Architecture_Part_02_Discovery_Detection_RU_v1_1_Reconstructed_Audited.docx` | 5 | 41125 | `b60919cd50eb45c674fbf0226c1480ada824d97cc9bbbfbbf0776867813bfed0` | `3f1025960d04e2c41d766349853b516f8cd40416` | PASS |
| `ARCH-010_Plugin_Architecture_Part_02_Discovery_Detection_DE_v1_1_Reconstructed_Audited.docx` | 5 | 40944 | `612e4d4c0884c174e8336fc3523f988310fb4f356ea400cd81644d85b22b6841` | `c8c53372a6717ace3526df03681643085286f9ae` | PASS |

The binaries are not yet committed to GitHub in this audit step; the hashes above are the upload verification gate.

## 3. Part-by-part correction map

### Part 01 — Introduction / Boundaries

Retain:

- Plugin Architecture is isolated from Core/domain/persistence internals;
- Game Adapter is the game integration boundary;
- plugin input is untrusted;
- plugin persistence uses Repository boundary;
- significant validated changes may publish semantic events.

Correct/add:

- Application Host / Composition Root owns Plugin Detection construction/startup/shutdown wiring;
- Core and Scheduler do not own detector internals;
- Game Adapter capability mapping remains separate from detection evidence;
- Save Manager owns restore-compatibility decision where Save compatibility is concerned;
- `Event Engine` is the semantic-event boundary, not a universal RPC transport;
- Configuration supplies typed validated views through composition, not a global mutable config object.

### Part 02 — Discovery / Detection / Change Tracking

Replace the fake legacy Part 02 body entirely with the audited reconstruction.

Mandatory invariants:

- only approved sources are scanned;
- no path traversal/arbitrary crawling;
- deterministic identity normalization;
- deterministic fingerprint/change evidence;
- `Added / Removed / Changed / Unchanged` classification;
- unreadable source != mass removal;
- partial scan does not replace last known valid baseline;
- `scan completed != baseline committed`;
- no plugin code execution.

### Part 03 — Metadata Extraction / Validation

Retain most of the legacy design.

Clarify:

- Metadata Extraction/Validation receives normalized candidates from Part 02;
- malformed/untrusted metadata never grants privileges;
- AI-derived metadata remains candidate/non-authoritative until deterministic validation;
- validation success does not imply global game compatibility or Save compatibility;
- Game Adapter supplies game-specific format/capability interpretation only through approved integration contracts;
- no direct SQLite access.

### Part 04 — Registry / Baseline

Retain:

- Registry/Baseline is technical plugin-environment state, not gameplay truth;
- persistence behind Repository boundary;
- failed writes do not replace previous valid baseline;
- controlled rebuild/recovery.

Correct:

- remove any implication that MOD-013 Save Manager is the owner of general plugin-registry persistence;
- Plugin Registry/Repository owns plugin-baseline persistence mechanics;
- Save Manager is only a downstream consumer of relevant fingerprint/profile evidence for restore compatibility;
- baseline promotion occurs only after required validation + successful persistence;
- original plugin files are not copied into world/save DB automatically.

### Part 05 — Dependencies / Load Order / Compatibility

Legacy wording uses `compatibility` too broadly.

Final scope must distinguish:

1. **Plugin dependency-analysis state** — owned by Plugin Architecture;
2. **Game capability/support mapping** — owned by Game Adapter;
3. **Save restore compatibility** — owned by Save Manager;
4. **user/system enablement policy** — owned by Configuration/application policy.

Part 05 may calculate structural dependency/load-order analysis such as missing required references, cycles, conflicts and declared version-range compatibility, but must not publish one universal `Compatible=true` as global truth.

Actual game load order and game-runtime capability remain Game Adapter/integration concerns.

### Part 06 — Events / Knowledge

Retain Event Engine publication for validated semantic facts.

Correct:

- consumers subscribe through Event Engine for semantic notifications; this does not mean every command/query uses Event Engine;
- Game Adapter does not directly own detector lifecycle;
- Scheduler may publish a generic scheduled trigger/event if configured, but does not own Plugin Detection internals;
- plugin-related AI/knowledge extraction cannot persist Memory directly; validated candidates go through Memory Engine ownership;
- event content grants no filesystem/database privilege.

### Part 07 — Security / Sandbox

Legacy security model is largely valid.

Explicitly retain/add:

- no arbitrary plugin code execution;
- no dynamic privilege grant from metadata;
- no direct Database/Secrets/Game internals access;
- bounded parser/resource usage;
- path/root constraints;
- malformed plugin cannot corrupt last valid baseline;
- AI semantic analysis, if later enabled, remains behind AI Adapter and non-authoritative.

### Part 08 — Testing / Acceptance

Retain broad test categories but update acceptance assertions:

- initial valid scan produces candidate baseline evidence;
- baseline becomes committed only after required validation + persistence;
- partial/unreadable scan preserves previous valid baseline;
- no false mass-removal on source failure;
- structural dependency analysis is distinct from Game Adapter capability and Save restore compatibility;
- Host owns lifecycle wiring;
- Event Engine is semantic-event transport only;
- Repository is plugin-registry persistence boundary;
- no Service Locator or direct detector ownership by Core/Scheduler.

## 4. Canonical reconstructed pipeline

```text
Application Host / Composition Root
        |
        v
Plugin Discovery / Detection
        |
        v
Metadata Extraction / Validation
        |
        v
Plugin Registry / Baseline Persistence
        |
        +--> Event Engine — validated semantic evidence
        |
        +--> Game Adapter — capability mapping evidence
        |
        +--> Save Manager — restore-compatibility evidence
```

The arrows above are runtime/data/evidence relations. Static dependencies remain governed by ARCH-005/ADR-008.

## 5. Complete rebuild order

The new Complete RU+DE must be assembled in exactly this order:

1. Audited Part 01;
2. Reconstructed audited Part 02;
3. Audited Part 03;
4. Audited Part 04;
5. Audited Part 05;
6. Audited Part 06;
7. Audited Part 07;
8. Audited Part 08.

The legacy Complete files must not be used as a merge base without removing the duplicate Part 01 and applying the corrections above.

## 6. Final rebuild gate

ARCH-010 may be marked `Architecture Audited` only when:

- RU and DE Complete contain exactly Parts 01–08 once each;
- reconstructed Part 02 is explicitly traceable as reconstruction;
- all corrected ownership boundaries above are present;
- RU/DE semantic parity passes;
- full DOCX render QA passes page-by-page;
- plugin dependency analysis is not confused with global compatibility;
- static dependency graph remains acyclic.
