# STD-001 World Engine Glossary

Версия 1.0 · Русский · 2026-09-05 · Current normative standard

## 1. Назначение

STD-001 устанавливает единый словарь World Engine для архитектуры, документации, кода, тестов и эксплуатации. Его задача — исключить ситуацию, когда одинаковое слово обозначает разные вещи либо разные слова ошибочно принимаются за разные сущности.

Канонический технический token приводится на английском и остаётся стабильным в RU и DE документах. Перевод объясняет смысл читателю, но не меняет имя типа, события, API, поля, метрики или конфигурационного ключа.

## 2. Нормативность и приоритет

Определения STD-001 обязательны для новых документов и новой реализации. Если ранний документ использует термин иначе, действует смысл более конкретного принятого ARCH, ADR или модульного контракта; расхождение регистрируется и устраняется контролируемой редакцией. Глоссарий не может незаметно изменить уже принятое архитектурное решение.

| Уровень | Источник | Правило при конфликте |
|---|---|---|
| 1 | Accepted ADR | Управляет самим решением и его мотивом |
| 2 | Current ARCH | Управляет системной границей и зависимостями |
| 3 | Current module specification | Управляет контрактом конкретного модуля |
| 4 | STD-001 | Управляет общим значением и переводом термина |
| 5 | Локальная заметка или draft | Не меняет канонический смысл |

## 3. Языковая модель

RU и DE версии должны ссылаться на один английский canonical token. В обычной прозе допустим естественный перевод; при первом важном употреблении указывается token. В коде, схемах, логах, events и конфигурации используется только утверждённая английская форма.

| Элемент | RU документ | DE документ | Код и данные |
|---|---|---|---|
| Концепция | Перевод плюс canonical token при первом употреблении | Übersetzung plus canonical token bei erster Verwendung | Canonical English token |
| Идентификатор | Не переводится | Wird nicht übersetzt | Exact identifier |
| Статус | Английский статус можно пояснить по-русски | Englischer Status kann deutsch erklärt werden | Exact status value |
| Числовой предел | Одинаковое значение и единица | Gleicher Wert und gleiche Einheit | Machine-readable unit suffix |

## 4. Статусы документа и реализации

| Canonical token | Русский смысл | Немецкий эквивалент | Нормативное определение |
|---|---|---|---|
| Planned | Запланировано | Geplant | Артефакт предусмотрен, но канонической версии нет |
| Draft | Черновик | Entwurf | Содержание существует, но все gates ещё не пройдены |
| Design Baseline | Проектная база | Entwurfsbaseline | Контракты достаточны для начала реализации после разрешения |
| Accepted | Принято | Angenommen | Решение формально принято уполномоченным владельцем |
| Complete | Документ завершён | Dokument vollständig | Документ самодостаточен и прошёл обязательную QA |
| Not Started | Не начато | Nicht begonnen | Реализация или проверка отсутствует |
| Prototype | Прототип | Prototyp | Исследовательская реализация без заявленной production readiness |
| Implemented | Реализовано | Implementiert | Код существует, собирается и соответствует заявленному baseline |
| Verified | Проверено | Verifiziert | Поведение доказано определёнными тестами и сохранёнными результатами |
| Operational | Эксплуатационно готово | Betriebsbereit | Deployment, monitoring, recovery и операционная пригодность доказаны |
| Current | Действующее | Aktuell | Версия управляет текущей работой |
| Superseded | Заменено | Ersetzt | Версия сохранена для истории, но больше не управляет работой |
| Living | Ведущийся документ | Fortgeschrieben | Документ регулярно обновляется по установленному cadence |

**Запрещённое сокращение смысла:** слова «готово», «закрыто» и «завершено» без указания объекта и статуса. Следует писать: «спецификация Complete; implementation Not Started» или «implementation Verified в среде Skyrim SE 1.6.x».

## 5. Документация и управление

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Canonical Document | Канонический документ | Kanonisches Dokument | Единственная действующая версия, явно указанная в canonical index |
| Canonical Index | Канонический индекс | Kanonischer Index | Реестр implementation-facing документов и их текущих версий |
| Control Artifact | Контрольный артефакт | Kontrollartefakt | План, QA report или register, управляющий доказательствами, но не входящий в delivery denominator |
| Delivery Artifact | Поставляемый артефакт | Lieferartefakt | Одна учитываемая позиция Master Plan, обычно двуязычная документная пара |
| Source Document | Исходный документ | Quelldokument | Редактируемая смысловая основа, из которой создаётся публикуемая версия |
| Reference Document | Документ-образец | Referenzdokument | Неизменяемый источник визуальной или структурной системы |
| Historical Snapshot | Исторический снимок | Historischer Snapshot | Сохранённое состояние заменённой версии |
| Traceability | Прослеживаемость | Rückverfolgbarkeit | Связь требования с источником, реализацией, тестом и результатом |
| Coverage Register | Реестр покрытия | Abdeckungsregister | Список источников и доказательство их включения или контролируемого исключения |
| Change Log | Журнал изменений | Änderungsjournal | Версионная запись фактических смысловых изменений |
| Gate | Контрольный допуск | Freigabestufe | Формальная точка решения, блокирующая следующий класс работы |
| Definition of Done | Критерий завершения | Fertigstellungsdefinition | Набор обязательных доказательств завершённости определённого результата |

## 6. Система и архитектура

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| World Engine | World Engine | World Engine | Модульная система симуляции живого мира, интегрируемая с играми через adapters |
| System Boundary | Граница системы | Systemgrenze | Явное разделение принадлежащего World Engine и внешней среды |
| Architecture | Архитектура | Architektur | Структура владельцев, границ, зависимостей и обязательных свойств системы |
| Layer | Слой | Schicht | Группа компонентов с одинаковой ответственностью и допустимым направлением зависимостей |
| Core Platform | Базовая платформа | Kernplattform | Инфраструктура lifecycle, module registry, events, scheduling и общих contracts |
| Simulation Layer | Слой симуляции | Simulationsschicht | Game-agnostic доменная логика памяти, персонажей, отношений и мира |
| Game Integration Layer | Слой интеграции с игрой | Spielintegrationsschicht | Граница между игровым runtime и внутренними contracts World Engine |
| AI Integration Layer | Слой AI-интеграции | AI-Integrationsschicht | Provider-neutral orchestration AI requests, context и fallbacks |
| Infrastructure | Инфраструктура | Infrastruktur | Технические реализации storage, filesystem, transport, process и provider access |
| Dependency Direction | Направление зависимости | Abhängigkeitsrichtung | Разрешённое статическое отношение между слоями и владельцами contracts |
| Static Dependency | Статическая зависимость | Statische Abhängigkeit | Compile-time или package-level ссылка одного компонента на другой |
| Runtime Interaction | Runtime-взаимодействие | Laufzeitinteraktion | Обмен сообщениями или вызовами во время выполнения без изменения ownership |
| Inversion of Control | Инверсия управления | Steuerungsumkehr | Зависимость на contract, принадлежащий потребляющей стороне, с внешней реализацией |

## 7. Модули и компоненты

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Module | Модуль | Modul | Версионируемая единица ответственности с lifecycle, contracts и владельцем |
| Component | Компонент | Komponente | Внутренняя часть модуля с конкретной технической ролью |
| Module Owner | Владелец модуля | Modulverantwortlicher | Роль, отвечающая за specification, conformance и evidence модуля |
| Module Registry | Реестр модулей | Modulregister | Core service, принимающий descriptors и управляющий разрешённым lifecycle |
| Module Descriptor | Дескриптор модуля | Moduldeskriptor | Метаданные ID, version, capabilities, dependencies и startup requirements |
| Lifecycle | Жизненный цикл | Lebenszyklus | Определённая последовательность регистрации, запуска, работы, остановки и failure |
| Capability | Возможность | Fähigkeit | Явно объявленная функция, которую consumer может проверить до использования |
| Contract | Контракт | Vertrag | Версионируемое обязательство о данных, поведении, ошибках и совместимости |
| Port | Порт | Port | Узкий interface на границе use case, принадлежащий стороне, которой он нужен |
| Adapter | Адаптер | Adapter | Реализация, преобразующая внешний protocol или runtime в внутренний contract |
| Repository | Репозиторий данных | Repository | Узкий persistence interface для domain/application use cases |
| Service | Сервис | Dienst | Поведенческий компонент; название требует уточнения роли и boundary |
| Use Case | Сценарий применения | Anwendungsfall | Одна оркестрируемая цель пользователя, системы или модуля |

## 8. События и сообщения

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Event | Событие | Ereignis | Неизменяемый факт о том, что уже произошло |
| Domain Event | Доменное событие | Domänenereignis | Факт, значимый для game-agnostic domain logic |
| Integration Event | Интеграционное событие | Integrationsereignis | Версионируемое сообщение для пересечения module boundary |
| Command | Команда | Befehl | Запрос выполнить действие; может быть отклонён |
| Query | Запрос чтения | Abfrage | Запрос данных без намерения изменить domain state |
| Result | Результат | Ergebnis | Типизированный исход command, query или operation |
| Event Envelope | Конверт события | Ereignisumschlag | Метаданные identity, version, time, correlation, causation и producer |
| Producer | Производитель | Erzeuger | Модуль, подтвердивший и опубликовавший факт |
| Consumer | Потребитель | Verbraucher | Компонент, подписанный на contract и обрабатывающий сообщение |
| Correlation ID | Идентификатор корреляции | Korrelationskennung | ID одной логической операции через несколько компонентов |
| Causation ID | Идентификатор причины | Ursachenkennung | ID непосредственного сообщения или действия, вызвавшего текущее |
| Idempotency Key | Ключ идемпотентности | Idempotenzschlüssel | Стабильный ключ, позволяющий распознать повтор одной операции |
| At-least-once Delivery | Доставка минимум один раз | Mindestens-einmal-Zustellung | Сообщение может быть доставлено повторно и требует идемпотентного consumer |
| Dead Letter | Необработанное сообщение | Unzustellbare Nachricht | Сообщение, изолированное после исчерпания разрешённых попыток |

## 9. Данные и persistence

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Domain State | Доменное состояние | Domänenzustand | Состояние с игровым смыслом, принадлежащее domain module |
| Durable State | Долговременное состояние | Dauerhafter Zustand | Данные, обязанные переживать process restart |
| Transient State | Временное состояние | Flüchtiger Zustand | Восстанавливаемые или одноразовые runtime-данные |
| Entity | Сущность | Entität | Объект с устойчивой identity во времени |
| Value Object | Объект-значение | Wertobjekt | Неизменяемое значение, определяемое содержимым, а не identity |
| Aggregate | Агрегат | Aggregat | Consistency boundary с одним управляющим root |
| Schema | Схема | Schema | Версионируемая структура данных и invariants |
| Logical Schema | Логическая схема | Logisches Schema | Модель данных без привязки к физической layout конкретного engine |
| Physical Schema | Физическая схема | Physisches Schema | Таблицы, columns, indexes и constraints конкретного storage |
| Migration | Миграция | Migration | Контролируемое преобразование между schema versions |
| Transaction | Транзакция | Transaktion | Atomic unit persistence changes с определённой isolation boundary |
| Repository Contract | Контракт репозитория | Repository-Vertrag | Domain-facing операции persistence без утечки SQL или connection details |
| Outbox | Исходящий журнал | Outbox | Transactionally stored records для надёжной последующей публикации events |
| Snapshot | Снимок состояния | Zustandsabbild | Версионированное представление state в определённой точке |
| Backup | Резервная копия | Sicherung | Независимая восстанавливаемая копия durable data |

## 10. Save и restore

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Save Manager | Менеджер сохранения | Speicherstandsmanager | MOD-013 coordinator, управляющий общим save/restore protocol |
| Save Participant | Участник сохранения | Speicherteilnehmer | Модульный contract подготовки, проверки и активации своего snapshot |
| Checkpoint | Контрольная точка | Prüfpunkt | Координированный набор совместимых participant snapshots |
| Save Lineage | Линия сохранения | Speicherstandslinie | Устойчивая ветвь происхождения сохранённого мира |
| Stage | Подготовительная стадия | Vorbereitungsstufe | Создание non-live candidate без изменения активного state |
| Validate | Проверка кандидата | Kandidatenprüfung | Детерминированная проверка структуры и cross-participant invariants |
| Commit | Фиксация | Festschreibung | Необратимый в рамках операции переход подготовленного изменения в принятое |
| Atomic Restore | Атомарное восстановление | Atomare Wiederherstellung | Одновременная логическая активация всех проверенных participant states |
| Rollback | Откат операции | Rücknahme | Возврат к последнему доказанному состоянию после неуспешной попытки |
| Recovery | Восстановление после сбоя | Wiederanlauf | Процедура возвращения системы к корректному состоянию после failure |
| Compatibility Fixture | Фикстура совместимости | Kompatibilitätsfixture | Сохранённый тестовый образец старой версии для проверки загрузки и migration |

## 11. Конфигурация и deployment

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Configuration | Конфигурация | Konfiguration | Версионируемые входные значения, изменяющие разрешённое поведение |
| Configuration Schema | Схема конфигурации | Konfigurationsschema | Типы, defaults, ranges, dependencies и validation rules настроек |
| Profile | Профиль | Profil | Именованный набор конфигурации для конкретной среды или режима |
| Safe Default | Безопасное значение по умолчанию | Sicherer Standardwert | Значение, минимизирующее повреждение данных и неожиданное расширение scope |
| Secret | Секрет | Geheimnis | Credential или key, запрещённый в документах, логах и обычной конфигурации |
| Deployment | Развёртывание | Bereitstellung | Установка проверенного build и его конфигурации в целевую среду |
| Environment | Среда | Umgebung | Определённая комбинация OS, game runtime, dependencies, config и data |
| Build Artifact | Артефакт сборки | Build-Artefakt | Версионированный результат build pipeline |
| Feature Flag | Флаг функции | Funktionsschalter | Контролируемый переключатель поведения с owner, default и removal plan |
| Health Check | Проверка работоспособности | Zustandsprüfung | Ограниченная диагностика readiness или liveness без изменения domain state |
| Degraded Mode | Ограниченный режим | Eingeschränkter Modus | Явно описанный режим с сохранёнными safety guarantees и уменьшенной функцией |

## 12. Игра и адаптер

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Game Adapter | Игровой адаптер | Spieladapter | MOD-002 boundary между Skyrim runtime и World Engine contracts |
| Game Runtime | Исполняемая среда игры | Spiel-Laufzeit | Запущенная игра, её APIs, hooks и process constraints |
| Skyrim SE | Skyrim Special Edition | Skyrim Special Edition | Поддерживаемая ветвь Skyrim в текущем scope |
| Skyrim AE | Skyrim Anniversary Edition | Skyrim Anniversary Edition | Поддерживаемая distribution/runtime ветвь в текущем scope |
| Skyrim VR | Skyrim VR | Skyrim VR | Отдельная платформа, явно исключённая до нового ADR и tests |
| SKSE | Skyrim Script Extender | Skyrim Script Extender | Внешний runtime extension; не является внутренним World Engine API |
| Plugin | Игровой плагин | Spiel-Plugin | ESP, ESM, ESL или иной game-recognized content artifact |
| Native Plugin | Нативный плагин | Natives Plugin | Runtime binary, загружаемый через разрешённый Skyrim extension mechanism |
| Load Order | Порядок загрузки | Ladereihenfolge | Определённая последовательность game plugins, влияющая на resolved records |
| Form ID | Идентификатор формы | Form-ID | Skyrim-specific identity, требующая нормализации и load-order context |
| Game Event | Игровое событие | Spielereignis | Raw runtime signal, ещё не обязательно являющийся validated domain event |
| Normalized Event | Нормализованное событие | Normalisiertes Ereignis | Проверенное game-agnostic сообщение после adapter mapping |
| Capability Detection | Определение возможностей | Fähigkeitserkennung | Runtime-проверка доступных hooks, versions и optional features |

## 13. Memory и симуляция

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Memory Engine | Движок памяти | Memory Engine | MOD-003, владелец значения, проверки, хранения и retrieval памяти NPC |
| Memory Record | Запись памяти | Memory-Datensatz | Durable validated fact с identity, provenance, time и retention metadata |
| Memory Candidate | Кандидат памяти | Memory-Kandidat | Непроверенное предложение, которое не является durable truth |
| Subject | Субъект памяти | Memory-Subjekt | Entity, чья память или точка зрения представлена |
| Provenance | Происхождение | Herkunft | Проверяемая информация о source, producer и пути появления данных |
| Authority | Авторитет источника | Quellenautorität | Класс доверия, определяющий допустимость и конфликтное разрешение |
| Confidence | Уверенность | Konfidenz | Ограниченная оценка неопределённости, не заменяющая authority |
| Salience | Значимость | Bedeutsamkeit | Детерминированно вычисляемая полезность записи для конкретного purpose |
| Retention | Удержание | Aufbewahrung | Политика срока, архивации, удаления или сохранения записи |
| Visibility | Видимость | Sichtbarkeit | Правило, кто и для какой цели может получить memory content |
| Conflict Group | Группа конфликта | Konfliktgruppe | Набор несовместимых claims, сохраняемых без ложного слияния |
| Supersession | Замещение записи | Ablösung | Явная связь новой записи с заменяемой без физического переписывания истории |
| Retrieval | Извлечение памяти | Abruf | Ограниченный query path, возвращающий релевантные разрешённые records |
| Context Projection | Проекция контекста | Kontextprojektion | Bounded представление memory data для конкретного consumer |

## 14. AI, dialogue и voice

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| AI Provider | AI-провайдер | AI-Anbieter | Внешняя или локальная реализация model inference behind a port |
| Local AI | Локальный AI | Lokale AI | Model runtime на контролируемом пользователем устройстве |
| LLM | Большая языковая модель | Großes Sprachmodell | Probabilistic text model; не является source of truth |
| Prompt | Инструкция модели | Modellanweisung | Версионируемый input template с owner и injection boundary |
| Context | Контекст модели | Modellkontext | Bounded набор проверенных данных, передаваемый для одного inference |
| Context Manager | Менеджер контекста | Kontextmanager | Компонент, управляющий budget, ordering, provenance и truncation |
| AI Candidate | AI-кандидат | AI-Kandidat | Непроверенный результат AI, требующий deterministic validation |
| Deterministic Fallback | Детерминированный fallback | Deterministische Rückfalllogik | Не-AI результат с предсказуемым поведением при failure или timeout |
| Dialogue Engine | Диалоговый движок | Dialog-Engine | MOD-005, владелец dialogue orchestration, не memory persistence |
| Voice Engine | Голосовой движок | Sprach-Engine | MOD-014, владелец TTS/STT integration и audio lifecycle |
| Hallucination | Недостоверная генерация | Halluzination | Output без достаточной опоры на разрешённые данные; не может стать truth напрямую |
| Prompt Injection | Инъекция инструкции | Prompt-Injektion | Попытка недоверенного content изменить policy или authority модели |

## 15. Тестирование и доказательства

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Test Case | Тестовый случай | Testfall | Идентифицированный input, preconditions, action и expected result |
| Unit Test | Модульный тест | Unit-Test | Изолированная проверка малой единицы поведения |
| Integration Test | Интеграционный тест | Integrationstest | Проверка contract между двумя или более реальными boundaries |
| Acceptance Test | Приёмочный тест | Abnahmetest | Проверка пользовательского или системного результата против requirement |
| Regression Test | Регрессионный тест | Regressionstest | Защита ранее доказанного поведения от непреднамеренного изменения |
| Fixture | Фикстура | Fixture | Версионированный воспроизводимый input или environment state для теста |
| Test Harness | Тестовый стенд | Test-Harness | Инструменты запуска, наблюдения и контроля тестового сценария |
| Golden Corpus | Эталонный корпус | Referenzkorpus | Стабильный набор примеров и expected judgments для retrieval или AI evaluation |
| Coverage | Покрытие | Abdeckung | Измеренная связь tests с code paths или requirements; не синоним качества |
| Evidence | Доказательство | Nachweis | Сохраняемый результат, позволяющий независимо подтвердить claim |
| Pass | Пройдено | Bestanden | Наблюдаемый результат соответствует заранее определённому expectation |
| Waiver | Временное исключение | Ausnahmegenehmigung | Ограниченное принятие незакрытого условия с owner, reason и expiry |
| Blocker | Блокирующая проблема | Blocker | Условие, запрещающее переход gate до устранения или допустимого решения |

## 16. Безопасность и privacy

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Trust Boundary | Граница доверия | Vertrauensgrenze | Место, где данные или управление переходят между разными уровнями доверия |
| Untrusted Input | Недоверенный ввод | Nicht vertrauenswürdige Eingabe | Любые внешние данные до validation и authorization |
| Validation | Валидация | Validierung | Проверка формы, диапазона, инвариантов и разрешённости до использования |
| Authorization | Разрешение действия | Autorisierung | Проверка права выполнить операцию над определённым scope |
| Authentication | Подтверждение субъекта | Authentifizierung | Проверка заявленной identity, если она применяется |
| Least Privilege | Минимальные полномочия | Minimale Rechte | Выдача только необходимых capabilities на необходимое время |
| Sensitive Data | Чувствительные данные | Schutzbedürftige Daten | Данные, требующие ограниченного хранения, отображения или передачи |
| Credential | Учётный секрет | Zugangsdaten | Token, password или key для доступа к защищённому ресурсу |
| Redaction | Сокрытие данных | Schwärzung | Контролируемое удаление sensitive content из представления или evidence |
| Audit Trail | Аудиторский след | Prüfpfad | Неизменяемая последовательность значимых действий и решений |
| Data Retention Policy | Политика хранения | Datenaufbewahrungsrichtlinie | Правила срока, deletion, archive и legal/project constraints |

## 17. Ошибки, наблюдаемость и эксплуатация

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Error | Ошибка результата | Fehler | Ожидаемый типизированный отказ operation |
| Fault | Дефект выполнения | Störung | Неправильное внутреннее состояние или нарушение invariant |
| Failure | Наблюдаемый сбой | Ausfall | Неспособность предоставить обещанное поведение |
| Exception | Исключение runtime | Ausnahme | Механизм передачи abnormal control flow; не taxonomy domain errors |
| Error Code | Код ошибки | Fehlercode | Стабильный machine-readable identifier failure class |
| Retryable | Допускает повтор | Wiederholbar | Операция может быть безопасно повторена при определённых условиях |
| Timeout | Превышение времени | Zeitüberschreitung | Operation не завершилась в установленный budget; итог может требовать reconciliation |
| Log | Журнал выполнения | Laufzeitprotokoll | Структурированная запись диагностического факта |
| Metric | Метрика | Metrik | Числовой time-series signal с единицей и semantics |
| Trace | Трасса | Ablaufspur | Связанный путь одной операции через components |
| Observability | Наблюдаемость | Beobachtbarkeit | Способность объяснить внутреннее состояние по logs, metrics и traces |
| Runbook | Эксплуатационная инструкция | Betriebsanleitung | Проверенная последовательность диагностики, recovery и escalation |
| Incident | Инцидент | Vorfall | Сбой или риск, требующий управляемой response и lessons learned |

## 18. Версии, совместимость и релиз

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Version | Версия | Version | Идентификатор состояния contract, document или artifact |
| Semantic Versioning | Семантическое версионирование | Semantische Versionierung | MAJOR.MINOR.PATCH с объявленными compatibility semantics |
| Contract Version | Версия контракта | Vertragsversion | Версия public schema или behavior независимо от package build |
| Schema Version | Версия схемы | Schemaversion | Версия serialised или persisted structure |
| Backward Compatible | Обратно совместимо | Rückwärtskompatibel | Новый consumer или system продолжает принимать разрешённые старые inputs |
| Forward Compatible | Прямо совместимо | Vorwärtskompatibel | Старый consumer корректно обрабатывает или отвергает расширенный новый input |
| Breaking Change | Несовместимое изменение | Inkompatible Änderung | Изменение, нарушающее ранее обещанный contract |
| Deprecation | Объявление устаревания | Abkündigung | Период контролируемого вывода contract до removal |
| Release | Релиз | Veröffentlichung | Идентифицированный проверенный набор artifacts для определённого scope |
| Release Candidate | Кандидат релиза | Veröffentlichungskandidat | Immutable build, проходящий финальные gates |
| Rollback Plan | План отката | Rücknahmeplan | Проверенная процедура возврата к предыдущему release |

## 19. Git и рабочий процесс

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Repository | Git-репозиторий | Git-Repository | Версионируемая история проекта; не путать с data repository contract |
| Branch | Ветка | Zweig | Именованный указатель на последовательность commits |
| Worktree | Рабочее дерево | Arbeitsbaum | Отдельный checkout одной ветки, связанный с общим repository |
| Commit | Коммит | Commit | Atomic version-control record с одним объяснимым intent |
| Merge | Слияние | Zusammenführung | Интеграция проверенной истории одной ветки в другую |
| Rebase | Перенос истории | Umbasierung | Переписывание commit base; запрещено для опубликованной общей истории без правила |
| Pull Request | Запрос интеграции | Integrationsanfrage | Review boundary для изменений, evidence и решения merge |
| Main Branch | Основная ветка | Hauptzweig | Защищённая интеграционная история; не место незавершённой работы |
| Audit Branch | Аудиторская ветка | Auditzweig | Изолированная ветка для проверки и исправления документов или architecture |
| Feature Branch | Ветка изменения | Änderungszweig | Ограниченная ветка одной реализации или документационного пакета |
| Dirty Worktree | Рабочее дерево с изменениями | Geänderter Arbeitsbaum | Checkout с незакоммиченными tracked или untracked files |

## 20. Риски и решения

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Risk | Риск | Risiko | Неопределённое событие, способное изменить цель, срок, качество или безопасность |
| Issue | Текущая проблема | Aktuelles Problem | Уже возникшее состояние, требующее действия |
| Assumption | Допущение | Annahme | Условие, считающееся истинным до проверки |
| Constraint | Ограничение | Randbedingung | Условие, которое решение обязано соблюдать |
| Probability | Вероятность | Wahrscheinlichkeit | Оценка возможности наступления риска |
| Impact | Влияние | Auswirkung | Оценка последствий риска |
| Mitigation | Снижение риска | Risikominderung | Действие, уменьшающее probability или impact |
| Contingency | План на случай риска | Notfallmaßnahme | Действие после наступления trigger |
| Residual Risk | Остаточный риск | Restrisiko | Риск, оставшийся после controls |
| Decision Record | Запись решения | Entscheidungsprotokoll | Сохраняемый контекст, варианты, решение и последствия |
| Open Decision | Открытое решение | Offene Entscheidung | Вопрос с owner и deadline, ещё не меняющий baseline |

## 21. Время, порядок и конкурентность

| Canonical token | Русский смысл | Немецкий эквивалент | Определение |
|---|---|---|---|
| Instant UTC | Момент UTC | UTC-Zeitpunkt | Абсолютное machine timestamp в UTC |
| Domain Time | Доменное время | Domänenzeit | Игровая или simulation time шкала с собственными правилами |
| Monotonic Time | Монотонное время | Monotone Zeit | Источник duration, не уменьшающийся при изменении wall clock |
| Tick | Такт | Takt | Дискретный scheduler step; размер и semantics должны быть объявлены |
| Sequence Number | Порядковый номер | Sequenznummer | Монотонный номер внутри явно указанного scope |
| Deterministic Order | Детерминированный порядок | Deterministische Reihenfolge | Одинаковый порядок при одинаковых inputs и state |
| Concurrency | Конкурентное выполнение | Nebenläufigkeit | Перекрывающиеся операции, требующие явной consistency policy |
| Race Condition | Состояние гонки | Race Condition | Результат зависит от неконтролируемого interleaving |
| Cancellation | Отмена | Abbruch | Кооперативное прекращение operation с определённым outcome |
| Deadline | Предельный момент | Fristzeitpunkt | Абсолютная граница завершения operation |
| Duration | Длительность | Dauer | Интервал с обязательной единицей измерения |

## 22. Аббревиатуры

| Token | Полная форма | Значение в проекте |
|---|---|---|
| ADR | Architecture Decision Record | Запись архитектурного решения |
| API | Application Programming Interface | Версионируемая программная граница |
| AI | Artificial Intelligence | Общий класс AI integrations |
| LLM | Large Language Model | Языковая модель probabilistic generation |
| NPC | Non-Player Character | Игровой персонаж, не управляемый игроком |
| SE | Special Edition | Поддерживаемая Skyrim distribution ветвь |
| AE | Anniversary Edition | Поддерживаемая Skyrim distribution/runtime ветвь |
| VR | Virtual Reality | Исключённая текущая Skyrim platform |
| SKSE | Skyrim Script Extender | Внешний extension runtime |
| ESP | Elder Scrolls Plugin | Формат игрового plugin |
| ESM | Elder Scrolls Master | Master plugin format |
| ESL | Elder Scrolls Light | Light plugin format |
| TTS | Text to Speech | Синтез речи из текста |
| STT | Speech to Text | Распознавание речи в текст |
| FTS | Full Text Search | Полнотекстовый поиск, требующий отдельного решения |
| CI | Continuous Integration | Автоматическая интеграционная проверка |
| QA | Quality Assurance | Процесс и доказательства качества |
| UTC | Coordinated Universal Time | Базовая absolute time zone |

## 23. Запрещённые неоднозначности

| Не использовать без уточнения | Причина | Требуемая формулировка |
|---|---|---|
| Готово | Неясно, документ или код | `Document Complete`, `Implementation Verified` или точный status |
| Сервис | Неясна роль и boundary | `Application Service`, `Repository`, `Adapter` или конкретное имя |
| Менеджер | Может скрывать чрезмерную ответственность | Назвать owned lifecycle, например `Save Manager` |
| Данные | Неясны owner, durability и schema | `Domain State`, `Durable State`, `Configuration` или конкретный record |
| Событие | Может быть raw signal, command или fact | `Game Event`, `Domain Event`, `Integration Event` или `Command` |
| ID | Неясен scope и стабильность | `WorldId`, `ModuleId`, `CorrelationId` и подобный typed identifier |
| Версия | Неясен объект | `Document Version`, `Contract Version`, `Schema Version` или `Build Version` |
| Сохранение | Неясны game save и World Engine checkpoint | `Game Save`, `Checkpoint` или `Save Participant Snapshot` |
| Память | Неясны runtime memory и NPC memory | `Process Memory`, `Memory Record` или `Memory Engine` |
| Ошибка | Смешивает expected error и fault | `Error`, `Fault`, `Failure` или конкретный Error Code |
| Поддерживается | Не указана степень доказательства | Указать platform version, capability и test evidence |
| Совместимо | Не указано направление | `Backward Compatible` или `Forward Compatible` плюс версии |

## 24. Правила перевода

1. Идентификаторы кода, events, fields, metrics и config keys не переводятся.
2. Название World Engine и IDs документов остаются неизменными.
3. При первом употреблении специализированного термина перевод сопровождается canonical token.
4. RU и DE не создают независимые синонимы для одного technical token.
5. Модальные слова `MUST`, `SHOULD`, `MAY` сохраняют силу во всех языках.
6. Числа, units, versions, enum values и error codes должны совпадать точно.
7. Если естественный перевод меняет boundary, используется английский token с пояснением.

## 25. Введение нового термина

Новый public term сначала получает proposed canonical token, краткое определение, owner, scope, запрет смешения и минимум один пример. Термин, затрагивающий architecture semantics, требует review Architecture Owner; термин, меняющий принятое решение, требует ADR. После принятия обновляются STD-001, STD-002 при необходимости, затронутые contracts и tests.

| Gate | Вопрос | Доказательство |
|---|---|---|
| T1 Need | Нужна ли новая концепция, а не синоним | Поиск по canonical corpus и конфликтная карта |
| T2 Meaning | Есть ли одно проверяемое определение | Definition, scope, owner и non-example |
| T3 Naming | Соответствует ли token STD-002 | Naming review |
| T4 Compatibility | Ломает ли изменение существующие contracts | Impact и migration plan |
| T5 Publication | Обновлены ли обе языковые версии | RU/DE parity и canonical index |

## 26. Acceptance criteria

- Каждый public документ использует canonical status vocabulary.
- Каждый новый public identifier ссылается на определённый concept.
- RU и DE сохраняют одинаковые tokens, числовые значения и модальную силу.
- Поиск запрещённых неоднозначностей не обнаруживает необъяснённых употреблений в normative clauses.
- Термины module, port, adapter, repository, event, command, query и result не используются взаимозаменяемо.
- AI output нигде не называется truth до deterministic validation.
- Complete документа не интерпретируется как Implemented или Verified программного компонента.
- Skyrim VR нигде не считается частью текущего supported scope.

## Приложение A. Быстрая статусная формула

Рекомендуемая запись состояния имеет форму: `Document Status; Implementation Status; Verification Scope; Evidence`. Пример: `Complete; Prototype; build and in-game validation pending; MOD-002 specification v2.0`.

## Приложение B. Термины с разными значениями

| Token | Разрешённые значения | Обязательное уточнение |
|---|---|---|
| Repository | Git Repository или Repository Contract | Всегда указывать `Git` либо domain name |
| Runtime | Game Runtime, AI Runtime или Process Runtime | Всегда указывать владельца |
| Plugin | Game Plugin или Native Plugin | Указывать artifact kind |
| Save | Game Save или World Engine Checkpoint | Не использовать одиночное слово в contracts |
| Context | Operation Context, Permission Context или AI Context | Указывать purpose и boundary |
| Model | Domain Model, Data Model или AI Model | Указывать класс модели |

## Приложение C. Traceability

| Источник | Зафиксированное влияние |
|---|---|
| PLAN-001 | Статусы, delivery denominator и Foundation Gates |
| ARCH-002/004/005 | Layers, module boundaries и dependency direction |
| ARCH-007 | Runtime interactions и sequence terminology |
| ARCH-009 | Configuration vocabulary |
| ARCH-011 | Save Participant и atomic restore vocabulary |
| ARCH-012 | AI boundary и source-of-truth rules |
| ADR-002/003/005/008/009 | Events, persistence, save, contract ownership и Skyrim scope |
| MOD-001/002/003 | Core, Game Adapter и Memory Engine domain vocabulary |

## Приложение D. Журнал изменений

| Версия | Дата | Изменение |
|---|---|---|
| 1.0 | 2026-09-05 | Создан полный RU/DE/EN glossary для governance, architecture, modules, events, persistence, save, AI, testing, operations, Git и risk terminology. |
