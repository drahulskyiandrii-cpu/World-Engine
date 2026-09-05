# DOC-009 — World Engine Developer Journal

Version 1.0 · Deutsch · 2026-09-05 · Current / Living

## 1. Zweck

DOC-009 ist das Living Developer Journal von World Engine. Es bewahrt die Chronologie von Entscheidungen, Experimenten, Blockern, Audits, Dokumentänderungen und tatsächlichen Ergebnissen.

Das Journal ersetzt weder PLAN-001 noch ADR, Issue Tracker, Git History, Test Report oder Module Specification. Es verbindet diese und erklärt, was geschah, warum es geschah und welche Evidence existiert.

## 2. Geltungsbereich

Das Journal umfasst Governance, Architecture, MOD-001–MOD-014, Database, Events, APIs, AI, Tests, Risks, Standards, Research, Releases und IHK. In der aktuellen Phase ist Skyrim SE/AE die Plattform; Skyrim VR bleibt Excluded.

Ein Eintrag ist für jeden Material Work Pass erforderlich: angenommene Entscheidung, Baseline-Änderung, wesentliches Experiment, neuer Blocker, Gate-Abschluss, Publication oder Rollback.

## 3. Statushinweis

DOC-009 v1.0 ist nach bestandenem Document QA Current / Living. Ein Journal-Eintrag beweist nicht, dass Code implementiert ist. Dafür sind Source-, Build-, Test- und Runtime-Evidence erforderlich.

| Ebene | Aktuelle Wahrheit |
|---|---|
| Documentation | Architecture und ADR complete; MOD-001–003 Design Documents current; Foundation nicht abgeschlossen |
| Implementation | MOD-002 besitzt Prototype Evidence; vollständige In-game Validation pending; Codeabschluss für MOD-001/MOD-003 nicht behauptet |
| Platform | Skyrim SE/AE active; Skyrim VR excluded |
| Branch Policy | Foundation Work in `docs/pre-code-foundation-plan`; Main bleibt bis Gates und Integration Decision unverändert |

## 4. Journalprinzipien

1. Facts, Decisions, Plans, Assumptions und Opinions werden nicht vermischt.
2. Jede wichtige verifizierbare Aussage verweist auf Evidence oder ist als Pending markiert.
3. Der Eintrag ist für einen späteren Leser ohne Chat-Zugriff verständlich.
4. Failure und Rollback bleiben erhalten; Geschichte wird nicht in Erfolg umgeschrieben.
5. Secrets, Personal Data und unnötige Machine-specific Daten werden nicht erfasst.

## 5. Owner und Audience

Project Owner und Documentation Lead ist Andrii Drahulskyi. Der Autor eines Material Work Pass verantwortet Draft und Evidence Links; der Owner den korrekten Decision/Scope Status; der Reviewer die Erkennung unbelegter Claims.

Audience: Project Owner, künftige Contributors/Reviewers, IHK Evaluator und Operational Executor.

## 6. Entry Identifier

Format: `JRN-YYYYMMDD-NNN`. Die Nummer ist innerhalb des Datums eindeutig und wird nicht wiederverwendet. Zusätzliche Referenzen sind Document IDs, Module ID, ADR, Risk ID, Test ID, Issue/Commit/Branch und Release Tag.

## 7. Entry Lifecycle

`Draft → Reviewed → Current → Corrected/Closed`. Draft darf Pending Evidence enthalten. Reviewed bedeutet, dass Claims abgeglichen wurden. Current ist Teil des Journals. Ein später erkannter Fehler wird durch einen Correction Entry berichtigt, nicht durch stille Löschung.

## 8. Pflichtfelder

| Feld | Anforderung |
|---|---|
| Entry ID, Date/Time, Author | Eindeutige Identity und Ordering |
| Type und Status | Decision, Experiment, Blocker, Audit, Change, Implementation, Risk, Release oder Handoff |
| Scope | Betroffene Document/Module/Platform/Branch |
| Context | Problem oder Ziel, das die Arbeit auslöste |
| Action/Result | Was getan und tatsächlich erreicht wurde |
| Decision/Truth | Was angenommen und was nicht bewiesen ist |
| Evidence | Canonical Paths, Commit, Logs, Tests, Hashes, Screenshots oder Pending |
| Impact/Next | Dependencies, Risks, Follow-up, Owner und Due/Gate |

## 9. Evidence Classes

| Klasse | Beispiel | Beweist |
|---|---|---|
| E1 Source | Canonical Document/Source/Code Path | Existenz und Inhalt eines Artifact |
| E2 Version Control | Branch, Commit, Diff, Tag | Welches Change Set erfasst wurde |
| E3 Static/Structural | Lint, Schema, Document/A11y Audit | Formale Korrektheit |
| E4 Build/Test | Reproducible Command and Result | Buildable oder Tested Behavior |
| E5 Runtime/In-game | Scenario, Environment, Observed Result | Verhalten in der Zielumgebung |
| E6 Operational | Monitoring, Recovery, Rollback Record | Operational Readiness |

Höhere Evidence ersetzt niedrigere nicht: Ein In-game Run ohne Version und Konfiguration ist nicht reproduzierbar.

## 10. Decision Entry

Ein Decision Entry wird für eine lokale, reversible Entscheidung verwendet. Verändert die Entscheidung Cross-Module Architecture, Ownership, Persistence, Compatibility oder Platform Scope, ist ein ADR erforderlich. Der Entry verweist auf das ADR, ersetzt es aber nicht.

## 11. Experiment Entry

Ein Experiment erfasst Hypothesis, Setup, Controlled Variables, Input, Expected Result, Actual Result, Artifacts, Limitations und Conclusion. Ein fehlgeschlagener Versuch bleibt erhalten, wenn er Uncertainty reduziert.

## 12. Blocker Entry

Ein Blocker besitzt Severity, Affected Gate, Observed Symptom, Confirmed Cause oder Hypotheses, Attempts, Required Authority/Input, Owner und Next Review. `Blocked` wird nicht für normale Schwierigkeit verwendet; es erfordert echte Unmöglichkeit ohne externe Änderung.

## 13. Incident und Defect Entry

Ein Defect Entry enthält Expected/Actual, Reproduction, Scope, Severity, Suspected/Confirmed Cause, Workaround, Fix, Regression Test und Residual Impact. Dokumentänderung und Runtime-Behavior-Änderung werden getrennt erfasst.

## 14. Audit Entry

Ein Audit Entry erfasst Scope, Baseline/Ref, Method, Inspected Items, Findings nach Severity, Corrections, Remaining Gates und Final Disposition. „Gesamtes Repository geprüft“ ist nur mit Inventory und beschriebenen Exclusions zulässig.

## 15. Document Change Entry

Der Entry nennt Document IDs/Versions, Reason, Semantic Changes, Visual Changes, Superseded Artifacts, RU/DE Parity, Render Page Counts, Accessibility Result, Hashes und Canonical-Index Update. Die bloße Erzeugung eines DOCX bedeutet nicht Complete.

## 16. Module Implementation Entry

Für MOD-001–MOD-014 führt der Entry drei getrennte Achsen: Document Status, Implementation Status, Validation Status. Er verweist auf Module Requirements, Work Package, Code Paths, Builds, Tests und Runtime Evidence.

| Aussage | Erforderliche Evidence |
|---|---|
| Module Document Current | Final RU/DE Files, QA Report, Index |
| Implementation Started | Explizite Authorization, Branch/Work Package, initiale Codeänderung |
| Implementation Complete | Scope-complete Code, Build und Traceability |
| Module Verified | Blocking Test Matrix Passed |
| In-game Ready | Target-runtime Acceptance Scenarios Passed |

## 17. Risk Entry

Der Entry ersetzt das RISK Register nicht. Er erfasst Discovery/Change: Risk ID, Trigger, Probability/Impact Change, Immediate Response, Owner, Mitigation/Contingency und Verweis auf das Current Risk Artifact.

## 18. Release und Deployment Entry

Der Entry enthält Release/Version, Included Scope, Environment, Artifacts/Hashes, Migrations, Test Summary, Known Issues, Backup/Rollback Plan, Deployment Result und Post-deployment Verification. Ohne Runtime Evidence ist der Status höchstens Release Candidate.

## 19. Handoff Entry

Handoff erfasst Current Branch/Ref, Clean/Dirty State, Completed Work, Open Changes, bereits ausgeführte Commands/Checks, exakten Blocker, Next Safe Action und Paths to Evidence. Secrets und Credentials werden nicht übergeben.

## 20. Entry Cadence

| Ereignis | Wann der Eintrag verpflichtend ist |
|---|---|
| Material Work Pass | Vor Commit/Push oder im selben Change Set |
| Decision/Authorization | Sofort mit genauem Scope |
| Experiment | Nach Result, einschließlich Failure |
| Blocker/Incident | Sobald der Impact bestätigt ist |
| Gate/Release | Vor Bekanntgabe des Status |
| No-change Review | Wenn Cadence einen Review verlangt, aber keine Änderung vorliegt |

## 21. Korrekturen und Append-only History

Ein wesentlicher Fehler wird durch einen neuen Entry korrigiert, der auf den ursprünglichen verweist und Incorrect Claim, Corrected Truth, Cause und Impact zeigt. Stille Korrektur ist nur für bedeutungsneutrale Orthografie erlaubt.

## 22. Secrets, Privacy und Größe

Das Journal MUST NOT Access Tokens, Passwords, Private Keys, Personal Identifiers, Private Messages oder Full Environment Dumps enthalten. Ein Secret-Verweis nennt Location Class und Owner, nie den Wert. Lange Logs werden separat gespeichert; das Journal verlinkt und fasst zusammen.

## 23. Verbindung zu Git und Repository

Beschreibt ein Entry eine committed Change, nennt er Branch und Commit Hash nach dessen Entstehung. Ein geplanter Commit wird als Pending markiert. „In GitHub“ darf erst nach erfolgreichem Push und Upstream-Prüfung behauptet werden.

## 24. Verbindung zu PLAN-001 und Canonical Index

Beim Abschluss eines Deliverable werden im selben Change Set Artifact, QA Report, Canonical Index, PLAN-001 und Journal Entry aktualisiert. Progress-Zahlen im Journal verweisen immer auf eine PLAN-001-Version.

## 25. Modulregister MOD-001–MOD-014

| ID | Zweck | Document Status | Implementation Status |
|---|---|---|---|
| MOD-001 | Core | Complete Design Baseline | Not claimed complete |
| MOD-002 | Game Adapter | Current v2.0 | Prototype; Build/In-game Validation pending |
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

Das Register wird nur mit Evidence aktualisiert. Ein vollständiges MOD-Dokument ändert den Document Status, nicht den Implementation Status.

## 26. Qualitätskriterien eines Entry

Ein Entry besteht den Review, wenn er ohne Chat verständlich ist; Facts von Assumptions trennt; Decision und Owner nennt; Claims mit Evidence verbindet; Failure nicht verschweigt; eine ausführbare Next Action besitzt; keine Secrets enthält; IDs und Dates korrekt sind.

## 27. Archivierung und Retention

Die Current-Version des Journals enthält die aktive Project History. Bei kontrollierter Aufteilung besitzt die neue Current-Version einen Index und ein Boundary Date; die frühere wird Read-only Archive. Einträge zu Decisions, Releases, Incidents und Data Loss werden nicht gelöscht.

## 28. Review und Acceptance für DOC-009

DOC-009 v1.0 ist Current / Living, wenn das RU/DE-Paar dieselbe Struktur besitzt; Entry Types und Evidence Model vollständig sind; MOD-001–MOD-014 abgedeckt sind; Initial Records gegen Repository Evidence geprüft sind; alle Seiten gerendert und angesehen wurden; Structural/Accessibility QA bestanden ist; Index und PLAN-001 aktualisiert sind.

## 29. Initiales Entry Register

| Entry ID | Typ | Kurzergebnis | Evidence Status |
|---|---|---|---|
| JRN-20260901-001 | Audit | Architecture-consistency Audit in Audit Branch begonnen; Main unverändert | Repository History |
| JRN-20260904-001 | Document Change | Full-document Rebuild des Active Set mit Archive für Superseded Files | Canonical Index, Audit Records |
| JRN-20260904-002 | Decision | MOD-002 für Skyrim SE/AE autorisiert; Skyrim VR excluded | Owner Decision, ADR-009 |
| JRN-20260905-001 | Module | MOD-002 v2.0 Design Baseline current; Prototype vorhanden; Build/In-game Validation pending | MOD-002 Docs, ADR-009, Repository |
| JRN-20260905-002 | Module | MOD-003 Full Design Specification current; Implementation Not Started | MOD-003 Docs und QA |
| JRN-20260905-003 | Governance | PLAN-001 v2.0 korrigierte Programm auf 116 Positionen; Status 33/116 | PLAN-001 v2.0 QA/History |
| JRN-20260905-004 | Standard | STD-001/002 current; PLAN-001 v2.1 Status 35/116 | Canonical Files, QA, Commit 602ec87 |
| JRN-20260905-005 | Standard | DOC-009/STD-004 current; PLAN-001 v2.2 Status 37/116 | Final Files/QA; Commit nach Publication vergeben |

## 30. Detaillierte Initialeinträge

### JRN-20260901-001 — Architecture Consistency Audit

Type: Audit. Scope: ARCH-001–012, ADR, Module- und Deployment/Configuration-Consistency. Action: Audit Work in `audit/architecture-consistency-2026-09-01` isoliert; Full Rebuild und Formal Artifact QA in mehreren Passes fortgeführt. Truth: Audit Evidence gehört zur Audit Branch/History; Main wurde während Gates absichtlich nicht geändert. Evidence: Architecture Audit Record, Canonical Index und Git History. Next: Gate Evidence im Foundation Work erhalten.

### JRN-20260904-001 — Full-document Rebuild

Type: Document Change. Action: Active Documents als Full-content RU/DE Deliverables neu erstellt; frühere Snapshots unter datierten Archive Paths erhalten. Result: Canonical Index trennt Current Full Documents von Superseded Material. Truth: Page Count allein war keine Acceptance; Render-, Structure- und Accessibility-Checks waren erforderlich. Evidence: AUDIT-030, Coverage/QA Register und Archive Tree.

### JRN-20260904-002 — MOD-002 Scope Authorization

Type: Decision. Decision Owner: Andrii Drahulskyi. Decision: Implementation Work für MOD-002 darf Skyrim SE/AE adressieren; Skyrim VR ist Excluded. Governing Decision: ADR-009. Constraint: Authorization behauptet keinen Build- oder In-game-Erfolg. Next: anwendbare Foundation- und Runtime-Validation-Gates schließen.

### JRN-20260905-001 — MOD-002 Truth

Type: Module Implementation. Document Status: Current v2.0 Full Implementation Specification. Implementation Status: Prototype. Validation Status: Build und In-game Acceptance Pending. Evidence: MOD-002 RU/DE v2.0 Documents, ADR-009 und Repository Prototype Paths. Verbotener Claim: „MOD-002 complete“, bis Build, Tests und Target-game Evidence bestehen.

### JRN-20260905-002 — MOD-003 Design Baseline

Type: Module Documentation. Result: RU/DE v1.0 Full Design and Implementation Specifications mit 60 nummerierten Sections und Appendices. Document Status: Complete Design Baseline. Implementation Status: Not Started. Evidence: Canonical MOD-003 Files und QA Report. Next: keine Implementation ohne explizite Entscheidung und anwendbare Foundation Gates.

### JRN-20260905-003 — Master Plan Correction

Type: Governance. Result: PLAN-001 v2.0 zählte 116 Delivery Artifacts statt der fehlerhaften Summary 92 und trennte Document Readiness von Code Readiness. Baseline bei Publication: 33/116. Control Artifact PLAN-001 ist vom Nenner ausgeschlossen. Evidence: PLAN-001 v2.0 Source, DOCX Pair und QA History.

### JRN-20260905-004 — Glossary und Naming Baseline

Type: Standard. Result: STD-001 Glossary und STD-002 Naming Convention wurden Current; PLAN-001 v2.1 erhöhte Evidence-backed Progress auf 35/116. Module Impact: Terminology und Names gelten für alle MOD Documents; bestehende Public MOD-002 Identifiers bleiben bis Controlled Migration stabil. Evidence: Canonical RU/DE Files, QA Reports und Commit `602ec87` auf `docs/pre-code-foundation-plan`.

### JRN-20260905-005 — Documentation Standard und Journal

Type: Standard / Governance. Result: STD-004 definiert Mandatory Structure/Status/Traceability/QA einschließlich voller MOD-001–MOD-014 Requirements; DOC-009 etabliert dieses Living Journal. PLAN-001 v2.2 erhöht Progress auf 37/116; STD-003, STD-005, STD-006 und STD-007 verbleiben für F1. Evidence: Final RU/DE Files und QA Reports. Git Commit: Pending bis Atomic Publication. Next: publishen, Upstream prüfen und vier verbleibende F1 Standards fortsetzen.

## Anhang A. Universelle Entry-Vorlage

| Feld | Wert |
|---|---|
| Entry ID / Date / Author | `JRN-YYYYMMDD-NNN`, ISO Timestamp, Author |
| Type / Status | Entry Type; Draft/Reviewed/Current/Closed |
| Scope | Documents, Modules, Platform, Branch/Ref |
| Context | Trigger, Problem oder Objective |
| Facts | Beobachtete und belegte Aussagen |
| Decision / Assumptions | Explizite Trennung mit Owner |
| Action / Result | Ausgeführte Arbeit und tatsächliches Ergebnis |
| Evidence | Paths, Commit, Tests, Logs, Hashes oder Pending |
| Impact / Risks | Betroffene Contracts, Gates und Residual Risk |
| Next | Action, Owner, Due Date/Gate |

## Anhang B. Experiment-Vorlage

| Feld | Inhalt |
|---|---|
| Hypothesis | Falsifiable Expected Relationship |
| Environment | Versions, Hardware, Configuration, Seed |
| Method | Steps und Controlled Variables |
| Expected / Actual | Quantified Comparison |
| Evidence | Raw Output/Artifacts und Retention Path |
| Limitations | Bias, Missing Controls, Applicability |
| Conclusion | Supported/Rejected/Inconclusive |
| Follow-up | Decision oder Next Experiment mit Owner |

## Anhang C. Blocker-Vorlage

| Feld | Inhalt |
|---|---|
| Severity / Gate | Impact und Blocked Milestone |
| Symptom | Reproducible Observed Behavior |
| Cause | Confirmed Cause oder Ranked Hypotheses |
| Attempts | Bereits versuchte Actions und Results |
| Needed Input | Exakte Authority, Information oder External Change |
| Owner / Review | Responsible Person und Next Review Time |
| Workaround | Safe Temporary Path und Risk |
| Closure Evidence | Result, das Entfernung des Blockers beweist |

## Anhang D. Module-Status-Vorlage

| Feld | Inhalt |
|---|---|
| Module / Work Package | MOD-ID und Bounded Scope |
| Document Status | Planned/Draft/Design Baseline/Current |
| Implementation Status | Not Started/Prototype/Implemented |
| Validation Status | Not Run/Partial/Passed |
| Contracts Changed | Ports, Events, Schemas, Config und Compatibility |
| Evidence | Source Paths, Commit, Build/Tests/Runtime |
| Deviations | Spec Mismatch, Owner und Resolution |
| Next Gate | Exakte Pass Conditions |

## Anhang E. Änderungsjournal für DOC-009

| Version | Datum | Status | Änderung |
|---|---|---|---|
| 1.0 | 2026-09-05 | Current / Living | Evidence-based Developer Journal mit Entry Lifecycle, Templates, verifizierter Initial History und separatem Tracking für MOD-001–MOD-014 erstellt. |
