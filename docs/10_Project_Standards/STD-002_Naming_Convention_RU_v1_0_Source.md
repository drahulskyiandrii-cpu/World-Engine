# STD-002 World Engine Naming Convention

Версия 1.0 · Русский · 2026-09-05 · Current normative standard

## 1. Назначение

STD-002 устанавливает обязательные правила именования документов, каталогов, модулей, C++ symbols, contracts, events, schemas, configuration, database objects, logs, metrics, tests, builds и version-control artifacts World Engine. Цель — сделать имя предсказуемым, однозначным и пригодным для поиска до чтения реализации.

Стандарт закрепляет форму имени, но не меняет ownership. Например, правило записи Event Name не передаёт владение event schema от producer другому модулю. Значения терминов определяет STD-001.

## 2. Область и совместимость

Правила обязательны для новых public names после публикации STD-002. Уже опубликованные identifiers MOD-002 сохраняются и рассматриваются как совместимый baseline. Их переименование допускается только с migration и compatibility evidence; косметическая чистота не оправдывает breaking change.

| Объект | Действие для нового | Действие для существующего |
|---|---|---|
| Public contract | Сразу соблюдать STD-002 | Не переименовывать без contract versioning |
| Internal symbol | Соблюдать при создании и существенном редактировании | Можно исправить атомарным refactor с tests |
| Persisted field | Сразу соблюдать schema rule | Только через migration |
| Log или metric | Сразу соблюдать стабильное имя | Сохранять alias на период dashboard migration |
| Документ | Использовать canonical filename | Предыдущая версия остаётся в history/archive |

## 3. Нормативные слова

`MUST` означает обязательное правило; `MUST NOT` — запрет; `SHOULD` — предпочтение, от которого можно отступить с записанной причиной; `MAY` — разрешённый вариант. Перевод RU/DE не меняет силу этих слов.

## 4. Общие свойства хорошего имени

Имя MUST выражать domain meaning или техническую роль, иметь один scope и избегать лишней информации, уже заданной namespace или path. Имена MUST быть английскими ASCII identifiers. Комментарии и пользовательский текст могут быть RU или DE, но machine names не переводятся.

| Свойство | Требование | Плохой пример | Хороший пример |
|---|---|---|---|
| Однозначность | Один concept на имя | `data` | `memory_record` |
| Scope | Контекст виден в namespace/path | `Manager` | `SaveCoordinator` |
| Положительная форма | Boolean читается как утверждение | `not_disabled` | `enabled` |
| Единица | Число указывает unit | `timeout` | `timeout_ms` |
| Стабильность | Не включать временную реализацию | `sqlite_memory_service` | `MemoryRepository` |
| Поисковость | Не использовать неясные сокращения | `cfg_mgr` | `ConfigurationRegistry` |

## 5. Системы регистра

| Стиль | Форма | Применение |
|---|---|---|
| PascalCase | `MemoryRecord` | C++ types, enum types, concepts |
| camelCase | `memoryRecord` | Не используется в C++ public API; допускается только внешним provider schema при необходимости |
| snake_case | `memory_record` | C++ functions/variables, JSON/YAML fields, database objects |
| SCREAMING_SNAKE_CASE | `WORLD_ENGINE_VERSION` | Preprocessor macros только |
| kPascalCase | `kContractSchemaVersion` | C++ compile-time constants |
| kebab-case | `skyrim-se-ae` | Stable external IDs, profiles и selected package names |
| dotted.lowercase | `runtime.identity` | Capability tokens, metric namespaces и event name segments |

Смешение стилей в одном identifier запрещено, кроме утверждённых суффиксов version или unit.

## 6. Идентификаторы документов

Document ID состоит из uppercase family, дефиса и трёхзначного номера: `ARCH-004`, `ADR-009`, `MOD-003`, `STD-002`. Control families могут иметь отдельную серию, например `PLAN-001`. ID не переиспользуется после удаления или supersede.

| Family | Назначение | Пример |
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

## 7. Имена файлов документов

Canonical Word filename MUST иметь форму `<ID>_<English_Title>_<LANG>_v<major>_<minor>_<Lifecycle>.docx`. Слова разделяются underscore, title использует Pascal-like words без пунктуации. `LANG` равен `RU` или `DE`.

| Элемент | Правило | Пример |
|---|---|---|
| ID | Exact document ID | `STD-002` |
| English Title | Стабильное английское имя | `Naming_Convention` |
| Language | `RU` или `DE` | `RU` |
| Version | `v1_0`, без точки в filename | `v1_0` |
| Lifecycle | `Current`, `Accepted`, `Audited_Complete` или утверждённая специфика | `Current` |
| Source | Заменяет `.docx` на `_Source.md` с тем же version prefix | `STD-002_Naming_Convention_RU_v1_0_Source.md` |
| QA | Один language-neutral отчёт | `STD-002_v1_0_Full_Document_QA.md` |

Дата не включается в canonical filename, если version уже обеспечивает identity. Дата допускается в archive folder или одноразовом audit evidence.

## 8. Каталоги документации

Top-level каталог docs использует двухзначный numeric order и English_Title: `10_Project_Standards`. Каталог конкретного ARCH, ADR или MOD использует полный ID и English_Title: `MOD-003_Memory_Engine`. Project-management и standards files могут находиться прямо в своих stage folders, если структура остаётся однозначной.

| Тип | Pattern | Пример |
|---|---|---|
| Stage folder | `<NN>_<English_Title>` | `05_Event_System` |
| Item folder | `<ID>_<English_Title>` | `ARCH-011_Save_System_Architecture` |
| Archive root | `<NN>_<Purpose>_<YYYY-MM-DD>` | `05_MOD002_M2_1_Baseline_2026-09-05` |
| Scratch folder | Dot-prefixed and ignored | `.foundation-work` |

`final`, `new`, `old`, `temp`, `misc`, `latest` и `copy` запрещены как единственный differentiator в canonical paths.

## 9. Версии

Документы используют `major.minor`; public contracts и packages SHOULD использовать Semantic Versioning `major.minor.patch`. Filename преобразует точку в underscore только для document version. Schema major может быть compact suffix `v1` внутри type или event identity.

| Изменение | Document version | Contract version |
|---|---|---|
| Редакционная ясность без изменения смысла | Minor | Без изменения |
| Новая совместимая секция или правило | Minor | Minor при public effect |
| Несовместимое изменение требования | Major | Major |
| Исправление implementation bug | Не обязательно | Patch package version |

## 10. Module IDs и names

Номер модуля фиксирован в формате `MOD-001`…`MOD-014`. Human title использует Pascal-spaced English phrase: `Memory Engine`. Machine module key использует lowercase dotted или kebab form в зависимости от среды.

| Представление | Pattern | Пример |
|---|---|---|
| Document ID | `MOD-NNN` | `MOD-002` |
| Folder | `MOD-NNN_English_Title` | `MOD-002_Game_Adapter` |
| C++ namespace segment | `snake_case` | `game_adapter` |
| Stable adapter ID | `world-engine.mod-nnn.target` | `world-engine.mod-002.skyrim` |
| CMake target | `world_engine_<module>` | `world_engine_game_adapter` |
| Display title | English words | `Game Adapter` |

Номер модуля не включается в C++ type name, если namespace уже однозначен.

## 11. C++ namespaces

Root namespace MUST быть `world_engine`. Каждый модуль получает один основной snake_case segment: `world_engine::memory_engine`. Внутренние детали могут находиться в `detail`, но public headers MUST NOT требовать знания `detail`.

| Scope | Правило | Пример |
|---|---|---|
| Root | Fixed | `world_engine` |
| Module | snake_case | `world_engine::game_adapter` |
| Tests | Same namespace или anonymous namespace | `world_engine::game_adapter` |
| Private translation-unit helper | Anonymous namespace | `namespace { ... }` |
| Generated/provider code | Isolated subtree | `world_engine::ai::provider` |

`using namespace` запрещён в public headers. В test `.cpp` он MAY использоваться только для одного module namespace и при отсутствии collision.

## 12. C++ types

Classes, structs, enums, aliases и concepts используют PascalCase. Имя отвечает на вопрос «что это», а не «что оно делает сейчас». Суффиксы применяются только при устойчивой роли.

| Категория | Pattern | Пример |
|---|---|---|
| Entity/value type | PascalCase noun | `MemoryRecord` |
| Input envelope | `<Purpose>Envelope` | `IngressEnvelope` |
| Result | `<Operation>Result` или общий `OperationResult` | `RestoreResult` |
| Configuration type | `<Scope>Config` | `GameAdapterConfig` |
| Evidence type | `<Subject>Evidence` | `CapabilityEvidence` |
| Descriptor | `<Subject>Descriptor` | `ModuleDescriptor` |
| Policy | `<Subject>Policy` | `RetentionPolicy` |
| Exception class | `<Cause>Error` только для exceptional API | `ConfigurationError` |

Суффиксы `Data`, `Info`, `Object`, `Thing`, `Helper`, `Util`, `Manager` запрещены без конкретной семантики.

## 13. C++ interfaces, ports и adapters

Чистый abstract C++ interface использует префикс `I` для совместимости с текущим MOD-002 baseline. Port name MUST указывать direction или capability; adapter name указывает внешний target.

| Роль | Pattern | Пример |
|---|---|---|
| Inbound port | `I<Domain><Purpose>Port` | `IGameIngressPort` |
| Outbound port | `I<Target><Purpose>Port` | `IGameOutputCapabilityPort` |
| Repository contract | `I<Aggregate>Repository` | `IMemoryRepository` |
| Status sink | `I<Subject>StatusSink` | `IGameAdapterStatusSink` |
| Runtime gateway | `I<Target>Bridge` | `IRuntimeBridge` |
| Concrete adapter | `<Target><Role>Adapter` | `SkyrimRuntimeAdapter` |

`IService`, `IManager` и `IRepository` без domain noun запрещены. Contract owner определяется architecture, не названием реализации.

## 14. C++ functions и methods

Functions и methods используют snake_case verb phrase. Query-like method описывает возвращаемое значение; command-like method — действие. Boolean predicate начинается с `is_`, `has_`, `can_` или `should_`.

| Тип | Pattern | Пример |
|---|---|---|
| Command | verb_object | `publish_status` |
| Query | noun или verb_query | `capabilities`, `find_memories` |
| Boolean | predicate prefix | `is_ready`, `has_capability` |
| Conversion | `to_<target>` | `to_string` |
| Factory | `create_<type>` или named outcome | `create_checkpoint` |
| Validation | `validate_<subject>` | `validate_profile` |
| Try operation | `try_<verb>` только с non-throwing outcome | `try_restore` |

`get_` SHOULD использоваться только когда операция действительно является accessor без I/O или существенной computation. Имена `process`, `handle`, `do`, `run` требуют объекта или context.

## 15. C++ variables, fields и constants

Variables и fields используют snake_case nouns. Unit входит в имя числового значения. Constants используют `kPascalCase`. Enum values используют snake_case для совместимости с MOD-002.

| Категория | Pattern | Пример |
|---|---|---|
| Local variable | snake_case | `runtime_profile` |
| Parameter | snake_case | `correlation_id` |
| Data member | snake_case | `max_payload_bytes` |
| Compile-time constant | kPascalCase | `kContractSchemaVersion` |
| Macro | SCREAMING_SNAKE_CASE | `WORLD_ENGINE_EXPORT` |
| Enum type | PascalCase | `AdapterState` |
| Enum value | snake_case | `initializing` |
| Boolean | positive predicate | `validation_enabled` |

Однобуквенные имена допустимы только для короткого index/math scope. Hungarian notation и type prefixes запрещены.

## 16. Commands, queries и results

Public operation type MUST выражать intent и outcome. Command name uses imperative concept; event uses past fact; query uses requested view. Suffix указывается только если это реальный contract type.

| Contract kind | Pattern | Пример |
|---|---|---|
| Command type | `<Verb><Subject>Command` | `CreateCheckpointCommand` |
| Query type | `<FindOrGet><Subject>Query` | `FindMemoriesQuery` |
| Result type | `<Operation>Result` | `CreateCheckpointResult` |
| Request DTO | `<Operation>Request` | `DialogueContextRequest` |
| Response DTO | `<Operation>Response` | `DialogueContextResponse` |

`Command` не называется `Event`, даже если передаётся через тот же transport.

## 17. Event names

Общий lexical baseline: lowercase dotted segments, domain first, факт в past-tense, major schema version в конце. Полная event taxonomy, namespace allocation и lifecycle принадлежат EVT-002 и EVT-004; они MUST быть совместимы с базовыми правилами STD-002.

| Элемент | Правило | Пример |
|---|---|---|
| Domain | lowercase noun | `memory` |
| Subject | lowercase noun | `record` |
| Fact | past-tense token | `created` |
| Major version | `v<integer>` | `v1` |
| Full name | `<domain>.<subject>.<fact>.v<major>` | `memory.record.created.v1` |
| C++ type | PascalCase plus Event | `MemoryRecordCreatedEvent` |

Нельзя включать environment, transport, timestamp или producer implementation в event name. `new_memory`, `memory_update`, `onActor` и `event1` запрещены.

## 18. Capability tokens

Capability token uses dotted.lowercase verb-oriented segments and remains stable across profiles. Current MOD-002 tokens are valid examples: `runtime.identity`, `dialogue.context.observe`, `ui.subtitle.show`, `safe_point.evidence`.

| Правило | Пример |
|---|---|
| Начинать с domain или target | `player.location.observe` |
| Завершать action/capability | `voice.playback.trigger` |
| Не включать availability state | `world.time.observe`, не `world.time.available` |
| Version выносить в profile/contract | `runtime.identity`, не `runtime.identity.v1` без необходимости |

## 19. Error codes

Machine error code uses lowercase dotted segments: `<domain>.<operation>.<reason>`. Он стабилен, не содержит локализованный текст и не раскрывает secret. C++ enum value может использовать snake_case equivalent.

| Уровень | Пример |
|---|---|
| Domain | `memory` |
| Operation | `candidate.validate` |
| Reason | `invalid_provenance` |
| Full code | `memory.candidate.validate.invalid_provenance` |
| Generic boundary code | `game_adapter.runtime.unsupported` |

`failed`, `error`, `unknown` без domain и reason запрещены для public result, кроме последнего internal fallback с отдельной evidence записью.

## 20. Configuration names

Config files use kebab-case basename and explicit format: `skyrim-se-ae.profile.json`. Fields use snake_case. Hierarchical keys use nested objects, а не имитацию path в одном имени, если format поддерживает nesting.

| Объект | Rule | Пример |
|---|---|---|
| Profile file | `<target>.profile.json` | `skyrim-se-ae.profile.json` |
| Schema file | `<subject>.schema.json` | `skyrim-profile.schema.json` |
| Field | snake_case | `max_payload_bytes` |
| Duration | suffix `_ms`, `_s`, `_min` | `request_timeout_ms` |
| Size | `_bytes`, `_kib`, `_mib` | `max_payload_bytes` |
| Boolean | positive adjective | `fallback_enabled` |
| Secret reference | `_secret_ref` | `provider_api_secret_ref` |

Поле `password`, `token` или `api_key` в обычном config запрещено; хранится только reference на secret provider.

## 21. JSON и serialized fields

JSON property names use snake_case ASCII. Enum values use lowercase snake_case. Optionality определяется schema, а не суффиксом имени. Version fields are explicit.

| Назначение | Pattern | Пример |
|---|---|---|
| Object identity | `<subject>_id` | `event_id` |
| Foreign reference | `<subject>_id` или `<subject>_ids` | `related_entity_ids` |
| Version | `<subject>_version` | `profile_version` |
| Timestamp | `<meaning>_at_unix_ms` для numeric epoch | `observed_at_unix_ms` |
| ISO timestamp | `<meaning>_at` с schema format | `recorded_at` |
| Raw serialized object | `_json` только при сознательной boundary | `payload_json` |

`payload_json` является допустимым transitional contract только при bounded validation; предпочтительнее typed nested schema.

## 22. Database objects

Physical database names use lowercase snake_case. Table name SHOULD быть plural для collections; primary key `<singular>_id`; foreign key совпадает с referenced primary key. Constraint и index names строятся детерминированно.

| Объект | Pattern | Пример |
|---|---|---|
| Table | plural snake_case | `memory_records` |
| Primary key | `<entity>_id` | `memory_id` |
| Foreign key | referenced key name | `world_id` |
| Unique constraint | `uq_<table>_<columns>` | `uq_memory_records_memory_id` |
| Foreign-key constraint | `fk_<table>_<ref_table>` | `fk_memory_records_worlds` |
| Index | `ix_<table>_<columns>` | `ix_memory_records_subject_id` |
| Migration | `<sequence>_<verb>_<subject>` | `0001_create_memory_records` |

Окончательные table/index rules утверждаются DB-004–006; они MUST сохранять этот lexical baseline или явно версионировать отклонение.

## 23. Logs, metrics и traces

Log event name и metric name являются machine contracts. Log name uses dotted.lowercase fact; structured fields use snake_case. Metric follows `<namespace>_<subject>_<measurement>_<unit>` для Prometheus-like exports.

| Объект | Pattern | Пример |
|---|---|---|
| Log event | `<domain>.<operation>.<fact>` | `memory.restore.completed` |
| Log field | snake_case | `correlation_id` |
| Counter | suffix `_total` | `world_engine_events_rejected_total` |
| Duration metric | suffix `_seconds` | `world_engine_restore_duration_seconds` |
| Size metric | suffix `_bytes` | `world_engine_checkpoint_size_bytes` |
| Trace span | `<domain>.<operation>` | `memory.retrieve` |

Metric names never include dynamic IDs. Dynamic identity belongs to bounded labels and MUST NOT create unbounded cardinality.

## 24. Tests

Test source file uses `<subject>_tests.cpp`. Test case name describes condition and outcome. Requirement ID appears in metadata/tag или nearby comment, not as единственное читаемое имя.

| Уровень | Pattern | Пример |
|---|---|---|
| File | `<subject>_tests.cpp` | `game_adapter_tests.cpp` |
| Suite | `<Subject><Level>Tests` | `MemoryRepositoryIntegrationTests` |
| Case | `condition_expected_outcome` | `duplicate_event_is_ignored` |
| Fixture type | `<Subject>TestFixture` | `SaveRestoreTestFixture` |
| Fake | `Fake<Role>` | `FakeRuntimeBridge` |
| Stub | `Stub<Role>` | `StubAiProvider` |
| Golden data | `<subject>_<scenario>_v<major>` | `memory_conflict_restore_v1` |

`test1`, `works`, `happy_path`, `misc_tests` и names, повторяющие только function name, запрещены.

## 25. Build targets и packages

| Объект | Pattern | Пример |
|---|---|---|
| CMake library target | `world_engine_<module>` | `world_engine_game_adapter` |
| CMake test target | `<library>_tests` | `world_engine_game_adapter_tests` |
| Native plugin target | `world_engine_<game>_plugin` | `world_engine_skyrim_plugin` |
| Install component | `world-engine-<component>` | `world-engine-game-adapter` |
| Package file | `<name>-<semver>-<platform>` | `world-engine-0.1.0-windows-x64` |

Debug/release markers принадлежат build metadata и не заменяют semantic package version.

## 26. Game, platform и plugin IDs

Stable external ID uses kebab-case lowercase. Distribution и runtime version являются отдельными fields, не склеиваются в неразбираемую строку.

| Concept | Pattern | Пример |
|---|---|---|
| Game family | kebab-case | `skyrim` |
| Supported scope ID | kebab-case composite | `skyrim-se-ae` |
| Excluded platform | explicit ID | `skyrim-vr` |
| Distribution | lowercase enum | `steam`, `gog` |
| Edition | lowercase enum | `anniversary`, `special_legacy` |
| Runtime version | vendor numeric string | `1.6.1179` |
| Adapter ID | reverse project hierarchy | `world-engine.mod-002.skyrim` |

Marketing title не используется как machine identity без normalized ID.

## 27. Branch и commit lexical baseline

Полный workflow принадлежит STD-005–007. STD-002 задаёт только форму имени. Branch uses `<kind>/<scope>-<short-description>` lowercase kebab-case. Commit subject uses `<type>(<scope>): <imperative summary>`.

| Object | Pattern | Пример |
|---|---|---|
| Documentation branch | `docs/<scope>-<description>` | `docs/pre-code-foundation-plan` |
| Feature branch | `feature/<scope>-<description>` | `feature/mod-002-game-adapter-se-ae` |
| Audit branch | `audit/<scope>-<date>` | `audit/architecture-consistency-2026-09-01` |
| Fix branch | `fix/<scope>-<description>` | `fix/save-restore-validation` |
| Commit | `type(scope): summary` | `docs(standards): define canonical glossary` |

`master-final`, `new`, `test`, `andrii`, `work2` и branch names без scope запрещены.

## 28. Время, размеры и units

Числовое поле MUST включать unit, если dimension не задан type system. Используются SI или IEC suffixes последовательно. Timestamp name сообщает representation.

| Dimension | Suffix | Пример |
|---|---|---|
| Milliseconds | `_ms` | `timeout_ms` |
| Seconds | `_s` или `_seconds` для metric | `retry_delay_s` |
| Bytes | `_bytes` | `payload_size_bytes` |
| KiB/MiB | `_kib`, `_mib` | `cache_limit_mib` |
| Count | `_count` | `retry_count` |
| Ratio | `_ratio` или bounded type | `cache_hit_ratio` |
| UTC ISO instant | `_at` | `created_at` |
| Unix epoch ms | `_at_unix_ms` | `observed_at_unix_ms` |

Plural unit и singular unit не смешиваются в одной API family.

## 29. Acronyms и сокращения

Известный acronym в PascalCase type рассматривается как обычное слово: `AiProvider`, `ApiVersion`, `NpcId`, а не `AIProvider`, если это новый C++ symbol. Исключения возможны для закреплённых внешних названий `SKSE`, `TTS`, `STT` в prose или constant tokens.

В существующем опубликованном symbol compatibility важнее косметического исправления. Новые abbreviations должны быть внесены в STD-001 до public use.

## 30. Запрещённые patterns

| Pattern | Почему запрещён | Замена |
|---|---|---|
| `data`, `info`, `thing`, `object` | Не выражает domain meaning | Конкретный record/value/entity |
| `manager`, `service`, `helper`, `util` | Скрывает responsibility | Role-specific noun |
| `temp`, `new`, `old`, `final`, `latest` | Не versioned и быстро устаревает | Version/lifecycle metadata |
| `flag1`, `value2`, `misc` | Не searchable | Purpose-specific name |
| Отрицательный boolean | Трудно читать | Positive predicate |
| Type encoded in variable | Создаёт шум и drift | Semantic noun |
| Локализованный machine name | Ломает parity и tooling | English canonical token |
| Dynamic ID в metric name | Cardinality explosion | Bounded label |
| Secret value в имени или path | Data leak | Secret reference |

## 31. Rename, deprecation и migration

Public rename считается contract change. Новый name добавляется вместе с alias/adaptor, deprecation notice, compatibility window, migration tool или data migration и removal condition. Persisted field нельзя переименовать только изменением serializer.

| Шаг | Обязательное доказательство |
|---|---|
| Identify | Старое имя, owner, consumers и persisted locations |
| Decide | Причина, compatibility class и target version |
| Bridge | Alias, parser compatibility или migration |
| Observe | Usage metric или repository search |
| Remove | Expired window, zero supported consumers и release note |

## 32. Review checklist

- Имя использует canonical concept из STD-001.
- Casing соответствует типу artifact.
- Scope не повторяется без необходимости.
- Boolean положительный; number имеет unit.
- Public name не содержит implementation detail.
- Event является past fact, command — intent, query — read request.
- Error code содержит domain, operation и reason.
- Persisted/API rename имеет migration и compatibility plan.
- RU/DE documents используют одинаковый machine token.
- Existing MOD-002 identifiers не сломаны косметическим refactor.

## 33. Acceptance criteria

STD-002 считается внедрённым в новый пакет, если automated lint или review подтверждает filename, path, language, C++ casing, JSON field, event/capability token, error code, test name и unit suffix rules. Исключение MUST иметь owner, reason, affected identifiers, expiry и ссылку на decision record.

## Приложение A. Сквозной пример

| Layer | Name |
|---|---|
| Document | `MOD-003_Memory_Engine_RU_v1_0_Full_Content_Implementation_Specification.docx` |
| Namespace | `world_engine::memory_engine` |
| Port | `IMemoryContextSourcePort` |
| Record | `MemoryRecord` |
| Field | `memory_id` |
| Event | `memory.record.created.v1` |
| C++ event type | `MemoryRecordCreatedEvent` |
| Error | `memory.candidate.validate.invalid_provenance` |
| Metric | `world_engine_memory_candidates_rejected_total` |
| Test | `invalid_provenance_is_rejected` |

## Приложение B. Совместимость с MOD-002

Текущие `world_engine::game_adapter`, `GameAdapter`, `IGameIngressPort`, `IRuntimeBridge`, `AdapterState::initializing`, `kContractSchemaVersion`, `max_payload_bytes`, `runtime.identity`, `skyrim-se-ae.profile.json` и `world-engine.mod-002.skyrim` соответствуют STD-002 и не требуют rename.

## Приложение C. Traceability

| Источник | Зафиксированное влияние |
|---|---|
| STD-001 | Canonical meanings and abbreviations |
| PLAN-001 | Document families and Foundation Gates |
| ARCH-004/005 and ADR-008 | Ownership-neutral naming and dependency boundaries |
| ADR-002 | Event fact semantics and idempotency metadata |
| ARCH-009 | Typed configuration and safe defaults |
| ADR-003 and MOD-003 | Repository, schema and migration names |
| ADR-009 and MOD-002 | Skyrim SE/AE scope and existing machine identifiers |

## Приложение D. Журнал изменений

| Версия | Дата | Изменение |
|---|---|---|
| 1.0 | 2026-09-05 | Создан полный naming baseline для documents, paths, versions, modules, C++, contracts, events, capabilities, errors, configuration, schemas, database, observability, tests, builds, platforms и version-control artifacts. |
