# PLAN-001 — World Engine Master Documentation Plan

Версия 2.2 · Русский · 2026-09-05 · Текущий управляющий документ

## 1. Назначение документа

PLAN-001 является единым управляющим планом документации World Engine. Он отвечает на пять практических вопросов: какие документы существуют; какие из них действительно завершены; какие артефакты обязательны до следующего производственного кода; что создаётся непосредственно перед реализацией конкретного модуля; по каким доказательствам работа может считаться завершённой.

Этот план не заменяет архитектурные документы, ADR или спецификации модулей. Он связывает их в проверяемую программу работ и устраняет неоднозначность между «документ написан», «решение принято», «код реализован», «поведение проверено» и «компонент готов к эксплуатации».

PLAN-001 — контрольный артефакт, а не один из 116 поставляемых документов программы. Поэтому он не включается в знаменатель прогресса и не создаёт искусственного увеличения объёма при каждом собственном обновлении.

## 2. Область действия и ограничения

План охватывает управление проектом, архитектуру, ADR, четырнадцать модулей, базу данных, событийную систему, API, AI, тестирование, риски, стандарты, исследования, базу знаний и IHK-материалы. Активная первая игровая платформа — Skyrim Special Edition / Anniversary Edition. Skyrim VR не входит в текущую реализацию и требует отдельного решения, исследования и набора acceptance tests.

Каталог `sources/`, исторические архивы и superseded snapshots являются доказательствами происхождения, но не активными implementation-facing артефактами. Каноническим считается только документ, явно внесённый в `docs/CANONICAL_DOCUMENTATION_INDEX.md`.

## 3. Исправление прежнего Master Plan

Прежняя таблица указывала итог `28 / 92`, хотя перечисляла 114 позиций. После появления ADR-008 и ADR-009 фактический объём программы составляет 116 поставляемых артефактов. После публикации DOC-009, STD-001, STD-002 и STD-004 подтверждены 37 из 116.

Также исправлены два смысловых искажения. `Future Ideas` отсутствует в активном каноническом наборе и поэтому не может иметь статус Complete. Наличие короткого корневого `README.md` не означает завершение полноценного стандарта README из этапа 11. Чекбокс документа никогда не означает, что соответствующий программный модуль уже реализован.

| Показатель | Прежнее значение | Проверенное значение | Основание |
|---|---:|---:|---|
| Всего поставляемых артефактов | 92 | 116 | Пересчёт всех строк этапов 1–14 с ADR-008 и ADR-009 |
| Завершено | 28 | 37 | Активный канонический индекс и проверенные файлы |
| Architecture Decision Records | 7 / 7 | 9 / 9 | ADR-008 и ADR-009 приняты и опубликованы |
| Спецификации модулей | 1 / 15 | 4 / 15 | Template, MOD-001, MOD-002 и MOD-003 |
| Future Ideas | Complete | Planned | Активного канонического документа нет |
| Реализация модулей | Не различалась | Учитывается отдельно | Design, Implemented, Verified и Operational — разные состояния |

## 4. Текущий исполнительный статус

Документационный фундамент проекта значителен, но ещё не является полным pre-code foundation. Архитектура и ADR закрыты. Три модульные спецификации после шаблона готовы. Однако самостоятельные стандарты, общие контракты базы данных, событий, API, тестирования и рисков пока не опубликованы.

| Раздел | Завершено | Всего | Состояние |
|---|---:|---:|---|
| Управление проектом | 9 | 9 | Полный baseline; DOC-009 ведётся как Living |
| Архитектура | 12 | 12 | Полный audited baseline |
| ADR | 9 | 9 | Полный принятый baseline |
| Спецификации модулей | 4 | 15 | Template и MOD-001–003 готовы |
| База данных | 0 | 8 | Только решения внутри архитектуры и ADR |
| Event System | 0 | 6 | Только решения внутри архитектуры и ADR |
| API | 0 | 6 | Самостоятельные контракты не опубликованы |
| AI | 0 | 7 | Архитектурный baseline есть, детальные документы отсутствуют |
| Тестирование | 0 | 6 | Требования распределены по текущим документам |
| Управление рисками | 0 | 4 | Реестр и процесс отсутствуют |
| Стандарты проекта | 3 | 10 | STD-001, STD-002 и STD-004 Current; остальные позиции Planned |
| Research | 0 | 6 | Канонические исследования не опубликованы |
| Knowledge Base | 0 | 10 | Активных канонических документов нет |
| IHK | 0 | 8 | Материалы не начаты |
| **Итого** | **37** | **116** | **31,9 % поставляемых артефактов** |

## 5. Модель статусов

Статусы применяются отдельно к документу и к программному компоненту. Документ может быть Complete, пока код остаётся Not Started. Такой случай сейчас является нормой для MOD-001 и MOD-003.

| Статус | Значение | Минимальное доказательство |
|---|---|---|
| Planned | Артефакт предусмотрен, но нормативной версии нет | Строка в PLAN-001 с владельцем и зависимостями |
| Draft | Содержание создано, но не прошло все проверки | Редактируемый source и черновой DOCX |
| Design Baseline | Контракты достаточны для реализации | Утверждённый документ, traceability и закрытые design gaps |
| Accepted | Решение формально принято | ADR или подписанный decision record |
| Implemented | Код существует и соответствует baseline | Ссылка на исходники и успешную сборку |
| Verified | Поведение доказано тестами | Отчёты unit, integration или acceptance tests |
| Operational | Компонент пригоден для целевой среды | Развёртывание, мониторинг, recovery и эксплуатационные доказательства |
| Superseded | Версия заменена и не управляет реализацией | Архивная ссылка и указание преемника |
| Living | Документ обновляется в течение проекта | Владелец, cadence и журнал изменений |

## 6. Правило готовности документа

Артефакт может быть отмечен Complete только если он является самостоятельным, имеет RU и DE версии там, где двуязычность обязательна, содержит границы ответственности, нормативные требования, зависимости, ошибки и recovery, безопасность, тестируемые acceptance criteria, traceability, историю изменений и честный implementation status. Для Word-документов обязательны полный рендер всех страниц, визуальная проверка, структурный аудит и accessibility audit.

Короткий конспект, презентация, устаревший файл, пустая папка, `.gitkeep`, заметка в другом документе или существование исходного кода сами по себе не закрывают позицию Master Plan.

## 7. Классы времени подготовки

| Класс | Когда нужен | Правило |
|---|---|---|
| A — Active baseline | Уже действует | Не менять без change control |
| F — Foundation first | До нового production-кода | Блокирует начало новых реализаций |
| J — Just in time | До кода зависимого модуля | Готовится непосредственно перед модулем |
| P — Parallel | Параллельно реализации или перед релизом | Не блокирует текущую design-фазу |
| L — Later | После стабилизации v1.0 baseline | Не расширять ранний MVP |

## 8. Pre-Code Foundation Pack

Foundation Pack превращает существующую архитектуру в исполнимый контракт. Новая производственная реализация после текущего прототипа MOD-002 не должна расширяться, пока не закрыт минимальный набор F1–F5. Допускаются документация, безопасные исследования, тестовые harnesses и исправления уже существующего прототипа, если они не закрепляют новые неописанные контракты.

| Gate | Состав | Результат | Состояние |
|---|---|---|---|
| F0 Master Control | PLAN-001 RU/DE, индекс и QA | Реальный реестр, порядок и критерии | Complete |
| F1 Governance | DOC-009; STD-001–007 | Общий язык, правила документов и Git | In progress — DOC-009, STD-001/002/004 complete |
| F2 Verification & Risk | TEST-001–003, TEST-006; RISK-001–003 | Единая доказательная модель и risk ownership | Planned |
| F3 Persistence | DB-001–003, DB-006–008 | Логическая модель, миграции, восстановление | Planned |
| F4 Events | EVT-001–006 | Имена, lifecycle, типы и каталог событий | Planned |
| F5 APIs | API-001, API-005, API-006; skeleton API-002–004 | Стабильные границы и versioning | Planned |
| F6 Module Readiness | Полная спецификация целевого модуля | Разрешение начинать код конкретного модуля | MOD-001–003 готовы как design docs |

## 9. Условия допуска к новому production-коду

Решение о старте реализации принимается не по количеству страниц, а по закрытым интерфейсам и рискам. Для любого нового модуля обязательны следующие доказательства.

1. Спецификация модуля имеет Design Baseline и не содержит неизвестных владельцев данных.
2. Входные и выходные ports, события и ошибки определены в общих каталогах.
3. Persistence и migration ownership определены до первой durable записи.
4. Unit, integration и acceptance tests имеют идентификаторы и ожидаемые результаты.
5. Конфигурация, observability, failure handling и rollback описаны.
6. Зависимости соответствуют ARCH-004, ARCH-005 и ADR-008.
7. Никакой прямой игровой или SQLite-зависимости не пересекает установленную границу.
8. Для Skyrim SE/AE есть воспроизводимый test environment; Skyrim VR не включается неявно.

## 10. Рекомендуемая последовательность продолжения

После публикации DOC-009 и STD-004 следует закрыть оставшиеся F1-позиции STD-003, STD-005, STD-006 и STD-007, а не все 79 оставшихся документов подряд. Это даст достаточную определённость до кода и не превратит проект в бесконечное написание документов.

| Порядок | Пакет | Зачем сейчас | Выходной критерий |
|---:|---|---|---|
| 1 | F1 Governance | Устраняет терминологические и процессные расхождения | DOC-009 и STD-001–007 Current |
| 2 | F2 Verification & Risk | Позволяет измерять готовность и принимать риск | TEST-001–003, TEST-006 и RISK-001–003 Current |
| 3 | F3 Persistence | Нужен Core, Memory, Save и почти всем симуляционным модулям | DB logical baseline и migration/recovery policy Current |
| 4 | F4 Events | События — основной межмодульный язык | Полный event contract и каталог v1 Current |
| 5 | F5 APIs | Запирает границы между Core, adapters и modules | Internal/Module APIs и versioning Current |
| 6 | Module gate | Проверка конкретной спецификации перед кодом | Решение Implement / Revise / Defer |
| 7 | Production code | Только после закрытия применимых gates | Сборка и тестовые доказательства |

## 11. Этап 1 — Управление проектом

| ID | Артефакт | Статус | Класс | Каноническое доказательство / условие закрытия |
|---|---|---|---|---|
| DOC-001 | Vision | Complete | A | `docs/00_Project_Management/DOC-001_*_Audited_Complete.docx` |
| DOC-002 | Project Principles | Complete | A | `docs/00_Project_Management/DOC-002_*_Audited_Complete.docx` |
| DOC-003 | Product Roadmap | Complete | A | `docs/00_Project_Management/DOC-003_*_Audited_Complete.docx` |
| DOC-004 | Implementation Roadmap | Complete | A | `docs/00_Project_Management/DOC-004_*_Audited_Complete.docx` |
| DOC-005 | Project Charter | Complete | A | `docs/00_Project_Management/DOC-005_*_Audited_Complete.docx` |
| DOC-006 | Project Scope | Complete | A | `docs/00_Project_Management/DOC-006_*_Audited_Complete.docx` |
| DOC-007 | Milestones | Complete | A | `docs/00_Project_Management/DOC-007_*_Audited_Complete.docx` |
| DOC-008 | Release Plan | Complete | A | `docs/00_Project_Management/DOC-008_*_Audited_Complete.docx` |
| DOC-009 | Developer Journal | Complete / Living | F | `DOC-009_*_v1_0_Current.docx`; decision, experiment, blocker and evidence records |

## 12. Этап 2 — Архитектура

Все двенадцать документов завершены и являются активным baseline. Изменения проходят через ADR или формальный пересмотр соответствующего ARCH-документа.

| ID | Артефакт | Статус | Класс | Каноническое доказательство |
|---|---|---|---|---|
| ARCH-001 | Overall System Architecture | Complete | A | `docs/01_Architecture/ARCH-001_Overall_System_Architecture/` |
| ARCH-002 | Module Architecture | Complete | A | `docs/01_Architecture/ARCH-002_Module_Architecture/` |
| ARCH-003 | Data Flow | Complete | A | `docs/01_Architecture/ARCH-003_Data_Flow/` |
| ARCH-004 | Architecture Principles & Dependency Rules | Complete | A | `docs/01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/` |
| ARCH-005 | Module Dependencies | Complete | A | `docs/01_Architecture/ARCH-005_Module_Dependencies/` |
| ARCH-006 | Component Diagram | Complete | A | DOCX pair and audited SVG in `ARCH-006_Component_Diagram/` |
| ARCH-007 | Sequence Diagrams | Complete | A | `docs/01_Architecture/ARCH-007_Sequence_Diagrams/` |
| ARCH-008 | Deployment Architecture | Complete | A | `docs/01_Architecture/ARCH-008_Deployment_Architecture/` |
| ARCH-009 | Configuration Architecture | Complete | A | `docs/01_Architecture/ARCH-009_Configuration_Architecture/` |
| ARCH-010 | Plugin Architecture | Complete | A | `docs/01_Architecture/ARCH-010_Plugin_Architecture/` |
| ARCH-011 | Save System Architecture | Complete | A | `docs/01_Architecture/ARCH-011_Save_System_Architecture/` |
| ARCH-012 | AI Integration Architecture | Complete | A | `docs/01_Architecture/ARCH-012_AI_Integration_Architecture/` |

## 13. Этап 3 — Architecture Decision Records

| ID | Решение | Статус | Класс | Каноническое доказательство |
|---|---|---|---|---|
| ADR-001 | Core Architecture | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-001_Core_Architecture/` |
| ADR-002 | Event System | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-002_Event_System/` |
| ADR-003 | Database | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-003_Database/` |
| ADR-004 | Local AI | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-004_Local_AI/` |
| ADR-005 | Save System | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-005_Save_System/` |
| ADR-006 | Plugin Detection | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-006_Plugin_Detection/` |
| ADR-007 | Multi-Game Support | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/` |
| ADR-008 | Dependency Semantics & Contract Ownership | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/` |
| ADR-009 | MOD-002 Skyrim Integration | Accepted | A | SE/AE boundary accepted; VR explicitly excluded |

## 14. Этап 4 — Спецификации модулей

| ID | Артефакт | Статус документа | Статус реализации | Следующее условие |
|---|---|---|---|---|
| TEMPLATE | Module Specification Template | Complete | Not applicable | Использовать для каждой новой спецификации |
| MOD-001 | Core | Complete | Not Started | Foundation gates и implementation decision |
| MOD-002 | Game Adapter | Complete | Prototype | Build, automated tests и in-game SE/AE validation |
| MOD-003 | Memory Engine | Complete | Not Started | Foundation gates, DB/event/API contracts и implementation decision |
| MOD-004 | Character Engine | Planned | Not Started | MOD-003 contracts и полная MOD-004 specification |
| MOD-005 | Dialogue Engine | Planned | Not Started | MOD-003/004, AI contracts и fallback policy |
| MOD-006 | Organization Engine | Planned | Not Started | Character/relationship/event contracts |
| MOD-007 | Relationship Engine | Planned | Not Started | MOD-003/004 и deterministic update rules |
| MOD-008 | Economy Engine | Planned | Not Started | v1.0 baseline stabilized; economic model approved |
| MOD-009 | Politics Engine | Planned | Not Started | Organization/economy baseline and risk review |
| MOD-010 | Quest Engine | Planned | Not Started | Event, adapter and save contracts |
| MOD-011 | Event Engine | Planned | Not Started | EVT-001–006 and API contracts complete |
| MOD-012 | Scheduler | Planned | Not Started | Clock, persistence and deterministic ordering contracts |
| MOD-013 | Save Manager | Planned | Not Started | DB baseline and ARCH-011 participant protocol |
| MOD-014 | Voice Engine | Planned | Not Started | Dialogue baseline, provider abstraction and privacy review |

## 15. Этап 5 — База данных

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| DB-001 | Database Architecture | Planned | F | Boundaries, engine choice, connections, transactions and ownership |
| DB-002 | Entity Model | Planned | F | Stable entity identities, aggregates and ownership map |
| DB-003 | Database Schema | Planned | F | Logical schema, invariants and module mappings |
| DB-004 | Tables | Planned | J | Physical tables for approved module schemas |
| DB-005 | Relationships | Planned | J | Keys, cardinality and cross-module reference policy |
| DB-006 | Indexes | Planned | J | Workload-based indexes with measured trade-offs |
| DB-007 | Migrations | Planned | F | Forward migration, compatibility, failure and rollback policy |
| DB-008 | Backup Strategy | Planned | F | Backup, restore, corruption handling and recovery tests |

## 16. Этап 6 — Event System

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| EVT-001 | Event Catalog | Planned | F | Initial cross-module event list with owners and schemas |
| EVT-002 | Event Naming Convention | Planned | F | Namespace, tense, version and compatibility rules |
| EVT-003 | Event Priority | Planned | F | Priority classes, starvation prevention and deterministic tie-break |
| EVT-004 | Event Lifecycle | Planned | F | Publish, validate, route, handle, retry, dead-letter and observe |
| EVT-005 | Event History | Planned | F | Retention, replay, audit and privacy rules |
| EVT-006 | Event Types | Planned | F | Command, fact, request/result and diagnostic taxonomy |

## 17. Этап 7 — API

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| API-001 | Internal API | Planned | F | Stable application ports and error envelope |
| API-002 | Game Adapter API | Planned | J | MOD-002 input/output contract and capabilities |
| API-003 | Event API | Planned | J | Event publication, subscription and delivery semantics |
| API-004 | Save API | Planned | J | Staging, validate, commit, rollback and compatibility |
| API-005 | Module API | Planned | F | Registration, lifecycle, health and dependency access |
| API-006 | API Versioning | Planned | F | Compatibility matrix, deprecation and negotiation rules |

## 18. Этап 8 — AI

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| AI-001 | LLM Integration | Planned | J | Provider-neutral ports, timeouts, limits and observability |
| AI-002 | Prompt Architecture | Planned | J | Layering, prompt ownership, injection controls and versioning |
| AI-003 | Context Manager | Planned | J | Budgeting, provenance, ordering and deterministic truncation |
| AI-004 | Memory Retrieval | Planned | J | Query contract aligned with MOD-003 and relevance evaluation |
| AI-005 | Voice Integration | Planned | J | TTS/STT contracts, consent, caching and fallback |
| AI-006 | AI Configuration | Planned | J | Profiles, validation, secrets and safe defaults |
| AI-007 | Fallback Logic | Planned | J | Deterministic non-AI behavior and failure classification |

## 19. Этап 9 — Тестирование

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| TEST-001 | Testing Strategy | Planned | F | Test pyramid, environments, evidence and ownership |
| TEST-002 | Unit Testing | Planned | F | Framework, conventions, isolation and coverage policy |
| TEST-003 | Integration Testing | Planned | F | Boundaries, fixtures, deterministic replay and failure injection |
| TEST-004 | Performance Testing | Planned | P | Budgets, workloads, baselines and regression thresholds |
| TEST-005 | AI Evaluation | Planned | J | Datasets, rubrics, safety and reproducibility |
| TEST-006 | Acceptance Tests | Planned | F | Scenario format, traceability and release gates |

## 20. Этап 10 — Управление рисками

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| RISK-001 | Risk Register | Planned | F | Owner, probability, impact, trigger, mitigation and residual risk |
| RISK-002 | Risk Assessment | Planned | F | Common scoring and review cadence |
| RISK-003 | Risk Mitigation | Planned | F | Funded actions, deadlines and verification |
| RISK-004 | Incident Log | Planned | P | Living incident, response and lessons-learned record |

## 21. Этап 11 — Стандарты проекта

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| STD-001 | Glossary | Complete | A | Canonical RU/DE/EN terms, definitions, owners and forbidden ambiguities |
| STD-002 | Naming Convention | Complete | A | Normative names for modules, events, APIs, schemas, files and tests |
| STD-003 | Coding Guidelines | Planned | F | Language rules, errors, ownership, concurrency and review |
| STD-004 | Documentation Standard | Complete | F | `docs/10_Project_Standards/STD-004_*_v1_0_Current.docx`; required sections, statuses, traceability and QA |
| STD-005 | Git Workflow | Planned | F | Branch lifecycle, review, validation and integration |
| STD-006 | Commit Convention | Planned | F | Commit types, scope, evidence and atomicity |
| STD-007 | Branch Strategy | Planned | F | Protected branches, worktrees and release/hotfix rules |
| STD-008 | Release Process | Planned | P | Build, version, package, sign, publish and rollback |
| STD-009 | Contribution Guide | Planned | P | Environment, changes, review and contributor expectations |
| STD-010 | README | Planned | P | Full onboarding and project navigation; short root file is insufficient |

## 22. Этап 12 — Research

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| RES-001 | Skyrim Integration | Planned | J | SE/AE runtime matrix, hooks, constraints and reproducible findings |
| RES-002 | Plugin Detection Research | Planned | L | Formats, change detection, scale and risk evidence |
| RES-003 | Local AI Research | Planned | J | Model/runtime comparison against project workloads |
| RES-004 | VR Research | Planned | L | Separate VR compatibility study; no implicit enablement |
| RES-005 | Performance Research | Planned | P | Profiling evidence and capacity model |
| RES-006 | Other Games Research | Planned | L | Adapter feasibility after Skyrim baseline |

## 23. Этап 13 — Knowledge Base

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| KB-001 | Future Ideas | Planned | L | Prioritized idea backlog with provenance and non-scope label |
| KB-002 | NPC Design | Planned | J | Behavior concepts separated from normative module contracts |
| KB-003 | Organization Design | Planned | J | Domain concepts before MOD-006 |
| KB-004 | Economy Research | Planned | L | Models and trade-offs before MOD-008 |
| KB-005 | Politics Research | Planned | L | Models and trade-offs before MOD-009 |
| KB-006 | Gameplay Ideas | Planned | L | Curated backlog linked to product roadmap |
| KB-007 | Lore Ideas | Planned | L | Provenance, canon policy and content boundaries |
| KB-008 | AI Ideas | Planned | L | Experiments clearly separated from accepted AI architecture |
| KB-009 | Future Features | Planned | L | Version-targeted backlog and dependencies |
| KB-010 | Scratchpad | Planned | P | Noncanonical capture area with promotion rules |

## 24. Этап 14 — IHK

| ID | Артефакт | Статус | Класс | Условие закрытия |
|---|---|---|---|---|
| IHK-001 | Feasibility Study | Planned | P | Technical, schedule and operational feasibility |
| IHK-002 | Cost Estimation | Planned | P | Assumptions, one-time and recurring costs |
| IHK-003 | Resource Planning | Planned | P | Roles, availability, tools and constraints |
| IHK-004 | Time Planning | Planned | P | Work packages, dependencies and critical path |
| IHK-005 | Financial Planning | Planned | P | Funding, operating cost and sensitivity |
| IHK-006 | Risk Analysis | Planned | P | IHK-compatible view derived from RISK baseline |
| IHK-007 | Project Presentation | Planned | P | Final presentation based on verified evidence |
| IHK-008 | Demo Scenario | Planned | P | Repeatable end-to-end demonstration and recovery plan |

## 25. Зависимости между пакетами

Foundation не требует полного физического проектирования всех будущих таблиц. Сначала фиксируются логические правила и владельцы, затем физическая детализация создаётся just in time вместе с модулем. Аналогично AI prompts и performance thresholds должны опираться на реальные workloads, а не на догадки.

| Источник | Обязательный потребитель | Контроль |
|---|---|---|
| STD-001/002/004 | Все последующие документы | Термины, ID, шаблон и статусная модель едины |
| TEST-001/006 | Каждый module gate и release | Каждое требование имеет проверку |
| RISK-001/002 | Architecture, modules, releases | Риск имеет владельца и остаточную оценку |
| DB-001–003/007/008 | MOD-003, MOD-013 и durable state | Нет записи без ownership и migration path |
| EVT-001–006 | MOD-001, MOD-002, MOD-011 и simulation modules | Межмодульные факты типизированы и версионированы |
| API-001/005/006 | Все modules and adapters | Зависимости проходят только через ports/contracts |
| MOD specification | Код конкретного модуля | Нет production implementation без Design Baseline |

## 26. Контрольные точки программы

| Milestone | Содержание | Definition of Done |
|---|---|---|
| M-DOC-0 | PLAN-001 опубликован | RU/DE, 116-item register, QA и canonical index |
| M-DOC-1 | Governance baseline | DOC-009 и STD-001–007 Current; владельцы и change control назначены |
| M-DOC-2 | Verification baseline | TEST-001–003/006 и RISK-001–003 Current; traceability format работает |
| M-DOC-3 | Persistence baseline | DB logical package и recovery policy согласованы с ARCH-011/MOD-003 |
| M-DOC-4 | Event/API baseline | EVT package и core APIs Current; compatibility tests определены |
| M-IMP-1 | Первый разрешённый implementation gate | Выбран модуль, закрыты его gaps, владелец одобрил старт |
| M-REL-1 | Первый проверяемый вертикальный срез | Skyrim SE/AE event round-trip, persistence и recovery доказаны |

## 27. Change control

Любое изменение канонического документа должно указывать причину, затронутые требования, совместимость, миграцию, тесты и архивируемую предыдущую версию. Изменение архитектурного принципа требует ADR. Изменение только редакционной ясности может пройти document revision без нового ADR, если смысл контракта не меняется.

PLAN-001 обновляется после публикации или supersede любого учитываемого артефакта. Progress count меняется только после проверки канонической пары и внесения её в индекс. Незавершённый draft не повышает numerator.

## 28. Ответственность и ритм контроля

| Роль | Ответственность |
|---|---|
| Project owner | Scope, priorities, acceptance of material changes and implementation authorization |
| Architecture owner | Cross-document consistency, dependencies and ADR enforcement |
| Document owner | Completeness, bilingual parity, traceability and lifecycle |
| Module owner | Specification accuracy, code conformance and test evidence |
| QA owner | Render, accessibility, structural, test and release evidence |
| Risk owner | Trigger monitoring, mitigation and residual-risk acceptance |

Рекомендуемый ритм: обновление Developer Journal после каждого значимого рабочего прохода; проверка PLAN-001 при закрытии документа; review Risk Register минимум на каждом foundation gate и release milestone; полный consistency audit перед слиянием foundation baseline в основную ветку.

## 29. Основные риски документационной программы

| Риск | Ранний сигнал | Контроль |
|---|---|---|
| Документы создаются ради количества | Много страниц без testable clauses | Acceptance criteria и evidence обязательны |
| Документация опережает знания слишком далеко | Физические схемы и бюджеты основаны на предположениях | Just-in-time детализация и явные hypotheses |
| Код опережает контракты | Новые public types или durable data без ID требований | Foundation freeze и module gate |
| Статус Complete трактуется как реализовано | Нет build/runtime evidence | Раздельные document и implementation statuses |
| RU/DE расходятся по смыслу | Разные IDs, таблицы или числовые ограничения | Parity audit до публикации |
| Старые версии продолжают использоваться | Несколько активных файлов без преемника | Canonical index и archive/superseded links |
| Skyrim VR случайно попадает в scope | Общие формулировки без platform matrix | Явное исключение до отдельного ADR |

## 30. Следующее утверждённое действие

F1 Governance продолжается после завершения DOC-009, STD-001, STD-002 и STD-004. Следующая последовательность: STD-003 Coding Guidelines; STD-005 Git Workflow; STD-006 Commit Convention; STD-007 Branch Strategy. STD-008–010 закрываются ближе к первому release baseline.

MOD-004 не следует начинать раньше минимальных F1–F5, несмотря на готовность MOD-003 design baseline. Это не отмена разработки, а защита от повторного переписывания событий, API, persistence и тестовых контрактов.

## Приложение A. Итоговая матрица прогресса

| Группа | Complete / Accepted | Planned | Всего |
|---|---:|---:|---:|
| Project Management | 9 | 0 | 9 |
| Architecture | 12 | 0 | 12 |
| ADR | 9 | 0 | 9 |
| Module Specifications | 4 | 11 | 15 |
| Database | 0 | 8 | 8 |
| Event System | 0 | 6 | 6 |
| API | 0 | 6 | 6 |
| AI | 0 | 7 | 7 |
| Testing | 0 | 6 | 6 |
| Risk Management | 0 | 4 | 4 |
| Project Standards | 3 | 7 | 10 |
| Research | 0 | 6 | 6 |
| Knowledge Base | 0 | 10 | 10 |
| IHK | 0 | 8 | 8 |
| **Итого** | **37** | **79** | **116** |

## Приложение B. Проверка арифметики и полноты

Количество строк по этапам: `9 + 12 + 9 + 15 + 8 + 6 + 6 + 7 + 6 + 4 + 10 + 6 + 10 + 8 = 116`. Завершённые строки: `9 + 12 + 9 + 4 + 3 = 37`. Остаток: `116 − 37 = 79`. Доля выполнения: `37 / 116 = 31,9 %`.

PLAN-001 учитывается в каноническом индексе как control artifact, но не включается в эти 116 delivery items. QA-отчёты, coverage registers, audit records и archived snapshots также являются evidence/control artifacts и не меняют delivery denominator.

## Приложение C. Traceability baseline

| Требование | Управляющий источник | Доказательство |
|---|---|---|
| Активная платформа Skyrim SE/AE | ADR-009, MOD-002 | Platform matrix и in-game acceptance |
| Skyrim VR исключён | ADR-009, PLAN-001 | Нет VR target в текущих implementation gates |
| Dependency direction | ARCH-004, ARCH-005, ADR-008 | Contract audit и dependency tests |
| Event-driven integration | ADR-002, ARCH-003, ARCH-007 | EVT package и integration tests |
| Persistence and atomic restore | ADR-003, ADR-005, ARCH-011 | DB package, Save API и recovery tests |
| Memory ownership | MOD-003, ADR-008 | Repository/port contracts и module tests |
| AI is not source of truth | ARCH-012, ADR-004, MOD-003 | Validation and deterministic fallback tests |
| Documentation before module code | DOC-004, PLAN-001 | F6 gate record |

## Приложение D. Журнал изменений

| Версия | Дата | Изменение |
|---|---|---|
| 2.2 | 2026-09-05 | DOC-009 Developer Journal и STD-004 Documentation Standard переведены в Complete / Current; прогресс обновлён до 37/116; в F1 остались STD-003 и STD-005–007. |
| 2.1 | 2026-09-05 | STD-001 Glossary и STD-002 Naming Convention переведены в Complete; прогресс обновлён до 35/116; состав минимального F1 исправлен на DOC-009 и STD-001–007. |
| 2.0 | 2026-09-05 | Создан полный двуязычный управляющий план; исправлены 28/92 на 33/116; добавлены ADR-008/009, MOD-002/003, статусная модель, Foundation Pack, 116-item registry, gates и acceptance rules. |
