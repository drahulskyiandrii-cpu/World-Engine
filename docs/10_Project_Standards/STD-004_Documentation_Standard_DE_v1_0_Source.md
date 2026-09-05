# STD-004 — World Engine Documentation Standard

Version 1.0 · Deutsch · 2026-09-05 · Aktueller normativer Standard

## 1. Zweck

STD-004 legt fest, wie World Engine Dokumente erstellt, geprüft, veröffentlicht, aktualisiert und ersetzt. Ziel ist Dokumentation als verifizierbarer Vertrag, nicht als Sammlung von Behauptungen.

Der Standard ist für neue kanonische Artefakte und wesentliche Änderungen bereits veröffentlichter Dokumente verbindlich.

## 2. Geltungsbereich

Der Standard umfasst DOC, PLAN, ARCH, ADR, MOD, DB, EVT, API, AI, TEST, RISK, STD, RES, KB und IHK. Er gilt für Markdown Sources, DOCX/PDF Deliverables, Diagramme, Tabellen, Contract-Beispiele und zugehörige QA Reports.

Skyrim SE/AE ist das aktive Spielziel. Skyrim VR muss als Excluded markiert bleiben, bis eine separate Entscheidung den Scope ändert.

## 3. Autoritätsquellen

| Ebene | Quelle | Rolle |
|---|---|---|
| 1 | Explizite Entscheidung des Project Owner | Richtung, Scope und finale Annahme |
| 2 | Accepted ADR | Irreversible oder modulübergreifende Architekturentscheidung |
| 3 | Current ARCH, MOD, API, DB, EVT | Normative Design Baseline im jeweiligen Bereich |
| 4 | Current STD, TEST, RISK | Gemeinsame Regeln und Evidence Model |
| 5 | Living DOC-009, Research und Knowledge Base | Kontext und aktuelle Daten, kein versteckter ADR-Ersatz |

Bei einem Konflikt wird keine bequeme Formulierung ausgewählt. Der Autor erfasst die Inconsistency, bestimmt den Owner und führt Change Control durch.

## 4. Normative Sprache

| Begriff | Bedeutung |
|---|---|
| MUST / MUST NOT | Verbindliche Anforderung oder Verbot |
| SHOULD / SHOULD NOT | Bevorzugte Regel; Abweichung wird begründet |
| MAY | Zulässige Option |
| TBD | Offene Entscheidung mit Owner, Deadline und Blocking Impact |
| N/A | Nicht anwendbar mit kurzer Begründung |

Wörter wie „schnell“, „zuverlässig“, „fertig“ oder „vollständig“ sind ohne Metrik oder Kriterium nicht normativ.

## 5. Dokumentfamilien und Identifikatoren

Jedes Deliverable besitzt eine stabile ID nach STD-002. Die ID ändert sich weder durch Übersetzung noch durch neue Version oder Formatwechsel. Eine neue ID ist nur für eine neue Verantwortung erforderlich.

| Familie | Hauptergebnis |
|---|---|
| DOC / PLAN | Governance, Scope, Roadmap und Journal |
| ARCH / ADR | Systemstruktur und angenommene Entscheidungen |
| MOD | Vollständiger Module Design/Implementation Contract |
| DB / EVT / API / AI | Modulübergreifende technische Contracts |
| TEST / RISK / STD | Verifikation, Risiko und gemeinsame Regeln |
| RES / KB / IHK | Forschung, Wissen und Prüfungsunterlagen |

## 6. Dokumentlebenszyklus

`Planned → Draft → Review → Current/Accepted → Superseded → Archived` ist der Basis-Lifecycle. `Living` ist eine zusätzliche Eigenschaft eines Current-Dokuments, keine Lockerung der QA.

| Übergang | Verbindliche Evidence |
|---|---|
| Planned → Draft | Owner, Scope, Dependencies und Source |
| Draft → Review | Vollständige Required Sections und geschlossene Blocking TBDs |
| Review → Current | Content-, Structure-, Parity-, Render- und Accessibility-QA |
| Review → Accepted | Dasselbe plus formale Entscheidung des Owner |
| Current → Superseded | Neuer Successor, Migration Note und Index Update |

## 7. Dokumentstatus und Codestatus

Dokumentstatus und Status der Softwarekomponente MUST getrennt geführt werden. `Complete Design Baseline` bedeutet, dass die Spezifikation für den Implementierungsstart ausreicht. Es bedeutet nicht `Implemented`, `Verified` oder `Operational`.

| Dimension | Zulässige Zustände |
|---|---|
| Document | Planned, Draft, Review, Current, Accepted, Superseded, Living |
| Implementation | Not Started, Prototype, Implemented, Verified, Operational, Retired |
| Validation | Not Run, Failed, Partial, Passed, Waived |

## 8. Pflichtmetadaten

Jedes Current Deliverable MUST ID, Title, Version, Language, Date, Status, Owner, Scope, Purpose, Governing References, Implementation Truth, Change Summary und Canonical Path enthalten. Bei Living-Dokumenten kommen Cadence und Last Reviewed Date hinzu.

Metadaten in DOCX Properties ersetzen den sichtbaren Document-Control-Block nicht.

## 9. Titel und Eröffnung

Das Cover zeigt ID, genauen Titel, Version, Sprache, Status und eine kurze Geltungsgrenze. Die ersten Inhaltsabschnitte sind Purpose, Scope, Authority, Status Truth und Definitions/References. Der Leser muss die Kraft des Dokuments vor dem ersten technischen Abschnitt verstehen.

## 10. Gemeinsamer Inhaltskern

| Abschnitt | Anforderung |
|---|---|
| Purpose und Scope | Ziel, Audience, Inclusion und Non-goals |
| Context und Dependencies | Eingangsentscheidungen, Consumers und Prerequisites |
| Ownership | Owner von Daten, Entscheidungen, Code und Prüfungen |
| Normative Requirements | Atomare, testbare und widerspruchsfreie Anforderungen |
| Failure und Recovery | Fehler, Degradation, Retry, Rollback und Recovery |
| Security und Privacy | Trust Boundaries, Sensitive Data, Logging und Retention |
| Verification | Test IDs, Acceptance Criteria, Evidence und Residual Risk |
| Traceability | Verbindungen zu Upstream/Downstream Artifacts |
| Change History | Version, Datum, Grund und Impact |

## 11. Regeln für Architecture

Ein ARCH-Dokument erklärt Components, Boundaries, Dependency Direction, Runtime Topology, Data/Control Flow und System Qualities. Es MUST eine Legende für Diagramme enthalten und darf eine Architectural Decision nicht in einer Illustration verstecken.

Eine Änderung an Ownership, Dependency Direction, Persistence Boundary, Threading Model oder Cross-Module Contract erfordert ein ADR oder eine explizite Begründung, warum kein ADR erforderlich ist.

## 12. Regeln für ADR

Ein ADR MUST Context, Decision, Alternatives, Consequences, Compatibility, Migration, Verification und Status enthalten. Ein Accepted ADR wird nicht so editiert, als hätte die frühere Entscheidung nie existiert. Eine neue Entscheidung erstellt ein Successor ADR mit explizitem `Supersedes`-Verweis.

## 13. Pflichtstruktur für MOD-001–MOD-014

Jede Modulspezifikation MUST ein vollständiges Standalone-Dokument sein, keine kurze Zusammenfassung. Die Regel gilt für MOD-001 Core, MOD-002 Game Adapter, MOD-003 Memory Engine und alle MOD-004–MOD-014.

| Block | Pflichtinhalt |
|---|---|
| Identity und Scope | Zweck, Responsibilities, Non-goals, Platform Matrix |
| Architecture | Components, Ports/Adapters, Dependency Direction, Thread/Process Model |
| Contracts | Inputs, Outputs, Commands, Queries, Events, Error Envelope, Compatibility |
| State und Data | Ownership, Entities, Invariants, Persistence, Migration, Retention |
| Runtime | Lifecycle, Configuration, Scheduling, Concurrency, Budgets, Observability |
| Resilience | Failure Taxonomy, Fallback, Retry, Idempotency, Recovery, Rollback |
| Security | Trust Boundaries, Validation, Secrets, Privacy, Abuse Limits |
| Delivery | Work Breakdown, Test Matrix, Acceptance Gates, Deployment und Operations |
| Truth | Document Status, Implementation Status, Evidence, TBDs und Open Decisions |

## 14. Modulschnittstellen

Jeder exposed Port beschreibt Request/Response oder Event Schema, Preconditions, Postconditions, Error Codes, Timeout/Cancellation, Ordering, Idempotency, Versioning und Owner. Ein Verweis auf ein künftiges API/EVT-Dokument ersetzt keine lokal ausreichende Spezifikation.

Bereits in der Baseline angenommene Public MOD-002 Identifiers bleiben bis zu einer kontrollierten Migration stabil.

## 15. Moduldaten und Speichern

Für jeden Durable State nennt die Spezifikation Authoritative Owner, Schema/Version, Consistency Boundary, Transaction Semantics, Save Participant Behavior, Migration, Corruption Handling und Restore Validation. Ein Modul darf SQLite oder Game State nicht implizit durch Umgehung der Ports besitzen.

## 16. Modulfehler und Recovery

Ein Fehler MUST einen stabilen Code, Severity, Retryability, User Impact, Logging Policy und Recovery Owner besitzen. Die Spezifikation unterscheidet Validation Failure, Unavailable Dependency, Timeout, Incompatible Version, Corrupt State und Internal Defect.

## 17. Modultests

Ein Moduldokument MUST Unit-, Component-, Integration-, Persistence/Recovery-, Performance-, Fault-Injection- und Acceptance-Szenarien enthalten. Jede kritische Requirement besitzt eine Test ID und Expected Evidence. „Später testen“ ohne Owner und Gate ist unzulässig.

## 18. Module Readiness Gate

| Gate | Mindestbedingung |
|---|---|
| Design Ready | Keine Blocking TBDs; Owners, Ports, State, Errors und Tests definiert |
| Implement Ready | Gemeinsame DB/EVT/API Contracts reichen aus; Dependencies sind stabil; Owner hat Start erlaubt |
| Implemented | Code und Build Evidence entsprechen der Baseline |
| Verified | Alle Blocking Tests bestanden; Deviations erfasst |
| Operational | Deployment, Monitoring, Backup/Restore und Rollback nachgewiesen |

Kein Gate wird durch Umfang oder Seitenzahl geschlossen.

## 19. Dokumente DB, EVT, API und AI

Ein modulübergreifender Contract MUST Namespace, Owner, Version, Schema, Compatibility, Validation, Lifecycle, Errors, Observability und Security definieren. AI-Dokumente erfassen zusätzlich Model/Provider Boundary, Prompt Provenance, Context Budget, Deterministic Fallback, Safety, Privacy und Evaluation Metrics.

## 20. Dokumente TEST und RISK

TEST beschreibt Environments, Fixtures, Oracle, Pass/Fail Rule, Evidence Retention und Owner. RISK beschreibt Probability, Impact, Trigger, Mitigation, Contingency, Owner, Due Date und Residual Risk. Ein Text kann auf beide Klassen verweisen, ersetzt sie aber nicht.

## 21. Dokumente Research, Knowledge Base und IHK

Research trennt Observed Facts, Source Claims, Experiments, Inference und Recommendation. Knowledge Base darf Hypotheses und Ideas enthalten, muss sie aber als Non-normative markieren. IHK-Materialien müssen Zahlen, Assumptions und Quellen nachvollziehbar machen.

## 22. Zweisprachige RU/DE Parity

Ein kanonisches RU/DE-Paar MUST identische ID, Version, Status, Section Order, Requirement Count, Identifiers, Numbers, Units, Enums, Tables, Acceptance Gates und Change History besitzen. Die sprachliche Form darf abweichen; Normative Strength und technischer Sinn nicht.

## 23. Machine Tokens

Code Identifiers, Event Names, Config Keys, File Paths, CLI Commands, Error Codes und Schema Fields werden nicht übersetzt. Sie behalten Canonical English Spelling und werden als Inline Code formatiert. Begriffe werden gegen STD-001, Namen gegen STD-002 geprüft.

## 24. Anforderungen schreiben

Eine Requirement soll einen Owner, ein beobachtbares Verhalten, eine Grenze und eine Prüfmethode besitzen. Komplexe „und“-Sätze werden geteilt, wenn Teile unabhängig fehlschlagen können. Normativer Text darf nicht von verborgenem Chat-Kontext abhängen.

## 25. Requirement Identifiers

Requirements, Risiken, Tests und Decisions, die andere Artefakte referenzieren, besitzen stabile IDs: `<DOC-ID>-REQ-NNN`, `<DOC-ID>-RISK-NNN`, `<DOC-ID>-TEST-NNN`, `<DOC-ID>-DEC-NNN`. Eine entfernte ID wird nicht wiederverwendet.

## 26. Traceability

Für jede Blocking Requirement ist die Kette `Source → Requirement → Design element → Implementation target → Test → Evidence` erforderlich. Vor Codebeginn darf das Implementation Target ein geplanter Path/Component sein, aber nicht fälschlich als vorhanden bezeichnet werden.

| Source | Requirement | Implementation | Test | Evidence |
|---|---|---|---|---|
| ADR-009 | MOD-002-REQ-014 | Planned Adapter Boundary | MOD-002-TEST-021 | Pending In-game Run |
| ARCH-011 | MOD-013-REQ-008 | Planned Atomic Restore Coordinator | MOD-013-TEST-012 | Pending Implementation |

## 27. Referenzen und Provenance

Eine interne Referenz nennt die genaue Document ID und bei Compatibility-Bedeutung Version/Status. Eine externe Quelle erfasst Title, Author/Publisher, URL und Access Date. Zitat und Interpretation werden getrennt.

## 28. Assumptions, TBD und Open Decisions

Jede Assumption besitzt ID, Owner, Validation Method, Due/Gate und Failure Impact. Ein TBD ohne Owner und Termin ist ein QA Defect. Eine Blocking Open Decision verhindert Current/Design Ready.

## 29. Scope und Non-goals

Jedes Design-Dokument MUST Included, Excluded und Deferred explizit nennen. Eine verschobene Funktion wird nicht als teilweise implementiert beschrieben. Für Game-specific Features ist eine Platform Matrix verpflichtend.

## 30. Ownership und RACI

| Aktion | Accountable | Responsible | Consulted |
|---|---|---|---|
| Scope und finale Annahme | Project Owner | Document Owner | Architecture/Module Owners |
| Technische Korrektheit | Architecture Owner | Author/Reviewer | Contract Owners |
| RU/DE Parity | Documentation Lead | Author/Translator | Technical Reviewer |
| QA Evidence | Document Owner | QA Executor | Accessibility/Domain Reviewer |
| Canonical Publication | Documentation Lead | Repository Maintainer | Project Owner |

In der aktuellen Phase ist Andrii Drahulskyi Project Owner und Documentation Lead; dies ersetzt keine unabhängige Prüfung kritischer Contracts.

## 31. Versioning und Dateinamen

Die Version ändert sich bei normativer Änderung, nicht bei jedem Render Attempt. Major bedeutet inkompatible Contract- oder Scope-Änderung; Minor eine kompatible Erweiterung oder Klarstellung. Der Dateiname folgt STD-002 und zeigt ID, Topic, Language, Version und Publication Status.

## 32. Änderungsjournal

Jede Current-Version besitzt ein Change Log mit Version, Datum, Author/Owner, Reason, geänderten normativen Bereichen, Compatibility und Migration Impact. Kleine Rechtschreibkorrekturen dürfen gruppiert werden; Sinnänderungen nicht.

## 33. Review Workflow

1. Author schließt Required Sections und Self-Review Checklist.
2. Domain Review prüft Sinn, Ownership, Contracts und Risiken.
3. Cross-Document Review prüft ADR/ARCH/MOD/API/DB/EVT Consistency.
4. Language Parity Review vergleicht RU/DE Structure und Machine Tokens.
5. Artifact QA prüft DOCX Structure, Render und Accessibility.
6. Publication Review aktualisiert Index, PLAN-001, Journal und Git Evidence.

## 34. Content QA Gate

Content QA MUST Completeness, Internal Consistency, Upstream/Downstream Consistency, Numbers/Units, Identifier Uniqueness, keine offenen Blocking TBDs, keine erfundenen Implementation Claims, Acceptance Testability und explizite Residual Risks prüfen. Eine reine Rechtschreibprüfung schließt das Gate nicht.

## 35. Structural und Accessibility QA

Bei DOCX werden Styles, Heading Hierarchy, Table Headers, Repeating Headers, Non-split Rows, Page Numbers, Language Metadata, Document Properties, Alt Text für sinntragende Bilder/Tabellen und Reading Order geprüft. Farbe ist nicht der einzige Bedeutungsträger. Kritische Findings blockieren Publication.

## 36. Render und Visual QA

Jede Seite des finalen DOCX MUST gerendert und visuell geprüft werden. Geprüft werden Clipped Text, Overflow, Blank/Near-blank Pages, Orphan Headings, Broken Tables, Tiny Fonts, schlechte Page Breaks, Missing Images, Header/Footer Collisions und Inconsistent Whitespace. Nach jeder Content/Layout Correction wird erneut gerendert.

## 37. Tabellen, Diagramme und Abbildungen

Eine Tabelle besitzt eine Header Row, klare Units und eine textliche Schlussfolgerung. Ein Diagramm besitzt Purpose, Legend, Direction, Boundary Semantics, Stable Labels und Text Description. Ein dekoratives Bild darf technische Evidence nicht imitieren.

## 38. Code und API Examples

Ein Beispiel wird als `Normative`, `Illustrative`, `Pseudocode` oder `Prototype snapshot` markiert. Ein Normative Example muss buildbar/validierbar sein oder einen expliziten Prüfplan besitzen. Ein Code Snapshot im Dokument ist keine authoritative Source.

## 39. Evidence und Acceptance

| Claim | Mindest-Evidence |
|---|---|
| Document complete | Canonical Files, QA Report, Page Render, Index Entry |
| Build passes | Command/Environment, Exit Status, Artifact Hash/Log |
| Test passes | Test ID, Input/Version, Expected/Actual Result, Timestamp |
| Performance target met | Workload, Hardware, Configuration, Distribution, Threshold |
| In-game validated | Game/Runtime Versions, Load Order, Scenario, Result, Evidence |
| Recovery works | Fault Injection, Restore Target, Validation und Data-loss Result |

Ohne erzeugte Evidence bleibt der Status Pending/Not Run, auch wenn Erfolg erwartet wird.

## 40. Supersession, Archive und Canonical Index

Im aktiven Satz existiert pro ID/Language genau ein Current Artifact. Eine ersetzte Datei wird als Superseded markiert, für Provenance erhalten und nennt den Successor. `docs/CANONICAL_DOCUMENTATION_INDEX.md` wird im selben Change Set aktualisiert; Ordner oder Dateiname `Complete` ohne Index erzeugen keinen Kanon.

## 41. Publication und Repository Evidence

Publication umfasst Source, Final Artifact, QA Report, Index/PLAN Update und Atomic Git Commit. Branch und Commit dürfen unvollständige QA nicht verschleiern. Main oder Release Branch werden erst nach allen anwendbaren Gates und einer separaten Integration Decision geändert.

## 42. Exceptions und Waivers

Eine Ausnahme besitzt ID, Rule, Reason, Scope, Owner, Approver, Start/End, Risk, Mitigation und Exit Condition. Ein Waiver darf keinen falschen Implementation Status legitimieren, keinen Security Defect verbergen und keine Decision Authority aufheben.

## 43. Review Cadence

Living-Dokumente werden nach ihrem Cadence geprüft. PLAN-001 bei Abschluss eines Deliverable; DOC-009 nach jedem Material Work Pass; Risk Register an Gates und Releases; Consistency Audit vor Foundation/Release Merge. Auch „keine Änderung“ wird als Review Result erfasst.

## 44. Acceptance Criteria für STD-004

STD-004 gilt als Current, wenn das RU/DE-Paar dieselbe normative Struktur besitzt; die Regeln alle Document Families und MOD-001–MOD-014 abdecken; Document/Implementation Truth getrennt ist; QA Gates testbar sind; alle Seiten gerendert und geprüft wurden; Structural/Accessibility Audit keine offenen Findings besitzt; Canonical Index und PLAN-001 aktualisiert sind.

## Anhang A. Minimaler Document-Control-Block

| Feld | Beispiel |
|---|---|
| Document ID | MOD-004 |
| Version | 1.0 |
| Status | Complete Design Baseline; Implementation Not Started |
| Scope | Character Engine for Skyrim SE/AE; VR excluded |
| Owner | Named Accountable Owner |
| Governing References | ARCH-002, ADR-008, API/EVT Baselines |
| Last Reviewed | ISO Date |
| Canonical Path | Repository-relative Path |

## Anhang B. Prüfung eines Moduldokuments

| Kriterium | Pass Condition |
|---|---|
| Responsibilities/Non-goals | Grenzen überschneiden sich nicht mit benachbarten Modules |
| Ports und Contracts | Alle Inputs/Outputs besitzen Owner, Schema, Errors und Version |
| Data | Jedes Durable Field besitzt Owner, Migration und Retention |
| Lifecycle | Startup, Steady State, Shutdown, Save/Load und Recovery beschrieben |
| Non-functional | Performance Budgets, Security und Observability sind messbar |
| Tests | Requirements sind mit Test IDs und Evidence verknüpft |
| Status Truth | Design, Implementation und Validation werden nicht vermischt |

## Anhang C. Traceability-Vorlage

| Requirement ID | Source | Design Owner | Implementation Target | Test ID | Evidence | Status |
|---|---|---|---|---|---|---|
| `<DOC-ID>-REQ-001` | ADR/ARCH/Owner Decision | Component/Port | Existing oder Planned Path | `<DOC-ID>-TEST-001` | Link/Hash/Log | Planned/Passed |

## Anhang D. Publication Checklist

1. Source und Final Deliverable stimmen bei Version und Status überein.
2. RU/DE Headings, IDs, Tables, Numbers und Gates sind abgeglichen.
3. Content-, Cross-Document-, Structural- und Accessibility-Checks bestanden.
4. Alle finalen Seiten wurden gerendert und angesehen.
5. QA Report enthält Metrics, Findings, Fixes und Final Hashes.
6. Canonical Index, PLAN-001 und DOC-009 wurden aktualisiert.
7. Change Set ist atomar und in die autorisierte Branch gepusht.

## Anhang E. Änderungsjournal

| Version | Datum | Status | Änderung |
|---|---|---|---|
| 1.0 | 2026-09-05 | Current | Vollständiger zweisprachiger Documentation Standard mit verbindlichen Regeln für MOD-001–MOD-014 und Evidence-based QA erstellt. |
