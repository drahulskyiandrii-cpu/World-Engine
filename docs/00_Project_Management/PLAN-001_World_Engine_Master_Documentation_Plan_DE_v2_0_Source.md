# PLAN-001 — World Engine Master Documentation Plan

Version 2.0 · Deutsch · 2026-09-05 · Aktuelles Steuerungsdokument

## 1. Zweck des Dokuments

PLAN-001 ist der zentrale Steuerungsplan für die World-Engine-Dokumentation. Er beantwortet fünf praktische Fragen: Welche Dokumente existieren? Welche davon sind tatsächlich abgeschlossen? Welche Artefakte sind vor weiterem Produktionscode verpflichtend? Was muss unmittelbar vor der Implementierung eines bestimmten Moduls entstehen? Welche Nachweise sind erforderlich, damit Arbeit als abgeschlossen gelten darf?

Dieser Plan ersetzt weder Architekturunterlagen noch ADRs oder Modulspezifikationen. Er verbindet sie zu einem prüfbaren Arbeitsprogramm und trennt eindeutig zwischen „Dokument erstellt“, „Entscheidung angenommen“, „Code implementiert“, „Verhalten verifiziert“ und „Komponente betriebsbereit“.

PLAN-001 ist ein Kontrollartefakt und kein Bestandteil der 116 Lieferartefakte. Es wird deshalb nicht in den Fortschrittsnenner aufgenommen und vergrößert den Programmumfang nicht künstlich bei jeder eigenen Revision.

## 2. Geltungsbereich und Einschränkungen

Der Plan umfasst Projektmanagement, Architektur, ADRs, vierzehn Module, Datenbank, Event System, APIs, AI, Tests, Risiken, Standards, Research, Knowledge Base und IHK-Unterlagen. Die aktive erste Spieleplattform ist Skyrim Special Edition / Anniversary Edition. Skyrim VR gehört nicht zur aktuellen Implementierung und benötigt eine separate Entscheidung, Untersuchung und eigene Acceptance Tests.

Das Verzeichnis `sources/`, historische Archive und superseded snapshots dienen als Herkunftsnachweise, sind jedoch keine aktiven implementation-facing Artefakte. Kanonisch ist nur ein Dokument, das ausdrücklich in `docs/CANONICAL_DOCUMENTATION_INDEX.md` eingetragen ist.

## 3. Korrektur des bisherigen Master Plans

Die frühere Tabelle nannte einen Gesamtstand von `28 / 92`, obwohl sie 114 Positionen auflistete. Nach Aufnahme von ADR-008 und ADR-009 umfasst das tatsächliche Programm 116 Lieferartefakte. Zum Stand dieser Version sind 33 von 116 bestätigt.

Zwei inhaltliche Verzerrungen werden ebenfalls korrigiert. `Future Ideas` fehlt im aktiven kanonischen Satz und kann daher nicht Complete sein. Die Existenz einer kurzen `README.md` im Repository-Root schließt den vollständigen README-Standard aus Phase 11 nicht ab. Ein Dokument-Checkbox bedeutet niemals automatisch, dass das zugehörige Softwaremodul implementiert ist.

| Kennzahl | Früherer Wert | Verifizierter Wert | Begründung |
|---|---:|---:|---|
| Lieferartefakte gesamt | 92 | 116 | Neuzählung aller Einträge der Phasen 1–14 einschließlich ADR-008 und ADR-009 |
| Abgeschlossen | 28 | 33 | Aktiver kanonischer Index und verifizierte Dateien |
| Architecture Decision Records | 7 / 7 | 9 / 9 | ADR-008 und ADR-009 sind angenommen und veröffentlicht |
| Modulspezifikationen | 1 / 15 | 4 / 15 | Template, MOD-001, MOD-002 und MOD-003 |
| Future Ideas | Complete | Planned | Kein aktives kanonisches Dokument vorhanden |
| Modulimplementierung | Nicht getrennt | Separat bewertet | Design, Implemented, Verified und Operational sind unterschiedliche Zustände |

## 4. Aktueller Managementstatus

Die Dokumentationsbasis des Projekts ist umfangreich, aber noch kein vollständiges Pre-Code Foundation. Architektur und ADRs sind abgeschlossen. Drei Modulspezifikationen nach dem Template sind fertig. Eigenständige Standards sowie gemeinsame Verträge für Datenbank, Events, APIs, Tests und Risiken sind jedoch noch nicht veröffentlicht.

| Bereich | Abgeschlossen | Gesamt | Zustand |
|---|---:|---:|---|
| Projektmanagement | 8 | 9 | DOC-009 Developer Journal fehlt |
| Architektur | 12 | 12 | Vollständige audited baseline |
| ADR | 9 | 9 | Vollständige angenommene baseline |
| Modulspezifikationen | 4 | 15 | Template und MOD-001–003 fertig |
| Datenbank | 0 | 8 | Nur Entscheidungen in Architektur und ADRs |
| Event System | 0 | 6 | Nur Entscheidungen in Architektur und ADRs |
| API | 0 | 6 | Eigenständige Verträge nicht veröffentlicht |
| AI | 0 | 7 | Architektur-baseline vorhanden, Detaildokumente fehlen |
| Testing | 0 | 6 | Anforderungen sind auf bestehende Dokumente verteilt |
| Risikomanagement | 0 | 4 | Register und Verfahren fehlen |
| Projektstandards | 0 | 10 | Kurze Root-README zählt nicht als vollständiger Standard |
| Research | 0 | 6 | Keine kanonischen Studien veröffentlicht |
| Knowledge Base | 0 | 10 | Keine aktiven kanonischen Dokumente |
| IHK | 0 | 8 | Materialien nicht begonnen |
| **Gesamt** | **33** | **116** | **28,4 % der Lieferartefakte** |

## 5. Statusmodell

Statuswerte werden getrennt auf Dokumente und Softwarekomponenten angewendet. Ein Dokument kann Complete sein, während der Code Not Started bleibt. Dies ist aktuell bei MOD-001 und MOD-003 der korrekte Zustand.

| Status | Bedeutung | Mindestnachweis |
|---|---|---|
| Planned | Artefakt vorgesehen, keine normative Version | PLAN-001-Eintrag mit Owner und Abhängigkeiten |
| Draft | Inhalt vorhanden, noch nicht vollständig geprüft | Editierbare Quelle und Draft-DOCX |
| Design Baseline | Verträge reichen für Implementierung | Angenommenes Dokument, Traceability und geschlossene Design Gaps |
| Accepted | Entscheidung formal angenommen | ADR oder unterzeichneter Decision Record |
| Implemented | Code existiert und entspricht der Baseline | Source-Link und erfolgreicher Build |
| Verified | Verhalten durch Tests nachgewiesen | Unit-, Integration- oder Acceptance-Testberichte |
| Operational | Komponente ist für die Zielumgebung nutzbar | Deployment-, Monitoring-, Recovery- und Betriebsnachweise |
| Superseded | Version ersetzt und nicht mehr steuernd | Archivlink und benannter Nachfolger |
| Living | Dokument wird während des Projekts fortgeschrieben | Owner, Cadence und Änderungsjournal |

## 6. Regel für Dokumentenabschluss

Ein Artefakt darf nur Complete heißen, wenn es eigenständig ist, RU- und DE-Versionen besitzt, sofern Zweisprachigkeit vorgeschrieben ist, und Verantwortungsgrenzen, normative Anforderungen, Abhängigkeiten, Fehler und Recovery, Sicherheit, prüfbare Acceptance Criteria, Traceability, Änderungshistorie und einen ehrlichen Implementation Status enthält. Word-Dokumente benötigen vollständiges Rendering aller Seiten, visuelle Prüfung, Strukturaudit und Accessibility Audit.

Eine Kurznotiz, Präsentation, veraltete Datei, leeres Verzeichnis, `.gitkeep`, Erwähnung in einem anderen Dokument oder vorhandener Source Code schließen einen Master-Plan-Eintrag nicht für sich allein ab.

## 7. Zeitklassen der Erstellung

| Klasse | Zeitpunkt | Regel |
|---|---|---|
| A — Active baseline | Bereits wirksam | Nur über Change Control ändern |
| F — Foundation first | Vor weiterem Produktionscode | Blockiert neue Implementierungen |
| J — Just in time | Vor Code des abhängigen Moduls | Unmittelbar vor dem Modul erstellen |
| P — Parallel | Parallel zur Implementierung oder vor Release | Blockiert die aktuelle Designphase nicht |
| L — Later | Nach Stabilisierung der v1.0 baseline | Frühen MVP nicht erweitern |

## 8. Pre-Code Foundation Pack

Das Foundation Pack verwandelt die bestehende Architektur in ausführbare Verträge. Neue Produktionsimplementierung über den aktuellen MOD-002-Prototyp hinaus soll nicht erweitert werden, bevor der Mindestsatz F1–F5 abgeschlossen ist. Dokumentation, sichere Forschung, Test Harnesses und Korrekturen am bestehenden Prototyp sind zulässig, sofern sie keine neuen unbeschriebenen Verträge festschreiben.

| Gate | Umfang | Ergebnis | Zustand |
|---|---|---|---|
| F0 Master Control | PLAN-001 RU/DE, Index und QA | Reales Register, Reihenfolge und Kriterien | Wird mit dieser Version abgeschlossen |
| F1 Governance | DOC-009; STD-001–008 | Gemeinsame Sprache, Dokument- und Git-Regeln | Planned |
| F2 Verification & Risk | TEST-001–003, TEST-006; RISK-001–003 | Einheitliches Nachweismodell und Risk Ownership | Planned |
| F3 Persistence | DB-001–003, DB-006–008 | Logisches Modell, Migrationen und Wiederherstellung | Planned |
| F4 Events | EVT-001–006 | Benennung, Lifecycle, Typen und Event Catalog | Planned |
| F5 APIs | API-001, API-005, API-006; Skeleton API-002–004 | Stabile Grenzen und Versionierung | Planned |
| F6 Module Readiness | Vollständige Spezifikation des Zielmoduls | Freigabe für den Code des konkreten Moduls | MOD-001–003 als Design Docs fertig |

## 9. Bedingungen für neuen Produktionscode

Der Implementierungsstart richtet sich nicht nach Seitenzahl, sondern nach geschlossenen Schnittstellen und Risiken. Für jedes neue Modul sind folgende Nachweise verpflichtend.

1. Die Modulspezifikation hat Design-Baseline-Status und keine ungeklärten Data Owner.
2. Eingangs- und Ausgangsports, Events und Fehler sind in den gemeinsamen Katalogen definiert.
3. Persistence und Migration Ownership stehen vor dem ersten dauerhaften Schreibvorgang fest.
4. Unit-, Integration- und Acceptance Tests besitzen IDs und erwartete Ergebnisse.
5. Konfiguration, Observability, Failure Handling und Rollback sind beschrieben.
6. Abhängigkeiten entsprechen ARCH-004, ARCH-005 und ADR-008.
7. Keine direkte Game- oder SQLite-Abhängigkeit überschreitet die festgelegte Grenze.
8. Für Skyrim SE/AE existiert eine reproduzierbare Testumgebung; Skyrim VR wird nicht implizit aufgenommen.

## 10. Empfohlene Fortsetzungsreihenfolge

Nach Veröffentlichung von PLAN-001 sollen nicht alle 83 verbleibenden Dokumente nacheinander geschrieben werden. Stattdessen wird das minimale Foundation des kritischen Pfads geschlossen. So entsteht ausreichende Sicherheit vor dem Code, ohne das Projekt in endlose Dokumentproduktion zu verwandeln.

| Reihenfolge | Paket | Grund für den jetzigen Zeitpunkt | Abschlusskriterium |
|---:|---|---|---|
| 1 | F1 Governance | Beseitigt Terminologie- und Prozessabweichungen | DOC-009 und STD-001–008 Current |
| 2 | F2 Verification & Risk | Macht Readiness messbar und Risiken entscheidbar | TEST-001–003, TEST-006 und RISK-001–003 Current |
| 3 | F3 Persistence | Benötigt von Core, Memory, Save und fast allen Simulation Modules | DB logical baseline und migration/recovery policy Current |
| 4 | F4 Events | Events sind die zentrale Sprache zwischen Modulen | Vollständiger Event Contract und Katalog v1 Current |
| 5 | F5 APIs | Fixiert Grenzen zwischen Core, Adaptern und Modulen | Internal/Module APIs und Versionierung Current |
| 6 | Module gate | Prüfung der konkreten Spezifikation vor Code | Entscheidung Implement / Revise / Defer |
| 7 | Production code | Erst nach Abschluss der anwendbaren Gates | Build und Testnachweise |

## 11. Phase 1 — Projektmanagement

| ID | Artefakt | Status | Klasse | Kanonischer Nachweis / Abschlussbedingung |
|---|---|---|---|---|
| DOC-001 | Vision | Complete | A | `docs/00_Project_Management/DOC-001_*_Audited_Complete.docx` |
| DOC-002 | Project Principles | Complete | A | `docs/00_Project_Management/DOC-002_*_Audited_Complete.docx` |
| DOC-003 | Product Roadmap | Complete | A | `docs/00_Project_Management/DOC-003_*_Audited_Complete.docx` |
| DOC-004 | Implementation Roadmap | Complete | A | `docs/00_Project_Management/DOC-004_*_Audited_Complete.docx` |
| DOC-005 | Project Charter | Complete | A | `docs/00_Project_Management/DOC-005_*_Audited_Complete.docx` |
| DOC-006 | Project Scope | Complete | A | `docs/00_Project_Management/DOC-006_*_Audited_Complete.docx` |
| DOC-007 | Milestones | Complete | A | `docs/00_Project_Management/DOC-007_*_Audited_Complete.docx` |
| DOC-008 | Release Plan | Complete | A | `docs/00_Project_Management/DOC-008_*_Audited_Complete.docx` |
| DOC-009 | Developer Journal | Planned | F | Living Journal mit Decision-, Experiment-, Blocker- und Evidence-Vorlagen |

## 12. Phase 2 — Architektur

Alle zwölf Dokumente sind abgeschlossen und bilden die aktive Baseline. Änderungen erfolgen über ein ADR oder eine formale Revision des betroffenen ARCH-Dokuments.

| ID | Artefakt | Status | Klasse | Kanonischer Nachweis |
|---|---|---|---|---|
| ARCH-001 | Overall System Architecture | Complete | A | `docs/01_Architecture/ARCH-001_Overall_System_Architecture/` |
| ARCH-002 | Module Architecture | Complete | A | `docs/01_Architecture/ARCH-002_Module_Architecture/` |
| ARCH-003 | Data Flow | Complete | A | `docs/01_Architecture/ARCH-003_Data_Flow/` |
| ARCH-004 | Architecture Principles & Dependency Rules | Complete | A | `docs/01_Architecture/ARCH-004_Architecture_Principles_and_Dependency_Rules/` |
| ARCH-005 | Module Dependencies | Complete | A | `docs/01_Architecture/ARCH-005_Module_Dependencies/` |
| ARCH-006 | Component Diagram | Complete | A | DOCX-Paar und audited SVG in `ARCH-006_Component_Diagram/` |
| ARCH-007 | Sequence Diagrams | Complete | A | `docs/01_Architecture/ARCH-007_Sequence_Diagrams/` |
| ARCH-008 | Deployment Architecture | Complete | A | `docs/01_Architecture/ARCH-008_Deployment_Architecture/` |
| ARCH-009 | Configuration Architecture | Complete | A | `docs/01_Architecture/ARCH-009_Configuration_Architecture/` |
| ARCH-010 | Plugin Architecture | Complete | A | `docs/01_Architecture/ARCH-010_Plugin_Architecture/` |
| ARCH-011 | Save System Architecture | Complete | A | `docs/01_Architecture/ARCH-011_Save_System_Architecture/` |
| ARCH-012 | AI Integration Architecture | Complete | A | `docs/01_Architecture/ARCH-012_AI_Integration_Architecture/` |

## 13. Phase 3 — Architecture Decision Records

| ID | Entscheidung | Status | Klasse | Kanonischer Nachweis |
|---|---|---|---|---|
| ADR-001 | Core Architecture | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-001_Core_Architecture/` |
| ADR-002 | Event System | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-002_Event_System/` |
| ADR-003 | Database | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-003_Database/` |
| ADR-004 | Local AI | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-004_Local_AI/` |
| ADR-005 | Save System | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-005_Save_System/` |
| ADR-006 | Plugin Detection | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-006_Plugin_Detection/` |
| ADR-007 | Multi-Game Support | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-007_Multi_Game_Support/` |
| ADR-008 | Dependency Semantics & Contract Ownership | Accepted | A | `docs/02_Architecture_Decision_Records/ADR-008_Dependency_Semantics_Contract_Ownership/` |
| ADR-009 | MOD-002 Skyrim Integration | Accepted | A | SE/AE-Grenze angenommen; VR ausdrücklich ausgeschlossen |

## 14. Phase 4 — Modulspezifikationen

| ID | Artefakt | Dokumentstatus | Implementierungsstatus | Nächste Bedingung |
|---|---|---|---|---|
| TEMPLATE | Module Specification Template | Complete | Not applicable | Für jede neue Spezifikation verwenden |
| MOD-001 | Core | Complete | Not Started | Foundation gates und implementation decision |
| MOD-002 | Game Adapter | Complete | Prototype | Build, automatisierte Tests und in-game SE/AE validation |
| MOD-003 | Memory Engine | Complete | Not Started | Foundation gates, DB-/Event-/API-Verträge und implementation decision |
| MOD-004 | Character Engine | Planned | Not Started | MOD-003 contracts und vollständige MOD-004 specification |
| MOD-005 | Dialogue Engine | Planned | Not Started | MOD-003/004, AI contracts und fallback policy |
| MOD-006 | Organization Engine | Planned | Not Started | Character-/Relationship-/Event-Verträge |
| MOD-007 | Relationship Engine | Planned | Not Started | MOD-003/004 und deterministische update rules |
| MOD-008 | Economy Engine | Planned | Not Started | v1.0 baseline stabilisiert; economic model angenommen |
| MOD-009 | Politics Engine | Planned | Not Started | Organization-/Economy-Baseline und risk review |
| MOD-010 | Quest Engine | Planned | Not Started | Event-, Adapter- und Save-Verträge |
| MOD-011 | Event Engine | Planned | Not Started | EVT-001–006 und API contracts complete |
| MOD-012 | Scheduler | Planned | Not Started | Clock-, Persistence- und deterministic-ordering contracts |
| MOD-013 | Save Manager | Planned | Not Started | DB baseline und ARCH-011 participant protocol |
| MOD-014 | Voice Engine | Planned | Not Started | Dialogue baseline, provider abstraction und privacy review |

## 15. Phase 5 — Datenbank

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| DB-001 | Database Architecture | Planned | F | Boundaries, Engine-Wahl, Connections, Transactions und Ownership |
| DB-002 | Entity Model | Planned | F | Stabile Entity Identities, Aggregates und Ownership Map |
| DB-003 | Database Schema | Planned | F | Logisches Schema, Invariants und Module Mappings |
| DB-004 | Tables | Planned | J | Physische Tabellen für angenommene Module Schemas |
| DB-005 | Relationships | Planned | J | Keys, Cardinality und Cross-Module Reference Policy |
| DB-006 | Indexes | Planned | J | Workload-basierte Indexes mit gemessenen Trade-offs |
| DB-007 | Migrations | Planned | F | Forward Migration, Compatibility, Failure und Rollback Policy |
| DB-008 | Backup Strategy | Planned | F | Backup, Restore, Corruption Handling und Recovery Tests |

## 16. Phase 6 — Event System

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| EVT-001 | Event Catalog | Planned | F | Initiale Cross-Module-Eventliste mit Owners und Schemas |
| EVT-002 | Event Naming Convention | Planned | F | Namespace-, Tense-, Version- und Compatibility-Regeln |
| EVT-003 | Event Priority | Planned | F | Priority Classes, Starvation Prevention und deterministic tie-break |
| EVT-004 | Event Lifecycle | Planned | F | Publish, validate, route, handle, retry, dead-letter und observe |
| EVT-005 | Event History | Planned | F | Retention, Replay, Audit und Privacy-Regeln |
| EVT-006 | Event Types | Planned | F | Command-, Fact-, Request/Result- und Diagnostic-Taxonomie |

## 17. Phase 7 — API

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| API-001 | Internal API | Planned | F | Stabile Application Ports und Error Envelope |
| API-002 | Game Adapter API | Planned | J | MOD-002 Input/Output Contract und Capabilities |
| API-003 | Event API | Planned | J | Event Publication, Subscription und Delivery Semantics |
| API-004 | Save API | Planned | J | Staging, Validate, Commit, Rollback und Compatibility |
| API-005 | Module API | Planned | F | Registration, Lifecycle, Health und Dependency Access |
| API-006 | API Versioning | Planned | F | Compatibility Matrix, Deprecation und Negotiation Rules |

## 18. Phase 8 — AI

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| AI-001 | LLM Integration | Planned | J | Provider-neutrale Ports, Timeouts, Limits und Observability |
| AI-002 | Prompt Architecture | Planned | J | Layering, Prompt Ownership, Injection Controls und Versioning |
| AI-003 | Context Manager | Planned | J | Budgeting, Provenance, Ordering und deterministic truncation |
| AI-004 | Memory Retrieval | Planned | J | Query Contract abgestimmt auf MOD-003 und relevance evaluation |
| AI-005 | Voice Integration | Planned | J | TTS/STT Contracts, Consent, Caching und Fallback |
| AI-006 | AI Configuration | Planned | J | Profiles, Validation, Secrets und Safe Defaults |
| AI-007 | Fallback Logic | Planned | J | Deterministisches Non-AI Behavior und Failure Classification |

## 19. Phase 9 — Testing

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| TEST-001 | Testing Strategy | Planned | F | Test Pyramid, Environments, Evidence und Ownership |
| TEST-002 | Unit Testing | Planned | F | Framework, Conventions, Isolation und Coverage Policy |
| TEST-003 | Integration Testing | Planned | F | Boundaries, Fixtures, Deterministic Replay und Failure Injection |
| TEST-004 | Performance Testing | Planned | P | Budgets, Workloads, Baselines und Regression Thresholds |
| TEST-005 | AI Evaluation | Planned | J | Datasets, Rubrics, Safety und Reproducibility |
| TEST-006 | Acceptance Tests | Planned | F | Scenario Format, Traceability und Release Gates |

## 20. Phase 10 — Risikomanagement

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| RISK-001 | Risk Register | Planned | F | Owner, Probability, Impact, Trigger, Mitigation und Residual Risk |
| RISK-002 | Risk Assessment | Planned | F | Gemeinsame Bewertung und Review Cadence |
| RISK-003 | Risk Mitigation | Planned | F | Finanzierte Maßnahmen, Fristen und Verifikation |
| RISK-004 | Incident Log | Planned | P | Living Incident-, Response- und Lessons-Learned-Record |

## 21. Phase 11 — Projektstandards

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| STD-001 | Glossary | Planned | F | Kanonische Begriffe RU/DE/EN und verbotene Mehrdeutigkeiten |
| STD-002 | Naming Convention | Planned | F | Namen für Modules, Events, APIs, Schemas, Files und Tests |
| STD-003 | Coding Guidelines | Planned | F | Sprachregeln, Errors, Ownership, Concurrency und Review |
| STD-004 | Documentation Standard | Planned | F | Pflichtabschnitte, Status, Traceability und QA |
| STD-005 | Git Workflow | Planned | F | Branch Lifecycle, Review, Validation und Integration |
| STD-006 | Commit Convention | Planned | F | Commit Types, Scope, Evidence und Atomicity |
| STD-007 | Branch Strategy | Planned | F | Protected Branches, Worktrees und Release-/Hotfix-Regeln |
| STD-008 | Release Process | Planned | P | Build, Version, Package, Sign, Publish und Rollback |
| STD-009 | Contribution Guide | Planned | P | Environment, Changes, Review und Contributor Expectations |
| STD-010 | README | Planned | P | Vollständiges Onboarding und Projektnavigation; kurze Root-Datei reicht nicht |

## 22. Phase 12 — Research

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| RES-001 | Skyrim Integration | Planned | J | SE/AE Runtime Matrix, Hooks, Constraints und reproduzierbare Findings |
| RES-002 | Plugin Detection Research | Planned | L | Formate, Change Detection, Scale und Risk Evidence |
| RES-003 | Local AI Research | Planned | J | Model-/Runtime-Vergleich anhand von Project Workloads |
| RES-004 | VR Research | Planned | L | Separate VR Compatibility Study; keine implizite Aktivierung |
| RES-005 | Performance Research | Planned | P | Profiling Evidence und Capacity Model |
| RES-006 | Other Games Research | Planned | L | Adapter Feasibility nach Skyrim Baseline |

## 23. Phase 13 — Knowledge Base

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| KB-001 | Future Ideas | Planned | L | Priorisiertes Idea Backlog mit Provenance und Non-Scope Label |
| KB-002 | NPC Design | Planned | J | Behavior Concepts getrennt von normativen Module Contracts |
| KB-003 | Organization Design | Planned | J | Domain Concepts vor MOD-006 |
| KB-004 | Economy Research | Planned | L | Models und Trade-offs vor MOD-008 |
| KB-005 | Politics Research | Planned | L | Models und Trade-offs vor MOD-009 |
| KB-006 | Gameplay Ideas | Planned | L | Kuratiertes Backlog mit Product-Roadmap-Links |
| KB-007 | Lore Ideas | Planned | L | Provenance, Canon Policy und Content Boundaries |
| KB-008 | AI Ideas | Planned | L | Experiments klar getrennt von angenommener AI Architecture |
| KB-009 | Future Features | Planned | L | Versionsbezogenes Backlog und Dependencies |
| KB-010 | Scratchpad | Planned | P | Noncanonical Capture Area mit Promotion Rules |

## 24. Phase 14 — IHK

| ID | Artefakt | Status | Klasse | Abschlussbedingung |
|---|---|---|---|---|
| IHK-001 | Feasibility Study | Planned | P | Technische, terminliche und operative Machbarkeit |
| IHK-002 | Cost Estimation | Planned | P | Annahmen, einmalige und laufende Kosten |
| IHK-003 | Resource Planning | Planned | P | Rollen, Verfügbarkeit, Tools und Constraints |
| IHK-004 | Time Planning | Planned | P | Work Packages, Dependencies und Critical Path |
| IHK-005 | Financial Planning | Planned | P | Finanzierung, Operating Cost und Sensitivity |
| IHK-006 | Risk Analysis | Planned | P | IHK-kompatible Sicht aus der RISK Baseline |
| IHK-007 | Project Presentation | Planned | P | Abschlusspräsentation auf Basis verifizierter Evidence |
| IHK-008 | Demo Scenario | Planned | P | Wiederholbare End-to-End-Demonstration und Recovery Plan |

## 25. Abhängigkeiten zwischen Paketen

Foundation verlangt nicht die vollständige physische Gestaltung aller zukünftigen Tabellen. Zuerst werden logische Regeln und Ownership fixiert; die physische Detaillierung folgt just in time mit dem jeweiligen Modul. Ebenso müssen AI Prompts und Performance Thresholds auf realen Workloads statt auf Vermutungen beruhen.

| Quelle | Verpflichtender Verbraucher | Kontrolle |
|---|---|---|
| STD-001/002/004 | Alle folgenden Dokumente | Begriffe, IDs, Template und Statusmodell sind einheitlich |
| TEST-001/006 | Jedes Module Gate und Release | Jede Anforderung besitzt eine Prüfung |
| RISK-001/002 | Architecture, Modules und Releases | Jedes Risiko hat Owner und Residual-Risk-Bewertung |
| DB-001–003/007/008 | MOD-003, MOD-013 und Durable State | Kein Write ohne Ownership und Migration Path |
| EVT-001–006 | MOD-001, MOD-002, MOD-011 und Simulation Modules | Cross-Module Facts sind typisiert und versioniert |
| API-001/005/006 | Alle Modules und Adapters | Abhängigkeiten laufen nur über Ports/Contracts |
| Modulspezifikation | Code des konkreten Moduls | Keine Production Implementation ohne Design Baseline |

## 26. Programmmeilensteine

| Milestone | Umfang | Definition of Done |
|---|---|---|
| M-DOC-0 | PLAN-001 veröffentlicht | RU/DE, 116-item register, QA und canonical index |
| M-DOC-1 | Governance baseline | DOC-009 und STD-001–007 Current; Owner und Change Control benannt |
| M-DOC-2 | Verification baseline | TEST-001–003/006 und RISK-001–003 Current; Traceability Format funktioniert |
| M-DOC-3 | Persistence baseline | DB logical package und recovery policy mit ARCH-011/MOD-003 abgestimmt |
| M-DOC-4 | Event/API baseline | EVT package und core APIs Current; Compatibility Tests definiert |
| M-IMP-1 | Erstes freigegebenes implementation gate | Modul gewählt, Gaps geschlossen, Start vom Owner angenommen |
| M-REL-1 | Erster verifizierbarer vertikaler Schnitt | Skyrim SE/AE Event Round-trip, Persistence und Recovery nachgewiesen |

## 27. Change Control

Jede Änderung eines kanonischen Dokuments muss Grund, betroffene Anforderungen, Compatibility, Migration, Tests und die archivierte Vorgängerversion nennen. Die Änderung eines Architekturprinzips benötigt ein ADR. Eine rein redaktionelle Klarstellung kann als Document Revision erfolgen, sofern die Contract Semantics unverändert bleiben.

PLAN-001 wird nach Veröffentlichung oder Supersede jedes gezählten Artefakts aktualisiert. Der Progress Count ändert sich erst nach Prüfung des kanonischen Paares und Eintrag in den Index. Ein unvollständiger Draft erhöht den Zähler nicht.

## 28. Verantwortung und Kontrollrhythmus

| Rolle | Verantwortung |
|---|---|
| Project owner | Scope, Prioritäten, Annahme wesentlicher Änderungen und Implementation Authorization |
| Architecture owner | Cross-Document Consistency, Dependencies und ADR Enforcement |
| Document owner | Vollständigkeit, bilingual parity, Traceability und Lifecycle |
| Module owner | Spezifikationsgenauigkeit, Code Conformance und Test Evidence |
| QA owner | Render-, Accessibility-, Struktur-, Test- und Release-Nachweise |
| Risk owner | Trigger Monitoring, Mitigation und Residual-Risk Acceptance |

Empfohlener Rhythmus: Developer Journal nach jedem wesentlichen Arbeitsdurchlauf; PLAN-001-Review bei Dokumentabschluss; Risk-Register-Review mindestens an jedem Foundation Gate und Release Milestone; vollständiger Consistency Audit vor Integration der Foundation Baseline in den Hauptzweig.

## 29. Hauptrisiken des Dokumentationsprogramms

| Risiko | Frühsignal | Kontrolle |
|---|---|---|
| Dokumente entstehen nur für die Menge | Viele Seiten ohne testable clauses | Acceptance Criteria und Evidence sind verpflichtend |
| Dokumentation eilt dem Wissen zu weit voraus | Physische Schemas und Budgets beruhen auf Annahmen | Just-in-time-Detaillierung und explizite Hypothesen |
| Code eilt Verträgen voraus | Neue Public Types oder Durable Data ohne Requirement IDs | Foundation Freeze und Module Gate |
| Complete wird als implementiert verstanden | Keine Build-/Runtime-Evidence | Separate Document- und Implementation-Statuswerte |
| RU/DE weichen semantisch ab | Unterschiedliche IDs, Tabellen oder numerische Limits | Parity Audit vor Veröffentlichung |
| Alte Versionen werden weiter genutzt | Mehrere aktive Dateien ohne Nachfolger | Canonical Index und Archive-/Superseded-Links |
| Skyrim VR gelangt unbeabsichtigt in Scope | Allgemeine Aussagen ohne Platform Matrix | Expliziter Ausschluss bis zu separatem ADR |

## 30. Nächste angenommene Aktion

Nach Abschluss der QA dieser Version ist F1 Governance das nächste Arbeitspaket. Empfohlene interne Reihenfolge: STD-001 Glossary; STD-002 Naming Convention; STD-004 Documentation Standard; DOC-009 Developer Journal; STD-003 Coding Guidelines; STD-005 Git Workflow; STD-006 Commit Convention; STD-007 Branch Strategy. STD-008–010 können näher an der ersten Release Baseline abgeschlossen werden.

MOD-004 soll trotz fertiger MOD-003 Design Baseline nicht vor dem Mindestsatz F1–F5 beginnen. Dies stoppt die Entwicklung nicht, sondern verhindert erneutes Umschreiben von Event-, API-, Persistence- und Testverträgen.

## Anhang A. Gesamtfortschrittsmatrix

| Gruppe | Complete / Accepted | Planned | Gesamt |
|---|---:|---:|---:|
| Project Management | 8 | 1 | 9 |
| Architecture | 12 | 0 | 12 |
| ADR | 9 | 0 | 9 |
| Module Specifications | 4 | 11 | 15 |
| Database | 0 | 8 | 8 |
| Event System | 0 | 6 | 6 |
| API | 0 | 6 | 6 |
| AI | 0 | 7 | 7 |
| Testing | 0 | 6 | 6 |
| Risk Management | 0 | 4 | 4 |
| Project Standards | 0 | 10 | 10 |
| Research | 0 | 6 | 6 |
| Knowledge Base | 0 | 10 | 10 |
| IHK | 0 | 8 | 8 |
| **Gesamt** | **33** | **83** | **116** |

## Anhang B. Prüfung von Arithmetik und Vollständigkeit

Anzahl der Einträge nach Phasen: `9 + 12 + 9 + 15 + 8 + 6 + 6 + 7 + 6 + 4 + 10 + 6 + 10 + 8 = 116`. Abgeschlossene Einträge: `8 + 12 + 9 + 4 = 33`. Rest: `116 − 33 = 83`. Fortschrittsanteil: `33 / 116 = 28,4 %`.

PLAN-001 wird als Control Artifact im kanonischen Index geführt, gehört jedoch nicht zu den 116 Delivery Items. QA-Berichte, Coverage Registers, Audit Records und Archived Snapshots verändern den Delivery Denominator ebenfalls nicht.

## Anhang C. Traceability Baseline

| Anforderung | Steuernde Quelle | Nachweis |
|---|---|---|
| Aktive Plattform Skyrim SE/AE | ADR-009, MOD-002 | Platform Matrix und In-game Acceptance |
| Skyrim VR ausgeschlossen | ADR-009, PLAN-001 | Kein VR Target in aktuellen Implementation Gates |
| Dependency Direction | ARCH-004, ARCH-005, ADR-008 | Contract Audit und Dependency Tests |
| Event-driven Integration | ADR-002, ARCH-003, ARCH-007 | EVT Package und Integration Tests |
| Persistence und Atomic Restore | ADR-003, ADR-005, ARCH-011 | DB Package, Save API und Recovery Tests |
| Memory Ownership | MOD-003, ADR-008 | Repository-/Port-Verträge und Module Tests |
| AI ist nicht Source of Truth | ARCH-012, ADR-004, MOD-003 | Validation und deterministic fallback tests |
| Dokumentation vor Modulcode | DOC-004, PLAN-001 | F6 Gate Record |

## Anhang D. Änderungsjournal

| Version | Datum | Änderung |
|---|---|---|
| 2.0 | 2026-09-05 | Vollständiger zweisprachiger Steuerungsplan erstellt; 28/92 zu 33/116 korrigiert; ADR-008/009 und MOD-002/003 aufgenommen; Statusmodell, Foundation Pack, 116-item registry, Gates und Acceptance Rules ergänzt. |

