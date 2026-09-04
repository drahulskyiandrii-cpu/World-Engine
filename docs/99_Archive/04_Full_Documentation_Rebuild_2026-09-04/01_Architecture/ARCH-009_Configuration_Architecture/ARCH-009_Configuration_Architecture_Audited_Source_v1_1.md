# ARCH-009 — Configuration Architecture — Audited Source v1.1

**Status:** Audit branch source candidate  
**Baseline:** ADR-008 + ADR amendments + ARCH-004/005 audited model + ARCH-008 audited source  
**Date:** 2026-09-01

## 1. Scope retained from v1.0

The existing configuration architecture remains valid in its core design:

- YAML is the primary human-readable configuration format for v1.0;
- `default` is the mandatory base profile;
- optional profiles may override only schema-allowed values;
- configuration source hierarchy is deterministic;
- validation happens before dependent runtime use;
- secrets are referenced, not stored directly in YAML;
- schema versioning/migration are explicit;
- runtime components consume normalized typed values, not raw YAML;
- effective runtime configuration is immutable by default after startup;
- hot reload is conservative and opt-in.

The audit correction clarifies **ownership, distribution and readiness scope**.

## 2. Configuration subsystem ownership

Configuration subsystem owns:

- locating allowed configuration sources;
- parsing YAML;
- schema identification and validation;
- deterministic source/profile/override merge;
- semantic normalization;
- secret-reference resolution through approved protected-storage boundaries;
- construction of the accepted effective configuration;
- configuration health/status evidence;
- controlled hot-reload candidate validation.

MOD-001 Core does not own the YAML schema, merge logic, secret resolution or global configuration distribution.

## 3. Canonical startup data path

```text
Built-in Safe Defaults
        +
Base YAML
        +
Selected Profile
        +
Permitted Explicit Overrides
        |
        v
Configuration Subsystem
  - parse
  - schema validate
  - resolve profile
  - merge
  - resolve required secret references
  - normalize
  - semantic validate
        |
        v
Validated Effective Configuration
        |
        v
Application Host / Composition Root
        |
        +--> typed Core configuration view
        +--> typed Event Engine configuration view
        +--> typed Scheduler configuration view
        +--> typed Game Adapter configuration view
        +--> typed AI Adapter configuration view
        +--> typed Save/Persistence configuration view
        +--> typed Logging/Diagnostics configuration view
        +--> other responsibility-specific typed views
```

The Host performs composition-time distribution. It does not expose a global Service Locator or mutable configuration tree.

## 4. Raw YAML boundary

- ordinary runtime modules do not read YAML files directly;
- ordinary runtime modules do not implement their own profile merge/parsing;
- raw YAML remains inside Configuration subsystem unless a narrowly documented tooling use case requires otherwise;
- secret values never become ordinary YAML-derived runtime fields;
- components receive only the typed subset required by their responsibilities.

## 5. Local AI configuration neutrality

ARCH-009 must not force localhost HTTP as the local-AI transport.

Transport-neutral example:

```yaml
ai:
  provider: local
  local_runtime_ref: local-ai-runtime
  timeout_ms: 30000
```

The effective configuration may contain an endpoint/transport descriptor after validation, but the exact protocol/port is owned by the deployment/technical transport decision.

For external provider mode, approved HTTPS endpoint/reference policy remains valid.

## 6. Startup lifecycle and readiness scope

Configuration validation contributes evidence to application startup but does not own application lifecycle.

Canonical sequence:

```text
1. Host resolves configuration root / requested profile.
2. Configuration subsystem loads defaults and allowed sources.
3. Parse + schema validation.
4. Profile resolution and permitted override merge.
5. Resolve only secrets needed by enabled capabilities.
6. Normalize into canonical typed runtime values.
7. Global semantic validation.
8. Subsystem-specific configuration validation through explicit validation contracts where required.
9. Configuration subsystem accepts one effective configuration candidate.
10. Host distributes typed views and starts components.
11. Components publish scoped readiness/health evidence.
12. Host determines application-level Ready / Degraded / Startup Failed / Recovery Required outcome.
```

The following are distinct:

- configuration health;
- component configuration acceptance;
- component readiness;
- Core coordination state;
- application startup outcome.

## 7. Application-level startup outcomes

- **Application Ready** — required capabilities have valid configuration and are ready according to policy.
- **Application Degraded** — required baseline is valid while explicitly optional capabilities are unavailable/misconfigured in an allowed way.
- **Application Startup Failed** — critical configuration or required dependency is invalid/unavailable; normal runtime is not activated.
- **Application Recovery Required** — startup detects a recoverable but non-normal state requiring controlled recovery.

Statements such as `Core ready but AI unavailable = Degraded` are replaced by the application-scoped model above.

## 8. Runtime configuration API

Runtime access is responsibility-scoped.

Preferred logical contracts:

- component-specific immutable configuration view;
- read-only configuration health/status contract for Host/Diagnostics;
- controlled change request API for supported hot-reload fields;
- no unrestricted `GetAllConfiguration()` for ordinary modules;
- no direct secret-store browsing from ordinary modules.

Core receives only Core-specific typed values and narrow configuration evidence required by a documented Core responsibility.

Schema/profile/health diagnostics primarily belong to Configuration subsystem + Host/Diagnostics, not generic Core ownership.

## 9. Hot reload retained with stronger transaction semantics

Conservative v1.0 policy remains:

1. detect/request candidate change;
2. identify changed keys;
3. reject if any changed key is not hot-reloadable;
4. build and validate a complete candidate effective configuration;
5. resolve required secrets;
6. identify affected components;
7. obtain component acceptance/preparation where required;
8. apply through a controlled atomic change boundary;
9. commit the new accepted effective configuration only after successful apply;
10. otherwise retain/revert to the previous accepted configuration.

No component may independently mutate its private copy in a way that silently diverges from the accepted effective configuration.

## 10. Secret model retained

- secret references are not secrets;
- exported/logged diagnostics exclude secret values;
- only enabled capabilities resolve required secret references;
- secret-store mechanism remains a separate Windows security decision;
- external AI credentials never become broad global configuration values;
- user configuration cannot grant AI Runtime direct SQLite access.

## 11. Schema/versioning retained

Configuration schema versioning, migration and backward-compatibility policy remain Configuration-subsystem responsibilities.

A schema change must not silently reinterpret old configuration. Unsupported/incompatible versions must fail with controlled diagnostics or an explicit migration path.

## 12. Traceability corrections

- ARCH-008 owns deployment paths/transport environment and defers concrete local AI protocol.
- ARCH-009 owns configuration schema/source/merge/normalization semantics.
- ADR-001/Core defines only Core's consumption boundary, not configuration ownership.
- MOD-001 may reference Core-specific typed configuration contracts but must not become a global configuration distributor.

## 13. Superseded v1.0 interpretations

The following readings are explicitly superseded:

- validated configuration is globally `published` directly to modules without composition ownership;
- `Ready/Degraded/Startup Failed` in ARCH-009 are MOD-001 Core states;
- Core owns configuration schema/governance;
- Core/Diagnostics are the default consumers of global profile/schema/health APIs;
- localhost HTTP is already fixed as the mandatory local-AI endpoint form.

## 14. Acceptance criteria after audit

ARCH-009 is acceptable when tests verify:

1. deterministic merge result;
2. unknown/invalid keys do not silently change behavior;
3. typed responsibility-specific views only;
4. no ordinary raw-YAML access;
5. no secret leakage;
6. correct application-vs-component readiness scoping;
7. transport-neutral local AI configuration;
8. controlled hot-reload apply/revert;
9. schema migration/version behavior;
10. no Core-owned configuration Service Locator.
