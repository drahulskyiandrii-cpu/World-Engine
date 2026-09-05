# STD-004 — World Engine Documentation Standard

Версия 1.0 · Русский · 2026-09-05 · Текущий нормативный стандарт

## 1. Назначение

STD-004 определяет, как World Engine создаёт, проверяет, публикует, обновляет и заменяет документы. Цель — превратить документацию в проверяемый контракт, а не в набор деклараций.

Стандарт обязателен для новых канонических артефактов и для существенных изменений уже опубликованных документов.

## 2. Область действия

Стандарт охватывает DOC, PLAN, ARCH, ADR, MOD, DB, EVT, API, AI, TEST, RISK, STD, RES, KB и IHK. Он распространяется на Markdown sources, DOCX/PDF deliverables, диаграммы, таблицы, примеры contracts и связанные QA reports.

Skyrim SE/AE является активной игровой целью. Skyrim VR должен быть явно отмечен как Excluded, пока отдельное решение не изменит scope.

## 3. Источники власти

| Уровень | Источник | Роль |
|---|---|---|
| 1 | Явное решение Project Owner | Направление, scope и финальное принятие |
| 2 | Accepted ADR | Необратимое или межмодульное архитектурное решение |
| 3 | Current ARCH, MOD, API, DB, EVT | Нормативный design baseline в своей области |
| 4 | Current STD, TEST, RISK | Общие правила и доказательная модель |
| 5 | Living DOC-009, research и knowledge base | Контекст и текущие данные, но не скрытая замена ADR |

При конфликте автор не выбирает удобную формулировку. Он фиксирует inconsistency, определяет владельца и проводит change control.

## 4. Нормативный язык

| Термин | Значение |
|---|---|
| MUST / MUST NOT | Обязательное требование или запрет |
| SHOULD / SHOULD NOT | Предпочтительное правило; отклонение обосновывается |
| MAY | Допустимый выбор |
| TBD | Незакрытое решение с Owner, deadline и blocking impact |
| N/A | Неприменимо с кратким обоснованием |

Слова «быстро», «надёжно», «готово» и «полностью» без метрики или критерия не являются нормативными.

## 5. Семейства и идентификаторы

Каждый deliverable имеет стабильный ID согласно STD-002. ID не меняется при переводе, новой версии или смене формата. Новый ID нужен только для новой ответственности.

| Семейство | Основной результат |
|---|---|
| DOC / PLAN | Управление, scope, roadmap и journal |
| ARCH / ADR | Структура системы и принятые решения |
| MOD | Полный модульный design/implementation contract |
| DB / EVT / API / AI | Межмодульные технические контракты |
| TEST / RISK / STD | Проверка, риск и единые правила |
| RES / KB / IHK | Исследования, знания и аттестационные материалы |

## 6. Жизненный цикл документа

`Planned → Draft → Review → Current/Accepted → Superseded → Archived` — базовый lifecycle. `Living` — дополнительное свойство Current-документа, а не ослабление QA.

| Переход | Обязательное доказательство |
|---|---|
| Planned → Draft | Owner, scope, dependencies и source |
| Draft → Review | Полные required sections и закрыте blocking TBD |
| Review → Current | Content, structure, parity, render и accessibility QA |
| Review → Accepted | То же и формальное решение Owner |
| Current → Superseded | Новый successor, migration note и index update |

## 7. Статус документа и статус кода

Статус документа и статус программного компонента MUST вестись раздельно. `Complete Design Baseline` означает, что спецификация достаточна для начала реализации. Это не означает `Implemented`, `Verified` или `Operational`.

| Измерение | Допустимые состояния |
|---|---|
| Document | Planned, Draft, Review, Current, Accepted, Superseded, Living |
| Implementation | Not Started, Prototype, Implemented, Verified, Operational, Retired |
| Validation | Not Run, Failed, Partial, Passed, Waived |

## 8. Обязательные метаданные

Каждый Current deliverable MUST содержать ID, title, version, language, date, status, owner, scope, purpose, governing references, implementation truth, change summary и canonical path. Для Living-документа добавляются cadence и last reviewed date.

Метаданные в DOCX properties не заменяют видимый document-control block.

## 9. Заголовок и открывающая часть

Обложка должна показывать ID, точное название, версию, язык, статус и краткую границу применения. Первые содержательные разделы — Purpose, Scope, Authority, Status truth и Definitions/References. Читатель должен понять силу документа до первого технического раздела.

## 10. Общее ядро содержания

| Раздел | Требование |
|---|---|
| Purpose и Scope | Цель, аудитория, включения и non-goals |
| Context и dependencies | Входные решения, consumers и prerequisites |
| Ownership | Владелец данных, решений, кода и проверок |
| Normative requirements | Атомарные, тестируемые и непротиворечивые требования |
| Failure и recovery | Ошибки, деградация, retry, rollback и recovery |
| Security и privacy | Trust boundaries, sensitive data, logging и retention |
| Verification | Test IDs, acceptance criteria, evidence и residual risk |
| Traceability | Связи с upstream/downstream artifacts |
| Change history | Версия, дата, причина и impact |

## 11. Правила для Architecture

ARCH-документ объясняет components, boundaries, dependency direction, runtime topology, data/control flow и system qualities. Он MUST содержать легенду для диаграмм и не должен скрывать architectural decision в иллюстрации.

Изменение ownership, dependency direction, persistence boundary, threading model или cross-module contract требует ADR или явного обоснования, почему ADR не нужен.

## 12. Правила для ADR

ADR MUST содержать Context, Decision, Alternatives, Consequences, Compatibility, Migration, Verification и Status. Accepted ADR не редактируется так, будто прежнего решения не было. Смена решения создаёт successor ADR с явной ссылкой `Supersedes`.

## 13. Обязательная структура MOD-001–MOD-014

Каждая спецификация модуля MUST быть полноценным standalone-документом, а не кратким рефератом. Требование действует для MOD-001 Core, MOD-002 Game Adapter, MOD-003 Memory Engine и всех MOD-004–MOD-014.

| Блок | Обязательное содержание |
|---|---|
| Identity и scope | Цель, responsibilities, non-goals, platform matrix |
| Architecture | Components, ports/adapters, dependency direction, thread/process model |
| Contracts | Inputs, outputs, commands, queries, events, error envelope, compatibility |
| State и data | Ownership, entities, invariants, persistence, migration, retention |
| Runtime | Lifecycle, configuration, scheduling, concurrency, budgets, observability |
| Resilience | Failure taxonomy, fallback, retry, idempotency, recovery, rollback |
| Security | Trust boundaries, validation, secrets, privacy, abuse limits |
| Delivery | Work breakdown, test matrix, acceptance gates, deployment и operations |
| Truth | Document status, implementation status, evidence, TBD и open decisions |

## 14. Модульные интерфейсы

Каждый exposed port описывает request/response или event schema, preconditions, postconditions, error codes, timeout/cancellation, ordering, idempotency, versioning и owner. Ссылка на будущий API/EVT документ не заменяет локально достаточную спецификацию.

Публичные MOD-002 identifiers, уже принятые в baseline, сохраняются до контролируемой migration.

## 15. Модульные данные и сохранение

Для каждого durable state спецификация указывает authoritative owner, schema/version, consistency boundary, transaction semantics, save participant behavior, migration, corruption handling и restore validation. Модуль не может неявно владеть SQLite или game state через обход портов.

## 16. Модульные ошибки и recovery

Ошибка MUST иметь стабильный code, severity, retryability, user impact, logging policy и recovery owner. Спецификация отличает validation failure, unavailable dependency, timeout, incompatible version, corrupt state и internal defect.

## 17. Модульное тестирование

Модульный документ MUST содержать unit, component, integration, persistence/recovery, performance, fault-injection и acceptance scenarios. Каждый критический requirement имеет test ID и expected evidence. Фраза «протестировать позже» без Owner и gate недопустима.

## 18. Модульный readiness gate

| Gate | Минимальное условие |
|---|---|
| Design Ready | Нет blocking TBD; owners, ports, state, errors и tests определены |
| Implement Ready | Общие DB/EVT/API contracts достаточны; dependencies стабильны; Owner разрешил старт |
| Implemented | Code и build evidence соответствуют baseline |
| Verified | Все blocking tests прошли; deviations зафиксированы |
| Operational | Deployment, monitoring, backup/restore и rollback доказаны |

Ни один gate не закрывается объёмом или числом страниц.

## 19. Документы DB, EVT, API и AI

Межмодульный contract MUST определять namespace, owner, version, schema, compatibility, validation, lifecycle, errors, observability и security. AI-документы дополнительно фиксируют model/provider boundary, prompt provenance, context budget, deterministic fallback, safety, privacy и evaluation metrics.

## 20. Документы TEST и RISK

TEST описывает среды, fixtures, oracle, pass/fail rule, evidence retention и Owner. RISK описывает probability, impact, trigger, mitigation, contingency, Owner, due date и residual risk. Один текст может ссылаться на оба класса, но не подменяет их.

## 21. Документы Research, Knowledge Base и IHK

Research отделяет observed facts, source claims, experiments, inference и recommendation. Knowledge Base может содержать hypotheses и ideas, но MUST маркировать их как non-normative. IHK-материалы должны отслеживать цифры, assumptions и источники.

## 22. Двуязычная RU/DE parity

Каноническая RU/DE пара MUST иметь одинаковые ID, version, status, section order, requirement count, identifiers, numbers, units, enums, tables, acceptance gates и change history. Литературная форма может различаться; normative strength и технический смысл — нет.

## 23. Машинные токены

Code identifiers, event names, config keys, file paths, CLI commands, error codes и schema fields не переводятся. Они сохраняют canonical English spelling и форматируются как inline code. Термины проверяются по STD-001, имена — по STD-002.

## 24. Запись требований

Требование должно иметь одного Owner, одно наблюдаемое поведение, границу и метод проверки. Сложные «и» разбиваются, если части могут провалиться независимо. Нормативный текст не полагается на скрытый контекст чата.

## 25. Идентификаторы требований

Требования, риски, тесты и decisions, на которые ссылаются другие артефакты, имеют стабильные IDs: `<DOC-ID>-REQ-NNN`, `<DOC-ID>-RISK-NNN`, `<DOC-ID>-TEST-NNN`, `<DOC-ID>-DEC-NNN`. Удалённый ID не переиспользуется.

## 26. Traceability

Для каждого blocking requirement нужна цепочка `Source → Requirement → Design element → Implementation target → Test → Evidence`. До начала кода implementation target может быть planned path/component, но не может ложно указываться как существующий.

| Source | Requirement | Implementation | Test | Evidence |
|---|---|---|---|---|
| ADR-009 | MOD-002-REQ-014 | Planned adapter boundary | MOD-002-TEST-021 | Pending in-game run |
| ARCH-011 | MOD-013-REQ-008 | Planned atomic restore coordinator | MOD-013-TEST-012 | Pending implementation |

## 27. Ссылки и provenance

Внутренняя ссылка указывает точный document ID и version/status, когда это важно для совместимости. Внешний источник фиксирует title, author/publisher, URL и access date. Цитата отделяется от интерпретации.

## 28. Assumptions, TBD и open decisions

Каждая assumption имеет ID, Owner, validation method, due/gate и impact при ошибке. TBD без Owner и срока является QA defect. Blocking open decision не допускает Current/Design Ready.

## 29. Scope и non-goals

Каждый design-document MUST содержать явные Included, Excluded и Deferred. Отложенная функция не описывается как частично реализованная. Для game-specific функций обязательна platform matrix.

## 30. Ownership и RACI

| Действие | Accountable | Responsible | Consulted |
|---|---|---|---|
| Scope и финальный acceptance | Project Owner | Document Owner | Architecture/Module Owners |
| Техническая корректность | Architecture Owner | Author/Reviewer | Contract Owners |
| RU/DE parity | Documentation Lead | Author/Translator | Technical Reviewer |
| QA evidence | Document Owner | QA executor | Accessibility/Domain reviewer |
| Canonical publication | Documentation Lead | Repository maintainer | Project Owner |

В текущей фазе Andrii Drahulskyi является Project Owner и Documentation Lead; это не отменяет независимую проверку критических contracts.

## 31. Versioning и имена файлов

Версия изменяется при нормативном изменении, а не при каждом render attempt. Major обозначает incompatible изменение контракта или scope; minor — compatible расширение/уточнение. Имя файла следует STD-002 и отражает ID, topic, language, version и publication status.

## 32. Журнал изменений

Каждая Current-версия имеет change log с version, date, author/owner, reason, changed normative areas, compatibility и migration impact. Мелкие орфографические исправления могут группироваться; изменение смысла — нет.

## 33. Review workflow

1. Author закрывает required sections и self-review checklist.
2. Domain review проверяет смысл, ownership, contracts и риски.
3. Cross-document review проверяет ADR/ARCH/MOD/API/DB/EVT consistency.
4. Language parity review сверяет RU/DE structure и machine tokens.
5. Artifact QA проверяет DOCX structure, render и accessibility.
6. Publication review обновляет index, PLAN-001, journal и Git evidence.

## 34. Content QA gate

Content QA MUST проверить completeness, internal consistency, upstream/downstream consistency, numbers/units, identifier uniqueness, no unresolved blocking TBD, no fabricated implementation claims, acceptance testability и explicit residual risks. Простая проверка орфографии не закрывает gate.

## 35. Structural и accessibility QA

Для DOCX проверяются styles, heading hierarchy, table headers, repeating headers, non-split rows, page numbers, language metadata, document properties, alt text для meaningful images/tables и порядок чтения. Цвет не является единственным носителем смысла. Критические findings блокируют публикацию.

## 36. Render и visual QA

Каждая страница final DOCX MUST быть отрендерена и визуально просмотрена. Проверяются clipped text, overflow, blank/near-blank pages, orphan headings, broken tables, tiny fonts, bad page breaks, missing images, footer/header collisions и inconsistent whitespace. После любой content/layout correction render повторяется.

## 37. Таблицы, диаграммы и иллюстрации

Таблица имеет одну header row, ясные units и текстовый вывод. Диаграмма имеет цель, legend, direction, boundary semantics, stable labels и текстовое описание. Декоративное изображение не должно имитировать техническое доказательство.

## 38. Code и API examples

Пример маркируется как `Normative`, `Illustrative`, `Pseudocode` или `Prototype snapshot`. Normative example должен собираться/валидироваться или иметь явный план проверки. Снапшот кода в документе не считается авторитетным исходником.

## 39. Evidence и acceptance

| Claim | Минимальное evidence |
|---|---|
| Document complete | Canonical files, QA report, page render, index entry |
| Build passes | Command/environment, exit status, artifact hash/log |
| Test passes | Test ID, input/version, expected/actual result, timestamp |
| Performance target met | Workload, hardware, configuration, distribution, threshold |
| In-game validated | Game/runtime versions, load order, scenario, result, evidence |
| Recovery works | Fault injection, restore target, validation и data-loss result |

Если evidence не создано, статус остаётся Pending/Not Run, даже если ожидается успех.

## 40. Supersession, archive и canonical index

В активном наборе на каждые ID/language существует один current artifact. Заменённый файл помечается Superseded, сохраняется для provenance и указывает successor. `docs/CANONICAL_DOCUMENTATION_INDEX.md` обновляется в том же change set; папка или имя `Complete` без индекса не создаёт канон.

## 41. Publication и repository evidence

Публикация включает source, final artifact, QA report, index/PLAN update и atomic Git commit. Branch и commit не должны скрывать незавершённый QA. Main или release branch изменяются только после прохождения применимых gates и отдельного integration decision.

## 42. Exceptions и waivers

Исключение имеет ID, rule, reason, scope, Owner, approver, start/end, risk, mitigation и exit condition. Waiver не может легализовать ложный implementation status, скрыть security defect или отменить decision authority.

## 43. Review cadence

Living-документы проверяются по своему cadence. PLAN-001 проверяется при закрытии deliverable; DOC-009 — после каждого material work pass; risk register — на gates и releases; consistency audit — перед foundation/release merge. Отсутствие изменений также фиксируется как review result.

## 44. Acceptance criteria STD-004

STD-004 считается Current, когда RU/DE пара имеет одинаковую нормативную структуру; правила охватывают все document families и MOD-001–MOD-014; document/implementation truth разделены; QA gates тестируемы; все страницы отрендерены и просмотрены; структурный/accessibility audit не имеет незакрытых findings; canonical index и PLAN-001 обновлены.

## Приложение A. Минимальный document-control block

| Поле | Пример |
|---|---|
| Document ID | MOD-004 |
| Version | 1.0 |
| Status | Complete Design Baseline; Implementation Not Started |
| Scope | Character Engine for Skyrim SE/AE; VR excluded |
| Owner | Named accountable owner |
| Governing references | ARCH-002, ADR-008, API/EVT baselines |
| Last reviewed | ISO date |
| Canonical path | Repository-relative path |

## Приложение B. Проверка модульного документа

| Критерий | Pass condition |
|---|---|
| Responsibilities/non-goals | Границы не пересекаются с соседними modules |
| Ports и contracts | Все inputs/outputs имеют owner, schema, errors и version |
| Data | Каждое durable field имеет owner, migration и retention |
| Lifecycle | Startup, steady state, shutdown, save/load и recovery описаны |
| Non-functional | Performance budgets, security и observability измеримы |
| Tests | Requirements привязаны к test IDs и evidence |
| Status truth | Design, implementation и validation не смешаны |

## Приложение C. Шаблон traceability

| Requirement ID | Source | Design owner | Implementation target | Test ID | Evidence | Status |
|---|---|---|---|---|---|---|
| `<DOC-ID>-REQ-001` | ADR/ARCH/Owner decision | Component/port | Existing or planned path | `<DOC-ID>-TEST-001` | Link/hash/log | Planned/Passed |

## Приложение D. Чек-лист публикации

1. Source и final deliverable совпадают по version и status.
2. RU/DE headings, IDs, tables, numbers и gates сверены.
3. Content, cross-document, structural и accessibility checks passed.
4. Все final pages отрендерены и просмотрены.
5. QA report содержит метрики, findings, fixes и final hashes.
6. Canonical index, PLAN-001 и DOC-009 обновлены.
7. Change set атомарен и отправлен в авторизованную branch.

## Приложение E. Журнал изменений

| Версия | Дата | Статус | Изменение |
|---|---|---|---|
| 1.0 | 2026-09-05 | Current | Создан полный двуязычный documentation standard, включая обязательные правила MOD-001–MOD-014 и evidence-based QA. |
