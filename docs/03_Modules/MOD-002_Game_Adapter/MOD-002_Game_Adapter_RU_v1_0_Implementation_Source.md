# MOD-002 Game Adapter

Полная спецификация реализации для Skyrim SE и AE

Версия 1.0  |  Статус In Development  |  Дата 2026-09-04

Владелец проекта: Andrii Drahulskyi

## 1 Решение и назначение

MOD-002 становится единственной утверждённой границей между World Engine и Skyrim. Первая реализация поддерживает семейство Skyrim Special Edition и Anniversary Edition на Windows. Skyrim VR, Classic Skyrim, Windows Store, Game Pass и Epic Games Store не входят в первый релиз.

Модуль переводит игровые наблюдения в нормализованные входные контракты World Engine и выполняет только разрешённые игровые действия через выходной порт возможностей. Он не владеет состоянием мира World Engine, логикой диалога, AI, сохранениями, обнаружением модов или глобальной готовностью приложения.

Работа начинается в статусе Prototype. Ни одна версия Skyrim не получает статус Supported до сборки SKSE-плагина и прохождения тестовой матрицы на соответствующей версии игры.

## 2 Архитектурная позиция

MOD-002 относится к Integration Layer. Runtime-поток двунаправленный, но статический граф остаётся ациклическим.

| Отношение | Направление | Семантика |
|---|---|---|
| Вход из игры | Skyrim Runtime к Skyrim Bridge к Game Adapter к Game Ingress Port | Runtime call и передача нормализованных данных |
| Выход в игру | Application Coordination к Game Output Capability Port к Game Adapter к Skyrim Bridge | Runtime call после проверки capability и разрешения |
| Статическая зависимость | Game Adapter к application-owned port abstractions | Зависимость только от контрактов |
| Готовность | Game Adapter к Application Host | Evidence flow, не передача ownership |
| Обнаружение модов | Plugin Detection к Game Adapter | Versioned evidence, не второй filesystem scan |

Application Host выбирает активный Game Profile, создаёт adapter и зависимости, запускает и останавливает компоненты, а также вычисляет application-level Ready, Degraded, Startup Failed или Recovery Required. MOD-002 публикует только собственное integration evidence.

## 3 Область ответственности

MOD-002 отвечает за следующие обязанности:

1. определение идентичности Skyrim runtime через доверенный SDK-механизм;
2. проверку активного Game Profile и совместимости runtime target;
3. обнаружение и публикацию фактического набора capabilities;
4. регистрацию безопасных game event hooks;
5. нормализацию игровых событий до application contracts;
6. проверку входных схем, размера, последовательности и идентичности session;
7. выполнение разрешённых игровых действий;
8. нормализацию ошибок Skyrim и SKSE;
9. публикацию health, readiness и capability evidence;
10. контролируемое quiesce, reconnect, revalidation и shutdown.

## 4 Явно исключённая ответственность

MOD-002 не должен:

- импортировать или вызывать реализации Dialogue Engine, Memory Engine, Character Engine, Save Manager, Repository, Database или AI Adapter;
- использовать Event Engine как RPC для команд с немедленным результатом;
- читать SQLite или изменять World Engine domain state напрямую;
- самостоятельно сканировать плагины, load order или каталоги модов;
- решать совместимость восстановления сохранения;
- выполнять недоверенный plugin или Papyrus content;
- предоставлять внешнему контенту доступ к процессам, сети, секретам или произвольной файловой системе;
- объявлять capability доступной только по имени игры или наличию файла;
- автоматически повторять игровое действие, если повтор может создать второй эффект.

## 5 Решение по технологии

Первая реализация разделяется на две части.

| Часть | Среда | Технология | Ответственность |
|---|---|---|---|
| Portable Game Adapter | World Engine process | C++20 без Skyrim SDK types | Lifecycle, profile, capabilities, validation, contracts, failure normalization |
| Skyrim Bridge | Skyrim process | C++23, SKSE64, CommonLibSSE NG | Game hooks, SDK translation, минимальное исполнение game actions |

Для SE и AE применяется flat runtime target CommonLibSSE NG. VR build flags отключены. Papyrus не является обязательным слоем первого vertical slice. Papyrus добавляется только для конкретного события или действия, которое нельзя безопасно и стабильно получить через SKSE/CommonLibSSE, и остаётся тонким мостом без domain logic.

Процессная граница между Skyrim Bridge и World Engine планируется как локальный Windows named pipe с доступом текущего пользователя. Протокол использует versioned length-prefixed UTF-8 JSON frames. Транспорт не предоставляет сетевой listener и не принимает соединения с других компьютеров.

## 6 Внешние зависимости

| Зависимость | Requiredness | Правило |
|---|---|---|
| Application-owned Game Ingress Port | Required | Единственный путь нормализованного входа в coordination layer |
| Application-owned Game Output Capability Port | Required | MOD-002 реализует порт, application вызывает абстракцию |
| Application Host status sink | Required | Принимает scoped evidence и вычисляет application outcome |
| SKSE64 runtime | Required for Skyrim integration | Версия должна соответствовать конкретному Skyrim runtime |
| CommonLibSSE NG flat target | Required for plugin build | Содержит SDK bindings; типы не покидают Skyrim Bridge |
| Validated Plugin Profile evidence | Conditional | Используется для capability mapping; источник принадлежит Plugin Detection |
| Save compatibility contract | Not owned | MOD-002 предоставляет integration evidence, решение принимает Save Manager |

## 7 Game Profile

Активная session имеет ровно один Game Profile. Profile содержит `game_id`, runtime version, distribution, adapter identity, adapter version, profile version, capability evidence и известные ограничения.

| Runtime target | Цель первого релиза | Текущий gate |
|---|---|---|
| Steam Anniversary Edition 1.7.104, SKSE 2.3.1 | Да | Pending build and in-game validation |
| Steam legacy Special Edition 1.5.97, SKSE 2.0.20 | Да | Pending build and in-game validation |
| GOG Anniversary Edition 1.6.1179, SKSE 2.2.6 | Да | Pending build and in-game validation |
| Skyrim VR | Нет | Explicitly excluded |
| Windows Store Game Pass | Нет | SKSE unsupported |
| Epic Games Store | Нет | SKSE unsupported |

Runtime numbers отражают состояние официальной страницы SKSE на 2026-09-04. Обновления Bethesda считаются compatibility event: новая версия не наследует статус Supported автоматически.

## 8 Capability model

Каждая capability имеет состояние `Unknown`, `Available`, `Degraded`, `Unavailable` или `Unsupported`. Отсутствие capability не превращается в пустой успех и не заменяется другим действием.

| Capability ID | Назначение | MVP | Владелец факта |
|---|---|---:|---|
| runtime.identity | Идентичность игры, версии и distribution | Required | Skyrim Bridge и validated profile |
| session.lifecycle | New Game, Load Game, revert и shutdown signals | Required | Skyrim Bridge |
| dialogue.context.observe | Контекст начала и завершения разговора | Required | Skyrim Bridge, нормализация MOD-002 |
| ui.subtitle.show | Показ разрешённого текста через игровой UI | Required | Game Adapter action path |
| safe_point.evidence | Факт безопасного момента для внешней policy | Required | Skyrim Bridge evidence only |
| world.time.observe | Нормализованное игровое время | Optional | Skyrim Runtime |
| player.location.observe | Location и cell context игрока | Optional | Skyrim Runtime |
| actor.basic.observe | Разрешённые базовые данные actor | Optional | Skyrim Runtime |
| voice.playback.trigger | Запуск подготовленного voice asset | Deferred | Game Adapter action path |

Capability token или эквивалентный проверенный authorization context обязателен для каждого outbound action. Token не создаёт capability и не обходит текущий profile.

## 9 Жизненный цикл

Семантические состояния: `Created`, `Initializing`, `Ready`, `Degraded`, `Unavailable`, `Recovering`, `Stopping`, `Stopped`, `Failed`.

| Из | В | Условие |
|---|---|---|
| Created или Stopped | Initializing | Host вызывает start один раз для новой generation |
| Initializing | Ready | Profile valid, все required capabilities Available |
| Initializing | Degraded | Required capabilities доступны, optional capability ограничена |
| Initializing | Unavailable | Required capability отсутствует или runtime несовместим |
| Degraded или Unavailable или Failed | Recovering | Host разрешил bounded recovery |
| Recovering | Initializing | Старый bridge quiesced, начинается новая generation |
| Любое operational state | Stopping | Host начинает shutdown |
| Stopping | Stopped | Hooks сняты, pending operations завершены или отменены, resources освобождены |
| Любое состояние | Failed | Fatal incompatibility или исчерпанная recovery policy |

Каждая initialization generation получает новый идентификатор. Результат старой generation не может изменить текущий state.

## 10 Startup sequence

1. Host загружает validated typed configuration и выбирает Game Profile.
2. Host создаёт Game Adapter с явными port dependencies.
3. Adapter входит в Initializing и публикует scoped evidence.
4. Skyrim Bridge проверяет SKSE interface и runtime identity.
5. Adapter сверяет `game_id`, distribution и runtime target с profile.
6. Adapter получает validated Plugin Profile evidence через утверждённый contract, если capability mapping зависит от модов.
7. Bridge регистрирует hooks и возвращает capability evidence.
8. Adapter проверяет required capabilities.
9. Adapter публикует Ready, Degraded или Unavailable.
10. Host вычисляет итоговый application state.

Никакой callback внешней системы не вызывается под внутренней блокировкой MOD-002.

## 11 Inbound pipeline

Входной путь имеет последовательные gates:

`Skyrim event -> capture -> bounded queue -> schema validation -> session and sequence validation -> normalization -> Game Ingress Port -> typed outcome`

Каждый `RuntimeObservation` содержит schema version, event identity, монотонную sequence внутри активной session, observed timestamp, game identity, event type и bounded payload. Нулевой sequence, повтор, событие старой session, неподдерживаемая schema или payload свыше лимита отклоняются до Core или domain effect.

Game Adapter не публикует domain event самостоятельно, если contract owner не назначил MOD-002 producer. По умолчанию он вызывает Game Ingress Port, а application coordination решает, является ли принятый факт синхронным input или semantic event для Event Engine.

## 12 Outbound pipeline

Выходной путь:

`Application command -> schema validation -> capability lookup -> authorization context -> state guard -> runtime translation -> Skyrim execution -> normalized result`

Команда содержит action ID, correlation ID, capability ID, capability token, action type и bounded payload. Игровой API вызывается только после успешных gates. `Unsupported`, `Unavailable`, `InvalidInput`, `TimedOut`, `Busy` и `Failed` являются отдельными результатами.

Повтор action разрешается только когда contract явно определяет idempotency. По умолчанию автоматический retry после неизвестного результата запрещён.

## 13 Safe point evidence

MOD-002 может сообщать, что Skyrim находится в состоянии, подходящем для определённой операции, например отсутствует loading screen, game process отвечает и активная session стабильна. Это только evidence.

Scheduler владеет autosave policy. Save Manager владеет save и restore semantics. Host владеет application recovery policy. MOD-002 не запускает autosave и не принимает решение об atomic restore.

## 14 Plugin evidence и моды

Plugin Detection остаётся единственным владельцем discovery, parsing, normalization и fingerprint. MOD-002 не читает load order и не сканирует каталоги независимо.

Game Adapter получает immutable validated Plugin Profile и преобразует его в capability evidence. Unknown plugin не предоставляет capability. Failed rescan не превращается в пустой valid profile. Последний действующий profile может использоваться только с явным degraded evidence и policy решениями владельцев.

## 15 Протокол и данные

Protocol version отделяется от adapter version и Game Profile version. Первая версия транспорта использует:

- локальный per-user named pipe;
- 32-bit little-endian frame length;
- UTF-8 JSON body;
- максимальный payload 1 MiB по умолчанию;
- обязательные `schema_version`, `message_id`, `session_id`, `generation`, `message_type`;
- correlation ID для request and result;
- монотонный sequence для inbound observations;
- отсутствие raw pointers, memory addresses и Skyrim SDK object layouts.

Raw form IDs не считаются глобальными World Engine identifiers. Если они нужны, adapter передаёт game-scoped identity с profile fingerprint и load-order context.

## 16 Threading и backpressure

Skyrim callbacks могут приходить на game threads. Callback выполняет только bounded capture и не ждёт World Engine, AI, disk или network. Дальнейшая обработка проходит через ограниченную очередь.

| Правило | Требование |
|---|---|
| Game thread blocking | Запрещено для внешнего ожидания |
| Queue capacity | Явно ограничена configuration |
| Overflow | Typed drop or backpressure policy по event class |
| Locks | Не удерживаются во время port, bridge или status callback |
| Cancellation | Передаётся всем долгим операциям |
| Shutdown | Новые inputs закрываются до drain или bounded cancel |

Critical lifecycle evidence не удаляется по той же политике, что background observation. Для каждого класса входа определяется отдельная overflow policy.

## 17 Error model

| Категория | Пример | Результат |
|---|---|---|
| Invalid input | Пустой event ID, неверная schema, oversized payload | Reject before downstream effect |
| Unsupported | Capability отсутствует в реализации | Typed Unsupported |
| Unavailable | Игра или required hook временно недоступны | Unavailable или adapter state transition |
| Incompatible | Runtime version не поддержан | Unavailable или Failed, no support claim |
| Timeout | Bridge operation не завершена до deadline | TimedOut, bounded recovery decision |
| Duplicate | Sequence уже принят | Rejected without repeated effect |
| SDK failure | Exception или API error | Normalized Failed with bounded diagnostics |
| Security rejection | Нет authorization context | Rejected and audited |

Пользовательский текст, полный path и содержимое диалогов не используются как metric labels.

## 18 Recovery

Recovery выполняет последовательность Detect, Quiesce, Disconnect, Reconnect, Revalidate Profile, Rediscover Capabilities и Resume. Каждый retry ограничен count и deadline. Возврат в Ready или Degraded возможен только после новой validation.

World Engine domain state остаётся authoritative. Неизвестный результат outbound action не трактуется как успех. После game load или revert adapter создаёт новую session identity и отклоняет старые callbacks.

## 19 Configuration

MOD-002 получает immutable typed configuration view. Он не читает raw YAML и не делает runtime lookup глобального config.

| Поле | Назначение | Default |
|---|---|---|
| expected_game_id | Ожидаемый profile | skyrim-se-ae |
| max_payload_bytes | Верхняя граница message payload | 1048576 |
| inbound_queue_capacity | Максимум pending observations | Определяется после измерений |
| connect_timeout_ms | Deadline соединения с bridge | Определяется после прототипа |
| operation_timeout_ms | Default action deadline | Определяется по action class |
| recovery_attempt_limit | Bounded retry count | Определяется operational policy |
| pipe_name | Logical local transport name | world-engine.mod002.v1 |

Изменение active profile является controlled restart, а не hot toggle. Hot reload не может менять security, transport identity или compatibility state без candidate validation и commit.

## 20 Безопасность

1. Named pipe доступен только текущему пользователю и ожидаемому process context.
2. Network exposure отсутствует по умолчанию.
3. Все message sizes и string lengths ограничены до parsing.
4. Capability token проверяется до game API call.
5. Plugin content и Papyrus text считаются untrusted.
6. MOD-002 не получает AI provider secrets.
7. Diagnostics редактируют paths, tokens и пользовательский content.
8. Skyrim Bridge не выполняет arbitrary command, script или file path из payload.
9. Ошибка authorization не переводится в retry.
10. Новая process или network boundary требует отдельного security review.

## 21 Наблюдаемость

Обязательные signals:

- adapter state и generation;
- active profile identity и validation result;
- capability state changes;
- initialization и capability discovery duration;
- ingress accepted, rejected, duplicate, out-of-order and dropped counts;
- outbound result counts по bounded action category;
- queue depth and overflow;
- reconnect attempts and recovery outcomes;
- incompatible runtime and unsupported capability counts.

Event ID, actor ID, dialogue text и произвольные error strings не используются как cardinality-unbounded labels.

## 22 Производительность

До измерений не фиксируются выдуманные latency SLO. Первый prototype должен получить baseline для callback capture time, queue delay, normalization time, bridge round trip, initialization, memory и CPU overhead.

Release gate требует, чтобы game-thread capture оставался bounded, очереди имели лимиты, capability discovery не выполнялся на каждом вызове, а inactive adapter не потреблял активные runtime resources.

## 23 Save и restore boundary

MOD-002 может предоставить game identity, runtime version, profile version, capability evidence и safe-point evidence. Save Manager самостоятельно сравнивает save baseline и текущую среду, создаёт restore candidate, выполняет cross-participant validation и atomic activation.

Game Adapter не вызывает participant commit, abort или rollback от имени Save Manager. Game-specific restore action выполняется только через отдельный approved contract и не становится общим решением совместимости.

## 24 Тестовая стратегия

| Уровень | Обязательное покрытие |
|---|---|
| Portable unit tests | Lifecycle, required and optional capabilities, validation, deduplication, typed failures |
| Contract tests | Game Ingress и Game Output Capability port semantics |
| Profile schema tests | Required fields, supported enums, version formats, exclusions |
| Transport tests | Framing, payload limit, disconnect, partial frame, malformed UTF-8 and JSON |
| Concurrency tests | Callback capture, queue overflow, cancellation, shutdown race, stale generation |
| SKSE bridge tests | Runtime identity, hook registration, event translation and action execution |
| Integration tests | Skyrim to adapter to fake application and application to adapter to Skyrim |
| Compatibility matrix | Steam AE current, Steam SE 1.5.97 and GOG AE 1.6.1179 |
| Failure tests | Missing SKSE, incompatible runtime, rejected action, lost connection and recovery exhaustion |
| Security tests | ACL, oversized frame, unknown action, missing token and untrusted content |

## 25 Acceptance criteria

MOD-002 v1.0 может перейти из Prototype в Supported только когда выполнены все условия:

1. portable library собирается с предупреждениями как errors;
2. все unit and contract tests проходят;
3. SKSE plugin собирается для SE/AE flat target без VR;
4. profile schema проходит validation;
5. required MVP capabilities подтверждены в каждой заявленной runtime target;
6. unsupported capability возвращает typed result;
7. invalid input не достигает Core или domain effect;
8. Core and domain packages не импортируют Skyrim or SKSE types;
9. Plugin Detection остаётся единственным discovery owner;
10. game update переводит неизвестную версию в incompatible или prototype, но не Supported;
11. shutdown и recovery не оставляют активных hooks или неограниченных retries;
12. diagnostics не раскрывают token или пользовательский content;
13. documentation, profile, tests and dependency graph обновлены в одном change set;
14. тестовый отчёт содержит точные версии Skyrim, SKSE, CommonLibSSE NG и mod manager environment.

## 26 План реализации

| Этап | Результат | Статус на 2026-09-04 |
|---|---|---|
| M2.1 Architecture and contract baseline | ADR, full specification, port types, lifecycle core | Implemented in branch |
| M2.2 Portable adapter verification | CMake build and dependency-free unit tests | Code ready, Windows toolchain required |
| M2.3 Local transport | Named pipe framing, ACL and host-side runtime bridge | Planned |
| M2.4 SKSE event bridge | Plugin lifecycle and required event hooks | Entry point scaffolded |
| M2.5 Outbound MVP | Subtitle and approved UI action path | Planned |
| M2.6 Profile and plugin evidence mapping | Capability decisions from validated Plugin Profile | Planned |
| M2.7 In-game compatibility validation | Steam AE, Steam SE legacy and GOG AE | Requires test installation |
| M2.8 Release gate | Packaging, diagnostics, installer and support declaration | Blocked until prior gates pass |

## 27 Open decisions

Следующие решения не блокируют M2.1, но должны быть закрыты до соответствующего этапа:

- точный CommonLibSSE NG release и package baseline;
- окончательная named pipe ACL и process authentication;
- wire schema для observations, actions and results;
- конкретные SKSE event hooks для dialogue context и safe-point evidence;
- MO2 и Vortex packaging layouts;
- retry policy per action class;
- numeric performance budgets после prototype measurements;
- необходимость минимального Papyrus bridge для отсутствующих hooks.

## 28 Трассируемость

| Требование MOD-002 | Источник |
|---|---|
| Единственная game boundary | ARCH-001, ARCH-002, ARCH-005 |
| Ingress и output через owned ports | ARCH-003, ARCH-005, ADR-007, ADR-008 |
| Host lifecycle and readiness ownership | ARCH-002, ARCH-008, ADR-008 |
| Capability states and explicit unsupported | ADR-007 |
| Plugin evidence ownership | ARCH-010, ADR-006 |
| Save compatibility ownership | ARCH-011, ADR-005 |
| Configuration candidate validation | ARCH-009 |
| Event semantics and no RPC | ADR-002 |
| SE/AE implementation technology | ADR-009 |

## 29 Внешние технические источники

- SKSE official site: https://skse.silverlock.org/
- SKSE source repository: https://github.com/ianpatt/skse64
- CommonLibSSE NG repository: https://github.com/CharmedBaryon/CommonLibSSE-NG

## 30 История изменений

| Версия | Дата | Изменение |
|---|---|---|
| 1.0 | 2026-09-04 | Зафиксированы авторизация MOD-002, Skyrim SE/AE scope, port boundaries, lifecycle, capability model, C++ and SKSE split, prototype code and release gates |
