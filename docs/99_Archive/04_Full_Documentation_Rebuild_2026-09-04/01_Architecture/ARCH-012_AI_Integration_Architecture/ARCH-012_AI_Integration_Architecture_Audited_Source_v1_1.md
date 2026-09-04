# ARCH-012 — AI Integration Architecture — Audited Source v1.1

**Status:** Audit branch source candidate  
**Baseline:** ADR-004 + amendment; ADR-008; ARCH-004/005 audited model; ARCH-008/009 audited sources  
**Date:** 2026-09-01

## 1. Scope retained

The v1.0 AI architecture remains valid in its fundamental design:

- Local AI and External LLM providers are replaceable implementations behind one provider-neutral boundary;
- Local AI is a separate optional process/runtime;
- AI has no direct Database/Save/Game/Secret authority;
- only approved context leaves the application boundary;
- AI output is non-authoritative until deterministic application/domain validation;
- Memory Engine owns memory persistence/retrieval semantics;
- provider fallback/replacement is policy-controlled and observable;
- concrete local IPC/loopback transport remains an implementation decision;
- consumers do not import vendor/model SDKs.

The audit correction clarifies consumer scope, Adapter/Context ordering, lifecycle ownership and game-output routing.

## 2. AI application entry boundary

AI Adapter / AI Capability Port is the **only approved application entry point for AI requests**.

Canonical consumer-facing runtime path:

```text
Approved AI Use-Case Consumer
        |
        v
AI Adapter / AI Capability Port
        |
        +--> validate request contract
        +--> enforce capability/policy/deadline
        |
        v
Context Manager
        |
        +--> approved Memory/Knowledge Context Source contracts
        |
        v
Provider Abstraction
        |
        +--> Local AI Provider implementation
        └--> External LLM Provider implementation
        |
        v
Normalized Provider Result
        |
        v
AI Adapter response normalization / policy checks
        |
        v
Owning application/domain use case
        |
        v
Deterministic validation / decision
```

Consumers do not bypass AI Adapter by calling Context Manager as the request entry point.

## 3. Approved consumers

Dialogue Engine is the primary AI consumer for v1.0.

Additional AI consumers are allowed only when an explicit use-case contract/specification defines:

- the purpose of the request;
- allowed context classes;
- result contract;
- deterministic validation owner;
- privacy/provider constraints;
- failure/fallback behavior.

Potential consumers may include approved Memory/Knowledge analysis or Plugin semantic-analysis use cases, but presence in the same process does not grant AI access automatically.

MOD-001 Core is **not a generic AI consumer by default**.

Core may consume scoped AI readiness/capability evidence for orchestration/readiness. A future Core-owned AI use case would require an explicit architecture/module decision rather than implicit access.

## 4. Context Manager ownership

Context Manager is an internal AI-integration collaborator behind AI Adapter from the consumer perspective.

It owns:

- approved context assembly;
- context-source selection according to request policy;
- data minimization;
- request/context budget enforcement;
- provenance/context metadata normalization;
- prompt/request semantic assembly before provider mapping.

It does not own domain truth or Memory persistence.

## 5. Memory retrieval contract

Canonical relation:

`Context Manager -> Memory Read / Context Source Port <- Memory Engine`

Rules:

- no direct Context Manager -> SQLite/Repository access;
- Memory Engine owns retrieval semantics and filtering of memory/knowledge records;
- returned records are normalized and provenance-preserving;
- Context Manager may apply additional AI-specific context-budget filtering, but it does not reinterpret Memory ownership rules;
- Memory failure may degrade a use case if that use case permits reduced-context operation.

## 6. Provider abstraction

Provider-neutral contract retains:

- capability identifier/use case;
- normalized request content;
- context payload/reference model;
- timeout/deadline/cancellation metadata;
- provider-neutral response content;
- provider-neutral usage/diagnostic metadata where safe;
- normalized error categories.

Provider-specific:

- authentication;
- endpoint/SDK;
- transport;
- request serialization;
- model naming;
- token/cost metadata;
- provider-specific retry mapping;

remain inside AI/provider integration.

## 7. Local AI process boundary

Local AI Runtime remains a separate optional process/service boundary.

Canonical deployment relation:

`AI Adapter/Provider Abstraction -> approved local loopback/IPC transport -> Local AI Runtime`

Concrete protocol/port remains TBD by a dedicated technical decision.

A local provider process may be restarted independently from world/domain state.

Local transport failure produces scoped AI capability failure, not Core/world/save corruption.

## 8. External provider boundary

External provider use requires:

- approved secure remote transport;
- explicit provider configuration;
- secret reference resolution inside approved provider/security boundary;
- per-request data-boundary/privacy classification;
- only approved minimized context;
- observable provider/fallback transition when privacy/cost characteristics change.

Consumer modules never receive provider credentials.

## 9. Lifecycle ownership

Application Host / Composition Root owns top-level construction/startup/shutdown wiring of AI Integration.

Application startup conceptually:

1. Host distributes typed AI configuration.
2. Construct AI Adapter, Context Manager and Provider Abstraction.
3. Bind approved Memory/Context Source contracts.
4. Initialize configured provider integration.
5. If Local AI is enabled and Host manages the process, start/check Local AI Runtime according to deployment policy.
6. Validate provider/capability health.
7. Publish scoped AI readiness/capability evidence.
8. Host aggregates it into application-level readiness policy.

This is not MOD-001 Core ownership of provider lifecycle.

Shutdown conceptually:

- stop accepting new AI requests;
- cancel/defer lower-priority work according to policy;
- allow bounded completion of latency-sensitive in-flight requests where safe;
- close provider resources;
- stop managed Local AI process if applicable;
- publish final scoped provider/AI status.

## 10. Readiness scope

Keep scoped states distinct:

- AI Adapter health;
- Provider availability;
- Capability availability;
- Local AI process health;
- Core coordination state;
- application startup outcome.

Example:

`CoreReady = true` and `AICapabilityUnavailable = true` may still yield `ApplicationDegraded` when AI is optional.

Do not describe this as Core itself being Degraded solely because AI is unavailable.

## 11. Workload priority

Replace `Core-first scheduling` with application/workload priorities.

AI workload must not starve:

- latency-sensitive game-control/application use cases;
- simulation work required for deterministic state progression;
- critical Event/Save/Recovery operations;
- bounded shutdown/recovery tasks.

AI queueing/concurrency/caching policies remain inside AI Integration and provider capability constraints.

## 12. Request queues and backpressure

- queues are bounded;
- lower-priority/background AI work is deferred/rejected first under pressure;
- deadlines/cancellation propagate through AI Adapter/provider boundary;
- duplicate/cacheable requests may be coalesced only when semantic equivalence is safe;
- provider-specific concurrency remains hidden behind abstraction;
- queue metrics must avoid high-cardinality uncontrolled labels.

## 13. Response authority

AI output is always one of:

- candidate text/content;
- candidate interpretation;
- candidate classification;
- candidate structured proposal/tool intent;
- failure/unavailable result.

AI output does not directly become:

- database mutation;
- Save artifact mutation;
- Game Adapter command;
- security-policy change;
- permission change;
- arbitrary process/file/network action.

The owning application/domain use case validates the candidate deterministically before any authoritative mutation/action.

## 14. Game output routing

AI result never routes directly to concrete Game Adapter.

Example dialogue path:

```text
Dialogue -> AI Adapter -> ... -> AI result
Dialogue -> deterministic validation -> final dialogue result
Dialogue -> Core/Application use-case response
Core/Application -> Game Output/Capability Port
Game Adapter implementation -> Game Runtime
```

Game Adapter is an integration implementation, not an AI decision owner.

## 15. Tool/action boundary

If a future AI use case supports tool/action proposals:

- AI proposes a typed intent only;
- deterministic handler validates authorization, schema, state preconditions and policy;
- handler invokes explicit application/domain command ports;
- AI cannot access concrete modules through a Service Locator;
- AI cannot call Repository/SQLite directly;
- AI cannot call concrete Game Adapter implementation directly;
- AI cannot read arbitrary files/processes/secrets.

## 16. AI-derived memory

Preserve the v1.0 ownership model:

```text
AI result/candidate
 -> owning use case validation
 -> MemoryCandidate with provenance
 -> Memory Engine validation/policy
 -> Memory Repository contract
```

AI Adapter does not persist memory directly.

Memory Engine decides:

- allowed memory type;
- duplicate/near-duplicate policy;
- retention/importance;
- source/provenance representation;
- persistence/expiration/supersession.

## 17. Provider fallback and switching

Fallback/switching is allowed only by configuration/runtime policy.

Before Local -> External transition:

- re-evaluate request data classification;
- verify external transmission is allowed;
- resolve required credentials safely;
- invalidate provider-incompatible cached results where necessary;
- expose the transition in diagnostics when privacy/cost characteristics change.

AI output itself cannot request or authorize its own provider switch.

## 18. Security invariants retained

- no direct AI -> Database;
- no direct AI -> Save Manager/Save files;
- no direct AI -> Game API;
- no direct AI -> protected secret store;
- no arbitrary internal-file access;
- external provider receives least approved context;
- prompt/provider content cannot grant additional permissions;
- provider response is treated as untrusted/non-authoritative input;
- logs exclude secrets and unnecessary sensitive context.

## 19. Superseded v1.0 interpretations

The following readings are superseded:

- `Core / Modules -> AI Adapter` means every module/Core is an approved generic AI consumer;
- consumers may enter the provider request path through Context Manager before AI Adapter;
- Core owns AI provider startup/shutdown;
- optional AI unavailability automatically defines Core's own readiness state;
- AI scheduling priority is defined as `Core-first` rather than workload/latency criticality;
- deterministic AI result may route directly to Game Adapter;
- `Plugin module` is a top-level consumer identity; use Plugin Detection integration use cases/contracts instead.

## 20. Acceptance criteria after audit

ARCH-012 is accepted when tests prove:

1. all AI requests enter through AI Adapter/AI Capability Port;
2. unapproved consumers cannot obtain provider access;
3. Context Manager uses Memory Context Source contract only;
4. no AI component accesses SQLite/Save/Game/Secrets directly;
5. Local AI can be unavailable without world-state corruption;
6. application/Core/AI readiness states remain scoped;
7. Local->External switch re-evaluates privacy boundary;
8. bounded queue/backpressure/cancellation work;
9. AI-derived memory remains candidate until Memory Engine accepts it;
10. game-facing action uses Game Output/Capability Port after deterministic use-case validation.
