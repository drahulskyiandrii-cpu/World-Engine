# STD-002 World Engine Naming Convention

Version 1.0 · Deutsch · 2026-09-05 · Current normative standard

## 1. Zweck

STD-002 legt die verbindlichen Benennungsregeln für Dokumente, Verzeichnisse, Module, C++ Symbols, Contracts, Events, Schemas, Configuration, Database Objects, Logs, Metrics, Tests, Builds und Version-control Artifacts der World Engine fest. Ein Name soll vor dem Lesen der Implementierung vorhersagbar, eindeutig und suchbar sein.

Der Standard bestimmt die Form eines Namens, ändert aber kein Ownership. Die Schreibweise eines Event Name überträgt zum Beispiel den Besitz seines Schemas nicht vom Producer an ein anderes Modul. Die Bedeutung der Begriffe bestimmt STD-001.

## 2. Geltungsbereich und Compatibility

Die Regeln sind für neue Public Names nach Veröffentlichung von STD-002 verbindlich. Bereits veröffentlichte MOD-002 Identifiers bleiben bestehen und gelten als kompatible Baseline. Ein Rename ist nur mit Migration und Compatibility Evidence zulässig; kosmetische Reinheit rechtfertigt keinen Breaking Change.

| Objekt | Vorgehen für neue Namen | Vorgehen für vorhandene Namen |
|---|---|---|
| Public contract | STD-002 sofort einhalten | Kein Rename ohne Contract Versioning |
| Internal symbol | Bei Erstellung und wesentlicher Bearbeitung einhalten | Atomic Refactor mit Tests zulässig |
| Persisted field | Schema Rule sofort einhalten | Nur über Migration |
| Log oder metric | Stabilen Namen sofort einhalten | Alias während Dashboard Migration erhalten |
| Dokument | Canonical Filename verwenden | Vorgängerversion bleibt in History oder Archive |

## 3. Normative Wörter

`MUST` kennzeichnet eine Pflicht; `MUST NOT` ein Verbot; `SHOULD` eine starke Empfehlung mit dokumentationspflichtiger Abweichung; `MAY` eine zulässige Option. RU- oder DE-Übersetzungen verändern diese Stärke nicht.

## 4. Allgemeine Eigenschaften guter Namen

Ein Name MUST Domain Meaning oder technische Rolle ausdrücken, genau einen Scope besitzen und Informationen vermeiden, die bereits durch Namespace oder Path gegeben sind. Identifiers MUST englische ASCII-Namen sein. Kommentare und User Text dürfen RU oder DE sein; Machine Names werden nicht übersetzt.

| Eigenschaft | Anforderung | Schlechtes Beispiel | Gutes Beispiel |
|---|---|---|---|
| Eindeutigkeit | Ein Konzept pro Name | `data` | `memory_record` |
| Scope | Context in Namespace oder Path sichtbar | `Manager` | `SaveCoordinator` |
| Positive Form | Boolean liest sich als Aussage | `not_disabled` | `enabled` |
| Einheit | Zahl nennt ihre Unit | `timeout` | `timeout_ms` |
| Stabilität | Keine temporäre Implementierung im Namen | `sqlite_memory_service` | `MemoryRepository` |
| Suchbarkeit | Keine unklaren Abkürzungen | `cfg_mgr` | `ConfigurationRegistry` |

Gemischte Casing Styles in einem Identifier sind verboten, ausgenommen angenommene Version- oder Unit-Suffixe.

## 5. Casing-Systeme

| Style | Form | Verwendung |
|---|---|---|
| PascalCase | `MemoryRecord` | C++ Types, Enum Types und Concepts |
| camelCase | `memoryRecord` | Nicht im C++ Public API; nur für externes Provider Schema falls nötig |
| snake_case | `memory_record` | C++ Functions/Variables, JSON/YAML Fields und Database Objects |
| SCREAMING_SNAKE_CASE | `WORLD_ENGINE_VERSION` | Nur Preprocessor Macros |
| kPascalCase | `kContractSchemaVersion` | C++ Compile-time Constants |
| kebab-case | `skyrim-se-ae` | Stable External IDs, Profiles und ausgewählte Package Names |
| dotted.lowercase | `runtime.identity` | Capability Tokens, Metric Namespaces und Event Segments |

## 6. Dokument-Identifiers

Eine Document ID besteht aus uppercase family, Bindestrich und dreistelliger Nummer: `ARCH-004`, `ADR-009`, `MOD-003`, `STD-002`. Control Families können eine eigene Reihe wie `PLAN-001` besitzen. Eine ID wird nach Entfernung oder Supersede niemals wiederverwendet.

| Family | Zweck | Beispiel |
|---|---|---|
| DOC | Project management delivery documents | `DOC-009` |
| PLAN | Documentation control plans | `PLAN-001` |
| ARCH | Architecture documents | `ARCH-011` |
| ADR | Architecture Decision Records | `ADR-008` |
| MOD | Module specifications | `MOD-003` |
| DB | Database documents | `DB-001` |
| EVT | Event System documents | `EVT-004` |
| API | API documents | `API-006` |
| AI | AI documents | `AI-003` |
| TEST | Testing documents | `TEST-001` |
| RISK | Risk documents | `RISK-001` |
| STD | Project standards | `STD-002` |
| RES | Research documents | `RES-004` |
| KB | Knowledge Base documents | `KB-001` |
| IHK | IHK deliverables | `IHK-008` |

## 7. Dateinamen von Dokumenten

Ein canonical Word filename MUST dem Muster `<ID>_<English_Title>_<LANG>_v<major>_<minor>_<Lifecycle>.docx` folgen. Wörter werden mit underscore getrennt; der Title nutzt Pascal-like words ohne Satzzeichen. `LANG` ist `RU` oder `DE`.

| Element | Regel | Beispiel |
|---|---|---|
| ID | Exact document ID | `STD-002` |
| English Title | Stabiler englischer Name | `Naming_Convention` |
| Language | `RU` oder `DE` | `DE` |
| Version | `v1_0` ohne Punkt im filename | `v1_0` |
| Lifecycle | `Current`, `Accepted`, `Audited_Complete` oder angenommene Spezifik | `Current` |
| Source | `.docx` wird durch `_Source.md` mit gleichem Version Prefix ersetzt | `STD-002_Naming_Convention_DE_v1_0_Source.md` |
| QA | Ein language-neutral Report | `STD-002_v1_0_Full_Document_QA.md` |

Ein Datum gehört nicht in den canonical filename, wenn die Version bereits Identity liefert. Es ist in Archive Folder oder einmaliger Audit Evidence zulässig.

## 8. Dokumentationsverzeichnisse

Ein Top-level docs folder nutzt zweistellige numeric order plus English Title: `10_Project_Standards`. Ein einzelner ARCH-, ADR- oder MOD-Ordner nutzt vollständige ID plus English Title: `MOD-003_Memory_Engine`. Project-management- und Standards-Dateien dürfen direkt im Stage Folder liegen, wenn die Struktur eindeutig bleibt.

| Typ | Pattern | Beispiel |
|---|---|---|
| Stage folder | `<NN>_<English_Title>` | `05_Event_System` |
| Item folder | `<ID>_<English_Title>` | `ARCH-011_Save_System_Architecture` |
| Archive root | `<NN>_<Purpose>_<YYYY-MM-DD>` | `05_MOD002_M2_1_Baseline_2026-09-05` |
| Scratch folder | Dot-prefixed and ignored | `.foundation-work` |

`final`, `new`, `old`, `temp`, `misc`, `latest` und `copy` sind als einziger Differentiator in Canonical Paths verboten.

## 9. Versionen

Dokumente verwenden `major.minor`; Public Contracts und Packages SHOULD Semantic Versioning `major.minor.patch` verwenden. Im Filename wird der Punkt nur bei der Dokumentversion zu underscore. Schema Major kann als kompakter Suffix `v1` in Type oder Event Identity stehen.

| Änderung | Document version | Contract version |
|---|---|---|
| Redaktionelle Klarheit ohne Semantikänderung | Minor | Keine Änderung |
| Neue kompatible Section oder Rule | Minor | Minor bei Public Effect |
| Inkompatible Requirement-Änderung | Major | Major |
| Implementation-Bugfix | Nicht zwingend | Patch Package Version |

## 10. Module IDs und Names

Die Modulnummer ist als `MOD-001` bis `MOD-014` fixiert. Der Human Title verwendet eine englische Wortgruppe: `Memory Engine`. Der Machine Module Key verwendet je nach Medium lowercase dotted oder kebab form.

| Darstellung | Pattern | Beispiel |
|---|---|---|
| Document ID | `MOD-NNN` | `MOD-002` |
| Folder | `MOD-NNN_English_Title` | `MOD-002_Game_Adapter` |
| C++ namespace segment | `snake_case` | `game_adapter` |
| Stable adapter ID | `world-engine.mod-nnn.target` | `world-engine.mod-002.skyrim` |
| CMake target | `world_engine_<module>` | `world_engine_game_adapter` |
| Display title | English words | `Game Adapter` |

Die Modulnummer wird nicht in einen C++ Type Name aufgenommen, wenn der Namespace bereits eindeutig ist.

## 11. C++ Namespaces

Der Root Namespace MUST `world_engine` sein. Jedes Modul erhält genau ein primäres snake_case segment, zum Beispiel `world_engine::memory_engine`. Interne Details dürfen in `detail` liegen; Public Headers MUST NOT Wissen über `detail` verlangen.

| Scope | Regel | Beispiel |
|---|---|---|
| Root | Fixed | `world_engine` |
| Module | snake_case | `world_engine::game_adapter` |
| Tests | Same namespace oder anonymous namespace | `world_engine::game_adapter` |
| Private translation-unit helper | Anonymous namespace | `namespace { ... }` |
| Generated/provider code | Isolated subtree | `world_engine::ai::provider` |

`using namespace` ist in Public Headers verboten. In Test-`.cpp` MAY es für genau einen Module Namespace ohne Collision verwendet werden.

## 12. C++ Types

Classes, Structs, Enums, Aliases und Concepts verwenden PascalCase. Der Name beantwortet „was ist es“ und nicht „was tut es gerade“. Suffixe werden nur für stabile Rollen verwendet.

| Kategorie | Pattern | Beispiel |
|---|---|---|
| Entity/value type | PascalCase noun | `MemoryRecord` |
| Input envelope | `<Purpose>Envelope` | `IngressEnvelope` |
| Result | `<Operation>Result` oder gemeinsames `OperationResult` | `RestoreResult` |
| Configuration type | `<Scope>Config` | `GameAdapterConfig` |
| Evidence type | `<Subject>Evidence` | `CapabilityEvidence` |
| Descriptor | `<Subject>Descriptor` | `ModuleDescriptor` |
| Policy | `<Subject>Policy` | `RetentionPolicy` |
| Exception class | `<Cause>Error` nur für exceptional API | `ConfigurationError` |

Die Suffixe `Data`, `Info`, `Object`, `Thing`, `Helper`, `Util` und `Manager` sind ohne konkrete Semantics verboten.

## 13. C++ Interfaces, Ports und Adapters

Ein reines abstract C++ interface verwendet für Compatibility mit dem aktuellen MOD-002 Baseline das Präfix `I`. Ein Port Name MUST Direction oder Capability zeigen; ein Adapter Name nennt den externen Target.

| Rolle | Pattern | Beispiel |
|---|---|---|
| Inbound port | `I<Domain><Purpose>Port` | `IGameIngressPort` |
| Outbound port | `I<Target><Purpose>Port` | `IGameOutputCapabilityPort` |
| Repository contract | `I<Aggregate>Repository` | `IMemoryRepository` |
| Status sink | `I<Subject>StatusSink` | `IGameAdapterStatusSink` |
| Runtime gateway | `I<Target>Bridge` | `IRuntimeBridge` |
| Concrete adapter | `<Target><Role>Adapter` | `SkyrimRuntimeAdapter` |

`IService`, `IManager` und `IRepository` ohne Domain Noun sind verboten. Contract Ownership entsteht durch Architecture, nicht durch den Namen einer Implementierung.

## 14. C++ Functions und Methods

Functions und Methods verwenden eine snake_case verb phrase. Eine Query-like Method beschreibt den Return; eine Command-like Method die Action. Boolean Predicate beginnt mit `is_`, `has_`, `can_` oder `should_`.

| Typ | Pattern | Beispiel |
|---|---|---|
| Command | verb_object | `publish_status` |
| Query | noun oder verb_query | `capabilities`, `find_memories` |
| Boolean | predicate prefix | `is_ready`, `has_capability` |
| Conversion | `to_<target>` | `to_string` |
| Factory | `create_<type>` oder named outcome | `create_checkpoint` |
| Validation | `validate_<subject>` | `validate_profile` |
| Try operation | `try_<verb>` nur mit non-throwing outcome | `try_restore` |

`get_` SHOULD nur für einen Accessor ohne I/O oder wesentliche Computation stehen. Namen wie `process`, `handle`, `do` und `run` benötigen Object oder Context.

## 15. C++ Variables, Fields und Constants

Variables und Fields verwenden snake_case nouns. Eine Unit gehört in den Namen eines numeric value. Constants verwenden `kPascalCase`. Enum Values verwenden für MOD-002 Compatibility snake_case.

| Kategorie | Pattern | Beispiel |
|---|---|---|
| Local variable | snake_case | `runtime_profile` |
| Parameter | snake_case | `correlation_id` |
| Data member | snake_case | `max_payload_bytes` |
| Compile-time constant | kPascalCase | `kContractSchemaVersion` |
| Macro | SCREAMING_SNAKE_CASE | `WORLD_ENGINE_EXPORT` |
| Enum type | PascalCase | `AdapterState` |
| Enum value | snake_case | `initializing` |
| Boolean | positive predicate | `validation_enabled` |

Einbuchstabige Namen sind nur in kurzem Index- oder Math-Scope zulässig. Hungarian Notation und Type Prefixes sind verboten.

## 16. Commands, Queries und Results

Ein Public Operation Type MUST Intent und Outcome ausdrücken. Command Name nutzt das imperative Konzept, Event den vergangenen Fakt und Query die gewünschte View. Ein Suffix wird nur für einen echten Contract Type verwendet.

| Contract kind | Pattern | Beispiel |
|---|---|---|
| Command type | `<Verb><Subject>Command` | `CreateCheckpointCommand` |
| Query type | `<FindOrGet><Subject>Query` | `FindMemoriesQuery` |
| Result type | `<Operation>Result` | `CreateCheckpointResult` |
| Request DTO | `<Operation>Request` | `DialogueContextRequest` |
| Response DTO | `<Operation>Response` | `DialogueContextResponse` |

Ein `Command` heißt nicht `Event`, auch wenn beide über denselben Transport laufen.

## 17. Event Names

Der allgemeine lexical baseline lautet: lowercase dotted segments, Domain zuerst, Fakt in Past Tense, Major Schema Version am Ende. Vollständige Event Taxonomy, Namespace Allocation und Lifecycle gehören EVT-002 und EVT-004; sie MUST mit STD-002 kompatibel sein.

| Element | Regel | Beispiel |
|---|---|---|
| Domain | lowercase noun | `memory` |
| Subject | lowercase noun | `record` |
| Fact | past-tense token | `created` |
| Major version | `v<integer>` | `v1` |
| Full name | `<domain>.<subject>.<fact>.v<major>` | `memory.record.created.v1` |
| C++ type | PascalCase plus Event | `MemoryRecordCreatedEvent` |

Environment, Transport, Timestamp oder Producer Implementation gehören nicht in den Event Name. `new_memory`, `memory_update`, `onActor` und `event1` sind verboten.

## 18. Capability Tokens

Ein Capability Token nutzt dotted.lowercase action-oriented segments und bleibt über Profiles stabil. Aktuelle gültige MOD-002-Beispiele sind `runtime.identity`, `dialogue.context.observe`, `ui.subtitle.show` und `safe_point.evidence`.

| Regel | Beispiel |
|---|---|
| Mit Domain oder Target beginnen | `player.location.observe` |
| Mit Action oder Capability enden | `voice.playback.trigger` |
| Availability State nicht einbauen | `world.time.observe`, nicht `world.time.available` |
| Version in Profile oder Contract führen | `runtime.identity`, nicht unnötig `runtime.identity.v1` |

## 19. Error Codes

Ein Machine Error Code nutzt lowercase dotted segments: `<domain>.<operation>.<reason>`. Er ist stabil, enthält keinen lokalisierten Text und kein Secret. Ein C++ Enum Value darf das snake_case equivalent verwenden.

| Ebene | Beispiel |
|---|---|
| Domain | `memory` |
| Operation | `candidate.validate` |
| Reason | `invalid_provenance` |
| Full code | `memory.candidate.validate.invalid_provenance` |
| Generic boundary code | `game_adapter.runtime.unsupported` |

`failed`, `error` oder `unknown` ohne Domain und Reason sind in einem Public Result verboten, außer als letzter Internal Fallback mit separatem Evidence Record.

## 20. Configuration Names

Config Files nutzen einen kebab-case basename und ein explizites Format, zum Beispiel `skyrim-se-ae.profile.json`. Fields verwenden snake_case. Hierarchical Keys verwenden Nested Objects statt eines simulierten Path in einem Namen, wenn das Format Nesting unterstützt.

| Objekt | Rule | Beispiel |
|---|---|---|
| Profile file | `<target>.profile.json` | `skyrim-se-ae.profile.json` |
| Schema file | `<subject>.schema.json` | `skyrim-profile.schema.json` |
| Field | snake_case | `max_payload_bytes` |
| Duration | suffix `_ms`, `_s`, `_min` | `request_timeout_ms` |
| Size | `_bytes`, `_kib`, `_mib` | `max_payload_bytes` |
| Boolean | positive adjective | `fallback_enabled` |
| Secret reference | `_secret_ref` | `provider_api_secret_ref` |

Ein Field `password`, `token` oder `api_key` ist in normaler Config verboten; nur eine Reference auf den Secret Provider wird gespeichert.

## 21. JSON und Serialized Fields

JSON Property Names verwenden snake_case ASCII. Enum Values nutzen lowercase snake_case. Optionality wird im Schema definiert, nicht durch einen Namenssuffix. Version Fields sind explizit.

| Zweck | Pattern | Beispiel |
|---|---|---|
| Object identity | `<subject>_id` | `event_id` |
| Foreign reference | `<subject>_id` oder `<subject>_ids` | `related_entity_ids` |
| Version | `<subject>_version` | `profile_version` |
| Timestamp | `<meaning>_at_unix_ms` für numeric epoch | `observed_at_unix_ms` |
| ISO timestamp | `<meaning>_at` mit schema format | `recorded_at` |
| Raw serialized object | `_json` nur an bewusster Boundary | `payload_json` |

`payload_json` ist nur bei Bounded Validation ein zulässiger Transitional Contract; ein Typed Nested Schema ist vorzuziehen.

## 22. Database Objects

Physische Database Names verwenden lowercase snake_case. Ein Table Name SHOULD für Collections plural sein; der Primary Key heißt `<singular>_id`; ein Foreign Key entspricht dem referenced primary key. Constraint und Index Names werden deterministisch gebildet.

| Objekt | Pattern | Beispiel |
|---|---|---|
| Table | plural snake_case | `memory_records` |
| Primary key | `<entity>_id` | `memory_id` |
| Foreign key | referenced key name | `world_id` |
| Unique constraint | `uq_<table>_<columns>` | `uq_memory_records_memory_id` |
| Foreign-key constraint | `fk_<table>_<ref_table>` | `fk_memory_records_worlds` |
| Index | `ix_<table>_<columns>` | `ix_memory_records_subject_id` |
| Migration | `<sequence>_<verb>_<subject>` | `0001_create_memory_records` |

Die endgültigen Table- und Index-Regeln werden in DB-004–006 angenommen; sie MUST diesen Lexical Baseline erhalten oder eine Abweichung ausdrücklich versionieren.

## 23. Logs, Metrics und Traces

Log Event Name und Metric Name sind Machine Contracts. Ein Log Name nutzt dotted.lowercase fact; Structured Fields nutzen snake_case. Eine Metric folgt für Prometheus-like Exports `<namespace>_<subject>_<measurement>_<unit>`.

| Objekt | Pattern | Beispiel |
|---|---|---|
| Log event | `<domain>.<operation>.<fact>` | `memory.restore.completed` |
| Log field | snake_case | `correlation_id` |
| Counter | suffix `_total` | `world_engine_events_rejected_total` |
| Duration metric | suffix `_seconds` | `world_engine_restore_duration_seconds` |
| Size metric | suffix `_bytes` | `world_engine_checkpoint_size_bytes` |
| Trace span | `<domain>.<operation>` | `memory.retrieve` |

Metric Names enthalten keine Dynamic IDs. Dynamic Identity gehört nur in Bounded Labels und MUST keine unbounded cardinality erzeugen.

## 24. Tests

Eine Test Source File heißt `<subject>_tests.cpp`. Ein Test Case beschreibt Condition und Outcome. Eine Requirement ID steht in Metadata, Tag oder Nearby Comment und ist nicht der einzige lesbare Name.

| Ebene | Pattern | Beispiel |
|---|---|---|
| File | `<subject>_tests.cpp` | `game_adapter_tests.cpp` |
| Suite | `<Subject><Level>Tests` | `MemoryRepositoryIntegrationTests` |
| Case | `condition_expected_outcome` | `duplicate_event_is_ignored` |
| Fixture type | `<Subject>TestFixture` | `SaveRestoreTestFixture` |
| Fake | `Fake<Role>` | `FakeRuntimeBridge` |
| Stub | `Stub<Role>` | `StubAiProvider` |
| Golden data | `<subject>_<scenario>_v<major>` | `memory_conflict_restore_v1` |

`test1`, `works`, `happy_path`, `misc_tests` und Namen, die nur den Function Name wiederholen, sind verboten.

## 25. Build Targets und Packages

| Objekt | Pattern | Beispiel |
|---|---|---|
| CMake library target | `world_engine_<module>` | `world_engine_game_adapter` |
| CMake test target | `<library>_tests` | `world_engine_game_adapter_tests` |
| Native plugin target | `world_engine_<game>_plugin` | `world_engine_skyrim_plugin` |
| Install component | `world-engine-<component>` | `world-engine-game-adapter` |
| Package file | `<name>-<semver>-<platform>` | `world-engine-0.1.0-windows-x64` |

Debug- und Release-Marker gehören in Build Metadata und ersetzen keine Semantic Package Version.

## 26. Game, Platform und Plugin IDs

Eine Stable External ID nutzt lowercase kebab-case. Distribution und Runtime Version sind separate Fields und werden nicht in einen unlesbaren String geklebt.

| Konzept | Pattern | Beispiel |
|---|---|---|
| Game family | kebab-case | `skyrim` |
| Supported scope ID | kebab-case composite | `skyrim-se-ae` |
| Excluded platform | explicit ID | `skyrim-vr` |
| Distribution | lowercase enum | `steam`, `gog` |
| Edition | lowercase enum | `anniversary`, `special_legacy` |
| Runtime version | vendor numeric string | `1.6.1179` |
| Adapter ID | reverse project hierarchy | `world-engine.mod-002.skyrim` |

Ein Marketing Title dient nicht ohne normalized ID als Machine Identity.

## 27. Branch und Commit Lexical Baseline

Der vollständige Workflow gehört STD-005–007. STD-002 definiert nur die Namensform. Ein Branch nutzt `<kind>/<scope>-<short-description>` in lowercase kebab-case. Ein Commit Subject nutzt `<type>(<scope>): <imperative summary>`.

| Object | Pattern | Beispiel |
|---|---|---|
| Documentation branch | `docs/<scope>-<description>` | `docs/pre-code-foundation-plan` |
| Feature branch | `feature/<scope>-<description>` | `feature/mod-002-game-adapter-se-ae` |
| Audit branch | `audit/<scope>-<date>` | `audit/architecture-consistency-2026-09-01` |
| Fix branch | `fix/<scope>-<description>` | `fix/save-restore-validation` |
| Commit | `type(scope): summary` | `docs(standards): define canonical glossary` |

`master-final`, `new`, `test`, Personennamen, `work2` und Branch Names ohne Scope sind verboten.

## 28. Zeit, Größen und Units

Ein Numeric Field MUST die Unit im Namen tragen, sofern das Type System keine Dimension erzwingt. SI- oder IEC-Suffixe werden konsistent verwendet. Ein Timestamp Name benennt seine Representation.

| Dimension | Suffix | Beispiel |
|---|---|---|
| Milliseconds | `_ms` | `timeout_ms` |
| Seconds | `_s` oder `_seconds` für metric | `retry_delay_s` |
| Bytes | `_bytes` | `payload_size_bytes` |
| KiB/MiB | `_kib`, `_mib` | `cache_limit_mib` |
| Count | `_count` | `retry_count` |
| Ratio | `_ratio` oder bounded type | `cache_hit_ratio` |
| UTC ISO instant | `_at` | `created_at` |
| Unix epoch ms | `_at_unix_ms` | `observed_at_unix_ms` |

Plural und Singular einer Unit werden nicht innerhalb einer API Family vermischt.

## 29. Acronyms und Abkürzungen

Ein bekannter Acronym wird im PascalCase Type wie ein normales Wort behandelt: `AiProvider`, `ApiVersion`, `NpcId` statt `AIProvider`, sofern es ein neuer C++ Symbol ist. Ausnahmen gelten für etablierte externe Namen `SKSE`, `TTS`, `STT` im Fließtext oder in Constant Tokens.

Bei vorhandenen veröffentlichten Symbols steht Compatibility über kosmetischer Korrektur. Neue Abbreviations müssen vor Public Use in STD-001 aufgenommen werden.

## 30. Verbotene Patterns

| Pattern | Verbotener Grund | Ersatz |
|---|---|---|
| `data`, `info`, `thing`, `object` | Keine Domain Meaning | Konkreter Record, Value oder Entity |
| `manager`, `service`, `helper`, `util` | Verdeckt Responsibility | Role-specific noun |
| `temp`, `new`, `old`, `final`, `latest` | Nicht versioniert und schnell veraltet | Version oder lifecycle metadata |
| `flag1`, `value2`, `misc` | Nicht suchbar | Purpose-specific name |
| Negativer Boolean | Schwer lesbar | Positive predicate |
| Type im Variable Name | Erzeugt Noise und Drift | Semantic noun |
| Lokalisierter Machine Name | Bricht Parity und Tooling | English canonical token |
| Dynamic ID im Metric Name | Cardinality Explosion | Bounded label |
| Secret im Namen oder Path | Data Leak | Secret reference |

## 31. Rename, Deprecation und Migration

Ein Public Rename ist ein Contract Change. Der neue Name wird mit Alias oder Adapter, Deprecation Notice, Compatibility Window, Migration Tool oder Data Migration und Removal Condition eingeführt. Ein Persisted Field darf nicht allein durch Änderung des Serializers umbenannt werden.

| Schritt | Pflichtnachweis |
|---|---|
| Identify | Alter Name, Owner, Consumers und Persisted Locations |
| Decide | Grund, Compatibility Class und Target Version |
| Bridge | Alias, Parser Compatibility oder Migration |
| Observe | Usage Metric oder Repository Search |
| Remove | Expired Window, keine Supported Consumers und Release Note |

## 32. Review Checklist

- Der Name verwendet ein Canonical Concept aus STD-001.
- Das Casing entspricht dem Artifact Type.
- Scope wird nicht unnötig wiederholt.
- Boolean ist positiv; Numeric Value besitzt eine Unit.
- Public Name enthält kein Implementation Detail.
- Event ist Past Fact, Command ein Intent und Query eine Read Request.
- Error Code enthält Domain, Operation und Reason.
- Persisted- oder API-Rename besitzt Migration und Compatibility Plan.
- RU- und DE-Dokumente verwenden denselben Machine Token.
- Vorhandene MOD-002 Identifiers werden nicht durch kosmetischen Refactor gebrochen.

## 33. Acceptance Criteria

STD-002 gilt in einem neuen Package als umgesetzt, wenn Automated Lint oder Review Filename, Path, Language, C++ Casing, JSON Field, Event- und Capability Token, Error Code, Test Name und Unit Suffix Rules bestätigt. Eine Ausnahme MUST Owner, Reason, affected identifiers, Expiry und Link zum Decision Record enthalten.

## Anhang A. Durchgängiges Beispiel

| Layer | Name |
|---|---|
| Document | `MOD-003_Memory_Engine_DE_v1_0_Full_Content_Implementation_Specification.docx` |
| Namespace | `world_engine::memory_engine` |
| Port | `IMemoryContextSourcePort` |
| Record | `MemoryRecord` |
| Field | `memory_id` |
| Event | `memory.record.created.v1` |
| C++ event type | `MemoryRecordCreatedEvent` |
| Error | `memory.candidate.validate.invalid_provenance` |
| Metric | `world_engine_memory_candidates_rejected_total` |
| Test | `invalid_provenance_is_rejected` |

## Anhang B. Compatibility mit MOD-002

Die bestehenden Namen `world_engine::game_adapter`, `GameAdapter`, `IGameIngressPort`, `IRuntimeBridge`, `AdapterState::initializing`, `kContractSchemaVersion`, `max_payload_bytes`, `runtime.identity`, `skyrim-se-ae.profile.json` und `world-engine.mod-002.skyrim` entsprechen STD-002 und benötigen keinen Rename.

## Anhang C. Traceability

| Quelle | Festgelegter Einfluss |
|---|---|
| STD-001 | Canonical meanings and abbreviations |
| PLAN-001 | Document families and Foundation Gates |
| ARCH-004/005 and ADR-008 | Ownership-neutral naming and dependency boundaries |
| ADR-002 | Event fact semantics and idempotency metadata |
| ARCH-009 | Typed configuration and safe defaults |
| ADR-003 and MOD-003 | Repository, schema and migration names |
| ADR-009 and MOD-002 | Skyrim SE/AE scope and existing machine identifiers |

## Anhang D. Änderungsjournal

| Version | Datum | Änderung |
|---|---|---|
| 1.0 | 2026-09-05 | Vollständiger Naming Baseline für Documents, Paths, Versions, Modules, C++, Contracts, Events, Capabilities, Errors, Configuration, Schemas, Database, Observability, Tests, Builds, Platforms und Version-control Artifacts erstellt. |
