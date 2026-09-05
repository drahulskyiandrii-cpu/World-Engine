# DOC-009 — World Engine Developer Journal

Версия 1.0 · Русский · 2026-09-05 · Current / Living

## 1. Назначение

DOC-009 — живой журнал разработки World Engine. Он сохраняет хронологию решений, экспериментов, блокеров, аудитов, изменений документов и фактических результатов.

Журнал не заменяет PLAN-001, ADR, issue tracker, Git history, test report или module specification. Он связывает их и объясняет, что произошло, почему и какое evidence существует.

## 2. Область действия

Журнал охватывает управление, архитектуру, MOD-001–MOD-014, базу данных, events, APIs, AI, tests, risks, standards, research, releases и IHK. В текущей фазе платформа — Skyrim SE/AE; Skyrim VR остаётся Excluded.

Запись требуется для material work pass: принятого решения, изменения baseline, существенного эксперимента, нового блокера, закрытия gate, публикации или отката.

## 3. Статусная оговорка

DOC-009 v1.0 является Current / Living после прохождения document QA. Текущая запись не доказывает, что код реализован. Для этого нужны source/build/test/runtime evidence.

| Слой | Текущая истина |
|---|---|
| Documentation | Architecture и ADR complete; MOD-001–003 design documents current; foundation не завершён |
| Implementation | MOD-002 имеет prototype evidence; полная in-game validation pending; MOD-001/MOD-003 code completion не заявлена |
| Platform | Skyrim SE/AE active; Skyrim VR excluded |
| Branch policy | Foundation work в `docs/pre-code-foundation-plan`; main не изменяется до gates и integration decision |

## 4. Принципы журнала

1. Facts, decisions, plans, assumptions и opinions не смешиваются.
2. Каждая проверяемая ключевая фраза ссылается на evidence или явно маркируется Pending.
3. Запись пишется для будущего читателя без доступа к чату.
4. Неудача и откат сохраняются; история не переписывается под успех.
5. Секреты, personal data и ненужные machine-specific данные не записываются.

## 5. Владелец и аудитория

Project Owner и Documentation Lead — Andrii Drahulskyi. Автор material work pass отвечает за черновик записи и evidence links; Owner — за правильность decision/scope status; reviewer — за обнаружение неподтверждённых claims.

Аудитория: владелец проекта, будущие contributors/reviewers, IHK evaluator и операционный исполнитель.

## 6. Идентификатор записи

Формат: `JRN-YYYYMMDD-NNN`. Номер уникален в пределах даты и не переиспользуется. Дополнительные ссылки: document IDs, module ID, ADR, risk ID, test ID, issue/commit/branch и release tag.

## 7. Жизненный цикл записи

`Draft → Reviewed → Current → Corrected/Closed`. Draft может содержать Pending evidence. Reviewed означает, что claims сверены. Current запись входит в журнал. Позднейшая ошибка исправляется correction-записью, а не тихим удалением истории.

## 8. Обязательные поля

| Поле | Требование |
|---|---|
| Entry ID, date/time, author | Однозначная identity и ordering |
| Type и status | Decision, Experiment, Blocker, Audit, Change, Implementation, Risk, Release или Handoff |
| Scope | Затронутые document/module/platform/branch |
| Context | Какая проблема/цель вызвала работу |
| Action/result | Что сделано и что фактически получено |
| Decision/truth | Что принято; что не доказано |
| Evidence | Canonical paths, commit, logs, tests, hashes, screenshots или Pending |
| Impact/next | Dependencies, risks, follow-up, Owner и due/gate |

## 9. Классы evidence

| Класс | Пример | Что доказывает |
|---|---|---|
| E1 Source | Canonical document/source/code path | Существование и содержание artifact |
| E2 Version control | Branch, commit, diff, tag | Какое change set было зафиксировано |
| E3 Static/structural | Lint, schema, document/a11y audit | Формальную корректность |
| E4 Build/test | Reproducible command and result | Buildable или tested behavior |
| E5 Runtime/in-game | Scenario, environment, observed result | Поведение в целевой среде |
| E6 Operational | Monitoring, recovery, rollback record | Эксплуатационную готовность |

Higher evidence не отменяет lower: in-game run без версии и конфигурации невоспроизводим.

## 10. Decision entry

Decision entry используется для локального обратимого решения. Если решение меняет cross-module architecture, ownership, persistence, compatibility или platform scope, нужен ADR. Запись ссылается на ADR, но не подменяет его.

## 11. Experiment entry

Эксперимент фиксирует hypothesis, setup, controlled variables, input, expected result, actual result, artifacts, limitations и conclusion. Неудачный опыт сохраняется, если он снижает неопределённость.

## 12. Blocker entry

Blocker имеет severity, affected gate, observed symptom, confirmed cause или hypotheses, attempts, required authority/input, Owner и next review. Статус `Blocked` не используется для обычной сложности; нужна реальная невозможность продолжать без внешнего изменения.

## 13. Incident и defect entry

Запись дефекта содержит expected/actual, reproduction, scope, severity, suspected/confirmed cause, workaround, fix, regression test и residual impact. Изменение в документе и изменение runtime behavior регистрируются раздельно.

## 14. Audit entry

Audit entry фиксирует scope, baseline/ref, method, inspected items, findings by severity, corrections, remaining gates и final disposition. Фраза «весь репозиторий проверен» допустима только с inventory и описанными exclusions.

## 15. Document change entry

Запись указывает document IDs/versions, reason, semantic changes, visual changes, superseded artifacts, RU/DE parity, render page counts, accessibility result, hashes и canonical-index update. Простое создание DOCX не означает Complete.

## 16. Module implementation entry

Для MOD-001–MOD-014 запись ведёт три раздельные оси: document status, implementation status, validation status. Она ссылается на module requirements, work package, code paths, builds, tests и runtime evidence.

| Утверждение | Нужное evidence |
|---|---|
| Module document Current | Final RU/DE files, QA report, index |
| Implementation started | Explicit authorization, branch/work package, initial code change |
| Implementation complete | Scope-complete code, build и traceability |
| Module verified | Blocking test matrix Passed |
| In-game ready | Target-runtime acceptance scenarios Passed |

## 17. Risk entry

Запись не заменяет RISK register. Она фиксирует discovery/change: risk ID, trigger, probability/impact change, immediate response, Owner, mitigation/contingency и ссылку на current risk artifact.

## 18. Release и deployment entry

Запись включает release/version, included scope, environment, artifacts/hashes, migrations, test summary, known issues, backup/rollback plan, deployment result и post-deployment verification. Без runtime evidence статус не выше Release Candidate.

## 19. Handoff entry

Handoff записывает current branch/ref, clean/dirty state, completed work, open changes, commands/checks already run, exact blocker, next safe action и paths to evidence. Секреты и credentials не передаются.

## 20. Ритм записей

| Событие | Когда запись обязательна |
|---|---|
| Material work pass | До commit/push или в том же change set |
| Decision/authorization | Немедленно с точным scope |
| Experiment | После получения result, включая failure |
| Blocker/incident | При подтверждении impact |
| Gate/release | До объявления status |
| No-change review | Если cadence требует review, но изменений нет |

## 21. Коррекции и append-only history

Существенная ошибка исправляется новой записью, которая ссылается на исходную, показывает incorrect claim, corrected truth, cause и impact. Тихая правка допустима только для орфографии, не меняющей смысл.

## 22. Секреты, privacy и размер

Journal MUST NOT содержать access tokens, passwords, private keys, personal identifiers, private messages или full environment dumps. Ссылка на secret описывает location class и Owner, но не value. Длинные logs хранятся отдельно; журнал ссылается на них и кратко резюмирует.

## 23. Связь с Git и repository

Когда запись описывает committed change, она указывает branch и commit hash после его появления. Планируемый commit маркируется Pending. Запись не может утверждать «в GitHub» до успешного push и проверки upstream.

## 24. Связь с PLAN-001 и canonical index

При закрытии deliverable в одном change set обновляются artifact, QA report, canonical index, PLAN-001 и journal entry. Числа прогресса в журнале всегда ссылаются на версию PLAN-001.

## 25. Модульный реестр MOD-001–MOD-014

| ID | Назначение | Document status | Implementation status |
|---|---|---|---|
| MOD-001 | Core | Complete Design Baseline | Not claimed complete |
| MOD-002 | Game Adapter | Current v2.0 | Prototype; build/in-game validation pending |
| MOD-003 | Memory Engine | Complete Design Baseline | Not Started |
| MOD-004 | Character Engine | Planned | Not Started |
| MOD-005 | Dialogue Engine | Planned | Not Started |
| MOD-006 | Organization Engine | Planned | Not Started |
| MOD-007 | Relationship Engine | Planned | Not Started |
| MOD-008 | Economy Engine | Planned | Not Started |
| MOD-009 | Politics Engine | Planned | Not Started |
| MOD-010 | Quest Engine | Planned | Not Started |
| MOD-011 | Event Engine | Planned | Not Started |
| MOD-012 | Scheduler | Planned | Not Started |
| MOD-013 | Save Manager | Planned | Not Started |
| MOD-014 | Voice Engine | Planned | Not Started |

Реестр обновляется только с evidence. Появление полноценного MOD-документа меняет Document status, но не Implementation status.

## 26. Критерии качества записи

Запись проходит review, если она понятна без чата; facts отделены от assumptions; decisions и Owner явны; claims привязаны к evidence; failure не скрыт; next action выполним; secrets отсутствуют; IDs и даты корректны.

## 27. Архивирование и retention

Текущая версия журнала хранит active project history. При управляемом разделении новая current version содержит index и boundary date, а прежняя становится read-only archive. Записи о решениях, releases, incidents и потере данных не удаляются.

## 28. Review и acceptance DOC-009

DOC-009 v1.0 считается Current / Living, если RU/DE пара имеет одинаковую структуру; entry types и evidence model полны; MOD-001–MOD-014 охвачены; initial records проверены по repository evidence; все страницы отрендерены и просмотрены; structural/accessibility QA пройден; index и PLAN-001 обновлены.

## 29. Начальный реестр записей

| Entry ID | Тип | Краткий результат | Evidence status |
|---|---|---|---|
| JRN-20260901-001 | Audit | Начат architecture-consistency audit в audit branch; main не изменялся | Repository history |
| JRN-20260904-001 | Document change | Выполнен full-document rebuild active set с архивом superseded files | Canonical index, audit records |
| JRN-20260904-002 | Decision | MOD-002 authorized для Skyrim SE/AE; Skyrim VR excluded | Owner decision, ADR-009 |
| JRN-20260905-001 | Module | MOD-002 v2.0 design baseline current; prototype exists; build/in-game validation pending | MOD-002 docs, ADR-009, repository |
| JRN-20260905-002 | Module | MOD-003 full design specification current; implementation Not Started | MOD-003 docs and QA |
| JRN-20260905-003 | Governance | PLAN-001 v2.0 исправил программу до 116 позиций; status 33/116 | PLAN-001 v2.0 QA/history |
| JRN-20260905-004 | Standard | STD-001/002 current; PLAN-001 v2.1 status 35/116 | Canonical files, QA, commit 602ec87 |
| JRN-20260905-005 | Standard | DOC-009/STD-004 current; PLAN-001 v2.2 status 37/116 | Final files/QA; commit assigned after publication |

## 30. Детальные начальные записи

### JRN-20260901-001 — Architecture consistency audit

Type: Audit. Scope: ARCH-001–012, ADR, module and deployment/configuration consistency. Action: audit work isolated in `audit/architecture-consistency-2026-09-01`; full rebuild and formal artifact QA continued in successive passes. Truth: audit evidence belongs to the audit branch/history; main was intentionally not changed during gates. Evidence: architecture audit record, canonical index and Git history. Next: preserve gate evidence during foundation work.

### JRN-20260904-001 — Full-document rebuild

Type: Document change. Action: active documents were rebuilt as full-content RU/DE deliverables; prior snapshots were retained under dated archive paths. Result: canonical index distinguishes current full documents from superseded material. Truth: page count alone was not acceptance; render, structure and accessibility checks were required. Evidence: AUDIT-030, coverage/QA register and archive tree.

### JRN-20260904-002 — MOD-002 scope authorization

Type: Decision. Decision owner: Andrii Drahulskyi. Decision: implementation work for MOD-002 may target Skyrim SE/AE; Skyrim VR is excluded. Governing decision: ADR-009. Constraint: authorization does not claim build or in-game success. Next: close applicable foundation and runtime-validation gates.

### JRN-20260905-001 — MOD-002 truth

Type: Module implementation. Document status: Current v2.0 full implementation specification. Implementation status: Prototype. Validation status: build and in-game acceptance Pending. Evidence: MOD-002 RU/DE v2.0 documents, ADR-009 and repository prototype paths. Prohibited claim: “MOD-002 complete” until build, tests and target-game evidence pass.

### JRN-20260905-002 — MOD-003 design baseline

Type: Module documentation. Result: RU/DE v1.0 full design and implementation specifications produced with 60 numbered sections and appendices. Document status: Complete Design Baseline. Implementation status: Not Started. Evidence: canonical MOD-003 files and QA report. Next: no implementation without explicit decision and applicable foundation gates.

### JRN-20260905-003 — Master plan correction

Type: Governance. Result: PLAN-001 v2.0 counted 116 delivery artifacts instead of the incorrect 92 summary and separated document readiness from code readiness. Baseline at publication: 33/116. Control artifact PLAN-001 is excluded from the denominator. Evidence: PLAN-001 v2.0 source, DOCX pair and QA history.

### JRN-20260905-004 — Glossary and naming baseline

Type: Standard. Result: STD-001 Glossary and STD-002 Naming Convention became Current; PLAN-001 v2.1 moved evidence-backed progress to 35/116. Module impact: terminology and names apply to all MOD documents; existing public MOD-002 identifiers remain stable until controlled migration. Evidence: canonical RU/DE files, QA reports and commit `602ec87` on `docs/pre-code-foundation-plan`.

### JRN-20260905-005 — Documentation standard and journal

Type: Standard / Governance. Result: STD-004 defines mandatory structure/status/traceability/QA, including full MOD-001–MOD-014 requirements; DOC-009 establishes this Living journal. PLAN-001 v2.2 moves progress to 37/116 and leaves STD-003, STD-005, STD-006 and STD-007 to close F1. Evidence: final RU/DE files and QA reports. Git commit: Pending until atomic publication. Next: publish, verify upstream, then continue the four remaining F1 standards.

## Приложение A. Универсальный шаблон записи

| Поле | Значение |
|---|---|
| Entry ID / Date / Author | `JRN-YYYYMMDD-NNN`, ISO timestamp, author |
| Type / Status | Entry type; Draft/Reviewed/Current/Closed |
| Scope | Documents, modules, platform, branch/ref |
| Context | Trigger, problem or objective |
| Facts | Observed and evidenced statements |
| Decision / Assumptions | Explicit separation with Owner |
| Action / Result | Work performed and actual outcome |
| Evidence | Paths, commit, tests, logs, hashes or Pending |
| Impact / Risks | Affected contracts, gates and residual risk |
| Next | Action, Owner, due date/gate |

## Приложение B. Шаблон эксперимента

| Поле | Содержание |
|---|---|
| Hypothesis | Falsifiable expected relationship |
| Environment | Versions, hardware, configuration, seed |
| Method | Steps and controlled variables |
| Expected / Actual | Quantified comparison |
| Evidence | Raw output/artifacts and retention path |
| Limitations | Bias, missing controls, applicability |
| Conclusion | Supported/rejected/inconclusive |
| Follow-up | Decision or next experiment with Owner |

## Приложение C. Шаблон блокера

| Поле | Содержание |
|---|---|
| Severity / Gate | Impact and blocked milestone |
| Symptom | Reproducible observed behavior |
| Cause | Confirmed cause or ranked hypotheses |
| Attempts | Actions and results already tried |
| Needed input | Exact authority, information or external change |
| Owner / Review | Responsible person and next review time |
| Workaround | Safe temporary path and its risk |
| Closure evidence | Result proving the blocker is removed |

## Приложение D. Шаблон module status

| Поле | Содержание |
|---|---|
| Module / Work package | MOD-ID and bounded scope |
| Document status | Planned/Draft/Design Baseline/Current |
| Implementation status | Not Started/Prototype/Implemented |
| Validation status | Not Run/Partial/Passed |
| Contracts changed | Ports, events, schemas, config and compatibility |
| Evidence | Source paths, commit, build/tests/runtime |
| Deviations | Spec mismatch, Owner and resolution |
| Next gate | Exact pass conditions |

## Приложение E. Журнал изменений DOC-009

| Версия | Дата | Статус | Изменение |
|---|---|---|---|
| 1.0 | 2026-09-05 | Current / Living | Создан evidence-based Developer Journal с entry lifecycle, templates, initial verified history и отдельным учётом MOD-001–MOD-014. |
