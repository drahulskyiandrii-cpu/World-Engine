# MOD-003 Memory Engine — полный нормативный источник

Полная архитектурная и реализационная спецификация до начала разработки

Версия 1.0 | Статус Design Baseline — Implementation Not Started | Дата 2026-09-05

## 1 Как читать документ

Этот документ является самостоятельным техническим заданием на MOD-003 Memory Engine. Он фиксирует назначение, границы владения, модель данных, порты, алгоритмические политики, ошибки, сохранение и восстановление, безопасность, производительность, тесты и поэтапные gates. Для понимания обязательного поведения не требуется собирать смысл из старых файлов; связанные архитектурные документы нужны для трассируемости и разрешения межмодульных решений.

Ключевые слова MUST, MUST NOT, SHOULD, SHOULD NOT и MAY определяют силу требований. MUST и MUST NOT требуют автоматического теста, статической проверки либо подписанного acceptance record. SHOULD допускает отклонение только после записи причины и влияния. MAY описывает опциональное поведение, которое не изменяет обязательные контракты.

Документ различает три состояния: **Designed** — контракт утверждён здесь; **Implemented** — код присутствует в рабочей ветке; **Verified** — поведение подтверждено воспроизводимым тестом. На дату версии 1.0 весь материал имеет состояние Designed. Наличие подробной схемы или псевдокода не означает наличия реализации.

| Метка | Смысл | Допустимое доказательство |
|---|---|---|
| Designed | Нормативное решение принято | Этот документ и связанные ARCH/ADR |
| Implemented | Код и миграции существуют | Исходный код, schema, migration manifest |
| Verified | Поведение проверено | Автоматический тест или подписанный manual record |
| Operational | Модуль пригоден для обычного запуска | Все обязательные gates и эксплуатационные проверки |
| Deferred | Возможность сознательно отложена | Roadmap item с явной границей |

## 2 Решение, назначение и статус

MOD-003 создаёт долговременную, проверяемую и извлекаемую память персонажей и других разрешённых субъектов мира. Модуль принимает типизированные кандидаты воспоминаний, проверяет их согласно доменным политикам, сохраняет через узкий repository port и возвращает нормализованные записи для контекста, диалога и других use cases.

Memory Engine является владельцем **значения памяти**, но не владельцем физической базы данных, AI-генерации, диалоговой логики или глобального save workflow. Он отвечает за допустимые типы записей, идентичность субъекта, provenance, authority, дедупликацию, исправления, retention, ranking и семантику выдачи.

Статус документа — полный design baseline. Реализация не начата; поэтому ни один API, migration, индекс, latency target или integration path не обозначается как подтверждённый. Начало кодирования разрешается только после Gate M3.0, а объявление модуля Operational — после M3.7.

## 3 Положение в архитектуре

MOD-003 расположен в Simulation Layer и не зависит от Skyrim, SKSE, UI, transport или конкретного AI provider. Все game-specific идентификаторы приходят после нормализации владельцем интеграции. Потребители обращаются к Memory Engine через узкие application/domain ports; они не получают SQL, таблицы, connection handles или raw persistence models.

Основные отношения:

| Сторона | Отношение | Правило |
|---|---|---|
| Core/Application | Координация lifecycle и use case | Не владеет memory semantics |
| Repository implementation | Реализует persistence port | Владеет SQL, mapping, connections и migrations |
| Context Manager | Читает через MemoryContextSourcePort | Не обращается к Repository/SQLite |
| Dialogue Engine | Запрашивает approved context | Не изменяет память напрямую |
| AI Adapter | Возвращает candidate output | Не сохраняет память и не назначает authority |
| Save Manager | Координирует SaveParticipant | Не знает внутренних memory invariants |
| Event Engine | Доставляет semantic events | Не является RPC и не владеет memory state |

Статическая зависимость направлена к абстракции, а runtime-вызов не меняет ownership. Composition Root создаёт concrete repository и связывает его с потребительским портом Memory Engine.

## 4 Ответственность Memory Engine

Модуль MUST:

- принимать только versioned команды и кандидаты с устойчивыми идентификаторами;
- валидировать структуру, субъект, область, время, provenance и допустимость содержимого;
- назначать тип, authority class, retention class и вычисляемую salience по детерминированным правилам;
- подавлять точные и близкие дубликаты до дорогой записи;
- сохранять изменения через `IMemoryRepositoryPort` атомарно в границе одного use case;
- поддерживать исправление, опровержение и supersession без незаметной перезаписи истории;
- возвращать bounded, нормализованные результаты с признаками происхождения и авторитетности;
- участвовать в staged save/restore через `SaveParticipant`;
- публиковать semantic events только после успешного commit;
- предоставлять health, readiness и безопасную телеметрию без раскрытия содержимого.

Любая политика, влияющая на то, что NPC «помнит» или «считает известным», принадлежит этому модулю либо явно указанному владельцу world fact, но не Repository и не AI Adapter.

## 5 Явные исключения

MOD-003 MUST NOT:

- генерировать реплики, мотивы или поведение NPC;
- решать, какое игровое действие выполнить;
- обращаться к Skyrim runtime, SKSE или файлам модов;
- предоставлять общий CRUD или raw SQL другим модулям;
- хранить секреты, ключи API, произвольные промпты или полный provider transcript;
- считать Event History источником истины без отдельного contract;
- восстанавливать весь application state самостоятельно;
- выполнять distributed synchronization или cloud replication в версии 1.0;
- использовать vector database как обязательную зависимость версии 1.0;
- превращать предположение AI в канонический факт без детерминированной проверки.

Диалог, отношения, квесты и политика мира могут использовать память, но сохраняют владение собственными решениями и инвариантами.

## 6 Владение контрактами и зависимостями

Порт определяется у потребителя, которому он нужен. Memory Engine владеет `IMemoryRepositoryPort`, потому что формулирует необходимые операции над owned data. Context Manager владеет своим `IMemoryContextSourcePort`; Memory Engine реализует этот порт, не раскрывая repository contract. Save Manager владеет `SaveParticipant` abstraction; Memory Engine предоставляет adapter к нему.

| Контракт | Владелец | Реализатор | Запрещённая утечка |
|---|---|---|---|
| IMemoryCommandPort | Application/Memory use case | Memory Engine | SQL и AI provider types |
| IMemoryQueryPort | Потребляющий use case | Memory Engine | Physical rows и mutable entities |
| IMemoryRepositoryPort | Memory Engine | Persistence adapter | SQLite handles и table names |
| IMemoryContextSourcePort | Context Manager | Memory Engine | Repository access |
| SaveParticipant | Save Manager | Memory adapter | Global save orchestration |
| IEventPublisherPort | Producer/use-case boundary | Event Engine adapter | Synchronous request semantics |

Запрещены runtime Service Locator, общая `GetRepository<T>()`, прямой импорт persistence package в domain code и циклическая статическая зависимость Memory ↔ Dialogue.

## 7 Термины и инварианты

**MemoryRecord** — подтверждённая, versioned и сохраняемая запись. **MemoryCandidate** — непроверенное предложение на запись. **Subject** — владелец памяти, обычно NPC. **Object** — сущность, о которой запись. **Provenance** — проверяемое происхождение. **Authority** — класс доверия источнику, не вероятность. **Confidence** — ограниченная оценка неопределённого утверждения. **Salience** — вычисляемая значимость для ranking. **Retention** — политика срока и свёртки. **Supersession** — связь новой записи с заменяемой без уничтожения аудита.

Глобальные инварианты:

1. У каждой durable записи есть `memory_id`, `subject_id`, `type`, `occurred_at`, `recorded_at`, `provenance` и `schema_version`.
2. `recorded_at` не заменяет время события.
3. Canonical world fact имеет приоритет над AI inference при конфликте.
4. Отсутствие записи не означает отрицательный факт.
5. Удаление или expiry не должны оставлять висячие ссылки.
6. Read result является immutable snapshot.
7. Повтор команды с тем же idempotency key не создаёт вторую память.

## 8 Lifecycle и readiness

Lifecycle: `Constructed → Configured → RepositoryReady → RestoredOrEmpty → Ready → Degraded → Stopping → Stopped`. Переходы односторонние, кроме контролируемого `Degraded → Ready` после подтверждённого восстановления зависимости.

На startup Host передаёт typed configuration и ports. Memory Engine проверяет schema version конфигурации, лимиты, разрешённые типы и repository readiness. Migrations выполняет persistence subsystem до `RepositoryReady`. Затем Save Manager либо активирует согласованный snapshot, либо явно подтверждает новый мир. Только после этого модуль публикует Ready.

| Состояние | Чтение | Запись | Условие выхода |
|---|---|---|---|
| Constructed | Нет | Нет | Зависимости внедрены |
| Configured | Нет | Нет | Конфигурация валидна |
| RepositoryReady | Ограничено | Нет | Restore/new-world decision |
| Ready | Да | Да | Нормальная работа |
| Degraded | По политике | Обычно reject/queue bounded | Dependency recovery |
| Stopping | Только drain | Новые команды reject | Bounded shutdown |

Shutdown закрывает приём команд, завершает разрешённые транзакции, сбрасывает bounded metrics и освобождает repository scope. Бесконечное ожидание запрещено.

## 9 Таксономия памяти

Версия 1.0 использует закрытый registry типов. Расширение требует versioned schema change и тестов совместимости.

| Тип | Назначение | Типичный источник | Базовая retention |
|---|---|---|---|
| Observation | Наблюдённое субъектом событие | Game/world use case | Normal |
| Interaction | Значимое взаимодействие с actor | Dialogue/trade/combat use case | Normal |
| Knowledge | Усвоенное утверждение | Approved knowledge path | Policy-based |
| ReputationEvidence | Основание отношения/репутации | Relationship use case | Long |
| QuestOutcome | Итог квестового события | Quest use case | Long |
| Rumor | Неподтверждённая переданная информация | Social/Dialogue use case | Short/Normal |
| Commitment | Обещание, долг, договорённость | Owning use case | Until resolved |
| WorldChange | Изменение значимого состояния мира | World owner | Long |
| Correction | Исправление прежнего утверждения | Owner/validation workflow | Follows target |
| Summary | Производная свёртка группы записей | Memory maintenance | Replaces details by policy |

Тип задаёт обязательные поля, допустимые provenance kinds, authority floor, retention и retrieval eligibility. Произвольная строка type запрещена.

## 10 Идентичность, область и изоляция

Идентификаторы являются game-agnostic стабильными значениями: `world_id`, `save_lineage_id`, `subject_id`, `entity_id`, `location_id`, `quest_id`. Runtime handles и указатели никогда не сохраняются. Game Adapter либо owning use case преобразует их в каноническую форму до команды Memory Engine.

Каждая запись принадлежит ровно одному `world_id` и одной lineage. Запрос без явной области отклоняется. Между разными мирами, профилями или линиями сохранений нет неявного поиска и дедупликации. Shared lore допускается только как отдельная read-only authority source, а не копированием записей между мирами.

Subject identity может быть aliased, но canonicalization выполняется до persistence. Merge двух субъектов требует отдельной migration/administrative operation с dry-run, collision report и recovery plan. Автоматическое объединение по имени запрещено.

## 11 Модель MemoryRecord

`MemoryRecord` — immutable value object; изменение создаёт новую revision или отдельную correction/supersession запись. Минимальный набор полей приведён в приложении A.

| Группа | Поля | Правило |
|---|---|---|
| Identity | memory_id, subject_id, world_id, lineage_id | Неизменяемы после commit |
| Semantics | type, predicate, object_refs, content | Content bounded и нормализован |
| Time | occurred_at, recorded_at, valid_from/to | UTC + domain chronology marker |
| Trust | provenance, authority, confidence | Независимые понятия |
| Policy | retention_class, salience_inputs, visibility | Назначаются детерминированно |
| Evolution | schema_version, revision, supersedes | Проверяемая цепочка |
| Integrity | content_hash, idempotency_key | Для duplicate/retry control |

Domain content хранится структурировано. Display text MAY храниться как локализованный projection, но не является единственным источником смысла. Полный prompt/response AI не входит в запись.

## 12 Модель MemoryCandidate

Candidate отделяет недоверенный input от durable state. Он содержит proposal, source descriptor, scope, timestamps и correlation metadata, но не имеет окончательного `memory_id`, authority и retention до validation.

Состояния кандидата: `Received → StructurallyValid → SemanticallyValid → Deduplicated → Accepted/Rejected → Committed`. Любой terminal result неизменяем и имеет reason code. Candidate не должен оставаться в бесконечной очереди; TTL и максимальное число pending задаются конфигурацией.

| Источник | Требования |
|---|---|
| Deterministic game event | Versioned event ID, actor scope, observed fact |
| Dialogue outcome | Owning use-case approval и typed meaning |
| AI inference | Provider correlation, model/policy metadata, untrusted flag |
| Administrative import | Signed/authorized operation, dry-run и report |
| Derived summary | Полный список source memory IDs и algorithm version |

Принятие кандидата возвращает `Accepted(memory_id, revision)` или `Duplicate(existing_id)`; отказ — `Rejected(code, safe_details)`.

## 13 Provenance

Provenance MUST позволять ответить: кто предложил факт, через какой use case, на основании какого события или записи, когда и по какой версии политики он был принят. Она не обязана содержать чувствительный исходный payload.

Минимальные поля: `source_kind`, `source_id`, `producer_module`, `use_case`, `correlation_id`, `causation_id`, `policy_version`, `observed_by`, `accepted_at`. Для AI добавляются provider class, model identifier/version when available и prompt template version, но не secret и не полный prompt.

Цепочка производной записи содержит bounded список source IDs либо ссылку на immutable provenance bundle. Цикл provenance запрещён. Если источник удалён по retention, остаётся tombstone/hash и причина удаления, достаточные для аудита без восстановления запрещённого content.

## 14 Authority и confidence

Authority — дискретный класс источника:

| Класс | Пример | Конфликтное правило |
|---|---|---|
| Canonical | Подтверждённый world-state owner | Побеждает inference и rumor |
| Witnessed | Событие, наблюдённое субъектом | Может быть субъективным, но не AI guess |
| Reported | Информация от другого actor | Сохраняет цепочку передачи |
| Inferred | Вывод разрешённого алгоритма/AI | Никогда не повышается автоматически |
| Rumor | Явно неподтверждённое сообщение | Не используется как canonical fact |
| Administrative | Контролируемая правка | Требует audit metadata |

Confidence допустим только для типов, где неопределённость является частью модели, и ограничен диапазоном `[0,1]`. Он не заменяет authority. Высокая confidence AI не превращает inferred record в canonical. Уменьшение confidence MAY происходить по versioned decay policy, но factual canonical record не «забывается» таким способом.

## 15 Временная модель

Memory Engine различает `occurred_at`, `observed_at`, `recorded_at` и optional `valid_from/valid_to`. Все machine timestamps хранятся в UTC; внутриигровая хронология представлена отдельным typed marker, чтобы ускорение игрового времени не искажало wall-clock diagnostics.

Out-of-order событие допустимо, если его identity и chronology валидны. Оно не переписывает более новую запись автоматически. Ranking использует возраст относительно выбранной timeline, а не смешивает игровое и системное время.

Срок retention начинается по явно указанной базе: `recorded_at` по умолчанию, `resolved_at` для commitments, либо policy-specific. Clock injection обязателен для тестов. Системный clock jump не должен массово удалить записи без dry-run maintenance decision.

## 16 Ingestion pipeline

Полный write path:

`Producer → Application-owned command → MemoryCandidate → structural validation → scope/identity validation → policy classification → semantic validation → duplicate/conflict analysis → repository transaction → post-commit event`.

До commit запрещено публиковать `MemoryUpdated`. После commit event содержит ID, subject, type, revision и policy-safe metadata, но не полный content по умолчанию. Если event publication не удалась, durable запись остаётся корректной; event delivery/recovery обрабатывается согласно Event System contract.

Каждый этап возвращает typed result. Любая неоднозначность repository commit разрешается проверкой idempotency key, а не слепым повтором. Producer не получает внутренние SQL ошибки.

## 17 Structural и semantic validation

Structural validation проверяет schema version, обязательные поля, длины, enum, формат IDs, timestamp bounds и отсутствие неизвестных полей в strict mode. Semantic validation проверяет правила типа: допустимый субъект, совместимость object refs, chronology, provenance authority, content classification и permission context.

Validation MUST быть детерминированной для одинакового candidate, policy version и reference snapshot. Внешний AI-вызов внутри validation запрещён. Reference facts читаются через утверждённые порты до начала транзакции или в явно документированном consistency scope.

Отказ не изменяет durable state. Error detail содержит field path и безопасный reason, но не копирует весь недоверенный payload в log.

## 18 Дедупликация и near-duplicate policy

Exact duplicate определяется по нормализованному canonical key и `content_hash`. Idempotency duplicate определяется по `(producer, idempotency_key, world_id)`. Near-duplicate определяется versioned алгоритмом по type-specific fields; его результат не может молча объединять записи разных authority или субъектов.

| Результат | Действие |
|---|---|
| Exact duplicate | Вернуть существующий ID, не публиковать второй create event |
| Retry duplicate | Вернуть исходный terminal result |
| Compatible near-duplicate | Увеличить evidence link/observed count по policy |
| Conflicting near-duplicate | Создать отдельную запись и conflict link |
| Ambiguous | Reject или manual review; не угадывать |

Алгоритм, threshold и normalization version записываются в diagnostic evidence. Vector similarity MAY быть будущим ускорителем, но не обязательным источником истины v1.0.

## 19 Конфликт, исправление и supersession

Конфликтующие записи сохраняются как отдельные утверждения с authority и provenance. Query projection может выбрать наиболее авторитетное актуальное утверждение, но audit view сохраняет все варианты.

Исправление создаёт `Correction` с `supersedes` либо `contradicts`. Исходная запись становится `superseded` для обычной выдачи, но не удаляется немедленно. Administrative correction требует actor/operation ID и reason. Canonical owner может опровергнуть rumor; rumor не может опровергнуть canonical fact.

Цепочка revisions должна быть ацикличной и bounded для чтения. Maintenance MAY свернуть длинную цепочку, сохранив audit summary и хэши. Hard delete разрешён только privacy/security policy или подтверждённой corruption recovery процедурой.

## 20 Retention, expiry и forgetting

Retention class определяет длительность, условия expiry, возможность summary и требования к audit trail. Значения версии 1.0: `Ephemeral`, `Short`, `Normal`, `Long`, `Permanent`, `UntilResolved`, `LegalOrSecurityHold`.

| Класс | Поведение |
|---|---|
| Ephemeral | Не становится durable либо удаляется после короткого bounded окна |
| Short | Подходит rumor/незначительным наблюдениям |
| Normal | Стандартная история субъекта |
| Long | Quest outcomes, отношения и важные world changes |
| Permanent | Только явно разрешённые canonical milestones |
| UntilResolved | Commitment живёт до typed resolution |
| LegalOrSecurityHold | Техническая блокировка удаления по формальной политике |

Maintenance сначала строит dry-run plan, затем применяет bounded batch. Удаление не выполняется в critical gameplay path. Expired запись исключается из обычного retrieval до физической очистки. Forgetting является политикой данных, а не случайным следствием ошибок или лимита контекста.

## 21 Salience и приоритет

Salience вычисляется из type weight, authority, recency, relationship relevance, unresolved state, repetition evidence и explicit importance. Конфигурация задаёт ограниченные коэффициенты; произвольный AI score не является единственным основанием.

Результат нормализуется и хранит `algorithm_version` и входные признаки, необходимые для повторного вычисления. Projection MAY пересчитывать динамическую часть без переписывания записи. Защита от feedback loop ограничивает рост значимости из-за многократного извлечения одной памяти.

Critical commitments и active quest facts получают policy floor, но всё равно фильтруются по world/subject/visibility. Высокая salience не обходит безопасность или authority conflict.

## 22 Retrieval contract

Запрос содержит scope, subject, purpose, allowed types, time window, related entity IDs, authority floor, limit, byte/token budget и stable cursor. Purpose обязателен и выбирается из registry: `DialogueContext`, `BehaviorContext`, `RelationshipEvidence`, `QuestReasoning`, `Audit`, `Maintenance`.

Ответ — immutable `MemoryQueryResult` с нормализованными summaries, provenance markers, score explanation, truncation flag, snapshot/version marker и next cursor. Raw table rows и lazy database objects запрещены.

Запросы MUST быть bounded. `limit=unbounded`, отрицательные окна и неизвестный purpose отклоняются. Пустой результат означает «нет доступных подходящих записей», но не «событие не происходило».

## 23 Фильтрация и ranking

Порядок обработки: tenant/world isolation → subject visibility → type/purpose eligibility → validity/expiry → authority constraints → entity/time filters → candidate fetch → deterministic scoring → diversity policy → budget packing → stable tie-break.

Ranking обязан быть воспроизводимым при одинаковом snapshot. Stable tie-break: score descending, occurred_at descending, memory_id ascending. Diversity policy предотвращает заполнение всего ответа повторениями одного события или типа. Conflict groups возвращают canonical projection плюс marker о наличии альтернатив, если purpose разрешает.

Score explanation не раскрывает security-sensitive configuration, но указывает contribution classes. Для audit purpose доступен расширенный trace при соответствующей авторизации.

## 24 Context budget и упаковка

Memory Engine не формирует финальный AI prompt. Он возвращает Context Manager bounded записи в нормализованном формате. Budget выражается одновременно максимальным числом записей, serialized bytes и optional token estimate. Превышение любого лимита прекращает упаковку и выставляет `truncated=true`.

Приоритет упаковки: обязательные active commitments → canonical/relevant facts → высокозначимые interactions → diversity additions. Summary используется только если его provenance и freshness валидны. Raw content сокращается по type-specific summarizer, а не произвольным обрезанием UTF-8.

Недоступность optional memory context приводит к incomplete context marker. Context Manager или owning use case решает, можно ли продолжить; запрещено подставлять выдуманные факты.

## 25 Интеграция с Dialogue Engine

Dialogue Engine (MOD-005) читает approved memory projection через application-owned query contract. Он передаёт purpose, participants, topic/entity refs и budget. Memory Engine не получает prompt и не выбирает реплику.

После диалога owning use case MAY предложить типизированный candidate: обещание, сообщённый факт, значимое оскорбление, сделка. Полный transcript не сохраняется автоматически. Dialogue Engine не вызывает `IMemoryRepositoryPort` и не создаёт durable record напрямую.

При ошибке memory read Dialogue получает `ContextIncomplete` с reason category. Ответ может продолжиться только по задокументированной reduced-context policy и обязан избегать утверждений, требующих отсутствующей памяти.

## 26 Интеграция с AI

AI Adapter возвращает structured candidate output. Application use case проверяет provider result schema и разрешённость операции, затем создаёт `MemoryCandidate` с `authority=Inferred` или `Reported` по фактическому происхождению. Memory Engine выполняет собственную validation и может reject независимо от валидности provider response.

AI MUST NOT:

- назначать canonical authority;
- передавать SQL или storage IDs;
- писать в Repository;
- изменять retention/security class;
- скрывать отсутствие provenance;
- создавать память только потому, что текст звучит правдоподобно.

Provider timeout или malformed output не создаёт candidate. Accepted AI-derived record всегда имеет model/policy provenance и может быть отфильтрован отдельно.

## 27 Character и Relationship integration

Character Engine может использовать память для характерных реакций, но владеет traits и behavior policy. Relationship Engine владеет состоянием отношений; Memory Engine хранит evidence, объясняющее изменение, но не вычисляет итоговую relationship state, если это не определено отдельным port contract.

Изменение отношений сначала commit у owning module, затем semantic event или explicit candidate создаёт evidence memory. Обратный порядок запрещён, если он создаёт ложную память при failed relationship transaction.

Query для RelationshipEvidence возвращает relevant records и confidence/authority markers, не итоговое число отношения. Это исключает два конкурирующих источника истины.

## 28 Quest, Organization и World integration

Quest Engine публикует подтверждённые quest milestones после commit. Memory Engine создаёт `QuestOutcome` или `Knowledge` только для субъектов, которые могли узнать событие согласно witness/distribution policy. Organization и World owners действуют аналогично.

Global world event не копируется автоматически всем NPC. Распространение знания — отдельный use case с bounded fan-out, eligibility rules и provenance chain. Массовая запись выполняется batch-операцией с checkpoint и idempotency, но каждая память остаётся subject-scoped.

Если upstream event исправлен, владелец публикует correction event; Memory Engine не угадывает новый state по отсутствию события.

## 29 Event integration

Memory Engine потребляет только versioned semantic events из catalog. Event не используется как синхронная команда, когда нужен немедленный результат. Producer commit происходит до publish.

Публикуемые события версии 1.0: `MemoryAccepted`, `MemoryCorrected`, `MemoryExpired`, `MemorySummaryCreated`, `MemoryMaintenanceFailed`, `MemoryReadinessChanged`. Payload содержит stable IDs, version, correlation/causation и минимальные metadata.

Event delivery может быть at-least-once; handlers обязаны использовать event ID как idempotency key. Replay разрешён только для заявленных replay-safe handlers. Event History не заменяет Memory Repository и не используется для полного восстановления без отдельного решения.

## 30 Repository boundary

`IMemoryRepositoryPort` выражает use-case операции, а не таблицы: `CommitCandidate`, `GetById`, `QuerySnapshot`, `ApplyCorrection`, `PlanRetention`, `ApplyRetentionBatch`, `ExportParticipantSnapshot`, `StageParticipantRestore`.

Repository implementation владеет SQL, mapping, connection pool/scope, transactions, busy retry, physical indexes, migrations и backup mechanics. Memory Engine передаёт domain values и transaction intent; SQLite types не пересекают boundary.

Generic `Save(entity)`, `Find(table, predicate)` и получение connection запрещены. Ошибки нормализуются в typed persistence result. Connection failure никогда не превращается в пустой query result.

## 31 SQLite mapping constraints

Версия 1.x использует локальный SQLite через persistence subsystem в соответствии с ADR-003. Одна domain entity не обязана совпадать с одной таблицей. Физическая схема может нормализовать object refs, provenance и conflict links, если domain semantics сохраняются.

Обязательны foreign keys/constraints там, где они защищают структурную целостность, unique index для idempotency scope, индекс subject/world/time, индекс retrieval eligibility и migration metadata с checksum. Конкретные имена таблиц не являются частью MOD-003 public contract.

WAL, busy timeout, connection count и vacuum strategy определяются persistence configuration после измерений. Документ не объявляет конкретные значения подтверждёнными до benchmark gate.

## 32 Транзакции и idempotency

Одна команда принятия кандидата выполняется в одной логической транзакции: duplicate check, record insert, provenance/link insert и outbox/evidence step, если выбран transaction-aware event pattern. Частичный commit запрещён.

Idempotency key уникален в scope producer/world/operation kind. Повтор после timeout сначала читает terminal result. Key reuse с иным content hash возвращает `IdempotencyConflict` и security-relevant diagnostic.

Длинные multi-subject операции разбиваются на bounded batches с operation ID и checkpoint. Это не отменяет атомарность отдельной записи. Transaction не должна охватывать AI call, network, gameplay callback или ожидание внешнего subscriber.

## 33 Concurrency и backpressure

Reads используют immutable snapshot/version marker. Writes к одному subject сериализуются логически или защищаются optimistic revision check. Параллельные writers разных subjects MAY выполняться конкурентно в пределах persistence policy.

| Очередь | Политика переполнения |
|---|---|
| Interactive writes | Reject Busy до enqueue либо короткий bounded wait вне game critical path |
| Background imports | Pause/checkpoint; не вытеснять interactive work |
| Maintenance | Низкий приоритет, cancellable batches |
| Event-derived candidates | Bounded queue, duplicate suppression до persistence |
| Diagnostics | Drop/coalesce с counter; content не логируется |

Внешний callback не вызывается под internal lock. Shutdown закрывает producers, отменяет maintenance, drains bounded work и завершает remaining commands typed result.

## 34 Cache и indexing

Кэш — оптимизация, не источник истины. Допустимы immutable recent-memory cache per subject, query-plan cache и reference lookup cache. Ключ всегда включает world, lineage, subject, purpose-relevant version и policy version.

Commit инвалидирует или обновляет cache только после durable success. Restore полностью меняет generation и делает прежние entries недействительными. Negative cache имеет короткий TTL и не превращает transient repository failure в «нет данных».

Index design подтверждается representative corpus. Индекс, ускоряющий один запрос, не принимается без измерения write cost, file growth и migration impact. FTS или vector index являются optional adapters за отдельным port/decision.

## 35 SaveParticipant contract

Memory Engine реализует Save Manager-owned `SaveParticipant`. Snapshot содержит logical module state, необходимый для согласованного restore: schema version, world/lineage, high-water marks, active policy version, unresolved commitments/maintenance markers и ссылки/данные согласно общей save policy.

`PrepareSnapshot` возвращает immutable candidate и checksum. Он не пишет глобальный save artifact. Save Manager собирает candidates всех участников, выполняет cross-participant validation и только затем commit artifact.

Обычная durable память в SQLite и save artifact имеют разные цели. Database backup не заменяет Save System; save artifact не является произвольной копией database file.

## 36 Staged restore и атомарная активация

Restore flow: `Read artifact → validate envelope → deserialize NON-LIVE candidate → Memory stage → cross-participant validation → atomic logical activation → post-activation evidence`.

До activation live state не меняется. Memory stage проверяет checksum, schema compatibility, world/lineage, ссылочную целостность, revision monotonicity, supported policy versions и budget. Ошибка любого participant отменяет весь restore; частичный success запрещён.

Activation меняет generation. In-flight queries старой generation завершаются старым snapshot либо cancel result, но не смешивают состояния. Repository side effects restore выполняются через отдельный transaction/recovery plan, согласованный Save Manager и persistence subsystem.

## 37 Schema versioning и migrations

Версионируются отдельно domain record schema, repository schema, configuration schema, event schema и save-participant schema. Одинаковый номер между ними не предполагается.

Каждая migration имеет source/target version, checksum, preconditions, dry-run/report, atomicity class, backup/recovery plan и integration tests на реальной temporary database. Future unsupported schema отклоняется без destructive downgrade. Migration failure сохраняет recoverable предыдущее состояние.

Domain semantic migration, например разделение типа `Interaction`, принадлежит Memory Engine; физическое преобразование выполняет repository implementation по утверждённому plan. После migration выполняются integrity scan и representative retrieval comparisons.

## 38 Configuration contract

Memory Engine получает только typed section `memory`, не полный YAML tree. Неизвестные поля отклоняются в strict mode. Безопасные defaults применяются только к optional параметрам; отсутствующий обязательный security или capacity limit вызывает startup failure.

Группы настроек: candidate limits, content limits, retention policies, salience weights, retrieval budgets, maintenance schedule, cache limits, repository timeouts reference, observability sampling и feature flags. Secrets запрещены.

Hot reload разрешён только для будущих запросов и после atomic candidate validation. Schema version, repository path, identity rules, encryption/security boundary и save compatibility требуют restart/migration. Если один validator отклоняет update, текущая effective configuration остаётся целиком.

## 39 Security и privacy

Все producer payloads считаются недоверенными. Валидация ограничивает размер, кодировку, nesting, identifiers и content classification. Output encoding выполняется потребителем; Memory Engine не хранит executable markup как доверенный content.

Запрещены secrets, access tokens, filesystem paths, memory addresses и необработанные provider diagnostics. Sensitive story content MAY храниться только при явной classification/visibility policy. Logs содержат IDs, hashes, sizes, result codes и durations, но не обычный memory text.

Administrative export/import требует authorization context, path policy, bounded archive, checksum и audit record. Импорт не исполняет скрипты и не доверяет embedded schema declaration.

## 40 Минимизация данных и удаление

Сохраняется минимальная семантика, необходимая для gameplay use case. Полный диалог, аудио, prompt, provider response и дублирующий world state не записываются по умолчанию. Structured predicate предпочтительнее длинного текста.

Удаление по privacy request выполняется по stable subject/entity scope с dry-run count, authorization, referential plan и post-delete report. Derived summaries и provenance links также обрабатываются; нельзя удалить source и оставить раскрывающую его копию.

Tombstone содержит только минимальные поля для idempotency/integrity, если политика это разрешает. Если retention/audit конфликтует с удалением, решение требует formal governance, а не скрытого игнорирования.

## 41 Наблюдаемость и health

Metrics: candidate accepted/rejected/duplicate, write/read latency, result size, truncation, queue depth, contention, cache hit, retention counts, restore duration, integrity failures. Labels bounded; memory_id, subject_id и content не используются как high-cardinality labels.

Structured logs включают operation, result code, correlation, world hash/alias, duration и counts. Tracing следует causation chain без payload. Health snapshot показывает lifecycle, repository availability, last successful write/read, queue saturation, migration/schema evidence и degraded reason.

Readiness не равна process liveness. Repository недоступен или restore не завершён — модуль не Ready даже при живом потоке.

## 42 Performance budgets

До benchmark gate значения являются целями проекта, не подтверждёнными обещаниями:

| Операция | Проектная цель p95 | Ограничение |
|---|---:|---|
| Get recent bounded context | ≤ 20 ms | Warm local DB, ≤ 32 records |
| Structured retrieval | ≤ 50 ms | Representative indexed corpus |
| Accept deterministic candidate | ≤ 25 ms | Без внешних вызовов |
| Duplicate rejection | ≤ 10 ms | Exact/idempotency path |
| Prepare participant snapshot | ≤ 100 ms | Bounded metadata/state |

Game main thread никогда не ждёт этих операций напрямую. Benchmark corpus должен включать 1k, 10k и 100k записей на мир, skewed subjects, conflicts и expired data. Отчёт фиксирует hardware, database size, cold/warm runs и percentiles.

## 43 Failure model

Категории: Validation, Authorization, Conflict, Capacity, PersistenceUnavailable, PersistenceBusy, SchemaMismatch, CorruptionSuspected, SaveRestore, Cancelled, InternalInvariant. Каждая ошибка имеет стабильный code, retryability и безопасные details.

Пустой результат не используется вместо failure. Timeout с неизвестным commit status возвращает `CommitOutcomeUnknown`; caller проверяет idempotency result. Corruption suspicion переводит writes в safe degraded/recovery-required, не запускает автоматический destructive repair.

Неожиданное исключение ловится на application boundary, связывается с correlation ID и не раскрывает content. Fatal invariant violation делает модуль NotReady.

## 44 Retry, recovery и integrity

Retry разрешён только для явно transient repository conditions, bounded attempts и jitter policy persistence layer. Validation, authorization, schema mismatch и idempotency conflict не повторяются автоматически.

Startup integrity checks: schema/checksum, required indexes/constraints evidence, last migration status, save generation compatibility и optional quick consistency scan. Глубокий scan выполняется offline/maintenance режимом.

Recovery options: retry open, restore approved backup, apply documented migration recovery, rebuild derived index, replay explicitly durable candidate/outbox. Нельзя восстанавливать authoritative memory из AI или обычных logs.

## 45 Degraded modes

| Причина | Чтение | Запись | Требуемый marker |
|---|---|---|---|
| Repository temporarily busy | Existing cache only по policy | Reject/short queue | Stale/Incomplete |
| Repository unavailable | Cache snapshot if safe | Reject | ContextIncomplete |
| Maintenance backlog | Normal | Normal with rate limit | MaintenanceDelayed |
| Optional index failed | Fallback structured query if bounded | Normal | RetrievalDegraded |
| Corruption suspected | Только verified snapshot/audit | Stop | RecoveryRequired |
| Restore pending | Нет live mixed reads | Нет | NotReady |

Reduced-context policy принадлежит вызывающему use case. Memory Engine сообщает факт неполноты и не симулирует данные. Возврат в Ready требует health check и evidence, а не только истечения таймера.

## 46 Public ports

Минимальная версия портов:

```text
IMemoryCommandPort
  SubmitCandidate(command) -> SubmitResult
  CorrectMemory(command) -> CorrectionResult
  ResolveCommitment(command) -> ResolutionResult

IMemoryQueryPort
  Query(query) -> MemoryQueryResult
  GetById(scope, memory_id, view) -> MemoryLookupResult

IMemoryContextSourcePort
  BuildContextSlice(request) -> ContextSliceResult

IMemoryMaintenancePort
  PlanRetention(request) -> RetentionPlan
  ApplyRetention(plan_id, batch_limit) -> MaintenanceResult
```

Все методы принимают cancellation/deadline context и возвращают typed result. Exceptions не являются обычным transport ошибок через module boundary. Версии contract types эволюционируют backward-compatible либо через новый major port.

## 47 Commands и results

Command envelope: `contract_version`, `operation_id`, `idempotency_key`, `scope`, `caller`, `deadline`, `correlation_id`, `payload`. Caller identity не является authorization сам по себе; Host/application передаёт проверенный capability context.

Successful result включает operation ID, memory/revision IDs, commit marker и warnings. Rejected result включает code, field paths и retryable flag. Никакой result не возвращает raw exception/SQL.

Deadline проверяется до expensive step и перед transaction begin. После commit deadline не превращает success в failure; результат может быть recovered по operation/idempotency key.

## 48 Event catalog

| Event | Когда публикуется | Минимальный payload |
|---|---|---|
| MemoryAccepted.v1 | После commit новой записи | memory_id, subject_id, type, revision |
| MemoryCorrected.v1 | После commit correction | old_id, correction_id, relation |
| MemoryExpired.v1 | После retention commit | memory_id или bounded batch reference |
| MemorySummaryCreated.v1 | После commit summary | summary_id, source_count, policy_version |
| MemoryReadinessChanged.v1 | После state transition | previous, current, reason |
| MemoryMaintenanceFailed.v1 | После terminal batch failure | operation_id, safe code, retryable |

Event schema owner указан в catalog. PII/sensitive content отсутствует по умолчанию. Breaking change создаёт новую major event version; producer не меняет смысл существующего поля.

## 49 Error code catalog

| Code | Retry | Meaning |
|---|---|---|
| MEM_VALIDATION_FAILED | No | Структура или семантика недопустима |
| MEM_SCOPE_MISMATCH | No | World/lineage/subject scope не совпал |
| MEM_UNAUTHORIZED | No | Capability context недостаточен |
| MEM_DUPLICATE | No | Повтор успешно сопоставлен существующей записи |
| MEM_IDEMPOTENCY_CONFLICT | No | Key повторён с другим payload |
| MEM_CAPACITY_EXCEEDED | Maybe | Bounded очередь/лимит исчерпан |
| MEM_REPOSITORY_BUSY | Yes, bounded | Временная contention |
| MEM_REPOSITORY_UNAVAILABLE | Yes, policy | Persistence недоступен |
| MEM_COMMIT_OUTCOME_UNKNOWN | Lookup | Требуется idempotency lookup |
| MEM_SCHEMA_UNSUPPORTED | No | Версия не поддерживается |
| MEM_INTEGRITY_FAILURE | No | Нарушен invariant |
| MEM_RESTORE_REJECTED | No | Staged restore не прошёл validation |
| MEM_CANCELLED | Caller | Операция отменена до commit |

## 50 Unit-test strategy

Unit tests покрывают pure normalization, policy classification, validation, authority ordering, time rules, salience, ranking, diversity, budgets, conflict graph и state machine. Clock, ID generator и policy registry внедряются.

Property tests проверяют: normalization idempotence; ranking determinism; отсутствие cross-world leakage; acyclic supersession; budget never exceeded; stable retry result. Fuzz tests на candidate/query parser ограничивают memory/time.

Каждое MUST domain rule имеет test ID и ссылку в traceability. Snapshot tests допустимы только для стабильных contract fixtures и не заменяют assert семантики.

## 51 Contract и integration tests

Contract suite запускается против in-memory fake и реального repository adapter. Она проверяет одинаковые typed results, pagination, transaction rollback, idempotency, optimistic conflict и failure mapping.

Integration scenarios соединяют Application, Memory Engine, Event adapter, Repository и fake Save Manager. Dialogue/AI используют contract doubles, а не internal access. Обязательны tests, доказывающие отсутствие прямых SQLite imports в Memory/consumer packages.

Compatibility tests читают fixtures предыдущих supported schema versions. Неподдерживаемая future version отвергается без изменения файла.

## 52 Persistence и migration tests

Каждый test создаёт временную database, применяет реальные migrations и включает foreign keys/production-like pragmas. Проверяются clean install, sequential upgrade, interrupted migration recovery, checksum mismatch, busy/locked mapping, rollback и data preservation.

Dataset tests включают Unicode, long-but-valid content, conflict chains, high fan-out entities, expired records и orphan-attempt cases. SQL plan/indices проверяются для critical queries, но тест не привязывает domain contract к конкретному plan.

Backup/restore database mechanics тестируются отдельно от Save System semantics.

## 53 Retrieval quality tests

Golden corpus описывает subjects, facts, rumors, conflicts, recency, authority и expected top-k sets. Проверяется не точное floating значение score, а обязательные ordering/eligibility свойства и допустимые ranges.

Метрики: precision@k для релевантных records, canonical-conflict accuracy, duplicate suppression, diversity coverage, truncation correctness и query latency. Любое изменение algorithm_version сравнивается с baseline report.

AI не используется как judge обязательных acceptance tests. Human review MAY дополнять corpus, но итоговые критерии должны быть воспроизводимыми.

## 54 Save/restore tests

Проверяются snapshot determinism, checksum, version compatibility, stage without live mutation, cross-participant reject, atomic activation, generation invalidation, cancelled restore и crash points. После failed restore live state полностью совпадает с состоянием до операции.

Round-trip test: создать records → snapshot → изменить live state → stage original → activate вместе с fake participants → сравнить semantic state и representative queries. Сравнение не требует идентичного physical SQLite layout.

Partial participant success никогда не маркируется общим success. Memory adapter не вызывает другие participants.

## 55 Security tests

Negative cases: oversized content, malformed UTF-8 at transport boundary, unknown enum, path traversal in import, formula-like export content, secret patterns, unauthorized audit query, cross-world ID, provenance cycle, decompression bomb и malicious future schema.

Logs сканируются на raw content, prompt, token, filesystem path и database row. Export/import использует безопасные filenames, bounded sizes и checksums. Fuzzing должен завершаться controlled result без process crash и unbounded allocation.

Threat model пересматривается перед Operational gate и при добавлении внешней синхронизации, vector service или administrative UI.

## 56 Load, stress и soak tests

Workloads: dialogue-heavy reads, event burst writes, one hot subject, many subjects, retention batch параллельно reads, save snapshot под load и restart after forced termination. Отчёт фиксирует percentiles, error rates, queue depth, DB growth, CPU, memory и recovery time.

Soak не менее проектно согласованного длительного окна без unbounded growth. Backpressure тест должен показать controlled Busy/Degraded, а не memory exhaustion. Crash-injection выполняется до transaction, внутри persistence adapter и после commit до event publication.

Пороговые значения утверждаются M3.6 на целевом Windows окружении; до этого таблица раздела 42 остаётся design target.

## 57 Acceptance criteria

MOD-003 может быть объявлен Operational только если:

1. Порты и contract types реализованы без запрещённых зависимостей.
2. Реальная SQLite migration chain проходит clean/upgrade/recovery tests.
3. Candidate validation, provenance, authority, dedup, correction и retention покрыты.
4. Retrieval bounded, deterministic и проходит golden corpus.
5. SaveParticipant проходит staged atomic restore tests.
6. AI/Dialogue не имеют прямой записи в memory repository.
7. Security и privacy negative suite проходит без утечек.
8. Load/soak подтверждает утверждённые budgets и backpressure.
9. RU/DE документация, schema catalog, event/error catalog и runbook синхронизированы.
10. Нет открытого blocker severity High/Critical.

Documentation Complete не равен Implementation Complete.

## 58 Implementation roadmap и gates

| Gate | Результат | Exit evidence |
|---|---|---|
| M3.0 Design freeze | Эта спецификация согласована | Review record, open decisions resolved |
| M3.1 Contracts | Value objects, ports, pure policies | Unit/property tests |
| M3.2 Repository | SQLite adapter и migrations | Integration/migration report |
| M3.3 Write path | Candidate-to-commit | Idempotency/failure tests |
| M3.4 Read path | Bounded retrieval/context | Golden corpus report |
| M3.5 Save/Event | Participant и post-commit events | Atomic restore/event tests |
| M3.6 Hardening | Security, load, recovery | Test reports and runbook |
| M3.7 Operational | Release evidence | Acceptance checklist signed |

Каждый gate завершается отдельным commit/review. Следующий gate не маскирует failure предыдущего. Feature flags не используются для объявления незавершённой функции готовой.

## 59 Operations и troubleshooting

Runbook должен включать: проверку readiness, schema/migration status, queue saturation, last successful operation, integrity scan, backup availability, retention backlog и safe export diagnostics.

Последовательность triage: определить scope/correlation → проверить lifecycle → repository health → schema/migration evidence → capacity → recent typed failures → безопасно воспроизвести query/command fixture. Нельзя начинать с ручного редактирования database.

При corruption suspicion остановить writes, сделать recoverable copy утверждённым инструментом, собрать hashes/metadata, выполнить documented scan и выбрать restore/migration recovery. Запрещены случайные SQL fixes на единственной копии.

## 60 Трассируемость, открытые решения и история

| Источник | Зафиксированное влияние |
|---|---|
| ARCH-002 | MOD-003 = Memory Engine, Simulation Layer, владелец памяти NPC |
| ARCH-003 | Асинхронность допустима, но поведенчески значимая память готова до следующего шага |
| ARCH-005 / ADR-008 | Static dependency и inverted port ownership |
| ARCH-007 | Dialogue path, AI candidate path, post-commit semantic event |
| ARCH-009 | Typed scoped configuration, atomic validation, safe defaults |
| ARCH-011 | SaveParticipant и staged atomic restore |
| ARCH-012 | Context source port, provenance и запрет AI direct persistence |
| ADR-002 | Semantic events, idempotent delivery, event history boundary |
| ADR-003 | SQLite behind narrow repositories, migrations/transactions ownership |
| MOD-001 | Core координирует, но не владеет memory domain logic |

Открытые design decisions до M3.0: конкретный persistence schema; exact salience coefficients; default retention durations; формат administrative export; необходимость FTS; outbox strategy; точный benchmark hardware profile. Они не меняют уже зафиксированные ownership и safety rules.

История: v1.0 от 2026-09-05 — создан полный RU design baseline MOD-003 до начала реализации.

## Приложение A — нормативная схема MemoryRecord

```text
MemoryRecord.v1
  memory_id: MemoryId
  world_id: WorldId
  lineage_id: SaveLineageId
  subject_id: SubjectId
  type: MemoryType
  predicate: PredicateId
  object_refs: List<EntityRef>          [0..16]
  content: StructuredContent            [bounded]
  occurred_at: DomainTimestamp
  observed_at: DomainTimestamp?
  recorded_at: InstantUtc
  valid_from: DomainTimestamp?
  valid_to: DomainTimestamp?
  provenance: ProvenanceRef
  authority: AuthorityClass
  confidence: Decimal01?
  retention_class: RetentionClass
  visibility: VisibilityPolicy
  salience_inputs: SalienceInputs
  schema_version: UInt16
  revision: UInt32
  supersedes: MemoryId?
  conflict_group_id: ConflictGroupId?
  content_hash: Hash256
  idempotency_key: IdempotencyKey
```

Обязательные ограничения: IDs не пусты; object refs уникальны; content проходит type-specific schema; confidence присутствует только для разрешённых типов; valid_to не раньше valid_from; supersedes не ссылается на себя; content hash вычисляется после canonical normalization.

## Приложение B — нормативная схема MemoryCandidate

```text
MemoryCandidate.v1
  candidate_id: OperationScopedId
  contract_version: SemVer
  scope: { world_id, lineage_id, subject_id }
  proposed_type: MemoryType
  proposed_fact: StructuredContent
  occurred_at: DomainTimestamp
  source:
    source_kind: SourceKind
    source_id: StableSourceId
    producer_module: ModuleId
    use_case: UseCaseId
    correlation_id: CorrelationId
    causation_id: CausationId?
    observed_by: List<EntityId>
    ai_metadata: AiProvenance?
  permission_context: CapabilityContext
  idempotency_key: IdempotencyKey
  received_at: InstantUtc
```

Candidate size проверяется до parsing nested content. `ai_metadata` никогда не содержит credential или полный prompt. Unknown source kind отклоняется.

## Приложение C — Query и Result schema

```text
MemoryQuery.v1
  scope, subject_id, purpose
  types[], related_entities[]
  occurred_window?, authority_floor?
  include_conflicts: bool
  max_records: 1..100
  max_serialized_bytes: 1..262144
  token_budget_hint?: 1..32768
  cursor?: StableCursor
  snapshot_hint?: SnapshotId

MemoryQueryResult.v1
  snapshot_id, policy_version
  items: List<MemoryProjection>
  truncated: bool
  next_cursor?: StableCursor
  context_completeness: Complete|Incomplete|Stale
  warnings[]
```

`MemoryProjection` содержит ID, type, structured summary, relevant entity refs, time, authority/confidence markers, provenance summary, salience explanation classes и conflict marker. Cursor подписывается/валидируется как opaque value и не раскрывает SQL offset.

## Приложение D — Configuration schema outline

```yaml
memory:
  schema_version: 1
  candidate_limits:
    max_bytes: <bounded integer>
    max_pending: <bounded integer>
    ttl_ms: <bounded duration>
  retrieval:
    default_max_records: <bounded integer>
    absolute_max_records: <bounded integer>
    default_max_bytes: <bounded integer>
  retention:
    policies: <closed typed map>
    batch_size: <bounded integer>
  salience:
    policy_version: <version>
    weights: <bounded typed map>
  cache:
    enabled: <boolean>
    max_entries: <bounded integer>
    ttl_ms: <bounded duration>
  maintenance:
    enabled: <boolean>
    interval: <duration>
  observability:
    metrics_enabled: <boolean>
    content_logging: false
```

Значения в angle brackets обязаны получить measured defaults на M3.6. `content_logging=true` schema отвергает, а не предупреждает.

## Приложение E — минимальный test catalogue

| ID | Сценарий | Ожидаемый результат |
|---|---|---|
| M3-VAL-001 | Candidate без subject | MEM_VALIDATION_FAILED, no write |
| M3-VAL-002 | Future contract version | MEM_SCHEMA_UNSUPPORTED |
| M3-ID-001 | Cross-world query | MEM_SCOPE_MISMATCH |
| M3-IDEM-001 | Повтор identical key/payload | Исходный result, одна запись |
| M3-IDEM-002 | Тот же key, другой hash | MEM_IDEMPOTENCY_CONFLICT |
| M3-AUTH-001 | AI conflict с canonical | Canonical projection first |
| M3-DUP-001 | Exact normalized duplicate | Existing ID, no create event |
| M3-DUP-002 | Near duplicate разных authority | Separate records/conflict link |
| M3-RET-001 | Expired record | Исключён из normal query |
| M3-RET-002 | Dry-run retention | No mutation, exact count |
| M3-QRY-001 | Byte budget reached | truncated=true, no overflow |
| M3-QRY-002 | Equal scores | Stable tie-break |
| M3-QRY-003 | Repository failure | Typed failure, не empty result |
| M3-EVT-001 | Commit fails | MemoryAccepted не публикуется |
| M3-EVT-002 | Duplicate delivery | One semantic effect |
| M3-SAVE-001 | Stage invalid checksum | Restore rejected, live unchanged |
| M3-SAVE-002 | Другой participant rejects | No activation |
| M3-SAVE-003 | Successful activation | New generation, consistent queries |
| M3-SEC-001 | Secret-like payload | Reject/classify by policy, no log leak |
| M3-SEC-002 | Unauthorized audit view | MEM_UNAUTHORIZED |
| M3-MIG-001 | Interrupted migration | Recoverable previous state |
| M3-PERF-001 | Hot subject write burst | Bounded queue/backpressure |
| M3-FAIL-001 | Commit outcome unknown | Lookup by idempotency key |
| M3-FAIL-002 | Corruption suspected | Writes stopped, RecoveryRequired |

## Приложение F — implementation checklist

- [ ] M3.0 ownership and open decisions reviewed.
- [ ] Public contract package contains no SQLite, Skyrim, AI SDK or UI types.
- [ ] Policy registry and record/candidate schemas versioned.
- [ ] Repository migrations carry checksum and recovery plan.
- [ ] All write paths are idempotent and post-commit event safe.
- [ ] Retrieval is bounded by count and bytes with stable ordering.
- [ ] Context result exposes provenance, authority and completeness.
- [ ] SaveParticipant stages non-live state and atomically activates.
- [ ] Security suite proves no raw content in ordinary logs.
- [ ] RU/DE specifications, event/error catalogs and runbook match.
- [ ] Acceptance evidence stored before status Operational.

## Приложение G — нормативные последовательности use cases

### G.1 Наблюдаемое игровое событие

1. Game Adapter нормализует runtime event и передаёт application use case stable IDs, generation и event evidence.
2. Owning world use case подтверждает, что событие действительно произошло, и определяет потенциальных witnesses.
3. Для каждого eligible subject создаётся отдельный `MemoryCandidate` с общей causation ID и уникальным idempotency key.
4. Memory Engine проверяет world/lineage, witness relation, type schema и provenance authority.
5. Duplicate policy выполняется до начала дорогой persistence operation.
6. Repository atomically записывает record и links; только после commit публикуется `MemoryAccepted.v1`.
7. Ошибка одного subject не объявляет успешным весь fan-out. Batch coordinator возвращает counts и per-item safe results.

### G.2 Факт, сообщённый в диалоге

Dialogue Engine получает из Context Manager разрешённый context slice. После реплики owning dialogue use case интерпретирует только структурированный outcome, а не весь текст. Candidate с authority `Reported` содержит speaker, listener, stated predicate и provenance диалога. Memory Engine проверяет, что listener является subject, speaker находится в разрешённой interaction scope, а тип допускает reported knowledge. При отсутствии такой проверки реплика не создаёт память.

### G.3 AI-derived inference

AI Adapter валидирует provider envelope и возвращает structured proposal. Application проверяет разрешённый use case, присваивает untrusted marker и формирует candidate. Memory Engine повторно проверяет тип, scope, evidence и policy; authority остаётся `Inferred`. Если предложение конфликтует с canonical fact, оно либо отклоняется, либо сохраняется как отдельное inference с conflict link — в зависимости от type policy. Provider success никогда не является persistence success.

### G.4 Correction canonical owner

Owning world module коммитит исправленный state, затем публикует correction event с prior fact reference. Memory Engine находит target в том же world/lineage, проверяет authority и создаёт `Correction`. В одной транзакции записываются новая запись, supersession relation и projection invalidation marker. После commit публикуется `MemoryCorrected.v1`. Старый record остаётся доступен Audit purpose.

### G.5 Bounded retrieval для диалога

Context Manager вызывает `BuildContextSlice` с subject, participants, topic refs, count/byte budgets и deadline. Memory Engine фиксирует snapshot, применяет security/visibility filters, извлекает кандидатов, детерминированно ранжирует и упаковывает. Ответ содержит completeness и truncation. Context Manager объединяет его с другими sources; Memory Engine не видит финальный prompt и не меняет данные из-за факта чтения.

### G.6 Retention maintenance

Maintenance scheduler инициирует `PlanRetention` вне critical path. План содержит policy version, snapshot ID, counts по классам и bounded batches. Operator/policy автоматически утверждает только разрешённые классы. `ApplyRetention` повторно проверяет generation, выполняет batch transaction, сохраняет tombstones/summary links по policy и публикует одно bounded event reference. При смене generation план становится stale и не применяется.

### G.7 Save snapshot

Save Manager запрашивает `PrepareSnapshot` у всех participants. Memory adapter фиксирует immutable generation, создаёт logical snapshot candidate и checksum, не меняя live state. Save Manager проверяет согласованность world/lineage и high-water marks между участниками, затем записывает общий artifact через save repository. Failed global commit не изменяет memory repository.

### G.8 Restore

Save Manager читает artifact, проверяет envelope и передаёт participant payload в `StageRestore`. Memory Engine десериализует его в non-live state, проверяет schema, references и policy. После успешной cross-validation Save Manager вызывает atomic logical activation всех staged candidates. Новый generation публикуется только после общего success. Любая ошибка до activation оставляет прежнее состояние полностью активным.

### G.9 Restart после неизвестного commit outcome

После process crash caller повторяет operation с прежним idempotency key. Memory Engine запрашивает terminal result. Если record уже коммитнут, возвращается исходный success без нового event effect; если key отсутствует, command проходит обычную validation. Если обнаружен key с другим hash, модуль возвращает `MEM_IDEMPOTENCY_CONFLICT` и не пытается «починить» данные автоматически.

## Приложение H — реестр конфигурации MOD-003

Конкретные числовые defaults утверждаются benchmark/security review. Таблица задаёт тип, допустимость hot reload и поведение при ошибке; она не маскирует ещё не выбранные значения.

| Ключ | Тип/ограничение | Reload | Ошибка |
|---|---|---|---|
| memory.schema_version | positive integer, exact supported | No | Startup fail |
| candidate_limits.max_bytes | bounded integer | Restricted | Reject config |
| candidate_limits.max_pending | bounded integer | Future only | Reject config |
| candidate_limits.ttl_ms | bounded duration | Future only | Reject config |
| candidate_limits.max_object_refs | 0..policy maximum | No | Reject config |
| content.max_text_bytes | bounded integer | Future only | Reject config |
| content.allowed_locales | closed locale list | No | Reject config |
| retrieval.default_max_records | 1..absolute maximum | Yes, atomic | Reject update |
| retrieval.absolute_max_records | bounded hard limit | No | Startup fail |
| retrieval.default_max_bytes | 1..absolute max bytes | Yes, atomic | Reject update |
| retrieval.absolute_max_bytes | bounded hard limit | No | Startup fail |
| retrieval.cursor_ttl_ms | bounded duration | Future only | Reject update |
| retrieval.allow_audit_view | false by safe default | Restricted | Deny capability |
| retention.policies | closed typed map | No/migration | Reject config |
| retention.batch_size | bounded integer | Yes, future batch | Reject update |
| retention.dry_run_required | true for destructive class | No | Startup fail |
| retention.maintenance_enabled | boolean | Yes | Keep prior value |
| retention.interval | bounded duration | Yes | Keep prior value |
| salience.policy_version | supported version | No/reindex | Startup fail |
| salience.weights | bounded typed map | Controlled | Reject whole update |
| salience.feedback_cap | decimal range | Controlled | Reject whole update |
| dedup.normalization_version | supported version | No/migration | Startup fail |
| dedup.near_duplicate_enabled | boolean | Controlled | Keep prior value |
| dedup.thresholds | type-specific bounded map | Controlled | Reject whole update |
| cache.enabled | boolean | Yes | Keep prior value |
| cache.max_entries | bounded integer | Yes with resize | Reject update |
| cache.ttl_ms | bounded duration | Yes | Reject update |
| repository.operation_timeout_ms | reference to persistence policy | Restricted | Startup fail |
| repository.busy_retry_policy | persistence-owned reference | No | Startup fail |
| save.max_participant_bytes | bounded integer | No | Startup fail |
| save.supported_schema_versions | closed list | No | Startup fail |
| observability.metrics_enabled | boolean | Yes | Keep prior value |
| observability.trace_sampling | 0..1 | Yes | Reject update |
| observability.content_logging | constant false | Never | Reject config |
| security.max_import_bytes | bounded integer | Restricted | Reject config |
| security.allowed_export_root | validated path reference | No | Startup fail |
| features.fts_adapter | boolean, default false | Restart | Reject update |
| features.vector_adapter | boolean, default false | Restart/ADR | Reject update |

Configuration snapshot и policy version входят в health evidence. Изменение, требующее migration/reindex, не выполняется обычным hot reload.

## Приложение I — матрица типов, authority и retention

| Memory type | Required semantics | Allowed authority | Default retention intent | Retrieval purposes |
|---|---|---|---|---|
| Observation | event, witness, place/time | Witnessed, Canonical | Normal | Dialogue, Behavior, Audit |
| Interaction | participants, outcome | Witnessed, Canonical | Normal | Dialogue, Relationship, Audit |
| Knowledge | predicate, source chain | Canonical, Witnessed, Reported, Inferred | Policy-based | Dialogue, Quest, Audit |
| ReputationEvidence | actor, action, affected party | Canonical, Witnessed, Reported | Long | Relationship, Behavior, Audit |
| QuestOutcome | quest, milestone, result | Canonical, Witnessed | Long | Dialogue, Quest, Audit |
| Rumor | speaker, statement, transmission | Reported, Rumor | Short/Normal | Dialogue, Audit |
| Commitment | parties, obligation, due/resolution | Canonical, Witnessed, Reported | UntilResolved then Long | Dialogue, Behavior, Audit |
| WorldChange | world entity, old/new semantic state | Canonical, Witnessed | Long/Permanent by policy | Dialogue, Quest, Audit |
| Correction | target, new assertion, reason | Canonical, Administrative | Target-aligned | All authorized purposes |
| Summary | sources, algorithm, covered window | Derived with inherited floor | Replaces details by policy | Dialogue, Behavior, Audit |

Дополнительные правила:

- Inferred не допускается для `QuestOutcome` и authoritative `WorldChange` без отдельного owning-use-case confirmation.
- Rumor не удовлетворяет query с authority floor `Witnessed`.
- Summary authority не превышает наиболее слабое критическое основание, если policy не умеет доказать canonical aggregation.
- Commitment expiry не наступает только из-за возраста; требуется resolution или специальная governance policy.
- Permanent назначается allowlist policy, а не producer request.

### I.1 Visibility classes

| Класс | Кто может читать | Пример ограничения |
|---|---|---|
| SubjectPrivate | Только use case от лица subject | Личное наблюдение |
| ParticipantShared | Участники зафиксированного interaction | Договорённость |
| OrganizationScoped | Авторизованные члены/системы организации | Внутренняя информация |
| PublicWorld | Любой eligible actor мира | Публичное событие |
| AuditRestricted | Только административный audit capability | Correction trail |

Visibility не выводится из типа автоматически. Утечка записи между классами является security failure.

## Приложение J — матрица требований и доказательств

| Requirement ID | Нормативное правило | Раздел | Обязательное доказательство |
|---|---|---:|---|
| M3-OWN-001 | Memory semantics принадлежит MOD-003 | 2–6 | Architecture dependency test/review |
| M3-OWN-002 | SQL не пересекает domain boundary | 6, 30–31 | Static import rule, contract test |
| M3-DAT-001 | Durable records versioned и scoped | 7, 10–12 | Schema tests |
| M3-DAT-002 | Runtime handles не сохраняются | 10 | Type/static tests |
| M3-PRV-001 | Каждая запись имеет provenance | 13 | Validation and migration tests |
| M3-AUT-001 | Canonical выше AI inference | 14, 26 | Conflict corpus |
| M3-TIM-001 | Event и record time различаются | 15 | Clock/chronology tests |
| M3-WRT-001 | Event только после commit | 16, 29, 32 | Failure injection test |
| M3-VAL-001 | Validation deterministic | 17 | Property tests |
| M3-DUP-001 | Retry не создаёт duplicate | 18, 32 | Idempotency integration test |
| M3-COR-001 | Correction не уничтожает audit | 19 | Revision-chain test |
| M3-RET-001 | Retention plan имеет dry run | 20 | Maintenance test |
| M3-SAL-001 | Salience versioned и bounded | 21 | Policy/property tests |
| M3-QRY-001 | Query ограничен count/bytes | 22–24 | Budget tests |
| M3-QRY-002 | Ranking deterministic | 23 | Golden corpus/property test |
| M3-CTX-001 | Incomplete context обозначается | 24–26, 45 | Failure integration test |
| M3-AI-001 | AI не пишет durable memory | 26 | Dependency/static test |
| M3-REL-001 | Relationship остаётся owner state | 27 | Contract review/test |
| M3-EVT-001 | Semantic events idempotent | 29, 48 | Duplicate delivery test |
| M3-REP-001 | Repository port use-case-oriented | 30 | API review |
| M3-TXN-001 | Candidate commit atomic | 32 | Crash/rollback tests |
| M3-CON-001 | Queues и retries bounded | 33, 44 | Stress test |
| M3-CAC-001 | Cache не source of truth | 34 | Restore/invalidation tests |
| M3-SAV-001 | Snapshot не пишет global artifact | 35 | Save contract test |
| M3-RST-001 | Restore staged и atomic | 36 | Cross-participant failure test |
| M3-MIG-001 | Migration recoverable/checksummed | 37 | Migration suite |
| M3-CFG-001 | Module видит только typed section | 38 | Configuration scope test |
| M3-SEC-001 | Logs не содержат memory content | 39–41 | Log scanning test |
| M3-DEL-001 | Delete обрабатывает derivatives | 40 | Privacy delete integration test |
| M3-OBS-001 | Health отделён от liveness | 41 | Startup/dependency test |
| M3-PER-001 | Budgets измерены на target | 42, 56 | Benchmark report |
| M3-ERR-001 | Empty result не маскирует failure | 43, 49 | Fault injection test |
| M3-OPS-001 | Recovery не использует AI/log truth | 44, 59 | Runbook review/exercise |

Требование считается закрытым только при наличии указанного evidence и ссылки из gate report. Сам факт наличия раздела документа закрывает Design, но не Verified.

## Приложение K — recovery playbooks

### K.1 Repository temporarily busy

Зафиксировать typed error rate и duration без content. Проверить long transaction/maintenance overlap. Разрешить только bounded persistence retry. Если лимит исчерпан, остановить новые background writes, сохранить interactive backpressure и сообщить Degraded. Не увеличивать timeout бесконечно и не отключать целостность.

### K.2 Repository unavailable

Перевести write path в reject, read path — в policy-approved cache only с `Incomplete/Stale`. Проверить path/configuration, filesystem availability и persistence health. После восстановления выполнить schema/integrity check и только затем Ready. Pending candidates без durable queue не объявляются сохранёнными.

### K.3 Migration failed

Не публиковать RepositoryReady. Сохранить исходную database и migration report, проверить checksum и preconditions. Выбрать документированный rollback/restore или исправленную forward migration. Запрещено менять migration manifest задним числом, чтобы скрыть несовпадение.

### K.4 Corruption suspected

Немедленно остановить writes, сохранить recoverable copy, hashes, file metadata и last-good backup reference. Выполнить утверждённую проверку на копии. Решение: restore backup, controlled export/import verified records или documented repair. Результат подтверждается integrity suite и representative queries до Ready.

### K.5 Event publication failure after commit

Не откатывать уже успешную memory transaction отдельной несогласованной операцией. Записать outbox/recovery evidence согласно выбранной M3.0 strategy и повторить delivery идемпотентно. Subscriber обязан принимать duplicate event ID. Если durable outbox не выбран, operational limitation документируется до release.

### K.6 Save restore rejected

Сохранить live generation, удалить/закрыть staged candidate, вернуть participant code Save Manager. Не активировать часть participants и не обновлять repository high-water mark. Diagnostic export содержит schema/checksum/counts, но не чувствительный content.

### K.7 Retention job interrupted

Возобновить по operation ID и checkpoint только при совпадении generation/policy version. Уже committed batches не повторяют side effects. При несовпадении создать новый dry-run plan. Никогда не продолжать stale destructive plan после restore.

## Приложение L — design review и acceptance ledger

### L.1 Решения, обязательные до M3.0

| Decision | Владелец review | Критерий закрытия |
|---|---|---|
| Physical schema и index plan | Memory + Persistence | Mapping сохраняет domain contracts; query plan measured |
| Outbox/event recovery strategy | Memory + Event + Persistence | Crash points и duplicate delivery покрыты |
| Default retention durations | Product/Memory/Security | Gameplay intent, privacy и storage budget согласованы |
| Salience coefficients | Memory + consuming use cases | Golden corpus baseline принят |
| FTS decision | Architecture/Memory | Польза доказана corpus benchmark либо отложена |
| Administrative export format | Operations/Security | Bounded, versioned, safe import round-trip |
| Benchmark target profile | Project/QA | Windows hardware и corpus size записаны |

### L.2 Gate review record

Каждый gate report содержит commit, schema versions, executed tests, environment, failures, waivers, performance figures и reviewer decision. Waiver имеет owner, reason, expiry и affected requirement IDs. High/Critical blocker не может быть waived для Operational.

### L.3 Definition of Done по артефактам

- Public contract package и API reference.
- Domain/schema catalog с version history.
- Repository migration manifest и recovery guide.
- Event/error catalog с owner и compatibility rule.
- Golden corpus и retrieval quality report.
- SaveParticipant compatibility fixtures.
- Security/privacy test report и threat-model update.
- Load/soak/benchmark report.
- Operations runbook и recovery exercise evidence.
- Синхронные RU/DE спецификации и canonical index update.

### L.4 Запись текущего baseline

| Поле | Значение |
|---|---|
| Document version | 1.0 |
| Documentation status | Complete design baseline |
| Implementation status | Not started |
| Authorized work | Documentation and pre-implementation specification |
| Implementation branch | Not created by this document |
| Current documentation branch | docs/mod-003-memory-engine-specification |
| Main branch changed | No |
| Next human decision | Review/approve M3.0 and unresolved decision table |
