# MOD-002 Game Adapter — детальная часть

Продолжение полной спецификации реализации для Skyrim SE и AE

Версия 2.0  |  Статус In Development — Prototype  |  Дата 2026-09-05

## 31 Как читать полную спецификацию

Разделы 1–30 фиксируют нормативную архитектурную базу. Разделы 31–60 превращают её в рабочую спецификацию для реализации, проверки, установки и сопровождения. Приложения содержат фактические исходные файлы текущего прототипа; поэтому документ можно использовать без параллельного открытия репозитория.

Ключевые слова MUST, MUST NOT, SHOULD, SHOULD NOT и MAY задают обязательность. MUST и MUST NOT проверяются тестом, статическим правилом либо release checklist. SHOULD допускает отклонение только при записанном техническом обосновании. MAY описывает опцию, которая не меняет совместимость остальных контрактов.

Документ различает три вида утверждений. «Реализовано» означает наличие кода в ветке MOD-002. «Спроектировано» означает нормативный контракт без завершённой реализации. «Подтверждено» означает наличие воспроизводимого тестового свидетельства. Текущий статус модуля — Prototype: portable lifecycle и contract types реализованы, SKSE bridge представлен только точкой входа, а игровые hooks, transport и compatibility matrix ещё не подтверждены.

| Метка | Значение | Допустимое доказательство |
|---|---|---|
| Implemented | Код присутствует и проходит статическую проверку | Исходный файл, diff, локальная сборка |
| Designed | Решение утверждено, но код может отсутствовать | Эта спецификация и ADR-009 |
| Verified | Поведение воспроизводимо проверено | Автоматический тест или подписанный manual test record |
| Supported | Все обязательные gates конкретного runtime пройдены | Compatibility report и release manifest |
| Deferred | Возможность сознательно вынесена за пределы релиза | Roadmap item без скрытого fallback |

## 32 Контекст системы и доверительные границы

World Engine Host и Skyrim выполняются в разных процессах. Внутри процесса Skyrim работает только минимальный SKSE plugin. Он считывает доступные через SDK факты, подписывается на разрешённые события и выполняет ограниченный набор игровых операций. Portable Adapter работает на стороне World Engine и не содержит типов Skyrim, SKSE или CommonLibSSE. Локальный transport соединяет эти части, но не меняет ownership: Game Adapter остаётся единственным владельцем интеграции, а Host — владельцем композиции и итоговой готовности приложения.

Доверительная граница проходит по каждому кадру протокола. Даже если оба процесса запущены одним пользователем, сообщение не считается корректным до проверки длины, кодировки, схемы, версии, session, generation и допустимого типа. Plugin data, названия форм, subtitle text и строки ошибок считаются недоверенным содержимым. Capability token является application-issued authorization context, а не секретом длительного хранения и не заменяет проверку capability state.

| Зона | Доверие | Разрешения | Запрещённые зависимости |
|---|---|---|---|
| Application coordination | Доверенный orchestration code | Выдаёт action request и authorization context | Не использует SKSE types |
| Portable Adapter | Доверенный integration code | Валидирует, нормализует, публикует evidence | Не читает game memory и load order напрямую |
| Local transport | Недоверенная граница | Передаёт только versioned frames | Нет сети, shell, произвольных файлов |
| Skyrim Bridge | Привилегированный in-process code | Минимальные hooks и actions | Нет domain logic, AI и database access |
| Mod/Papyrus content | Недоверенный input | Только явно описанные события | Нет произвольных команд World Engine |

За пределами границы остаются AI provider, база данных, Save Manager, Plugin Detection, Event Engine и доменные модули. Любая необходимость обратиться к ним реализуется через application-owned port, а не прямой импорт.

## 33 Внутренние компоненты и их роли

### 33.1 Portable Game Adapter

`GameAdapter` управляет состояниями, generation, активным profile, required capabilities, проверкой observation и action. Он реализует `IGameOutputCapabilityPort` и потребляет `IGameIngressPort`, `IGameAdapterStatusSink`, `IRuntimeBridge`. Объект не создаёт эти зависимости и не владеет временем жизни Host.

### 33.2 Profile Validator

Profile Validator проверяет JSON Schema, semantic version fields, уникальность capability IDs, допустимые distribution и edition, а также соответствие runtime target заявленному support status. Schema-valid profile ещё не становится supported: status `supported` разрешается только release pipeline после успешной compatibility matrix.

### 33.3 Capability Registry

Registry хранит снимок evidence для активной generation. Каждая запись содержит capability ID, state, reason code, evidence source и при необходимости ограниченный диагностический контекст. Изменение capability создаёт новый immutable snapshot и status evidence; читатель не наблюдает частично обновлённый набор.

### 33.4 Ingress Processor

Processor принимает observation после transport validation, проверяет session и sequence, нормализует game-scoped identifiers и вызывает application-owned ingress port. Резервирование sequence до внешнего вызова предотвращает неявный повтор side effect после неоднозначного результата.

### 33.5 Action Executor

Executor проверяет schema, identity, deadline, capability, authorization и idempotency policy. Только затем он создаёт transport request. Он нормализует результат Skyrim Bridge и никогда не превращает timeout в success.

### 33.6 Skyrim Bridge

Bridge изолирует CommonLibSSE/SKSE и имеет четыре подчасти: bootstrap, event capture, action handlers и transport endpoint. Bootstrap единожды инициализирует SKSE interfaces. Event capture выполняет минимальную работу на игровом потоке. Action handlers содержат только whitelisted operations. Transport endpoint не слушает TCP/UDP и не принимает произвольные method names.

## 34 Ownership, время жизни и запреты на вызовы

Application Host создаёт dependencies, затем `GameAdapter`, запускает его после Configuration и Plugin Detection readiness и останавливает до уничтожения портов. `IRuntimeBridge` должен переживать все вызовы adapter и корректно обрабатывать повторный disconnect. Ingress и status sink не вызываются под mutex adapter.

| Объект | Создаёт | Уничтожает | Владеет изменяемым состоянием |
|---|---|---|---|
| GameAdapter | Application Host | Application Host | lifecycle, generation, profile snapshot, last sequence |
| RuntimeBridge client | Host integration factory | Host после stop | connection и request correlation |
| Skyrim Bridge endpoint | SKSE plugin bootstrap | SKSE shutdown/process exit | hook registrations и transport handle |
| GameProfile snapshot | RuntimeBridge discovery | Adapter при новой generation | Только immutable snapshot |
| Capability token | Application authorization service | Истекает по policy | Scope, expiry, action binding |

Запрещены re-entrant вызовы `start`, `recover` и `stop` из status callback. Adapter не должен ждать игровой поток, удерживая mutex. Bridge не должен вызывать observation callback после подтверждённого shutdown; если callback уже находился в полёте, generation/session check обязан его отклонить.

## 35 Алгоритм идентификации runtime

Runtime identity строится из данных, которые возвращает SKSE/CommonLibSSE, и проверяется против активного profile. Имя executable или наличие DLL недостаточно. Минимальный identity tuple: `game_id`, distribution, edition, runtime version, SKSE version, adapter version, profile version и process architecture.

1. Bridge получает runtime version через SDK interface.
2. Distribution определяется поддерживаемым SDK/build channel признаком; эвристика пути может использоваться только как диагностический hint.
3. Adapter находит точное совпадение `distribution + edition + runtime_version` в profile.
4. Проверяется заявленная SKSE version и build compatibility.
5. Для target со значением `pending_game_test` возвращается Prototype/Unavailable evidence, но не Supported.
6. Неизвестный runtime не округляется до «ближайшего» и не наследует совместимость.
7. Identity фиксируется на generation; изменение процесса или load session создаёт новую session.

| Ситуация | Capability runtime.identity | Итог adapter |
|---|---|---|
| Точное совпадение, target validated | Available | Продолжить discovery |
| Точное совпадение, pending_game_test | Degraded с prototype reason | Только test mode |
| Версия отсутствует в profile | Unsupported | Unavailable |
| SKSE interface отсутствует | Unavailable | Unavailable |
| Distribution не поддерживается | Unsupported | Failed или Unavailable по host policy |

## 36 Полная валидация Game Profile

Валидация состоит из syntax, schema, semantic и release layers. Syntax layer требует корректный UTF-8 JSON. Schema layer отклоняет unknown fields, пустые массивы runtime targets и неверные enum. Semantic layer проверяет отсутствие дубликатов, непротиворечивость required/optional capabilities и исключение VR. Release layer запрещает support status `supported`, если хотя бы один заявленный target не имеет независимого test record.

Required и optional capability не могут пересекаться. `known_limitations` не заменяет capability state. Profile version изменяется при любом изменении semantics; adapter version изменяется при изменении исполняемого кода. Runtime target добавляется как `pending_game_test`, затем отдельным проверенным change set переводится в `validated`.

Ошибка текущего candidate profile не уничтожает последний подтверждённый snapshot. Host может сохранить прежний snapshot только в Degraded режиме и только если runtime identity не изменился. При несовпадении identity fallback запрещён.

## 37 Детальная state machine запуска

`start()` допустим только из Created или Stopped. Перед внешним connect adapter увеличивает generation, сбрасывает sequence и публикует Initializing. Connect, profile discovery и capability discovery выполняются без удержания внутреннего mutex. После каждого внешнего шага результат связывается с generation; устаревший результат игнорируется.

| Шаг | Deadline | Успех | Ошибка |
|---|---|---|---|
| Load typed config | Host-owned | Candidate config | Startup Failed до создания adapter |
| Connect transport | `connect_timeout_ms` | Connected evidence | Unavailable, bounded recovery |
| Handshake | Короткий protocol deadline | Совместимые protocol ranges | Incompatible |
| Discover profile | Operation deadline | Validated identity tuple | Unavailable/Failed |
| Map plugin evidence | Host policy | Capability inputs | Degraded без повторного scan |
| Register hooks | Per-hook result | Capability evidence | Required missing = Unavailable |
| Publish operational | Без внешней блокировки | Ready или Degraded | Host агрегирует итог |

Ready означает, что все required capabilities Available. Degraded допускает ограничение только optional capability либо явно разрешённый prototype mode. Unavailable означает невозможность выполнения обязательной функции; этот state не является crash. Failed используется для нарушенной инварианты, исключения bootstrap или исчерпанной recovery policy.

## 38 Shutdown, recovery и смена generation

Shutdown закрывает вход новых action, переводит adapter в Stopping, отменяет pending requests, просит Bridge снять hooks, завершает bounded drain и закрывает transport. Повторный `stop()` после Stopped возвращает успешный idempotent result. Если disconnect завис, Host завершает ожидание по deadline и записывает forced-cleanup evidence; бесконечное ожидание запрещено.

Recovery разрешён из Degraded, Unavailable или Failed только по решению Host. Последовательность: Quiesce, Disconnect, invalidate session, увеличить generation, Connect, Handshake, Discover Profile, Discover Capabilities, Publish. Retry budget хранится у Host, чтобы MOD-002 не мог организовать бесконечный цикл.

Старая generation не может публиковать capability snapshot, выполнять action result или принимать observation. Correlation map очищается при смене generation. Неизвестный результат неидемпотентного action возвращается как `TimedOut` либо `Failed` с `effect_unknown=true` в детальном протоколе; автоматический повтор запрещён.

## 39 Каталог inbound observations

Ниже определён нормативный MVP-каталог. Конкретный hook может измениться без изменения application contract, если сохраняются semantics и тесты.

| Event type | Обязательные поля payload | Capability | Overflow class |
|---|---|---|---|
| game.session.new | session_hint, game_time | session.lifecycle | Critical |
| game.session.loaded | save_hint, game_time | session.lifecycle | Critical |
| game.session.revert | reason | session.lifecycle | Critical |
| game.session.ending | reason | session.lifecycle | Critical |
| game.dialogue.started | speaker, target, location | dialogue.context.observe | Interactive |
| game.dialogue.ended | speaker, target, reason | dialogue.context.observe | Interactive |
| game.world.time.changed | day, hour, timescale | world.time.observe | Background |
| game.player.location.changed | worldspace, cell, position_bucket | player.location.observe | Coalescible |
| game.actor.basic.changed | actor, changed_fields | actor.basic.observe | Coalescible |
| game.safe_point.changed | state, blockers | safe_point.evidence | Critical |

`event_id` уникален в пределах session. `sequence` монотонно возрастает для каждого принятого observation stream. `observed_at_unix_ms` отражает время capture, а не время обработки. Payload не содержит raw memory address, неограниченный stack dump или локализованный текст как идентификатор.

Critical event не удаляется молча. При заполнении critical queue Bridge переходит в Degraded/Unavailable и сообщает loss-of-continuity. Interactive event имеет короткий bounded wait или отдельный резерв. Coalescible event может заменить старое непринятое значение того же key. Background event может быть отброшен с метрикой и reason code.

## 40 Нормализация идентификаторов и данных

Actor, cell, worldspace и form существуют только в контексте конкретной game profile. Нормализованный идентификатор включает `game_id`, `profile_fingerprint`, logical kind и stable game-local key. Raw form ID без load-order fingerprint запрещён как долговременный World Engine ID, потому что load order меняет старшие байты и может переназначить форму.

Строки нормализуются как UTF-8, проверяются на допустимую длину и не используются как ключ безопасности. Координаты округляются или bucketed, если точность не нужна контракту, чтобы ограничить объём и риск раскрытия. Отсутствующее значение передаётся как explicit null/optional, а не пустая строка с неоднозначной семантикой.

Normalization не обогащает наблюдение доменными выводами. Например, Bridge сообщает факт начала dialogue и участников, но не решает настроение NPC, цель разговора или память персонажа. Такие выводы принадлежат соответствующим application/domain компонентам.

## 41 Каталог outbound actions

MVP разрешает только фиксированные action types. Unknown action отклоняется до вызова Skyrim API. Поля `action_id`, `correlation_id`, `capability`, `capability_token`, `deadline` и `payload` обязательны на wire level.

| Action type | Capability | Payload | Idempotency | Текущий статус |
|---|---|---|---|---|
| game.ui.subtitle.show | ui.subtitle.show | text, duration_ms, speaker optional | Idempotent только при dedupe key | Designed |
| game.ui.subtitle.clear | ui.subtitle.show | presentation_id | Idempotent | Designed |
| game.voice.playback.start | voice.playback.trigger | asset_id, actor, volume | Non-idempotent | Deferred |
| game.voice.playback.stop | voice.playback.trigger | playback_id | Idempotent | Deferred |
| game.integration.ping | runtime.identity | nonce | Idempotent | Planned diagnostic |

Subtitle text имеет фиксированный maximum length, проходит control-character filtering и не интерпретируется как format string, Papyrus или console command. File path для voice asset не передаётся: разрешён только заранее зарегистрированный `asset_id`. Action handler проверяет, что соответствующий menu/game state допускает операцию; иначе возвращает Busy или Unavailable.

## 42 Авторизация, deadline и идемпотентность

Authorization context связывает token с capability, action class, session, generation, expiry и при необходимости actor scope. Bridge не принимает token как доказательство сам по себе: Portable Adapter проверяет его до отправки, а Bridge проверяет подписанный или negotiated session context согласно окончательному security design.

Deadline измеряется монотонными часами. Просроченная команда не входит в очередь. Если deadline истёк после отправки, adapter возвращает TimedOut и сохраняет correlation до получения позднего результата или до controlled cleanup. Поздний результат записывается для диагностики, но не заменяет уже опубликованный outcome без специального reconciliation contract.

Для idempotent action используется caller-supplied dedupe key. Для non-idempotent action автоматический retry отсутствует. Повтор с тем же action ID обязан вернуть сохранённый result либо explicit duplicate, а не снова вызвать игровой API.

## 43 Wire protocol v1

Frame состоит из 4-байтовой little-endian длины и UTF-8 JSON body. Длина относится только к body, не может быть нулевой и не превышает `max_payload_bytes`. Decoder обязан корректно собирать partial reads и обрабатывать несколько frames в одном read. Malformed length закрывает connection после bounded diagnostic.

| Поле envelope | Тип | Обязательность | Правило |
|---|---|---|---|
| schema_version | integer | MUST | Ровно 1 для текущего contract |
| message_id | UUID/string | MUST | Уникален в session |
| message_type | enum | MUST | hello, observation, action, result, evidence, shutdown |
| session_id | string | MUST | Negotiated при handshake |
| generation | uint64 string/number | MUST | Совпадает с active generation |
| correlation_id | string | Conditional | Обязателен для action/result |
| sent_at_unix_ms | integer | SHOULD | Диагностика, не security decision |
| payload | object | MUST | Дополнительная schema по message type |

Handshake обменивает protocol minimum/maximum, adapter identity, runtime identity, process ID и случайные nonces. Несовместимые ranges завершают connection как `protocol.incompatible`. Unknown поля envelope отклоняются в strict mode. Version upgrade выполняется через additive schema и feature negotiation; reinterpretation существующего поля запрещена.

## 44 Named pipe transport

Первая реализация использует Windows named pipe в local-only режиме. Server создаётся Skyrim Bridge или отдельным host-owned bootstrap решением, зафиксированным до M2.3. ACL разрешает текущему интерактивному пользователю и ожидаемому process context; широкие группы Everyone/Anonymous запрещены.

Transport имеет отдельные bounded read/write queues, maximum concurrent requests, connect deadline, idle heartbeat и shutdown handshake. Он не делает business retry. Разрыв соединения завершает pending request typed результатом и инициирует evidence для Host. Payload не записывается в обычный log; допускаются message type, byte count, duration, result code и redacted identifiers.

Для защиты от confused-deputy атаки endpoint сверяет handshake identity, generation nonce и ожидаемый executable context. Точная защита процесса должна быть подтверждена security test на поддерживаемых Windows версиях; до этого граница остаётся Prototype.

## 45 Concurrency и backpressure design

Игра не должна ждать AI, базы данных или World Engine orchestration. Hook callback копирует минимальный bounded набор данных в preallocated/bounded structure и возвращается. Serialization выполняется на worker thread. Outbound Skyrim API, требующий main thread, планируется через SKSE task interface и завершается асинхронным result.

| Очередь | Producer | Consumer | Политика переполнения |
|---|---|---|---|
| Critical lifecycle | Game hooks | Transport worker | Не терять молча; Degraded и reconnect/resync |
| Interactive dialogue | Game hooks | Transport worker | Reserved capacity, bounded wait только вне critical game path |
| Coalescible context | Game hooks | Normalizer | Replace pending value per key |
| Outbound actions | Host | Bridge worker/main task | Reject Busy до enqueue при лимите |
| Diagnostics | Все компоненты | Telemetry sink | Drop oldest с counter |

Lock order фиксируется: lifecycle mutex, connection state, correlation map, queue-local lock. Внешний callback никогда не вызывается под этими lock. Shutdown выставляет cancellation, закрывает producers, ожидает bounded drain, затем освобождает hooks и handles. ThreadSanitizer может использоваться для portable layer, но Windows/Skyrim runtime требует отдельного stress harness.

## 46 SKSE/CommonLibSSE integration plan

Текущий `plugin.cpp` только вызывает `SKSE::Init`; это честный scaffold, а не готовая интеграция. M2.4 добавляет plugin metadata, logging bootstrap, SKSE messaging subscription, lifecycle event registration, runtime identity reader, task scheduling и safe teardown.

Конкретные hooks выбираются по приоритету: публичный CommonLib/SKSE event interface, затем стабильный messaging callback, затем минимальный trampoline hook только при отсутствии публичного механизма. Каждый low-level hook требует отдельного compatibility test и documented signature source. Address Library/relocation используется по правилам выбранной CommonLibSSE NG версии; hard-coded addresses запрещены.

Dialogue context не считается подтверждённым, пока прототип не докажет устойчивый start/end signal для всех заявленных runtime targets. Если единого hook нет, profile может объявить capability Degraded или Unsupported; нельзя подменять его периодическим полным scan. Papyrus bridge допускается как ограниченный источник конкретного события, но все его inputs проходят ту же validation boundary.

## 47 Capability discovery и plugin evidence

Capability возникает из пересечения четырёх фактов: реализованный handler, совместимый runtime target, успешная hook/action registration и разрешающее validated Plugin Profile evidence. Любое отрицательное условие даёт Degraded, Unavailable или Unsupported с reason code.

| Причина | State | Пример reason code |
|---|---|---|
| Handler отсутствует в версии adapter | Unsupported | capability.not_implemented |
| Runtime известен, hook временно не зарегистрирован | Unavailable | capability.hook_registration_failed |
| Optional plugin конфликтует | Degraded | capability.plugin_conflict |
| Runtime target не поддержан | Unsupported | runtime.target_unsupported |
| Evidence ещё не получено | Unknown | evidence.pending |
| Все gates пройдены | Available | capability.available |

Plugin Detection поставляет immutable profile с fingerprint. Game Adapter не повторяет filesystem scan и не принимает имя ESP/ESM как достаточное доказательство. При rescan создаётся новый candidate; capability snapshot заменяется атомарно после validation. Потеря необязательной возможности не обнуляет весь adapter, но изменение required capability переводит его в Unavailable.

## 48 Полный каталог результатов и ошибок

Result имеет стабильный machine code и ограниченный detail. Код используется policy и тестами; detail предназначен человеку и не должен содержать secrets или полный пользовательский текст.

| Префикс | Владелец | Типовые коды |
|---|---|---|
| adapter.start | Lifecycle | invalid_state, exception, deadline |
| adapter.profile | Profile validation | game_id_mismatch, schema_invalid, target_unsupported |
| adapter.ingress | Observation path | not_operational, schema_unsupported, duplicate_or_out_of_order, payload_too_large |
| adapter.action | Outbound path | authorization_context_missing, capability_unavailable, capability_unsupported, exception |
| transport | Named pipe | connect_timeout, peer_closed, frame_too_large, malformed_json, protocol_incompatible |
| bridge | Skyrim integration | skse_missing, hook_failed, main_thread_timeout, action_rejected |
| security | Boundary checks | peer_not_allowed, token_invalid, message_type_denied |

InvalidInput означает ошибку caller или wire content. Rejected означает корректный запрос, который нарушает текущую policy/sequence. Unsupported постоянно для данной версии/target. Unavailable может измениться после recovery. Busy допускает безопасный caller retry только с backoff и тем же idempotency key. TimedOut не сообщает, был ли effect выполнен, если protocol не доказал обратное. Failed означает внутреннюю или SDK ошибку.

## 49 Threat model и security controls

Защищаемые активы: целостность процесса Skyrim, World Engine domain state, authorization context, пользовательский контент, локальные пути и стабильность игрового сеанса. Атакующие источники: ошибочный/вредоносный мод, поддельный local client, malformed frame, чрезмерный поток событий, path injection через asset request и невольно высококардинальные diagnostics.

| Угроза | Контроль | Проверка |
|---|---|---|
| Подмена клиента pipe | Per-user ACL, peer/process validation, nonce | Security integration test |
| Oversized/partial frame | Length gate до allocation, bounded decoder | Fuzz/property tests |
| Arbitrary action | Whitelist message/action enums | Unknown-action negative test |
| Replay | session, generation, message ID, sequence | Replay test после reconnect |
| Path traversal | Asset ID registry; paths отсутствуют в wire | Malicious payload test |
| Game-thread DoS | Bounded capture и queues | Load/stress test |
| Secret leakage | Structured redaction и запрещённые labels | Log inspection test |
| Untrusted Papyrus data | Schema/length/type validation | Papyrus boundary test |

Модуль не является security sandbox для Skyrim mods. Он минимизирует доступную поверхность и не добавляет сеть. Любое включение remote transport, scripting или arbitrary file access требует нового ADR и threat-model review.

## 50 Configuration contract

Configuration Provider читает raw configuration, создаёт typed candidate, проверяет constraints и передаёт immutable view. MOD-002 не читает YAML/JSON напрямую. Изменения security, pipe identity, runtime profile и queue geometry требуют controlled restart. Неблокирующие telemetry switches MAY быть hot-reloadable после отдельной candidate validation.

| Поле | Тип | Constraint | Reload |
|---|---|---|---|
| expected_game_id | string | `skyrim-se-ae` в первом релизе | Restart |
| max_payload_bytes | integer | 4096..1048576, общий hard ceiling 1 MiB | Restart |
| inbound_queue_capacity | integer | Positive, измеренный memory budget | Restart |
| outbound_queue_capacity | integer | Positive, reject-before-enqueue | Restart |
| connect_timeout_ms | integer | 100..30000 | Restart |
| operation_timeout_ms | integer | Per action class, bounded | Candidate-dependent |
| recovery_attempt_limit | integer | 0..configured maximum | Hot only via Host policy |
| pipe_name | string | Fixed prefix, no user-supplied path | Restart |
| diagnostics_level | enum | off, error, info, debug | Hot after validation |

Invalid candidate не меняет active configuration. Config snapshot получает version/fingerprint и включается в status evidence без секретных значений. Default значения документируются и тестируются; «0 означает бесконечность» запрещено для timeout и queue capacity.

## 51 Observability и измеряемые бюджеты

Все telemetry signals имеют bounded dimensions. Разрешённые labels: adapter version, profile version, runtime target из ограниченного набора, capability ID, result category и reason code enum. Actor ID, event ID, текст реплики, raw path и exception message не используются как labels.

| Signal | Тип | Назначение |
|---|---|---|
| mod002_state | Gauge/event | Текущий lifecycle state и generation |
| mod002_capability_state | Gauge | State каждого известного capability |
| mod002_ingress_total | Counter | accepted/rejected/dropped by bounded reason |
| mod002_action_total | Counter | result by action class |
| mod002_queue_depth | Gauge | Текущая и high-water mark |
| mod002_operation_duration | Histogram | Connect, discovery, normalization, action round trip |
| mod002_recovery_total | Counter | Attempts и outcomes |
| mod002_frame_bytes | Histogram | Размеры без payload content |

Численные SLO не объявляются до M2.3/M2.4 measurement. Release report обязан зафиксировать p50/p95/p99 capture duration, queue wait, transport round trip, memory delta и CPU delta в повторяемом сценарии. Неизмеренное значение маркируется `measurement pending`, а не нулём. Предварительная guardrail: никакого сетевого ожидания или disk I/O на game hook thread.

## 52 Build system и reproducibility

Portable target использует C++20, SKSE target — C++23. Warnings считаются errors. CMake option `WORLD_ENGINE_BUILD_SKSE_PLUGIN` по умолчанию выключен, чтобы portable tests не зависели от Skyrim SDK. Plugin build требует pinned CommonLibSSE NG integration и documented package lock.

Обязательные build artifacts: portable static library, portable tests, SKSE DLL, symbol/archive policy, validated profile, license notices, release manifest и checksums. Release manifest содержит compiler, Windows SDK, CMake, CommonLibSSE NG, SKSE, git commit, profile fingerprint и supported runtime list.

Reproducibility gate сравнивает clean builds в отдельной директории. Generated files не подменяют исходники. Debug build никогда не публикуется как release package. Подпись binary желательна, но отсутствие code signing не скрывается; hash verification остаётся обязательной.

## 53 Packaging для MO2/Vortex и ручной установки

Release package должен иметь стандартную Skyrim data layout и не включать World Engine secrets или user configuration. Предполагаемая структура подтверждается в M2.8 после реальной сборки.

| Путь пакета | Содержимое | Правило |
|---|---|---|
| SKSE/Plugins/WorldEngineGameAdapter.dll | In-process Bridge | Единственная runtime DLL |
| SKSE/Plugins/WorldEngineGameAdapter.toml | Необязательная plugin-local config | Только безопасные bootstrap параметры |
| WorldEngine/profiles/skyrim-se-ae.profile.json | Versioned profile | Schema-validated |
| docs/ | README, versions, troubleshooting | Без development-only files |

MO2 и Vortex рассматриваются как deployment environments, но не как источники plugin truth. После установки Plugin Detection получает load order собственным утверждённым способом. Installer не изменяет executable, не внедряет network service и не копирует DLL в случайные системные каталоги.

Upgrade выполняет stop World Engine, закрытие Skyrim, backup предыдущего пакета, замену файлов, hash verification и первый запуск в Prototype/validation режиме. Rollback восстанавливает полный согласованный package set; смешивание DLL v2 и profile v1 запрещено.

## 54 Deployment, update и rollback runbook

Перед deployment проверяются чистая установка игры, точная runtime version, соответствующий SKSE, отсутствие конфликтующей DLL и свободный log directory. Затем package устанавливается через mod manager, запускается SKSE loader, а World Engine Host подключается только после plugin bootstrap.

Smoke test проверяет: plugin load, handshake, runtime identity, required capability snapshot, один lifecycle observation, один subtitle action, controlled disconnect и чистый shutdown. Любая ошибка сохраняет status Prototype/Unavailable и блокирует support declaration.

Обновление Bethesda автоматически инвалидирует compatibility claim до повторной проверки. Пользователь получает понятный `runtime.target_unsupported`, а не crash. Rollback применяется, если новая версия plugin не загружается, нарушает handshake, теряет required capability или увеличивает crash/error rate. Save files не модифицируются MOD-002 напрямую.

## 55 Конкретный test catalog

Каждый test record содержит ID, build commit, environment, preconditions, steps, expected result, actual result и evidence link. Ниже минимальный каталог; новые tests добавляются без удаления обязательных IDs.

| ID | Область | Проверка | Gate |
|---|---|---|---|
| T-001 | Lifecycle | Created -> Initializing -> Ready | Unit |
| T-002 | Lifecycle | Optional limitation -> Degraded | Unit |
| T-003 | Lifecycle | Missing required -> Unavailable | Unit |
| T-004 | Lifecycle | start из Ready отклоняется | Unit |
| T-005 | Lifecycle | stop idempotent после Stopped | Unit |
| T-006 | Recovery | Recover увеличивает generation | Unit |
| T-007 | Recovery | Старый callback после recover отклоняется | Concurrency |
| T-008 | Recovery | Retry budget исчерпан | Host integration |
| T-009 | Profile | Неверный game_id отклоняется | Unit |
| T-010 | Profile | Unknown field отклоняется schema | Schema |
| T-011 | Profile | Duplicate capability отклоняется | Schema |
| T-012 | Profile | Required/optional overlap отклоняется | Semantic |
| T-013 | Profile | Supported без validated targets запрещён | Release |
| T-014 | Runtime | Unknown runtime -> Unsupported | Integration |
| T-015 | Runtime | Missing SKSE -> Unavailable | In-game |
| T-016 | Ingress | Valid observation достигает ingress | Unit |
| T-017 | Ingress | Sequence 0 отклоняется | Unit |
| T-018 | Ingress | Duplicate/out-of-order отклоняется | Unit |
| T-019 | Ingress | Неверная schema отклоняется | Unit |
| T-020 | Ingress | Oversized payload отклоняется до parsing | Security |
| T-021 | Ingress | game_id mismatch отклоняется | Unit |
| T-022 | Ingress | Critical overflow создаёт evidence | Stress |
| T-023 | Ingress | Coalescing сохраняет последнее значение | Unit |
| T-024 | Action | Valid subtitle вызывается один раз | Contract |
| T-025 | Action | Missing token отклоняется | Security |
| T-026 | Action | Unsupported capability typed result | Unit |
| T-027 | Action | Unknown action не достигает Bridge | Security |
| T-028 | Action | Expired deadline не enqueue | Unit |
| T-029 | Action | Повтор idempotent action deduplicated | Contract |
| T-030 | Action | Non-idempotent timeout не retry | Contract |
| T-031 | Transport | Partial header/body собирается | Unit |
| T-032 | Transport | Несколько frames в одном read | Unit |
| T-033 | Transport | Frame > hard limit закрывается | Fuzz |
| T-034 | Transport | Malformed UTF-8/JSON отклоняется | Fuzz |
| T-035 | Transport | Protocol range mismatch | Integration |
| T-036 | Transport | Peer close завершает pending | Integration |
| T-037 | Security | Недопустимый pipe peer отклоняется | Security |
| T-038 | Security | Replay старой session отклоняется | Security |
| T-039 | Security | Path traversal payload не исполняется | Security |
| T-040 | Security | Logs не содержат token/text/path | Inspection |
| T-041 | Threading | Hook callback не ждёт transport | Performance |
| T-042 | Threading | Shutdown race не вызывает use-after-free | Stress |
| T-043 | Threading | Callback не вызывается под adapter lock | Unit |
| T-044 | SKSE | Plugin bootstrap на Steam AE | In-game |
| T-045 | SKSE | Plugin bootstrap на Steam SE 1.5.97 | In-game |
| T-046 | SKSE | Plugin bootstrap на GOG AE | In-game |
| T-047 | SKSE | Lifecycle hooks регистрируются/снимаются | In-game |
| T-048 | SKSE | Dialogue start/end semantics | In-game |
| T-049 | SKSE | Subtitle show/clear | In-game |
| T-050 | SKSE | Main-thread scheduling deadline | In-game |
| T-051 | Plugin evidence | Нет повторного filesystem scan | Architecture |
| T-052 | Plugin evidence | Atomic profile snapshot replacement | Integration |
| T-053 | Save boundary | MOD-002 не решает restore compatibility | Architecture |
| T-054 | Packaging | Clean MO2 install/uninstall | Deployment |
| T-055 | Packaging | Clean Vortex install/uninstall | Deployment |
| T-056 | Upgrade | Mixed-version package отклоняется | Deployment |
| T-057 | Rollback | Предыдущий package восстанавливается | Deployment |
| T-058 | Performance | Capture/queue/RTT baselines записаны | Release |
| T-059 | Stability | 2-hour dialogue/location stress session | Release |
| T-060 | Shutdown | Нет hooks/handles после exit | Release |

## 56 Compatibility matrix procedure

Каждый runtime target проверяется отдельно на чистом профиле и на минимальном representative mod profile. Record фиксирует hash executable, Skyrim runtime, SKSE, CommonLibSSE build, mod manager, Windows version, plugin package hash и test IDs T-044–T-060.

Target может получить `validated` только если plugin загружается, required capabilities Available, smoke/integration/security tests проходят и нет необъяснённого crash. Ошибка одного target не аннулирует доказательства другого, но общий profile status остаётся ограниченным до явного перечня supported targets.

Skyrim VR не появляется в test matrix первого релиза. Его исключение является scope control, а не скрытым обещанием совместимости. Любая будущая VR работа требует отдельного architecture delta, build target и tests.

## 57 Операционное руководство и диагностика

Порядок диагностики всегда начинается с status evidence, а не с переустановки всех модов. Оператор фиксирует runtime target, SKSE version, adapter/profile version, lifecycle state и первый стабильный reason code.

| Симптом | Вероятная причина | Безопасное действие |
|---|---|---|
| Plugin не загружается | SKSE/runtime mismatch или missing dependency | Сверить точные версии и release manifest |
| Host не подключается | Pipe не создан, ACL или startup order | Проверить bridge status и per-user context |
| Adapter Unavailable | Required capability отсутствует | Читать capability evidence; не force-enable |
| Subtitle не появляется | UI state, handler отсутствует, timeout | Проверить action result и in-game smoke test |
| События дублируются | Неверная session/sequence handling | Сохранить bounded trace, перезапустить generation |
| Очередь переполняется | Event storm или consumer stall | Перейти в Degraded, собрать queue metrics |
| После обновления игры отказ | Новый runtime не validated | Откатить игру/plugin либо ждать validated target |

Логи для issue report редактируются: удаляются пользовательские пути, токены и тексты диалогов. Crash dump передаётся только с согласием пользователя и по отдельной privacy policy. «Исправление» отключением validation или ACL не допускается.

## 58 Implementation backlog и gates

| Gate | Deliverable | Exit evidence | Состояние |
|---|---|---|---|
| M2.1 | Architecture, contracts, prototype core | ADR-009, source, static QA | Выполнен в ветке |
| M2.2 | Portable build/test | Clean CMake build и test report | Ожидает Windows C++ toolchain |
| M2.3 | Named pipe protocol | Decoder, ACL, transport tests | Planned |
| M2.4 | SKSE event bridge | Runtime identity и lifecycle hooks | Scaffold only |
| M2.5 | Subtitle action | Handler, authorization, in-game test | Planned |
| M2.6 | Plugin evidence mapping | Immutable profile integration | Planned |
| M2.7 | Compatibility matrix | Три validated target records | Planned |
| M2.8 | Release package | MO2/Vortex package, rollback, manifest | Planned |

Работа выполняется вертикальными slices: сначала identity/handshake, затем lifecycle observation, затем subtitle action, потом dialogue context. Каждый slice включает contract, implementation, negative tests, metrics и documentation. Разработка последующего slice не меняет support status предыдущего без evidence.

## 59 Трассируемость требований к реализации

| Требование | Компонент | Проверка | Архитектурный источник |
|---|---|---|---|
| Единственная Skyrim boundary | Game Adapter/Bridge | Dependency scan | ARCH-002, ARCH-005 |
| SDK types изолированы | Skyrim Bridge | Include scan/build | ADR-009 |
| Host владеет readiness | Status sink | Lifecycle tests | ARCH-008, ADR-008 |
| Plugin Detection владеет discovery | Evidence mapper | Architecture test T-051 | ARCH-010, ADR-006 |
| Save Manager владеет restore | Только evidence contract | Architecture test T-053 | ARCH-011, ADR-005 |
| Нет Event Engine RPC | Ingress/Application coordination | Contract review | ADR-002 |
| Explicit unsupported | Capability Registry | T-026 | ADR-007 |
| Candidate config atomic | Configuration Provider boundary | Config tests | ARCH-009 |
| Bounded queues/deadlines | Transport/Processors | Stress and timeout tests | ARCH-004 |
| SE/AE, без VR | Profile/build flags | Schema/build inspection | ADR-009 |

## 60 Глоссарий, ограничения и история v2.0

| Термин | Определение |
|---|---|
| Adapter | Portable orchestration и validation часть MOD-002 |
| Bridge | Skyrim-process часть с SKSE/CommonLibSSE |
| Capability | Явно обнаруженная и проверенная интеграционная возможность |
| Evidence | Факт ограниченной области, из которого владелец policy делает вывод |
| Game Profile | Versioned описание runtime targets, capabilities и ограничений |
| Generation | Попытка initialization/recovery, изолирующая устаревшие результаты |
| Observation | Нормализуемый входной факт Skyrim |
| Action | Авторизованная просьба выполнить whitelisted game operation |
| Supported target | Точная комбинация runtime/SKSE/package с пройденными gates |
| Prototype | Реализация и дизайн в разработке без support claim |

Оставшиеся ограничения: нет подтверждённой portable сборки в текущей среде, named pipe не реализован, event hooks не реализованы, subtitle handler не реализован, численные performance budgets не измерены, runtime targets не прошли in-game validation. Эти ограничения намеренно видимы на обложке, в status matrix и acceptance gates.

| Версия | Дата | Изменение |
|---|---|---|
| 2.0 | 2026-09-05 | Базовая спецификация расширена полными runtime, protocol, security, testing, deployment и operations contracts; добавлены фактические source appendices |
