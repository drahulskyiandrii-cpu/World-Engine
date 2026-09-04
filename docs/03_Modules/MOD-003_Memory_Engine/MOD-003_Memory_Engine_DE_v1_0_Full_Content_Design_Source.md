# MOD-003 Memory Engine — vollständige normative Quelle

Vollständige Architektur- und Implementierungsspezifikation vor Entwicklungsbeginn

Version 1.0 | Status Design Baseline — Implementation Not Started | Datum 2026-09-05

## 1 Verwendung dieses Dokuments

Dieses Dokument ist die eigenständige technische Spezifikation für MOD-003 Memory Engine. Es definiert Zweck, Ownership-Grenzen, Datenmodell, Ports, algorithmische Richtlinien, Fehler, Save/Restore, Sicherheit, Performance, Tests und stufenweise Gates. Für das verpflichtende Verhalten müssen keine älteren Dokumente zusammengesucht werden; die verknüpften Architekturunterlagen dienen der Rückverfolgbarkeit und der Auflösung modulübergreifender Entscheidungen.

Die Schlüsselwörter MUST, MUST NOT, SHOULD, SHOULD NOT und MAY bestimmen die Verbindlichkeit. MUST und MUST NOT benötigen einen automatisierten Test, eine statische Prüfung oder einen signierten Acceptance Record. Von SHOULD darf nur mit dokumentierter Begründung und Auswirkungsanalyse abgewichen werden. MAY beschreibt eine Option, die andere Pflichtverträge nicht verändert.

Das Dokument trennt **Designed** — Vertrag hier beschlossen, **Implemented** — Code im Arbeitsbranch vorhanden, und **Verified** — Verhalten reproduzierbar getestet. Zum Stand v1.0 ist der gesamte Inhalt Designed. Detaillierte Schemas oder Pseudocode sind kein Implementierungsnachweis.

| Kennzeichnung | Bedeutung | Zulässiger Nachweis |
|---|---|---|
| Designed | Normative Lösung beschlossen | Dieses Dokument und zugehörige ARCH/ADR |
| Implemented | Code und Migrationen vorhanden | Source, Schema, Migration Manifest |
| Verified | Verhalten geprüft | Automatisierter Test oder signierter Manual Record |
| Operational | Für regulären Betrieb freigegeben | Alle Pflicht-Gates und Betriebsprüfungen |
| Deferred | Bewusst verschoben | Roadmap Item mit klarer Grenze |

## 2 Entscheidung, Zweck und Status

MOD-003 stellt dauerhafte, überprüfbare und abrufbare Erinnerungen für Figuren und andere zugelassene Subjekte der Welt bereit. Das Modul nimmt typisierte Memory Candidates an, validiert sie anhand von Domain Policies, persistiert sie über einen schmalen Repository Port und liefert normalisierte Records für Kontext, Dialog und weitere Use Cases.

Die Memory Engine besitzt die **Bedeutung von Erinnerung**, nicht jedoch die physische Datenbank, AI-Erzeugung, Dialoglogik oder den globalen Save Workflow. Sie verantwortet erlaubte Record Types, Subject Identity, Provenance, Authority, Deduplication, Korrekturen, Retention, Ranking und Ausgabesemantik.

Der Dokumentstatus ist vollständige Design Baseline. Die Implementierung hat nicht begonnen; API, Migration, Index, Latenzziel und Integration Path gelten daher nicht als verifiziert. Coding beginnt erst nach Gate M3.0; Operational ist erst nach M3.7 zulässig.

## 3 Architektonische Position

MOD-003 liegt im Simulation Layer und hängt weder von Skyrim noch von SKSE, UI, Transport oder einem konkreten AI Provider ab. Game-spezifische IDs erreichen es erst nach Normalisierung durch den Integrations- oder Use-Case-Owner. Consumer verwenden schmale Application/Domain Ports; SQL, Tabellen, Connection Handles und rohe Persistence Models werden nicht exponiert.

| Beteiligter | Beziehung | Regel |
|---|---|---|
| Core/Application | Lifecycle- und Use-Case-Koordination | Besitzt keine Memory Semantics |
| Repository Implementation | Implementiert Persistence Port | Besitzt SQL, Mapping, Connections und Migrationen |
| Context Manager | Liest über MemoryContextSourcePort | Kein Zugriff auf Repository/SQLite |
| Dialogue Engine | Fragt approved context ab | Keine direkte Memory-Änderung |
| AI Adapter | Liefert Candidate Output | Keine Persistenz und keine Authority-Zuweisung |
| Save Manager | Koordiniert SaveParticipant | Kennt interne Memory Invariants nicht |
| Event Engine | Liefert Semantic Events | Kein RPC und kein Owner des Memory State |

Statische Abhängigkeiten zeigen zur Abstraktion; Runtime-Aufrufrichtung ändert Ownership nicht. Der Composition Root erstellt das konkrete Repository und bindet es an den Consumer Port der Memory Engine.

## 4 Verantwortung der Memory Engine

Das Modul MUST:

- ausschließlich versionierte Commands und Candidates mit stabilen IDs annehmen;
- Struktur, Subject, Scope, Zeit, Provenance und Content-Zulässigkeit validieren;
- Type, Authority Class, Retention Class und deterministische Salience festlegen;
- exakte und nahe Duplikate vor teurer Persistenz behandeln;
- Änderungen über `IMemoryRepositoryPort` innerhalb eines Use Case atomar persistieren;
- Correction, Contradiction und Supersession ohne unbemerkte History-Überschreibung unterstützen;
- begrenzte, normalisierte Resultate mit Herkunfts- und Authority-Markern liefern;
- als `SaveParticipant` an staged Save/Restore teilnehmen;
- Semantic Events erst nach erfolgreichem Commit veröffentlichen;
- Health, Readiness und sichere Telemetrie ohne Content-Leak bereitstellen.

Jede Policy, die bestimmt, was ein NPC erinnert oder als bekannt betrachtet, gehört diesem Modul oder einem ausdrücklich benannten World-Fact-Owner, niemals dem Repository oder AI Adapter.

## 5 Explizite Ausschlüsse

MOD-003 MUST NOT:

- Dialogzeilen, Motive oder NPC-Verhalten erzeugen;
- Game Actions auswählen;
- Skyrim Runtime, SKSE oder Mod-Dateien direkt ansprechen;
- Generic CRUD oder Raw SQL für andere Module anbieten;
- Secrets, API Keys, beliebige Prompts oder vollständige Provider Transcripts speichern;
- Event History ohne Vertrag als Source of Truth behandeln;
- den gesamten Application State selbständig restaurieren;
- Distributed Sync oder Cloud Replication in v1.0 implementieren;
- eine Vector Database als Pflichtabhängigkeit von v1.0 einführen;
- AI-Vermutungen ohne deterministische Prüfung zu Canonical Facts erklären.

Dialogue, Relationship, Quest und World Policies dürfen Memory nutzen, behalten aber ihre eigenen Entscheidungen und Invariants.

## 6 Contract Ownership und Dependencies

Ein Port wird beim Consumer definiert, der ihn benötigt. Die Memory Engine besitzt `IMemoryRepositoryPort`, weil sie die benötigten Operationen über owned data formuliert. Der Context Manager besitzt `IMemoryContextSourcePort`; die Memory Engine implementiert ihn ohne Repository Contract offenzulegen. Der Save Manager besitzt die `SaveParticipant`-Abstraktion; MOD-003 liefert einen Adapter.

| Vertrag | Owner | Implementer | Verbotenes Leak |
|---|---|---|---|
| IMemoryCommandPort | Application/Memory Use Case | Memory Engine | SQL und AI Provider Types |
| IMemoryQueryPort | Consumer Use Case | Memory Engine | Physical Rows und Mutable Entities |
| IMemoryRepositoryPort | Memory Engine | Persistence Adapter | SQLite Handles und Tabellennamen |
| IMemoryContextSourcePort | Context Manager | Memory Engine | Repository Access |
| SaveParticipant | Save Manager | Memory Adapter | Global Save Orchestration |
| IEventPublisherPort | Producer/Use-Case Boundary | Event Engine Adapter | Synchronous Request Semantics |

Runtime Service Locator, gemeinsames `GetRepository<T>()`, direkte Persistence-Imports im Domain Code und zyklische statische Memory↔Dialogue-Abhängigkeiten sind verboten.

## 7 Begriffe und Invarianten

**MemoryRecord** ist ein bestätigter, versionierter und persistierbarer Record. **MemoryCandidate** ist ein ungeprüfter Schreibvorschlag. **Subject** ist der Träger der Erinnerung, meist ein NPC. **Object** ist eine referenzierte Entität. **Provenance** beschreibt nachvollziehbare Herkunft. **Authority** ist eine Vertrauensklasse, keine Wahrscheinlichkeit. **Confidence** ist eine begrenzte Unsicherheitsbewertung. **Salience** ist die berechnete Ranking-Relevanz. **Retention** bestimmt Lebensdauer und Verdichtung. **Supersession** verbindet einen neuen mit einem ersetzten Record, ohne Audit History zu zerstören.

Globale Invarianten:

1. Jeder durable Record besitzt `memory_id`, `subject_id`, `type`, `occurred_at`, `recorded_at`, `provenance` und `schema_version`.
2. `recorded_at` ersetzt nicht die Ereigniszeit.
3. Canonical World Facts haben bei Konflikt Vorrang vor AI Inference.
4. Fehlender Record bedeutet keinen negativen Fakt.
5. Expiry/Delete erzeugt keine dangling references.
6. Read Results sind immutable snapshots.
7. Derselbe Idempotency Key erzeugt keinen zweiten Record.

## 8 Lifecycle und Readiness

Lifecycle: `Constructed → Configured → RepositoryReady → RestoredOrEmpty → Ready → Degraded → Stopping → Stopped`. Übergänge sind einseitig, ausgenommen ein kontrolliertes `Degraded → Ready` nach nachgewiesener Recovery.

Beim Startup übergibt der Host typed configuration und Ports. MOD-003 prüft Configuration Schema, Limits, Types und Repository Readiness. Migrationen laufen im Persistence Subsystem vor `RepositoryReady`. Anschließend aktiviert der Save Manager einen konsistenten Snapshot oder bestätigt ausdrücklich eine neue Welt. Erst danach wird Ready veröffentlicht.

| Zustand | Lesen | Schreiben | Exit-Bedingung |
|---|---|---|---|
| Constructed | Nein | Nein | Dependencies injected |
| Configured | Nein | Nein | Configuration valid |
| RepositoryReady | Eingeschränkt | Nein | Restore/New World Decision |
| Ready | Ja | Ja | Normalbetrieb |
| Degraded | Policy-abhängig | Meist Reject/Bounded Queue | Dependency Recovery |
| Stopping | Nur Drain | Neue Commands Reject | Bounded Shutdown |

Shutdown schließt Command Intake, beendet erlaubte Transaktionen, flushes bounded metrics und gibt den Repository Scope frei. Unbegrenztes Warten ist verboten.

## 9 Memory-Taxonomie

v1.0 verwendet ein geschlossenes Type Registry. Erweiterungen benötigen eine versionierte Schema-Änderung und Compatibility Tests.

| Typ | Zweck | Typische Quelle | Basis-Retention |
|---|---|---|---|
| Observation | Vom Subject beobachtetes Ereignis | Game/World Use Case | Normal |
| Interaction | Bedeutende Actor-Interaktion | Dialogue/Trade/Combat Use Case | Normal |
| Knowledge | Erlernte Aussage | Approved Knowledge Path | Policy-based |
| ReputationEvidence | Grundlage für Beziehung/Reputation | Relationship Use Case | Long |
| QuestOutcome | Ergebnis eines Quest-Ereignisses | Quest Use Case | Long |
| Rumor | Unbestätigte übermittelte Information | Social/Dialogue Use Case | Short/Normal |
| Commitment | Versprechen, Schuld oder Vereinbarung | Owning Use Case | UntilResolved |
| WorldChange | Bedeutende Weltänderung | World Owner | Long |
| Correction | Korrektur einer früheren Aussage | Owner/Validation Workflow | Follows target |
| Summary | Abgeleitete Verdichtung mehrerer Records | Memory Maintenance | Policy-basiert |

Der Type bestimmt Pflichtfelder, zulässige Provenance Kinds, Authority Floor, Retention und Retrieval Eligibility. Freie String Types sind unzulässig.

## 10 Identity, Scope und Isolation

IDs sind game-unabhängige stabile Werte: `world_id`, `save_lineage_id`, `subject_id`, `entity_id`, `location_id`, `quest_id`. Runtime Handles und Pointer werden niemals persistiert. Der Game Adapter oder owning Use Case konvertiert sie vor dem Command in Canonical Form.

Jeder Record gehört genau zu einer `world_id` und Lineage. Queries ohne expliziten Scope werden abgewiesen. Zwischen Welten, Profilen oder Save Lineages gibt es weder implizite Suche noch Deduplication. Shared Lore ist nur als getrennte Read-only Authority Source zulässig, nicht durch Record Copy.

Subject Alias können normalisiert werden; Canonicalization geschieht vor Persistence. Subject Merge erfordert eine administrative Migration mit Dry Run, Collision Report und Recovery Plan. Automatisches Mergen nach Name ist verboten.

## 11 MemoryRecord-Modell

`MemoryRecord` ist ein immutable Value Object; Änderungen erzeugen neue Revisionen oder Correction/Supersession Records. Das vollständige Mindestmodell steht in Anhang A.

| Gruppe | Felder | Regel |
|---|---|---|
| Identity | memory_id, subject_id, world_id, lineage_id | Nach Commit unveränderlich |
| Semantics | type, predicate, object_refs, content | Content bounded und normalisiert |
| Time | occurred_at, recorded_at, valid_from/to | UTC plus Domain Chronology |
| Trust | provenance, authority, confidence | Unabhängige Begriffe |
| Policy | retention_class, salience_inputs, visibility | Deterministisch vergeben |
| Evolution | schema_version, revision, supersedes | Prüfbare Kette |
| Integrity | content_hash, idempotency_key | Duplicate/Retry Control |

Domain Content wird strukturiert gespeichert. Display Text MAY als lokalisierte Projection existieren, ist aber nie die einzige Bedeutungsquelle. Vollständige AI Prompts/Responses gehören nicht in den Record.

## 12 MemoryCandidate-Modell

Der Candidate trennt untrusted input von durable state. Er enthält Proposal, Source Descriptor, Scope, Timestamps und Correlation Metadata, aber vor Validation weder endgültige `memory_id` noch Authority oder Retention.

Zustände: `Received → StructurallyValid → SemanticallyValid → Deduplicated → Accepted/Rejected → Committed`. Jedes Terminal Result ist immutable und besitzt einen Reason Code. Candidates bleiben nicht unbegrenzt pending; TTL und Queue Limit kommen aus Configuration.

| Quelle | Pflichtangaben |
|---|---|
| Deterministic Game Event | Versioned Event ID, Actor Scope, observed fact |
| Dialogue Outcome | Owning-Use-Case Approval und typed meaning |
| AI Inference | Provider Correlation, Model/Policy Metadata, untrusted flag |
| Administrative Import | Autorisierte Operation, Dry Run und Report |
| Derived Summary | Vollständige Source Memory IDs und Algorithm Version |

Acceptance liefert `Accepted(memory_id, revision)` oder `Duplicate(existing_id)`; Rejection liefert `Rejected(code, safe_details)`.

## 13 Provenance

Provenance MUST beantworten, wer einen Fakt vorgeschlagen hat, über welchen Use Case, aufgrund welchen Events/Records, wann und mit welcher Policy Version er angenommen wurde. Sensitive Raw Payloads sind dafür nicht erforderlich.

Mindestfelder: `source_kind`, `source_id`, `producer_module`, `use_case`, `correlation_id`, `causation_id`, `policy_version`, `observed_by`, `accepted_at`. AI ergänzt Provider Class, verfügbare Model ID/Version und Prompt Template Version, aber weder Secret noch Full Prompt.

Derived Records enthalten eine begrenzte Source-ID-Liste oder Referenz auf ein immutable provenance bundle. Provenance-Zyklen sind verboten. Nach Retention Delete bleibt nur ein erlaubter Tombstone/Hash mit Delete Reason.

## 14 Authority und Confidence

| Authority Class | Beispiel | Konfliktregel |
|---|---|---|
| Canonical | Bestätigter World-State Owner | Vorrang vor Inference und Rumor |
| Witnessed | Vom Subject beobachtetes Event | Subjektiv möglich, aber kein AI Guess |
| Reported | Aussage eines anderen Actors | Übertragungskette bleibt erhalten |
| Inferred | Zulässiger Algorithmus/AI | Keine automatische Hochstufung |
| Rumor | Ausdrücklich unbestätigte Aussage | Kein Canonical Fact |
| Administrative | Kontrollierte Korrektur | Benötigt Audit Metadata |

Confidence ist nur bei Types mit modellierter Unsicherheit zulässig und liegt in `[0,1]`. Sie ersetzt Authority nicht. Hohe AI Confidence macht keinen Canonical Record. Versionierter Decay MAY Confidence reduzieren; Canonical Facts werden nicht auf diese Weise vergessen.

## 15 Zeitmodell

MOD-003 trennt `occurred_at`, `observed_at`, `recorded_at` und optional `valid_from/valid_to`. Machine Timestamps werden in UTC gehalten; In-game Chronology nutzt einen separaten typed marker, damit Game-Time Acceleration Wall-Clock Diagnostics nicht verfälscht.

Out-of-order Events sind zulässig, wenn Identity und Chronology gültig sind. Sie überschreiben jüngere Records nicht automatisch. Ranking verwendet das Alter relativ zur gewählten Timeline und vermischt keine Uhren.

Retention startet auf einer expliziten Basis: standardmäßig `recorded_at`, bei Commitments `resolved_at`, sonst policy-spezifisch. Eine injizierte Clock ist für Tests Pflicht. Clock Jumps lösen keine Massendeletion ohne Dry-run Maintenance Decision aus.

## 16 Ingestion Pipeline

Vollständiger Write Path:

`Producer → Application-owned Command → MemoryCandidate → Structural Validation → Scope/Identity Validation → Policy Classification → Semantic Validation → Duplicate/Conflict Analysis → Repository Transaction → Post-commit Event`.

Vor Commit darf `MemoryUpdated` nicht publiziert werden. Nach Commit enthält das Event IDs, Subject, Type, Revision und sichere Metadata, standardmäßig nicht den Content. Scheitert Event Publication, bleibt der durable Record korrekt; Delivery/Recovery folgt dem Event System Contract.

Jeder Schritt liefert ein typed result. Unklarer Commit Outcome wird per Idempotency Lookup geklärt, nicht durch blindes Retry. Producer sehen keine internen SQL Errors.

## 17 Structural und Semantic Validation

Structural Validation prüft Schema Version, Pflichtfelder, Längen, Enums, ID-Formate, Timestamp Bounds und Unknown Fields im Strict Mode. Semantic Validation prüft Type Rules, erlaubtes Subject, Object-Ref-Kompatibilität, Chronology, Provenance Authority, Content Classification und Permission Context.

Validation MUST für denselben Candidate, dieselbe Policy Version und denselben Reference Snapshot deterministisch sein. Externe AI Calls während Validation sind verboten. Reference Facts werden über approved ports vor der Transaktion oder in einem dokumentierten Consistency Scope gelesen.

Rejection verändert durable state nicht. Error Details nennen Field Path und sicheren Reason, kopieren aber kein vollständiges untrusted payload in Logs.

## 18 Deduplication und Near-Duplicate Policy

Exact Duplicate nutzt Canonical Key und `content_hash`; Idempotency Duplicate nutzt `(producer, idempotency_key, world_id)`. Near Duplicate folgt einem versionierten, type-spezifischen Algorithmus und darf Records unterschiedlicher Authority oder Subjects nicht stillschweigend zusammenführen.

| Ergebnis | Aktion |
|---|---|
| Exact Duplicate | Existing ID, kein zweites Create Event |
| Retry Duplicate | Ursprüngliches Terminal Result |
| Compatible Near Duplicate | Evidence Link/Observed Count nach Policy |
| Conflicting Near Duplicate | Getrennte Records mit Conflict Link |
| Ambiguous | Reject oder Manual Review; kein Raten |

Algorithmus, Threshold und Normalization Version werden in Evidence festgehalten. Vector Similarity MAY später optimieren, ist aber keine v1.0 Source of Truth.

## 19 Konflikt, Correction und Supersession

Konfliktierende Records bleiben getrennte Aussagen mit Authority und Provenance. Eine normale Query Projection darf die aktuell autoritativste Aussage wählen; Audit View bewahrt alle Varianten.

Eine Korrektur erzeugt `Correction` mit `supersedes` oder `contradicts`. Der ursprüngliche Record wird für normale Ausgabe superseded, aber nicht sofort gelöscht. Administrative Corrections erfordern Actor/Operation ID und Reason. Canonical Owner können Rumors widerlegen; Rumors widerlegen keine Canonical Facts.

Revision Chains sind azyklisch und für Reads begrenzt. Maintenance MAY lange Chains verdichten, muss Audit Summary und Hashes erhalten. Hard Delete ist nur per Privacy/Security Policy oder bestätigter Corruption Recovery erlaubt.

## 20 Retention, Expiry und Forgetting

v1.0 kennt `Ephemeral`, `Short`, `Normal`, `Long`, `Permanent`, `UntilResolved` und `LegalOrSecurityHold`.

| Klasse | Verhalten |
|---|---|
| Ephemeral | Nicht durable oder nach kurzem bounded window gelöscht |
| Short | Rumors und kleine Observations |
| Normal | Standardhistorie des Subjects |
| Long | Quest Outcomes, Relations und wichtige World Changes |
| Permanent | Nur ausdrücklich erlaubte Canonical Milestones |
| UntilResolved | Commitment bis typed resolution |
| LegalOrSecurityHold | Formale technische Delete-Sperre |

Maintenance erstellt zuerst einen Dry-run Plan und arbeitet danach in bounded batches. Expired Records verschwinden vor physical cleanup aus normal retrieval. Forgetting ist Data Policy, kein zufälliges Nebenprodukt von Fehlern oder Context Limits.

## 21 Salience und Priorität

Salience entsteht aus Type Weight, Authority, Recency, Relationship Relevance, unresolved state, repetition evidence und explicit importance. Configuration enthält begrenzte Koeffizienten; ein beliebiger AI Score ist niemals allein entscheidend.

Das Ergebnis speichert `algorithm_version` und reproduzierbare Input Classes. Die dynamische Projection MAY neu berechnen, ohne den Record umzuschreiben. Feedback-loop Protection verhindert, dass häufiges Retrieval denselben Record unbegrenzt aufwertet.

Critical Commitments und aktive Quest Facts erhalten Policy Floors, bleiben jedoch World/Subject/Visibility-Filtern unterworfen. Hohe Salience umgeht weder Security noch Authority Conflicts.

## 22 Retrieval Contract

Eine Query enthält Scope, Subject, Purpose, Allowed Types, Time Window, Related Entity IDs, Authority Floor, Limit, Byte/Token Budget und Stable Cursor. Purpose ist Pflicht: `DialogueContext`, `BehaviorContext`, `RelationshipEvidence`, `QuestReasoning`, `Audit` oder `Maintenance`.

Das Resultat ist ein immutable `MemoryQueryResult` mit normalisierten Summaries, Provenance Markers, Score Explanation, Truncation Flag, Snapshot/Version Marker und Next Cursor. Raw Rows und Lazy Database Objects sind verboten.

Queries sind immer bounded. Unbounded Limits, negative Windows und unbekannter Purpose werden abgewiesen. Ein leeres Resultat bedeutet „keine passenden zugänglichen Records“, nicht „Ereignis geschah nicht“.

## 23 Filtering und Ranking

Reihenfolge: World Isolation → Subject Visibility → Type/Purpose Eligibility → Validity/Expiry → Authority → Entity/Time Filters → Candidate Fetch → Deterministic Scoring → Diversity → Budget Packing → Stable Tie-break.

Ranking ist bei gleichem Snapshot reproduzierbar. Stable Tie-break: Score absteigend, occurred_at absteigend, memory_id aufsteigend. Diversity verhindert, dass Wiederholungen eines Ereignisses oder Types das ganze Resultat füllen. Conflict Groups liefern Canonical Projection plus Alternate Marker, wenn der Purpose dies erlaubt.

Score Explanation enthält Contribution Classes, aber keine sensitive Configuration. Audit Purpose darf nach Authorization einen erweiterten Trace liefern.

## 24 Context Budget und Packing

MOD-003 baut keinen finalen AI Prompt. Es liefert dem Context Manager begrenzte Records. Das Budget umfasst Count, Serialized Bytes und optional Token Estimate. Sobald ein Limit erreicht ist, endet Packing mit `truncated=true`.

Priorität: aktive Pflicht-Commitments → canonical/relevant facts → wichtige Interactions → Diversity Additions. Summaries werden nur mit gültiger Provenance/Freshness genutzt. Content-Kürzung verwendet einen type-spezifischen Summarizer und keine zufällige UTF-8-Truncation.

Fehlender optionaler Memory Context erzeugt einen Incomplete Marker. Der owning Use Case entscheidet über Fortsetzung; erfundene Ersatzfakten sind verboten.

## 25 Integration mit Dialogue Engine

Dialogue Engine (MOD-005) liest approved memory projection über einen application-owned query contract. Request enthält Purpose, Participants, Topic/Entity Refs und Budget. MOD-003 sieht keinen Prompt und wählt keine Dialogzeile.

Nach einem Dialog MAY der owning Use Case einen typisierten Candidate vorschlagen: Commitment, reported fact, bedeutende Beleidigung oder Handel. Der Full Transcript wird nicht automatisch gespeichert. Dialogue greift nie direkt auf `IMemoryRepositoryPort` zu.

Bei Read Failure erhält Dialogue `ContextIncomplete` mit Reason Category. Reduced-context continuation ist nur nach dokumentierter Policy erlaubt und darf keine Aussagen erzeugen, die fehlende Memory voraussetzen.

## 26 AI-Integration

Der AI Adapter liefert structured candidate output. Der Application Use Case prüft Provider Schema und Berechtigung und erzeugt danach einen `MemoryCandidate` mit `Inferred` oder `Reported` entsprechend der tatsächlichen Herkunft. MOD-003 validiert erneut und darf unabhängig ablehnen.

AI MUST NOT Authority canonical setzen, SQL/Storage IDs liefern, Repository schreiben, Retention/Security Class ändern, fehlende Provenance verschleiern oder Records allein wegen plausibler Formulierung erzeugen.

Provider Timeout oder malformed output erzeugt keinen Candidate. Jeder angenommene AI-derived Record trägt Model/Policy Provenance und bleibt separat filterbar.

## 27 Character- und Relationship-Integration

Character Engine darf Memory für Reaktionen nutzen, besitzt jedoch Traits und Behavior Policy. Relationship Engine besitzt Relationship State; MOD-003 speichert erklärendes Evidence, berechnet aber nicht den finalen Relationship State ohne separaten Port Contract.

Der owning module commit erfolgt zuerst, danach erzeugt ein Semantic Event oder explicit candidate das Evidence Memory. Der umgekehrte Weg ist verboten, wenn er bei failed transaction falsche Erinnerung hinterließe.

`RelationshipEvidence` Queries liefern Records und Trust Markers, nicht den endgültigen Relationship-Wert. Damit existieren keine konkurrierenden Sources of Truth.

## 28 Quest-, Organization- und World-Integration

Quest Engine publiziert bestätigte Milestones nach Commit. MOD-003 erzeugt `QuestOutcome` oder `Knowledge` nur für Subjects, die das Ereignis gemäß Witness/Distribution Policy kennen konnten. Organization und World Owner verhalten sich entsprechend.

Ein globales Event wird nicht automatisch für alle NPCs dupliziert. Knowledge Distribution ist ein eigener Use Case mit bounded fan-out, Eligibility Rules und Provenance Chain. Batch Writes besitzen Operation ID, Checkpoint und Idempotency; jeder Record bleibt Subject-scoped.

Korrigiert ein Upstream Owner sein Event, publiziert er ein Correction Event; MOD-003 errät keinen neuen State aus fehlenden Events.

## 29 Event-Integration

MOD-003 konsumiert nur versionierte Semantic Events aus dem Catalog. Ein Event ersetzt keinen synchronen Command mit sofortigem Ergebnis. Producer committen vor Publish.

Publizierte v1 Events: `MemoryAccepted`, `MemoryCorrected`, `MemoryExpired`, `MemorySummaryCreated`, `MemoryMaintenanceFailed`, `MemoryReadinessChanged`. Payload enthält stabile IDs, Version, Correlation/Causation und minimale Metadata.

Delivery kann at-least-once sein; Handler verwenden Event ID als Idempotency Key. Replay gilt nur für deklarierte replay-safe handlers. Event History ersetzt weder Repository noch Restore ohne gesonderte Entscheidung.

## 30 Repository Boundary

`IMemoryRepositoryPort` beschreibt Use-Case-Operationen statt Tabellen: `CommitCandidate`, `GetById`, `QuerySnapshot`, `ApplyCorrection`, `PlanRetention`, `ApplyRetentionBatch`, `ExportParticipantSnapshot`, `StageParticipantRestore`.

Die Implementation besitzt SQL, Mapping, Connection Scope, Transaktionen, Busy Retry, Physical Indexes, Migrationen und Backup Mechanics. MOD-003 übergibt Domain Values und Transaction Intent; SQLite Types überschreiten die Boundary nicht.

Generic `Save(entity)`, `Find(table, predicate)` und Connection Access sind verboten. Persistence Errors werden typisiert. Connection Failure wird nie als leeres Query Result ausgegeben.

## 31 SQLite-Mapping-Regeln

v1.x nutzt lokales SQLite hinter dem Persistence Subsystem gemäß ADR-003. Eine Domain Entity muss keiner einzelnen Tabelle entsprechen. Die physische Struktur darf Object Refs, Provenance und Conflict Links normalisieren, solange Domain Semantics erhalten bleiben.

Pflicht sind geeignete Foreign Keys/Constraints, Unique Index für Idempotency Scope, Subject/World/Time Index, Retrieval-Eligibility Index und Migration Metadata mit Checksum. Tabellennamen sind kein Public Contract von MOD-003.

WAL, Busy Timeout, Connection Count und Vacuum Strategy werden nach Messungen festgelegt. Vor Benchmark Gate sind keine konkreten Werte verifiziert.

## 32 Transaktionen und Idempotency

Candidate Acceptance bildet eine logische Transaktion: Duplicate Check, Record Insert, Provenance/Link Insert und gegebenenfalls Outbox Evidence. Partial Commit ist verboten.

Der Idempotency Key ist im Scope Producer/World/Operation Kind eindeutig. Retry nach Timeout liest zuerst das Terminal Result. Key Reuse mit anderem Content Hash erzeugt `IdempotencyConflict` plus Security Diagnostic.

Lange Multi-Subject Operations werden in bounded batches mit Operation ID und Checkpoint geteilt. Eine Transaktion umfasst niemals AI Call, Network, Gameplay Callback oder externen Subscriber Wait.

## 33 Concurrency und Backpressure

Reads nutzen immutable Snapshot/Version Markers. Writes auf dasselbe Subject werden logisch serialisiert oder per optimistic revision geschützt. Unterschiedliche Subjects MAY innerhalb der Persistence Policy parallel schreiben.

| Queue | Overflow Policy |
|---|---|
| Interactive Writes | Busy vor Enqueue oder kurzer bounded wait außerhalb kritischer Game Paths |
| Background Imports | Pause/Checkpoint; keine Verdrängung interaktiver Arbeit |
| Maintenance | Niedrige Priorität, cancellable batches |
| Event Candidates | Bounded queue, Dedup vor Persistence |
| Diagnostics | Drop/Coalesce mit Counter; kein Content Log |

Externe Callbacks laufen nie unter internen Locks. Shutdown schließt Producers, cancelt Maintenance, drains bounded work und beendet Commands mit typed results.

## 34 Cache und Indexing

Cache ist Optimierung, niemals Source of Truth. Zulässig sind immutable Recent-Memory Cache per Subject, Query-Plan Cache und Reference Lookup Cache. Keys enthalten World, Lineage, Subject, Purpose-relevante Version und Policy Version.

Commit invalidiert/aktualisiert Cache erst nach durable success. Restore wechselt Generation und entwertet alte Entries. Negative Cache besitzt kurze TTL und verwandelt Repository Failure nicht in „keine Daten“.

Index Design folgt Representative Corpus. Ein Index wird nur mit Messung von Write Cost, File Growth und Migration Impact angenommen. FTS/Vector Index bleiben optionale Adapter hinter separater Entscheidung.

## 35 SaveParticipant Contract

MOD-003 implementiert den Save-Manager-owned `SaveParticipant`. Der Snapshot enthält den für konsistenten Restore erforderlichen Logical State: Schema Version, World/Lineage, High-water Marks, aktive Policy Version, unresolved Commitments/Maintenance Markers und nach Save Policy erlaubte Daten/Referenzen.

`PrepareSnapshot` liefert immutable candidate und checksum, schreibt aber kein globales Save Artifact. Der Save Manager sammelt alle Participant Candidates, validiert sie quer und committet danach das Artifact.

Gewöhnliche SQLite Persistence und Save Artifact haben unterschiedliche Zwecke. Database Backup ersetzt das Save System nicht; ein Save Artifact ist keine beliebige Kopie der Database File.

## 36 Staged Restore und atomare Aktivierung

Restore: `Read Artifact → Validate Envelope → Deserialize NON-LIVE Candidate → Memory Stage → Cross-participant Validation → Atomic Logical Activation → Post-activation Evidence`.

Vor Activation bleibt Live State unverändert. Memory Stage prüft Checksum, Schema Compatibility, World/Lineage, Referential Integrity, Revision Monotonicity, Policy Versions und Budget. Lehnt ein Participant ab, scheitert der gesamte Restore; Partial Success ist verboten.

Activation wechselt Generation. In-flight Queries der alten Generation enden auf altem Snapshot oder als Cancel Result und mischen keine States. Repository Side Effects folgen einem mit Save Manager/Persistence abgestimmten Transaction/Recovery Plan.

## 37 Schema Versioning und Migrationen

Domain Record, Repository, Configuration, Event und Save-Participant Schema werden getrennt versioniert. Gleiche Nummern werden nicht vorausgesetzt.

Jede Migration besitzt Source/Target Version, Checksum, Preconditions, Dry Run/Report, Atomicity Class, Backup/Recovery Plan und Integration Tests auf realer Temporary Database. Unsupported Future Schema wird ohne destructive downgrade abgelehnt; Failure bewahrt recoverable previous state.

Semantic Migration gehört MOD-003; Physical Transformation implementiert das Repository nach approved plan. Danach folgen Integrity Scan und representative retrieval comparison.

## 38 Configuration Contract

MOD-003 erhält nur die typed Section `memory`, niemals den kompletten YAML Tree. Unknown Fields werden im Strict Mode abgelehnt. Safe Defaults gelten nur für optionale Werte; fehlende Pflicht-Security- oder Capacity-Limits führen zu Startup Failure.

Setting Groups: Candidate/Content Limits, Retention Policies, Salience Weights, Retrieval Budgets, Maintenance Schedule, Cache Limits, Repository Timeout Reference, Observability Sampling und Feature Flags. Secrets sind verboten.

Hot Reload betrifft nur künftige Requests und benötigt atomare Candidate Validation. Schema Version, Repository Path, Identity Rules, Security Boundary und Save Compatibility verlangen Restart/Migration. Bei Rejection bleibt die ganze bisherige Effective Configuration aktiv.

## 39 Security und Privacy

Producer Payloads gelten als untrusted. Validation begrenzt Größe, Encoding, Nesting, IDs und Classification. Output Encoding gehört dem Consumer; gespeichertes Markup ist kein trusted executable content.

Secrets, Access Tokens, File Paths, Memory Addresses und rohe Provider Diagnostics sind verboten. Sensitive Story Content MAY nur unter expliziter Classification/Visibility Policy bestehen. Logs enthalten IDs/Hashes, Sizes, Codes und Durations, aber keinen gewöhnlichen Memory Text.

Administrative Import/Export braucht Authorization Context, Path Policy, bounded archive, checksum und audit record. Import führt keine Scripts aus und vertraut eingebetteten Schema-Angaben nicht.

## 40 Datenminimierung und Löschung

Gespeichert wird die minimale Semantik für den Gameplay Use Case. Full Dialogue, Audio, Prompt, Provider Response und duplizierter World State werden standardmäßig nicht persistiert. Structured Predicates sind langen Texten vorzuziehen.

Privacy Delete läuft über stabilen Subject/Entity Scope mit Dry-run Count, Authorization, Referential Plan und Post-delete Report. Derived Summaries und Provenance Links werden mitbehandelt; es darf keine enthüllende Kopie zurückbleiben.

Tombstones enthalten nur minimal erlaubte Integrity/Idempotency Fields. Konflikte zwischen Retention/Audit und Delete benötigen formale Governance statt stiller Ignorierung.

## 41 Observability und Health

Metrics: accepted/rejected/duplicate, Read/Write Latency, Result Size, Truncation, Queue Depth, Contention, Cache Hit, Retention Counts, Restore Duration und Integrity Failures. High-cardinality Labels wie Memory/Subject IDs oder Content sind verboten.

Structured Logs enthalten Operation, Result Code, Correlation, World Alias/Hash, Duration und Counts. Tracing folgt Causation ohne Payload. Health Snapshot zeigt Lifecycle, Repository Availability, letzte erfolgreiche Operationen, Queue Saturation, Migration/Schema Evidence und Degraded Reason.

Readiness ist nicht Liveness. Ohne Repository oder abgeschlossenen Restore ist das Modul nicht Ready.

## 42 Performance Budgets

Bis Benchmark Gate sind dies Designziele, keine verifizierten Zusagen:

| Operation | Designziel p95 | Bedingung |
|---|---:|---|
| Recent bounded context | ≤ 20 ms | Warm local DB, ≤ 32 Records |
| Structured retrieval | ≤ 50 ms | Representative indexed corpus |
| Deterministic candidate accept | ≤ 25 ms | Keine externen Calls |
| Duplicate rejection | ≤ 10 ms | Exact/Idempotency Path |
| Participant snapshot | ≤ 100 ms | Bounded State |

Der Game Main Thread wartet nie direkt. Benchmark Corpus: 1k, 10k und 100k Records pro Welt, skewed subjects, conflicts und expired data. Report dokumentiert Hardware, DB-Größe, Cold/Warm Runs und Percentiles.

## 43 Failure Model

Kategorien: Validation, Authorization, Conflict, Capacity, PersistenceUnavailable, PersistenceBusy, SchemaMismatch, CorruptionSuspected, SaveRestore, Cancelled, InternalInvariant. Jeder Fehler hat stabilen Code, Retryability und sichere Details.

Empty Result ersetzt keinen Failure. Timeout mit unbekanntem Commit Status liefert `CommitOutcomeUnknown`; Lookup erfolgt über Idempotency. Corruption Suspicion schaltet Writes in RecoveryRequired und startet keine destructive repair.

Unerwartete Exceptions werden an der Application Boundary in Correlation Evidence übersetzt, ohne Content preiszugeben. Fatal Invariant Violation setzt NotReady.

## 44 Retry, Recovery und Integrity

Retry ist nur für explizit transiente Repository Conditions mit bounded attempts und Persistence Jitter Policy zulässig. Validation, Authorization, Schema Mismatch und Idempotency Conflict werden nicht automatisch wiederholt.

Startup Checks: Schema/Checksum, Index/Constraint Evidence, letzte Migration, Save Generation Compatibility und optional Quick Scan. Deep Scan läuft offline oder als Maintenance.

Recovery: Open Retry, approved Backup Restore, dokumentierter Migration Recovery, Rebuild eines derived index, Replay explizit durable Candidate/Outbox. Authoritative Memory wird nie aus AI oder normalen Logs rekonstruiert.

## 45 Degraded Modes

| Ursache | Lesen | Schreiben | Marker |
|---|---|---|---|
| Repository busy | Policy-sicherer Cache | Reject/Short Queue | Stale/Incomplete |
| Repository unavailable | Safe Cache Snapshot | Reject | ContextIncomplete |
| Maintenance backlog | Normal | Normal mit Rate Limit | MaintenanceDelayed |
| Optional index failed | Bounded fallback query | Normal | RetrievalDegraded |
| Corruption suspected | Verified Snapshot/Audit | Stop | RecoveryRequired |
| Restore pending | Keine Mixed Reads | Nein | NotReady |

Reduced-context Policy gehört dem aufrufenden Use Case. MOD-003 meldet Incompleteness und erfindet nichts. Rückkehr zu Ready braucht Health Evidence, nicht nur einen Timer.

## 46 Public Ports

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

Methoden nehmen Cancellation/Deadline Context und liefern typed results. Exceptions sind kein normaler Boundary Transport. Contract Types entwickeln sich backward-compatible oder über einen neuen Major Port.

## 47 Commands und Results

Command Envelope: `contract_version`, `operation_id`, `idempotency_key`, `scope`, `caller`, `deadline`, `correlation_id`, `payload`. Caller Identity allein ist keine Authorization; Host/Application liefert geprüften Capability Context.

Success enthält Operation ID, Memory/Revision IDs, Commit Marker und Warnings. Rejection enthält Code, Field Paths und Retryable Flag. Kein Result exponiert Raw Exception/SQL.

Deadline wird vor teuren Schritten und Transaction Begin geprüft. Nach Commit verwandelt eine abgelaufene Deadline Erfolg nicht in Fehler; das Resultat kann über Operation/Idempotency Key wiedergewonnen werden.

## 48 Event Catalog

| Event | Veröffentlichungszeitpunkt | Mindest-Payload |
|---|---|---|
| MemoryAccepted.v1 | Nach neuem Record Commit | memory_id, subject_id, type, revision |
| MemoryCorrected.v1 | Nach Correction Commit | old_id, correction_id, relation |
| MemoryExpired.v1 | Nach Retention Commit | memory_id oder bounded batch reference |
| MemorySummaryCreated.v1 | Nach Summary Commit | summary_id, source_count, policy_version |
| MemoryReadinessChanged.v1 | Nach State Transition | previous, current, reason |
| MemoryMaintenanceFailed.v1 | Nach terminalem Batch Failure | operation_id, safe code, retryable |

Event Schema Owner steht im Catalog. PII/Sensitive Content fehlt standardmäßig. Breaking Change erzeugt neue Major Event Version.

## 49 Error-Code-Catalog

| Code | Retry | Bedeutung |
|---|---|---|
| MEM_VALIDATION_FAILED | No | Struktur/Semantik ungültig |
| MEM_SCOPE_MISMATCH | No | World/Lineage/Subject Scope falsch |
| MEM_UNAUTHORIZED | No | Capability Context unzureichend |
| MEM_DUPLICATE | No | Existing Record gefunden |
| MEM_IDEMPOTENCY_CONFLICT | No | Gleicher Key, anderer Payload |
| MEM_CAPACITY_EXCEEDED | Maybe | Bounded Capacity erschöpft |
| MEM_REPOSITORY_BUSY | Bounded | Transiente Contention |
| MEM_REPOSITORY_UNAVAILABLE | Policy | Persistence nicht verfügbar |
| MEM_COMMIT_OUTCOME_UNKNOWN | Lookup | Idempotency Lookup nötig |
| MEM_SCHEMA_UNSUPPORTED | No | Unsupported Version |
| MEM_INTEGRITY_FAILURE | No | Invariant verletzt |
| MEM_RESTORE_REJECTED | No | Stage Validation fehlgeschlagen |
| MEM_CANCELLED | Caller | Vor Commit abgebrochen |

## 50 Unit-Test-Strategie

Unit Tests prüfen Normalization, Policy Classification, Validation, Authority Ordering, Time Rules, Salience, Ranking, Diversity, Budgets, Conflict Graph und State Machine. Clock, ID Generator und Policy Registry werden injiziert.

Property Tests: Normalization Idempotence, Ranking Determinism, keine Cross-world Leakage, azyklische Supersession, Budget Never Exceeded und Stable Retry Result. Fuzz Tests begrenzen Laufzeit und Speicher.

Jede MUST Domain Rule besitzt Test ID und Traceability Link. Snapshot Tests ersetzen keine semantischen Assertions.

## 51 Contract- und Integrationstests

Die Contract Suite läuft gegen In-memory Fake und echtes Repository. Sie prüft identische typed results, Pagination, Rollback, Idempotency, Optimistic Conflict und Failure Mapping.

Integration Scenarios verbinden Application, MOD-003, Event Adapter, Repository und Fake Save Manager. Dialogue/AI verwenden Contract Doubles statt internem Zugriff. Statische Tests beweisen, dass SQLite Types außerhalb des Persistence Adapter fehlen.

Compatibility Fixtures decken frühere supported schema versions ab; Future Version wird ohne File Mutation abgelehnt.

## 52 Persistence- und Migrationstests

Jeder Test erstellt eine Temporary Database, führt echte Migrationen aus und aktiviert Foreign Keys/production-like pragmas. Geprüft werden Clean Install, Sequential Upgrade, Interrupted Recovery, Checksum Mismatch, Busy/Locked Mapping, Rollback und Data Preservation.

Datasets enthalten Unicode, lange gültige Inhalte, Conflict Chains, High Fan-out, Expired Records und Orphan Attempts. Critical Query Plans/Indexes werden geprüft, ohne Domain Contracts an einen konkreten Plan zu binden.

Database Backup/Restore Mechanics werden getrennt von Save System Semantics getestet.

## 53 Retrieval-Quality-Tests

Ein Golden Corpus beschreibt Subjects, Facts, Rumors, Conflicts, Recency, Authority und erwartete Top-k Sets. Geprüft werden Ordering/Eligibility-Eigenschaften und zulässige Wertebereiche, nicht fragile Float-Gleichheit.

Metriken: precision@k, canonical-conflict accuracy, duplicate suppression, diversity coverage, truncation correctness und latency. Änderungen der Algorithm Version werden gegen einen Baseline Report verglichen.

AI ist kein Judge für verpflichtende Acceptance Tests. Human Review darf den Corpus erweitern, Kriterien bleiben reproduzierbar.

## 54 Save/Restore-Tests

Geprüft werden Snapshot Determinism, Checksum, Version Compatibility, Stage ohne Live Mutation, Cross-participant Reject, Atomic Activation, Generation Invalidation, Cancel und Crash Points. Nach failed restore entspricht Live State exakt dem Zustand davor.

Round Trip: Records erzeugen → Snapshot → Live State ändern → Original stage → gemeinsam mit Fake Participants aktivieren → Semantic State und Representative Queries vergleichen. Physical SQLite Layout muss nicht byte-identisch sein.

Partial Participant Success ist niemals globaler Success; der Memory Adapter ruft keine anderen Participants auf.

## 55 Security Tests

Negative Cases: Oversized Content, malformed UTF-8 am Transport, Unknown Enum, Path Traversal im Import, Formula-like Export, Secret Patterns, Unauthorized Audit Query, Cross-world ID, Provenance Cycle, Decompression Bomb und malicious Future Schema.

Logs werden auf Raw Content, Prompt, Token, File Path und Database Row geprüft. Import/Export nutzt sichere Filenames, bounded sizes und checksums. Fuzzing endet kontrolliert ohne Crash oder unbounded allocation.

Threat Model Review ist vor Operational und bei External Sync, Vector Service oder Admin UI Pflicht.

## 56 Load-, Stress- und Soak-Tests

Workloads: Dialogue-heavy Reads, Event Burst Writes, ein Hot Subject, viele Subjects, Retention parallel zu Reads, Save Snapshot unter Last und Restart nach erzwungenem Abbruch. Reports enthalten Percentiles, Error Rates, Queue Depth, DB Growth, CPU, Memory und Recovery Time.

Soak beweist fehlendes unbounded growth. Backpressure erzeugt kontrolliertes Busy/Degraded statt Memory Exhaustion. Crash Injection erfolgt vor Transaktion, im Persistence Adapter und nach Commit vor Event Publish.

Thresholds werden in M3.6 auf dem Ziel-Windows-System bestätigt; vorher bleiben Abschnitt 42 und Werte Designziele.

## 57 Acceptance Criteria

Operational ist nur zulässig, wenn:

1. Ports und Contract Types ohne verbotene Dependencies implementiert sind.
2. SQLite Migration Chain Clean/Upgrade/Recovery besteht.
3. Validation, Provenance, Authority, Dedup, Correction und Retention abgedeckt sind.
4. Retrieval bounded, deterministic und Golden-Corpus-geprüft ist.
5. SaveParticipant Staged Atomic Restore besteht.
6. AI/Dialogue keine direkte Repository-Schreibmöglichkeit besitzen.
7. Security/Privacy Negative Suite ohne Leaks besteht.
8. Load/Soak bestätigte Budgets und Backpressure zeigt.
9. RU/DE Docs, Schema-, Event-, Error-Catalog und Runbook synchron sind.
10. Kein High/Critical Blocker offen ist.

Documentation Complete ist nicht Implementation Complete.

## 58 Implementation Roadmap und Gates

| Gate | Ergebnis | Exit Evidence |
|---|---|---|
| M3.0 Design Freeze | Spezifikation freigegeben | Review Record, Decisions resolved |
| M3.1 Contracts | Value Objects, Ports, Pure Policies | Unit/Property Tests |
| M3.2 Repository | SQLite Adapter und Migrationen | Integration/Migration Report |
| M3.3 Write Path | Candidate-to-Commit | Idempotency/Failure Tests |
| M3.4 Read Path | Bounded Retrieval/Context | Golden Corpus Report |
| M3.5 Save/Event | Participant und Post-commit Events | Restore/Event Tests |
| M3.6 Hardening | Security, Load, Recovery | Reports und Runbook |
| M3.7 Operational | Release Evidence | Acceptance Checklist |

Jedes Gate endet mit getrenntem Commit/Review. Spätere Gates maskieren keine früheren Fehler. Feature Flags erklären unvollständige Funktionen nicht fertig.

## 59 Operations und Troubleshooting

Das Runbook enthält Readiness, Schema/Migration Status, Queue Saturation, letzte erfolgreiche Operation, Integrity Scan, Backup Availability, Retention Backlog und Safe Diagnostic Export.

Triage: Scope/Correlation bestimmen → Lifecycle prüfen → Repository Health → Schema/Migration Evidence → Capacity → Typed Failures → Query/Command Fixture sicher reproduzieren. Manuelles Editieren der Database ist kein erster Schritt.

Bei Corruption Suspicion: Writes stoppen, recoverable copy mit approved tool erzeugen, Hashes/Metadata sammeln, dokumentierten Scan ausführen, Restore/Migration Recovery wählen. Keine ad-hoc SQL Fixes auf der einzigen Kopie.

## 60 Traceability, offene Entscheidungen und Historie

| Quelle | Normativer Einfluss |
|---|---|
| ARCH-002 | MOD-003 = Memory Engine im Simulation Layer |
| ARCH-003 | Async erlaubt; verhaltensrelevanter State rechtzeitig verfügbar |
| ARCH-005 / ADR-008 | Static Dependencies und Inverted Port Ownership |
| ARCH-007 | Dialogue Path, AI Candidate Path, Post-commit Event |
| ARCH-009 | Typed Scoped Configuration und Atomic Validation |
| ARCH-011 | SaveParticipant und Staged Atomic Restore |
| ARCH-012 | Context Source Port, Provenance, kein AI Direct Persistence |
| ADR-002 | Semantic Events, Idempotency, Event History Boundary |
| ADR-003 | SQLite hinter Narrow Repositories, Migration/Transaction Ownership |
| MOD-001 | Core koordiniert, besitzt aber keine Memory Domain Logic |

Offen bis M3.0: Physical Persistence Schema, Salience-Koeffizienten, Default Retention Durations, Admin Export Format, FTS-Bedarf, Outbox Strategy und Benchmark Hardware Profile. Diese Punkte ändern weder Ownership noch Safety Rules.

Historie: v1.0 vom 2026-09-05 — vollständige deutsche Design Baseline vor Implementierungsbeginn erstellt.

## Anhang A — normatives MemoryRecord-Schema

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

Pflichtregeln: IDs nicht leer, Object Refs eindeutig, Content type-spezifisch gültig, Confidence nur für erlaubte Types, valid_to nicht vor valid_from, kein Self-supersession und Content Hash nach Canonical Normalization.

## Anhang B — normatives MemoryCandidate-Schema

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

Candidate Size wird vor Nested Parsing begrenzt. `ai_metadata` enthält weder Credentials noch Full Prompt. Unknown Source Kind wird abgewiesen.

## Anhang C — Query- und Result-Schema

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

`MemoryProjection` enthält ID, Type, Structured Summary, Entity Refs, Time, Authority/Confidence, Provenance Summary, Salience Explanation Classes und Conflict Marker. Cursor sind opaque und exponieren keinen SQL Offset.

## Anhang D — Configuration-Schema-Outline

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

Werte in spitzen Klammern erhalten measured defaults in M3.6. `content_logging=true` wird vom Schema abgelehnt.

## Anhang E — minimaler Test Catalog

| ID | Szenario | Erwartetes Ergebnis |
|---|---|---|
| M3-VAL-001 | Candidate ohne Subject | MEM_VALIDATION_FAILED, no write |
| M3-VAL-002 | Future Contract Version | MEM_SCHEMA_UNSUPPORTED |
| M3-ID-001 | Cross-world Query | MEM_SCOPE_MISMATCH |
| M3-IDEM-001 | Identischer Key/Payload Retry | Original Result, ein Record |
| M3-IDEM-002 | Gleicher Key, anderer Hash | MEM_IDEMPOTENCY_CONFLICT |
| M3-AUTH-001 | AI Conflict mit Canonical | Canonical Projection zuerst |
| M3-DUP-001 | Exact normalized duplicate | Existing ID, kein Create Event |
| M3-DUP-002 | Near duplicate anderer Authority | Separate Records/Conflict Link |
| M3-RET-001 | Expired Record | Nicht in Normal Query |
| M3-RET-002 | Dry-run Retention | Keine Mutation, exakter Count |
| M3-QRY-001 | Byte Budget erreicht | truncated=true, kein Overflow |
| M3-QRY-002 | Gleiche Scores | Stable Tie-break |
| M3-QRY-003 | Repository Failure | Typed Failure statt Empty Result |
| M3-EVT-001 | Commit Failure | Kein MemoryAccepted Event |
| M3-EVT-002 | Duplicate Delivery | Ein Semantic Effect |
| M3-SAVE-001 | Invalid Checksum Stage | Reject, Live State unverändert |
| M3-SAVE-002 | Anderer Participant Reject | Keine Activation |
| M3-SAVE-003 | Successful Activation | Neue Generation, konsistente Query |
| M3-SEC-001 | Secret-like Payload | Policy Reject/Classify, kein Log Leak |
| M3-SEC-002 | Unauthorized Audit View | MEM_UNAUTHORIZED |
| M3-MIG-001 | Interrupted Migration | Recoverable Previous State |
| M3-PERF-001 | Hot-subject Write Burst | Bounded Backpressure |
| M3-FAIL-001 | Unknown Commit Outcome | Lookup per Idempotency Key |
| M3-FAIL-002 | Corruption Suspected | Writes Stop, RecoveryRequired |

## Anhang F — Implementation Checklist

- [ ] M3.0 Ownership und offene Entscheidungen reviewed.
- [ ] Public Contract Package enthält keine SQLite-, Skyrim-, AI-SDK- oder UI-Types.
- [ ] Policy Registry und Record/Candidate Schemas sind versioniert.
- [ ] Repository Migrationen besitzen Checksum und Recovery Plan.
- [ ] Write Paths sind idempotent und Post-commit-event-safe.
- [ ] Retrieval ist nach Count/Bytes begrenzt und stabil sortiert.
- [ ] Context Results zeigen Provenance, Authority und Completeness.
- [ ] SaveParticipant staged Non-live State und aktiviert atomar.
- [ ] Security Suite beweist fehlenden Raw Content in normalen Logs.
- [ ] RU/DE Spezifikationen, Event/Error Catalogs und Runbook stimmen überein.
- [ ] Acceptance Evidence wird vor Status Operational gespeichert.

## Anhang G — normative Use-Case-Sequenzen

### G.1 Beobachtetes Game Event

1. Der Game Adapter normalisiert das Runtime Event und liefert dem Application Use Case stabile IDs, Generation und Event Evidence.
2. Der owning World Use Case bestätigt das Ereignis und bestimmt potenzielle Witnesses.
3. Für jedes eligible Subject entsteht ein eigener `MemoryCandidate` mit gemeinsamer Causation ID und eindeutigem Idempotency Key.
4. MOD-003 prüft World/Lineage, Witness Relation, Type Schema und Provenance Authority.
5. Duplicate Policy läuft vor der teuren Persistence Operation.
6. Das Repository speichert Record und Links atomar; erst danach wird `MemoryAccepted.v1` publiziert.
7. Ein fehlgeschlagenes Subject macht den gesamten Fan-out nicht erfolgreich. Der Batch Coordinator liefert Counts und sichere Per-item Results.

### G.2 Im Dialog berichteter Fakt

Dialogue erhält einen erlaubten Context Slice. Nach der Äußerung interpretiert der owning Dialogue Use Case ausschließlich ein strukturiertes Outcome, nicht den ganzen Text. Ein `Reported` Candidate enthält Speaker, Listener, typed predicate und Dialogue Provenance. MOD-003 prüft, dass der Listener Subject ist, der Speaker in der Interaction Scope lag und der Type reported knowledge erlaubt. Ohne diese Prüfung entsteht kein durable Record.

### G.3 AI-derived Inference

Der AI Adapter validiert das Provider Envelope und liefert ein Structured Proposal. Application prüft Use Case und Permission, markiert untrusted input und baut den Candidate. MOD-003 prüft Type, Scope, Evidence und Policy erneut; Authority bleibt `Inferred`. Bei Konflikt mit Canonical Facts folgt Reject oder separate Inference mit Conflict Link gemäß Type Policy. Provider Success ist nie Persistence Success.

### G.4 Korrektur durch Canonical Owner

Der World Owner committet zuerst den korrigierten State und publiziert anschließend ein Correction Event mit Previous Fact Reference. MOD-003 findet den Target Record im selben Scope, validiert Authority und erzeugt `Correction`. Neue Revision, Supersession Relation und Projection Invalidation werden atomar geschrieben. Erst nach Commit folgt `MemoryCorrected.v1`; Audit Purpose sieht weiterhin den alten Record.

### G.5 Bounded Retrieval für Dialogue

Der Context Manager ruft `BuildContextSlice` mit Subject, Participants, Topic Refs, Count/Byte Budgets und Deadline auf. MOD-003 fixiert einen Snapshot, filtert Visibility/Security, lädt Candidates, rankt deterministisch und packt das Budget. Das Resultat nennt Completeness und Truncation. Der Context Manager kombiniert weitere Quellen; MOD-003 kennt den finalen Prompt nicht und verändert Records nicht durch Retrieval.

### G.6 Retention Maintenance

Der Maintenance Scheduler startet `PlanRetention` außerhalb kritischer Paths. Der Plan enthält Policy Version, Snapshot ID, Counts und bounded batches. Nur erlaubte Klassen dürfen automatisch bestätigt werden. `ApplyRetention` prüft Generation erneut, committet einen Batch und bewahrt Tombstones/Summary Links nach Policy. Nach Generation Change ist der alte Plan stale und wird nicht angewandt.

### G.7 Save Snapshot

Der Save Manager fordert `PrepareSnapshot` von allen Participants. Der Memory Adapter fixiert eine immutable Generation, erstellt Logical Snapshot Candidate und Checksum und ändert Live State nicht. Der Save Manager validiert World/Lineage und High-water Marks quer und schreibt danach das globale Artifact. Ein fehlgeschlagener globaler Commit verändert das Memory Repository nicht.

### G.8 Restore

Der Save Manager liest das Artifact, prüft das Envelope und gibt Participant Payload an `StageRestore`. MOD-003 deserialisiert non-live state und prüft Schema, References und Policy. Nach erfolgreicher Cross-validation aktiviert der Save Manager alle Candidates logisch atomar. Die neue Generation wird erst nach globalem Erfolg sichtbar. Jeder Fehler vorher lässt den alten State vollständig aktiv.

### G.9 Restart nach unbekanntem Commit Outcome

Nach einem Crash wiederholt der Caller die Operation mit gleichem Idempotency Key. MOD-003 sucht zuerst das Terminal Result. Existiert der Commit, wird derselbe Success ohne zweiten Event Effect geliefert. Fehlt der Key, läuft normale Validation. Derselbe Key mit anderem Hash erzeugt `MEM_IDEMPOTENCY_CONFLICT`; automatische Datenkorrektur ist verboten.

## Anhang H — Configuration Registry für MOD-003

Konkrete numerische Defaults werden erst nach Benchmark/Security Review beschlossen. Die Tabelle definiert Type, Reload und Failure Behavior und stellt offene Werte nicht als implementiert dar.

| Key | Type/Constraint | Reload | Failure |
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

Configuration Snapshot und Policy Version gehören zu Health Evidence. Änderungen mit Migration/Reindex laufen nie als gewöhnlicher Hot Reload.

## Anhang I — Matrix für Types, Authority und Retention

| Memory Type | Required Semantics | Allowed Authority | Default Retention Intent | Retrieval Purposes |
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

Zusatzregeln:

- Inferred ist für `QuestOutcome` und authoritative `WorldChange` ohne Bestätigung des Owners nicht zulässig.
- Rumor erfüllt keine Query mit Authority Floor `Witnessed`.
- Summary Authority übersteigt nicht das schwächste kritische Evidence, außer eine Policy beweist Canonical Aggregation.
- Commitment Expiry entsteht nicht allein durch Alter; Resolution oder Governance Policy ist nötig.
- Permanent wird durch Allowlist Policy und nicht durch Producer Request vergeben.

### I.1 Visibility Classes

| Klasse | Leseberechtigung | Beispiel |
|---|---|---|
| SubjectPrivate | Nur Use Case im Kontext des Subjects | Private Observation |
| ParticipantShared | Participants der dokumentierten Interaction | Agreement |
| OrganizationScoped | Autorisierte Organization Members/Systems | Internal knowledge |
| PublicWorld | Jeder eligible Actor derselben Welt | Public event |
| AuditRestricted | Nur Administrative Audit Capability | Correction trail |

Visibility wird nicht automatisch aus Type abgeleitet. Cross-class Leakage ist ein Security Failure.

## Anhang J — Requirements-to-Evidence-Matrix

| Requirement ID | Normative Regel | Abschnitt | Pflichtnachweis |
|---|---|---:|---|
| M3-OWN-001 | Memory Semantics gehören MOD-003 | 2–6 | Architecture dependency test/review |
| M3-OWN-002 | SQL überschreitet Domain Boundary nicht | 6, 30–31 | Static import rule, contract test |
| M3-DAT-001 | Durable Records sind versioned/scoped | 7, 10–12 | Schema tests |
| M3-DAT-002 | Runtime Handles werden nicht persistiert | 10 | Type/static tests |
| M3-PRV-001 | Jeder Record hat Provenance | 13 | Validation/migration tests |
| M3-AUT-001 | Canonical vor AI Inference | 14, 26 | Conflict corpus |
| M3-TIM-001 | Event Time und Record Time getrennt | 15 | Clock/chronology tests |
| M3-WRT-001 | Event erst nach Commit | 16, 29, 32 | Failure injection test |
| M3-VAL-001 | Validation deterministisch | 17 | Property tests |
| M3-DUP-001 | Retry erzeugt kein Duplicate | 18, 32 | Idempotency integration test |
| M3-COR-001 | Correction zerstört Audit nicht | 19 | Revision-chain test |
| M3-RET-001 | Retention besitzt Dry Run | 20 | Maintenance test |
| M3-SAL-001 | Salience versioniert und bounded | 21 | Policy/property tests |
| M3-QRY-001 | Query nach Count/Bytes bounded | 22–24 | Budget tests |
| M3-QRY-002 | Ranking deterministisch | 23 | Golden corpus/property test |
| M3-CTX-001 | Incomplete Context markiert | 24–26, 45 | Failure integration test |
| M3-AI-001 | AI schreibt keine durable memory | 26 | Dependency/static test |
| M3-REL-001 | Relationship bleibt State Owner | 27 | Contract review/test |
| M3-EVT-001 | Semantic Events idempotent | 29, 48 | Duplicate delivery test |
| M3-REP-001 | Repository Port use-case-oriented | 30 | API review |
| M3-TXN-001 | Candidate Commit atomar | 32 | Crash/rollback tests |
| M3-CON-001 | Queues und Retries bounded | 33, 44 | Stress test |
| M3-CAC-001 | Cache ist keine Source of Truth | 34 | Restore/invalidation tests |
| M3-SAV-001 | Snapshot schreibt kein global artifact | 35 | Save contract test |
| M3-RST-001 | Restore staged und atomar | 36 | Cross-participant failure test |
| M3-MIG-001 | Migration recoverable/checksummed | 37 | Migration suite |
| M3-CFG-001 | Modul sieht nur typed section | 38 | Configuration scope test |
| M3-SEC-001 | Logs enthalten keinen Memory Content | 39–41 | Log scanning test |
| M3-DEL-001 | Delete behandelt Derivatives | 40 | Privacy delete integration test |
| M3-OBS-001 | Health von Liveness getrennt | 41 | Startup/dependency test |
| M3-PER-001 | Budgets auf Target gemessen | 42, 56 | Benchmark report |
| M3-ERR-001 | Empty Result maskiert keinen Failure | 43, 49 | Fault injection test |
| M3-OPS-001 | Recovery nutzt AI/Logs nicht als Truth | 44, 59 | Runbook exercise |

Ein Requirement ist erst mit Evidence und Link im Gate Report geschlossen. Der Dokumentabschnitt schließt Design, nicht Verified.

## Anhang K — Recovery Playbooks

### K.1 Repository Temporarily Busy

Typed Error Rate und Duration ohne Content erfassen. Long Transaction oder Maintenance Overlap prüfen. Nur bounded Persistence Retry zulassen. Danach Background Writes stoppen, Interactive Backpressure beibehalten und Degraded melden. Timeout nicht unbegrenzt erhöhen und Integrity nicht deaktivieren.

### K.2 Repository Unavailable

Write Path auf Reject; Read Path nur auf policy-approved cache mit `Incomplete/Stale`. Path/Configuration, Filesystem und Persistence Health prüfen. Nach Recovery Schema/Integrity validieren, erst dann Ready. Nicht-durable Pending Candidates gelten nicht als gespeichert.

### K.3 Migration Failed

RepositoryReady nicht veröffentlichen. Original Database und Migration Report bewahren, Checksum/Preconditions prüfen. Dokumentierten Rollback/Restore oder korrigierte Forward Migration wählen. Migration Manifest darf nicht rückwirkend verändert werden, um Mismatch zu verbergen.

### K.4 Corruption Suspected

Writes sofort stoppen, recoverable copy, Hashes, File Metadata und Last-good Backup Reference sichern. Approved Scan auf der Kopie ausführen. Recovery per Backup, controlled export/import verified records oder dokumentierter Repair. Vor Ready müssen Integrity Suite und Representative Queries bestehen.

### K.5 Event Publication Failure nach Commit

Eine erfolgreiche Memory Transaction nicht mit unkoordinierter Gegenoperation rückgängig machen. Outbox/Recovery Evidence gemäß M3.0 Strategy halten und Delivery idempotent wiederholen. Subscriber akzeptieren Duplicate Event IDs. Ohne Durable Outbox wird die Operational Limitation vor Release dokumentiert.

### K.6 Save Restore Rejected

Live Generation behalten, staged candidate schließen und Participant Code an Save Manager liefern. Keine Teilaktivierung und kein Update des Repository High-water Marks. Diagnostic Export enthält Schema/Checksum/Counts, nicht Sensitive Content.

### K.7 Retention Job Interrupted

Nur bei gleicher Generation/Policy Version per Operation ID und Checkpoint fortsetzen. Committed Batches wiederholen keine Side Effects. Bei Mismatch neuen Dry-run Plan erzeugen. Nach Restore niemals einen stale destructive plan fortführen.

## Anhang L — Design Review und Acceptance Ledger

### L.1 Vor M3.0 zu schließende Entscheidungen

| Decision | Review Owner | Closure Criterion |
|---|---|---|
| Physical Schema und Index Plan | Memory + Persistence | Domain Contracts erhalten; Query Plan gemessen |
| Outbox/Event Recovery Strategy | Memory + Event + Persistence | Crash Points und Duplicate Delivery geprüft |
| Default Retention Durations | Product/Memory/Security | Gameplay, Privacy, Storage Budget abgestimmt |
| Salience Coefficients | Memory + Consumers | Golden Corpus Baseline angenommen |
| FTS Decision | Architecture/Memory | Nutzen gemessen oder bewusst deferred |
| Administrative Export Format | Operations/Security | Bounded, versioned, Safe Round Trip |
| Benchmark Target Profile | Project/QA | Windows Hardware und Corpus dokumentiert |

### L.2 Gate Review Record

Jeder Gate Report enthält Commit, Schema Versions, Tests, Environment, Failures, Waivers, Performance Values und Reviewer Decision. Ein Waiver nennt Owner, Reason, Expiry und Requirement IDs. High/Critical Blocker dürfen für Operational nicht waived werden.

### L.3 Definition of Done für Artefakte

- Public Contract Package und API Reference.
- Domain/Schema Catalog mit Version History.
- Repository Migration Manifest und Recovery Guide.
- Event/Error Catalog mit Owner und Compatibility Rule.
- Golden Corpus und Retrieval Quality Report.
- SaveParticipant Compatibility Fixtures.
- Security/Privacy Test Report und Threat-model Update.
- Load/Soak/Benchmark Report.
- Operations Runbook und Recovery Exercise Evidence.
- Synchronisierte RU/DE-Spezifikationen und Canonical Index Update.

### L.4 Aktueller Baseline Record

| Feld | Wert |
|---|---|
| Document version | 1.0 |
| Documentation status | Complete design baseline |
| Implementation status | Not started |
| Authorized work | Documentation and pre-implementation specification |
| Implementation branch | Not created by this document |
| Current documentation branch | docs/mod-003-memory-engine-specification |
| Main branch changed | No |
| Next human decision | Review/approve M3.0 and unresolved decision table |
