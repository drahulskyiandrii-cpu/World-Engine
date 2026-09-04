# MOD-002 Game Adapter — Detaillierter Teil

Fortsetzung der vollständigen Implementierungsspezifikation für Skyrim SE und AE

Version 2.0  |  Status In Development — Prototype  |  Datum 2026-09-05

## 31 Leseführung für die vollständige Spezifikation

Die Abschnitte 1–30 bilden die normative Architekturgrundlage. Die Abschnitte 31–60 überführen diese Grundlage in eine ausführbare Spezifikation für Implementierung, Prüfung, Installation und Betrieb. Die Anhänge enthalten die tatsächlichen Quelldateien des gegenwärtigen Prototyps, sodass das Dokument ohne paralleles Öffnen des Repositorys nutzbar bleibt.

Die Schlüsselwörter MUST, MUST NOT, SHOULD, SHOULD NOT und MAY legen die Verbindlichkeit fest. MUST und MUST NOT werden durch Tests, statische Regeln oder die Release-Checkliste geprüft. Eine Abweichung von SHOULD braucht eine dokumentierte technische Begründung. MAY beschreibt eine Option, die die Kompatibilität der übrigen Verträge nicht ändert.

Das Dokument unterscheidet drei Aussagetypen. „Implemented“ bedeutet, dass Code im MOD-002-Zweig vorhanden ist. „Designed“ bezeichnet einen normativen Vertrag ohne zwingend abgeschlossene Implementierung. „Verified“ verlangt reproduzierbare Testnachweise. Der Modulstatus ist Prototype: Portable Lifecycle und Vertragstypen sind implementiert, die SKSE Bridge ist nur als Einstiegspunkt vorhanden; Game Hooks, Transport und Kompatibilitätsmatrix sind noch nicht bestätigt.

| Kennzeichnung | Bedeutung | Zulässiger Nachweis |
|---|---|---|
| Implemented | Code ist vorhanden und statisch prüfbar | Quelldatei, Diff, lokaler Build |
| Designed | Entscheidung ist festgelegt, Code kann fehlen | Diese Spezifikation und ADR-009 |
| Verified | Verhalten wurde reproduzierbar geprüft | Automatischer Test oder freigegebener manueller Testdatensatz |
| Supported | Alle Pflicht-Gates eines Runtime Targets sind erfüllt | Compatibility Report und Release Manifest |
| Deferred | Fähigkeit ist bewusst später eingeplant | Roadmap-Eintrag ohne versteckten Fallback |

## 32 Systemkontext und Vertrauensgrenzen

World Engine Host und Skyrim laufen in getrennten Prozessen. Im Skyrim-Prozess läuft nur ein minimaler SKSE-Plugin-Teil. Er liest über das SDK zugängliche Fakten, registriert erlaubte Ereignisse und führt eine begrenzte Menge von Spieloperationen aus. Der Portable Adapter läuft auf der World-Engine-Seite und enthält keine Skyrim-, SKSE- oder CommonLibSSE-Typen. Der lokale Transport verbindet beide Teile, ändert aber kein Ownership: Game Adapter bleibt alleiniger Eigentümer der Integration; der Host besitzt Komposition und Anwendungs-Readiness.

Jeder Protokollframe überschreitet eine Vertrauensgrenze. Selbst wenn beide Prozesse unter demselben Benutzer laufen, gilt eine Nachricht erst nach Prüfung von Länge, Kodierung, Schema, Version, Session, Generation und Nachrichtentyp als gültig. Plugin-Daten, Form-Namen, Untertiteltext und Fehlerstrings gelten als nicht vertrauenswürdig. Ein Capability Token ist ein von der Anwendung ausgegebener Autorisierungskontext, kein dauerhaftes Geheimnis und kein Ersatz für die Prüfung des Capability State.

| Zone | Vertrauen | Befugnis | Verbotene Abhängigkeiten |
|---|---|---|---|
| Application Coordination | Vertrauenswürdige Orchestrierung | Erstellt Action Request und Authorization Context | Keine SKSE-Typen |
| Portable Adapter | Vertrauenswürdiger Integrationscode | Validiert, normalisiert, publiziert Evidence | Kein direkter Game-Memory- oder Load-Order-Zugriff |
| Local Transport | Nicht vertrauenswürdige Grenze | Nur versionierte Frames | Kein Netzwerk, keine Shell, keine beliebigen Dateien |
| Skyrim Bridge | Privilegierter In-Process-Code | Minimale Hooks und Actions | Keine Domain-Logik, AI oder Datenbank |
| Mod/Papyrus Content | Nicht vertrauenswürdiger Input | Nur explizit beschriebene Events | Keine beliebigen World-Engine-Kommandos |

AI Provider, Datenbank, Save Manager, Plugin Detection, Event Engine und Domain-Module liegen außerhalb dieser Grenze. Benötigte Interaktion erfolgt über application-owned Ports und niemals durch direkten Import.

## 33 Interne Komponenten und Rollen

### 33.1 Portable Game Adapter

`GameAdapter` verwaltet Zustände, Generation, aktives Profil, Required Capabilities sowie die Prüfung von Observations und Actions. Er implementiert `IGameOutputCapabilityPort` und konsumiert `IGameIngressPort`, `IGameAdapterStatusSink` und `IRuntimeBridge`. Er erzeugt diese Abhängigkeiten nicht und besitzt den Host-Lebenszyklus nicht.

### 33.2 Profile Validator

Der Profile Validator prüft JSON Schema, Semantic-Version-Felder, eindeutige Capability IDs, zulässige Distribution und Edition sowie die Konsistenz zwischen Runtime Target und Support Status. Ein schema-gültiges Profil ist noch nicht supported. `supported` darf erst die Release Pipeline nach erfolgreicher Compatibility Matrix setzen.

### 33.3 Capability Registry

Die Registry hält einen Evidence-Snapshot der aktiven Generation. Jede Zeile enthält Capability ID, State, Reason Code, Evidence Source und bei Bedarf begrenzten Diagnosekontext. Eine Änderung erzeugt einen neuen unveränderlichen Snapshot; Leser sehen nie einen halb aktualisierten Satz.

### 33.4 Ingress Processor

Der Processor übernimmt Observations nach Transportvalidierung, prüft Session und Sequence, normalisiert game-scoped Identitäten und ruft den application-owned Ingress Port. Die Reservierung der Sequence vor dem externen Aufruf verhindert eine implizite Wiederholung von Side Effects nach unklarem Ergebnis.

### 33.5 Action Executor

Der Executor prüft Schema, Identität, Deadline, Capability, Autorisierung und Idempotency Policy. Erst danach entsteht ein Transport Request. Skyrim-Bridge-Ergebnisse werden typisiert; Timeout wird nie zu Success umgedeutet.

### 33.6 Skyrim Bridge

Die Bridge kapselt CommonLibSSE/SKSE und besteht aus Bootstrap, Event Capture, Action Handlers und Transport Endpoint. Bootstrap initialisiert die SKSE Interfaces einmal. Event Capture arbeitet auf dem Game Thread minimal. Action Handlers enthalten nur Whitelist-Operationen. Der Endpoint lauscht nicht auf TCP/UDP und akzeptiert keine beliebigen Methodennamen.

## 34 Ownership, Lebensdauer und Aufrufverbote

Der Application Host erstellt die Dependencies und anschließend `GameAdapter`, startet ihn nach Configuration- und Plugin-Detection-Readiness und stoppt ihn vor der Zerstörung der Ports. `IRuntimeBridge` muss alle Adapteraufrufe überleben und wiederholtes Disconnect sicher behandeln. Ingress und Status Sink werden nicht unter dem Adapter-Mutex aufgerufen.

| Objekt | Erzeugt durch | Zerstört durch | Besitz an mutable State |
|---|---|---|---|
| GameAdapter | Application Host | Application Host | Lifecycle, Generation, Profile Snapshot, letzte Sequence |
| RuntimeBridge Client | Host Integration Factory | Host nach stop | Connection und Request Correlation |
| Skyrim Bridge Endpoint | SKSE Plugin Bootstrap | SKSE Shutdown/Process Exit | Hook Registrations und Transport Handle |
| GameProfile Snapshot | RuntimeBridge Discovery | Adapter bei neuer Generation | Nur unveränderlicher Snapshot |
| Capability Token | Application Authorization Service | Läuft gemäß Policy ab | Scope, Expiry, Action Binding |

Re-entrant `start`, `recover` oder `stop` aus einem Status Callback ist verboten. Der Adapter wartet nicht auf den Game Thread, während er seinen Mutex hält. Nach bestätigtem Shutdown darf die Bridge keinen neuen Observation Callback auslösen; bereits laufende Callbacks werden durch Generation/Session Check verworfen.

## 35 Algorithmus zur Runtime-Identifikation

Runtime Identity entsteht aus Daten des SKSE/CommonLibSSE-Interfaces und wird gegen das aktive Profil geprüft. Der Name der Executable oder das Vorhandensein einer DLL reicht nicht. Das minimale Tupel enthält `game_id`, Distribution, Edition, Runtime Version, SKSE Version, Adapter Version, Profile Version und Prozessarchitektur.

1. Bridge liest Runtime Version über das SDK Interface.
2. Distribution wird über unterstützte SDK-/Build-Channel-Information bestimmt; ein Pfadhinweis ist höchstens Diagnose.
3. Adapter sucht exakte Übereinstimmung von `distribution + edition + runtime_version`.
4. Angegebene SKSE Version und Build Compatibility werden geprüft.
5. `pending_game_test` liefert Prototype/Unavailable Evidence, nie Supported.
6. Unbekannte Runtime wird nicht auf eine „nahe“ Version gerundet.
7. Identity bleibt pro Generation konstant; neuer Prozess oder Load erzeugt eine neue Session.

| Lage | runtime.identity | Adapter-Ergebnis |
|---|---|---|
| Exakter validierter Target | Available | Discovery fortsetzen |
| Exakt, aber pending_game_test | Degraded mit Prototype Reason | Nur Testbetrieb |
| Version fehlt im Profil | Unsupported | Unavailable |
| SKSE Interface fehlt | Unavailable | Unavailable |
| Distribution nicht unterstützt | Unsupported | Failed oder Unavailable nach Host Policy |

## 36 Vollständige Game-Profile-Validierung

Die Prüfung besteht aus Syntax-, Schema-, Semantic- und Release-Layer. Syntax verlangt UTF-8-JSON. Schema verwirft unbekannte Felder, leere Runtime-Target-Listen und ungültige Enums. Semantic prüft Duplikate, Widersprüche zwischen Required und Optional Capabilities und den Ausschluss von VR. Release verhindert `supported`, solange nicht jeder deklarierte Target einen unabhängigen Testdatensatz besitzt.

Required und Optional dürfen sich nicht überschneiden. `known_limitations` ersetzt keinen Capability State. Profile Version ändert sich bei semantischer Änderung; Adapter Version bei ausführbarem Code. Ein neuer Runtime Target startet als `pending_game_test` und wird erst in einem geprüften Change Set zu `validated`.

Ein fehlerhafter Candidate löscht nicht den letzten bestätigten Snapshot. Der Host darf ihn nur im Degraded-Modus weiterverwenden, wenn Runtime Identity unverändert ist. Bei Identity-Mismatch ist Fallback verboten.

## 37 Detaillierte Startup State Machine

`start()` ist nur aus Created oder Stopped zulässig. Vor dem externen Connect erhöht der Adapter die Generation, setzt die Sequence zurück und publiziert Initializing. Connect, Profile Discovery und Capability Discovery laufen ohne internen Mutex. Jedes externe Ergebnis wird an seine Generation gebunden; veraltete Ergebnisse werden ignoriert.

| Schritt | Deadline | Erfolg | Fehler |
|---|---|---|---|
| Typed Config laden | Host-owned | Candidate Config | Startup Failed vor Adapter-Erzeugung |
| Transport verbinden | `connect_timeout_ms` | Connected Evidence | Unavailable, begrenzte Recovery |
| Handshake | Kurze Protocol Deadline | Kompatible Protocol Ranges | Incompatible |
| Profil entdecken | Operation Deadline | Validiertes Identity-Tupel | Unavailable/Failed |
| Plugin Evidence abbilden | Host Policy | Capability Inputs | Degraded ohne erneuten Scan |
| Hooks registrieren | Ergebnis je Hook | Capability Evidence | Required fehlt = Unavailable |
| Operational publizieren | Kein externer Lock | Ready oder Degraded | Host aggregiert Ergebnis |

Ready verlangt alle Required Capabilities in Available. Degraded erlaubt nur optionale Einschränkungen oder einen explizit erlaubten Prototype Mode. Unavailable heißt, dass eine Pflichtfunktion nicht möglich ist, aber nicht, dass ein Crash vorliegt. Failed steht für verletzte Invarianten, Bootstrap-Exception oder ausgeschöpfte Recovery Policy.

## 38 Shutdown, Recovery und Generationswechsel

Shutdown sperrt neue Actions, setzt Stopping, storniert Pending Requests, lässt die Bridge Hooks entfernen, führt einen begrenzten Drain aus und schließt den Transport. Wiederholtes `stop()` nach Stopped ist idempotent erfolgreich. Hängt Disconnect, beendet der Host das Warten per Deadline und publiziert Forced-Cleanup Evidence.

Recovery ist aus Degraded, Unavailable oder Failed nur durch Host-Entscheidung erlaubt: Quiesce, Disconnect, Session invalidieren, Generation erhöhen, Connect, Handshake, Profile Discovery, Capability Discovery, Publish. Der Retry Budget liegt beim Host; MOD-002 darf keinen endlosen Loop erzeugen.

Eine alte Generation publiziert weder Snapshot noch Action Result und liefert keine Observation. Die Correlation Map wird beim Wechsel geleert. Ein unbekanntes Ergebnis einer nicht-idempotenten Action wird als `TimedOut` oder `Failed` mit `effect_unknown=true` behandelt und nicht automatisch wiederholt.

## 39 Katalog der Inbound Observations

Der folgende MVP-Katalog ist normativ. Ein konkreter Hook darf sich ändern, wenn Application Contract, Semantik und Tests erhalten bleiben.

| Event Type | Pflichtfelder im Payload | Capability | Overflow Class |
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

`event_id` ist innerhalb einer Session eindeutig. `sequence` steigt monoton pro Observation Stream. `observed_at_unix_ms` bezeichnet Capture-Zeit, nicht Verarbeitung. Payload enthält weder Raw Memory Address noch unbegrenzten Stack Dump oder lokalisierte Texte als Identität.

Critical Events werden nie still gelöscht. Bei voller Critical Queue wird Degraded/Unavailable mit Loss-of-Continuity Evidence gesetzt. Interactive besitzt reservierte Kapazität. Coalescible ersetzt den älteren Pending-Wert desselben Keys. Background darf mit Metrik und Reason Code verworfen werden.

## 40 Normalisierung von Identitäten und Daten

Actor, Cell, Worldspace und Form gelten nur im Kontext eines Game Profile. Eine normalisierte Identität enthält `game_id`, `profile_fingerprint`, logical kind und stable game-local key. Eine Raw Form ID ohne Load-Order-Fingerprint ist als langfristige World-Engine-ID verboten, weil Load Order höhere Bytes ändert und Formen neu zuordnen kann.

Strings werden als UTF-8 normalisiert, auf Länge begrenzt und nie als Security Key verwendet. Koordinaten werden gerundet oder gebucktet, wenn der Vertrag keine hohe Genauigkeit braucht. Fehlende Werte sind explicit null/optional statt mehrdeutiger Leerstrings.

Normalization erzeugt keine Domain-Schlüsse. Die Bridge meldet etwa Dialogue Start und Beteiligte, aber nicht Stimmung, Gesprächsziel oder Erinnerung eines NPC. Diese Interpretation gehört Application/Domain-Komponenten.

## 41 Katalog der Outbound Actions

MVP erlaubt nur feste Action Types. Unknown Actions werden vor jedem Skyrim-API-Aufruf verworfen. Auf Wire-Ebene sind `action_id`, `correlation_id`, `capability`, `capability_token`, `deadline` und `payload` Pflicht.

| Action Type | Capability | Payload | Idempotency | Status |
|---|---|---|---|---|
| game.ui.subtitle.show | ui.subtitle.show | text, duration_ms, speaker optional | Nur mit Dedupe Key idempotent | Designed |
| game.ui.subtitle.clear | ui.subtitle.show | presentation_id | Idempotent | Designed |
| game.voice.playback.start | voice.playback.trigger | asset_id, actor, volume | Non-idempotent | Deferred |
| game.voice.playback.stop | voice.playback.trigger | playback_id | Idempotent | Deferred |
| game.integration.ping | runtime.identity | nonce | Idempotent | Geplante Diagnose |

Untertiteltext besitzt ein festes Längenlimit, filtert Control Characters und wird nicht als Format String, Papyrus oder Console Command interpretiert. Für Voice wird nur ein registriertes `asset_id`, kein Dateipfad übertragen. Der Handler prüft zulässigen Menu/Game State; sonst folgt Busy oder Unavailable.

## 42 Autorisierung, Deadline und Idempotenz

Authorization Context bindet Token an Capability, Action Class, Session, Generation, Expiry und optional Actor Scope. Die Bridge akzeptiert ein Token nicht blind: Portable Adapter prüft vor Versand; die Bridge prüft den signierten oder ausgehandelten Session Context gemäß endgültigem Security Design.

Deadlines verwenden monotone Zeit. Eine abgelaufene Action wird nicht eingereiht. Läuft die Deadline nach Versand ab, folgt TimedOut; die Correlation bleibt bis Late Result oder Controlled Cleanup. Ein Late Result dient der Diagnose und überschreibt kein veröffentlichtes Outcome ohne Reconciliation Contract.

Idempotente Actions verwenden Caller-Dedupe-Key. Nicht-idempotente Actions werden nicht automatisch wiederholt. Eine Wiederholung derselben Action ID liefert gespeichertes Result oder Duplicate, niemals einen zweiten Game-API-Aufruf.

## 43 Wire Protocol v1

Ein Frame besteht aus vier Byte Little-Endian-Länge und UTF-8-JSON-Body. Die Länge betrifft nur den Body, ist größer null und maximal `max_payload_bytes`. Der Decoder verarbeitet Partial Reads und mehrere Frames pro Read. Malformed Length schließt die Verbindung nach begrenzter Diagnose.

| Envelope-Feld | Typ | Pflicht | Regel |
|---|---|---|---|
| schema_version | integer | MUST | Exakt 1 im aktuellen Contract |
| message_id | UUID/string | MUST | Eindeutig je Session |
| message_type | enum | MUST | hello, observation, action, result, evidence, shutdown |
| session_id | string | MUST | Im Handshake ausgehandelt |
| generation | uint64 | MUST | Gleich aktiver Generation |
| correlation_id | string | Conditional | Pflicht für action/result |
| sent_at_unix_ms | integer | SHOULD | Diagnose, keine Security-Entscheidung |
| payload | object | MUST | Zusätzliche Schema je Nachrichtentyp |

Der Handshake tauscht Protocol Minimum/Maximum, Adapter Identity, Runtime Identity, Process ID und zufällige Nonces. Nicht überlappende Ranges enden mit `protocol.incompatible`. Unbekannte Envelope-Felder werden im Strict Mode verworfen. Upgrades sind additive Schemas mit Feature Negotiation; Umdeutung bestehender Felder ist verboten.

## 44 Named-Pipe-Transport

Die erste Implementierung nutzt Windows Named Pipe nur lokal. Ob Server in Skyrim Bridge oder in einem Host-owned Bootstrap liegt, wird vor M2.3 festgelegt. ACL erlaubt den aktuellen interaktiven Benutzer und erwarteten Process Context; Everyone/Anonymous ist verboten.

Der Transport besitzt begrenzte Read/Write Queues, Maximum Concurrent Requests, Connect Deadline, Idle Heartbeat und Shutdown Handshake. Er macht keinen Business Retry. Verbindungsabbruch beendet Pending Requests typisiert und publiziert Evidence. Normale Logs enthalten keinen Payload, sondern nur Nachrichtentyp, Bytezahl, Dauer, Result Code und redigierte IDs.

Gegen Confused Deputy werden Handshake Identity, Generation Nonce und erwarteter Executable Context geprüft. Der genaue Process-Schutz braucht Security Tests auf unterstützten Windows-Versionen; bis dahin bleibt die Grenze Prototype.

## 45 Concurrency- und Backpressure-Design

Das Spiel wartet nicht auf AI, Datenbank oder Orchestrierung. Hook Callback kopiert nur einen minimalen begrenzten Datensatz in eine vorallokierte/begrenzte Struktur und kehrt zurück. Serialization läuft auf Worker Thread. Skyrim-API-Aufrufe, die Main Thread brauchen, gehen über SKSE Task Interface und liefern asynchrones Result.

| Queue | Producer | Consumer | Overflow Policy |
|---|---|---|---|
| Critical Lifecycle | Game Hooks | Transport Worker | Nie still verlieren; Degraded und Resync |
| Interactive Dialogue | Game Hooks | Transport Worker | Reservierte Kapazität |
| Coalescible Context | Game Hooks | Normalizer | Pending-Wert pro Key ersetzen |
| Outbound Actions | Host | Bridge Worker/Main Task | Busy vor Enqueue bei Limit |
| Diagnostics | Alle | Telemetry Sink | Ältesten Eintrag verwerfen, Counter erhöhen |

Lock Order: Lifecycle Mutex, Connection State, Correlation Map, Queue-local Lock. Externe Callbacks laufen unter keinem dieser Locks. Shutdown setzt Cancellation, schließt Producer, drainiert begrenzt und entfernt anschließend Hooks/Handles. Portable Layer kann ThreadSanitizer nutzen; Windows/Skyrim benötigt zusätzlich einen Stress Harness.

## 46 SKSE/CommonLibSSE-Integrationsplan

Das aktuelle `plugin.cpp` ruft nur `SKSE::Init` auf; das ist ein ehrliches Scaffold, keine fertige Integration. M2.4 ergänzt Plugin Metadata, Logging Bootstrap, SKSE Messaging Subscription, Lifecycle Registration, Runtime Identity Reader, Task Scheduling und sicheren Teardown.

Hook-Priorität: öffentliches CommonLib/SKSE Event Interface, dann stabiler Messaging Callback, dann minimaler Trampoline Hook nur ohne öffentliche Alternative. Jeder Low-Level Hook braucht eigenen Compatibility Test und dokumentierte Signature Source. Relocation folgt der gewählten CommonLibSSE-NG-Version; Hard-coded Addresses sind verboten.

Dialogue Context ist erst bestätigt, wenn ein Prototyp stabile Start/End-Signale für alle Targets belegt. Gibt es keinen einheitlichen Hook, ist Capability Degraded oder Unsupported; periodischer Full Scan ist kein Ersatz. Ein minimaler Papyrus Bridge darf ein konkretes Event liefern, durchläuft aber dieselbe Validierungsgrenze.

## 47 Capability Discovery und Plugin Evidence

Eine Capability entsteht aus der Schnittmenge von implementiertem Handler, kompatiblem Runtime Target, erfolgreicher Hook/Action Registration und erlaubendem validiertem Plugin Profile. Ein negatives Kriterium ergibt Degraded, Unavailable oder Unsupported mit Reason Code.

| Ursache | State | Beispiel-Reason-Code |
|---|---|---|
| Handler fehlt | Unsupported | capability.not_implemented |
| Runtime bekannt, Hook temporär fehlgeschlagen | Unavailable | capability.hook_registration_failed |
| Optionaler Plugin-Konflikt | Degraded | capability.plugin_conflict |
| Runtime Target nicht unterstützt | Unsupported | runtime.target_unsupported |
| Evidence noch nicht da | Unknown | evidence.pending |
| Alle Gates erfüllt | Available | capability.available |

Plugin Detection liefert ein unveränderliches Profil mit Fingerprint. Game Adapter scannt das Dateisystem nicht erneut und akzeptiert einen ESP/ESM-Namen nicht als ausreichenden Beweis. Rescan erzeugt Candidate; Capability Snapshot wird erst nach Validierung atomar ersetzt. Verlust einer optionalen Fähigkeit degradiert; Änderung einer Required Capability macht Unavailable.

## 48 Vollständiger Result- und Fehlerkatalog

Ein Result besitzt stabilen Machine Code und begrenztes Detail. Policy und Tests verwenden den Code; Detail ist für Menschen und enthält weder Secrets noch vollständigen Nutzertext.

| Präfix | Eigentümer | Typische Codes |
|---|---|---|
| adapter.start | Lifecycle | invalid_state, exception, deadline |
| adapter.profile | Profile Validation | game_id_mismatch, schema_invalid, target_unsupported |
| adapter.ingress | Observation Path | not_operational, schema_unsupported, duplicate_or_out_of_order, payload_too_large |
| adapter.action | Outbound Path | authorization_context_missing, capability_unavailable, capability_unsupported, exception |
| transport | Named Pipe | connect_timeout, peer_closed, frame_too_large, malformed_json, protocol_incompatible |
| bridge | Skyrim Integration | skse_missing, hook_failed, main_thread_timeout, action_rejected |
| security | Boundary Checks | peer_not_allowed, token_invalid, message_type_denied |

InvalidInput ist Caller- oder Wire-Fehler. Rejected ist formal gültig, verletzt aber Policy/Sequence. Unsupported bleibt für Version/Target konstant. Unavailable kann nach Recovery wechseln. Busy erlaubt sicheren Retry nur mit Backoff und gleichem Idempotency Key. TimedOut sagt ohne weiteren Beweis nichts über einen möglichen Effect. Failed bezeichnet internen oder SDK-Fehler.

## 49 Threat Model und Security Controls

Schutzgüter sind Integrität des Skyrim-Prozesses, World-Engine-Domain-State, Authorization Context, Nutzerinhalt, lokale Pfade und Session-Stabilität. Angriffsquellen sind fehlerhafte/bösartige Mods, gefälschte lokale Clients, malformed Frames, Event Flooding, Path Injection über Assets und unbeschränkte Diagnosedimensionen.

| Bedrohung | Kontrolle | Prüfung |
|---|---|---|
| Pipe-Client-Spoofing | Per-user ACL, Peer/Process Validation, Nonce | Security Integration Test |
| Oversized/partial Frame | Length Gate vor Allocation, bounded Decoder | Fuzz/Property Tests |
| Beliebige Action | Whitelist Enums | Unknown-Action Negative Test |
| Replay | Session, Generation, Message ID, Sequence | Replay Test nach Reconnect |
| Path Traversal | Asset-ID-Registry, keine Pfade auf Wire | Malicious Payload Test |
| Game-thread DoS | Bounded Capture und Queues | Load/Stress Test |
| Secret Leakage | Structured Redaction, verbotene Labels | Log Inspection |
| Untrusted Papyrus | Schema-/Längen-/Typprüfung | Papyrus Boundary Test |

Das Modul ist keine Security Sandbox für Skyrim Mods. Es reduziert die Oberfläche und fügt kein Netzwerk hinzu. Remote Transport, Scripting oder beliebiger Dateizugriff brauchen ein neues ADR und Threat-Model-Review.

## 50 Configuration Contract

Configuration Provider liest Raw Config, erzeugt einen typisierten Candidate, prüft Constraints und liefert eine unveränderliche View. MOD-002 liest YAML/JSON nicht selbst. Änderungen an Security, Pipe Identity, Runtime Profile oder Queue Geometry brauchen Controlled Restart. Nicht blockierende Telemetry Switches MAY nach Candidate Validation hot-reloadable sein.

| Feld | Typ | Constraint | Reload |
|---|---|---|---|
| expected_game_id | string | `skyrim-se-ae` im ersten Release | Restart |
| max_payload_bytes | integer | 4096..1048576, Hard Ceiling 1 MiB | Restart |
| inbound_queue_capacity | integer | Positiv, gemessener Memory Budget | Restart |
| outbound_queue_capacity | integer | Positiv, Reject vor Enqueue | Restart |
| connect_timeout_ms | integer | 100..30000 | Restart |
| operation_timeout_ms | integer | Je Action Class, begrenzt | Candidate-dependent |
| recovery_attempt_limit | integer | 0..konfiguriertes Maximum | Hot nur via Host Policy |
| pipe_name | string | Fester Präfix, kein Nutzerpfad | Restart |
| diagnostics_level | enum | off, error, info, debug | Hot nach Validation |

Ein ungültiger Candidate ändert die aktive Config nicht. Der Snapshot trägt Version/Fingerprint in Status Evidence, aber keine Secret Values. Defaults sind dokumentiert und getestet; „0 bedeutet unendlich“ ist für Timeout und Queue Capacity verboten.

## 51 Observability und messbare Budgets

Alle Telemetriedimensionen sind begrenzt. Zulässige Labels: Adapter Version, Profile Version, Runtime Target aus fester Menge, Capability ID, Result Category und Reason-Code-Enum. Actor ID, Event ID, Dialogtext, Raw Path und Exception Message sind keine Labels.

| Signal | Typ | Zweck |
|---|---|---|
| mod002_state | Gauge/Event | Lifecycle State und Generation |
| mod002_capability_state | Gauge | State jeder bekannten Capability |
| mod002_ingress_total | Counter | accepted/rejected/dropped nach Reason |
| mod002_action_total | Counter | Result je Action Class |
| mod002_queue_depth | Gauge | Aktueller Wert und High-water Mark |
| mod002_operation_duration | Histogram | Connect, Discovery, Normalization, RTT |
| mod002_recovery_total | Counter | Attempts und Outcomes |
| mod002_frame_bytes | Histogram | Größen ohne Payload Content |

Numerische SLOs werden erst nach M2.3/M2.4 festgelegt. Der Release Report erfasst p50/p95/p99 für Capture, Queue Wait, Transport RTT, Memory Delta und CPU Delta. Ein nicht gemessener Wert heißt `measurement pending`, nie null. Vorläufiger Guardrail: kein Netzwerk-Warten oder Disk I/O auf einem Game Hook Thread.

## 52 Build System und Reproduzierbarkeit

Portable Target verwendet C++20, SKSE Target C++23. Warnungen gelten als Fehler. `WORLD_ENGINE_BUILD_SKSE_PLUGIN` ist standardmäßig aus, damit Portable Tests ohne Skyrim SDK laufen. Der Plugin Build braucht gepinnte CommonLibSSE-NG-Integration und dokumentierten Package Lock.

Pflichtartefakte: portable Static Library, Tests, SKSE DLL, Symbol/Archive Policy, validiertes Profil, License Notices, Release Manifest und Checksums. Das Manifest nennt Compiler, Windows SDK, CMake, CommonLibSSE NG, SKSE, Git Commit, Profile Fingerprint und Supported Runtime List.

Reproducibility Gate vergleicht Clean Builds in separaten Verzeichnissen. Generated Files ersetzen keine Quellen. Debug Builds werden nie als Release ausgeliefert. Code Signing ist wünschenswert; fehlt es, bleibt Hash Verification Pflicht und der Zustand wird nicht verschwiegen.

## 53 Packaging für MO2/Vortex und manuelle Installation

Das Release Package hat Standard-Skyrim-Data-Layout und enthält keine World-Engine-Secrets oder User Config. Die angenommene Struktur wird in M2.8 durch echten Build bestätigt.

| Paketpfad | Inhalt | Regel |
|---|---|---|
| SKSE/Plugins/WorldEngineGameAdapter.dll | In-process Bridge | Einzige Runtime DLL |
| SKSE/Plugins/WorldEngineGameAdapter.toml | Optionale lokale Config | Nur sichere Bootstrap-Werte |
| WorldEngine/profiles/skyrim-se-ae.profile.json | Versioniertes Profil | Schema-validiert |
| docs/ | README, Versions, Troubleshooting | Keine Development-only Files |

MO2 und Vortex sind Deployment Environments, nicht Quelle der Plugin-Wahrheit. Plugin Detection liest Load Order über den eigenen freigegebenen Mechanismus. Installer verändert keine Executable, erzeugt keinen Network Service und kopiert nichts in beliebige Systemverzeichnisse.

Upgrade: World Engine stoppen, Skyrim schließen, altes Package sichern, Dateien ersetzen, Hash prüfen, erster Start in Prototype/Validation Mode. Rollback stellt einen vollständigen konsistenten Satz wieder her; DLL v2 mit Profile v1 ist verboten.

## 54 Deployment-, Update- und Rollback-Runbook

Vor Deployment werden saubere Installation, exakte Runtime, passendes SKSE, keine kollidierende DLL und verfügbares Log Directory geprüft. Danach installiert der Mod Manager das Package, SKSE Loader startet Skyrim und World Engine verbindet erst nach Bridge Bootstrap.

Smoke Test: Plugin Load, Handshake, Runtime Identity, Required-Capability-Snapshot, ein Lifecycle-Event, eine Subtitle Action, Controlled Disconnect, Clean Shutdown. Jeder Fehler erhält Prototype/Unavailable und blockiert Support Declaration.

Ein Bethesda Update invalidiert automatisch den Compatibility Claim bis zur Neuprüfung. Der Nutzer erhält `runtime.target_unsupported` statt Crash. Rollback erfolgt bei Load Failure, Handshake-Bruch, Verlust einer Required Capability oder erhöhtem Crash/Error Rate. MOD-002 verändert keine Save Files direkt.

## 55 Konkreter Testkatalog

Jeder Testdatensatz enthält ID, Build Commit, Environment, Preconditions, Steps, Expected, Actual und Evidence Link. Der Katalog ist minimal und wird nur erweitert.

| ID | Bereich | Prüfung | Gate |
|---|---|---|---|
| T-001 | Lifecycle | Created -> Initializing -> Ready | Unit |
| T-002 | Lifecycle | Optional limitation -> Degraded | Unit |
| T-003 | Lifecycle | Missing required -> Unavailable | Unit |
| T-004 | Lifecycle | start aus Ready abgelehnt | Unit |
| T-005 | Lifecycle | stop nach Stopped idempotent | Unit |
| T-006 | Recovery | Recover erhöht Generation | Unit |
| T-007 | Recovery | Alter Callback nach Recover verworfen | Concurrency |
| T-008 | Recovery | Retry Budget erschöpft | Host Integration |
| T-009 | Profile | Falsche game_id abgelehnt | Unit |
| T-010 | Profile | Unknown Field vom Schema abgelehnt | Schema |
| T-011 | Profile | Duplicate Capability abgelehnt | Schema |
| T-012 | Profile | Required/optional overlap abgelehnt | Semantic |
| T-013 | Profile | Supported ohne validierte Targets verboten | Release |
| T-014 | Runtime | Unknown Runtime -> Unsupported | Integration |
| T-015 | Runtime | Missing SKSE -> Unavailable | In-game |
| T-016 | Ingress | Gültige Observation erreicht Ingress | Unit |
| T-017 | Ingress | Sequence 0 abgelehnt | Unit |
| T-018 | Ingress | Duplicate/out-of-order abgelehnt | Unit |
| T-019 | Ingress | Falsches Schema abgelehnt | Unit |
| T-020 | Ingress | Oversized Payload vor Parsing abgelehnt | Security |
| T-021 | Ingress | game_id mismatch abgelehnt | Unit |
| T-022 | Ingress | Critical Overflow erzeugt Evidence | Stress |
| T-023 | Ingress | Coalescing behält letzten Wert | Unit |
| T-024 | Action | Gültiger Untertitel genau einmal | Contract |
| T-025 | Action | Missing Token abgelehnt | Security |
| T-026 | Action | Unsupported Capability typisiert | Unit |
| T-027 | Action | Unknown Action erreicht Bridge nicht | Security |
| T-028 | Action | Abgelaufene Deadline nicht eingereiht | Unit |
| T-029 | Action | Wiederholte idempotente Action dedupliziert | Contract |
| T-030 | Action | Non-idempotent Timeout nicht wiederholt | Contract |
| T-031 | Transport | Partial Header/Body zusammengesetzt | Unit |
| T-032 | Transport | Mehrere Frames pro Read | Unit |
| T-033 | Transport | Frame über Hard Limit schließt Verbindung | Fuzz |
| T-034 | Transport | Malformed UTF-8/JSON abgelehnt | Fuzz |
| T-035 | Transport | Protocol Range Mismatch | Integration |
| T-036 | Transport | Peer Close beendet Pending | Integration |
| T-037 | Security | Unerlaubter Pipe Peer abgelehnt | Security |
| T-038 | Security | Replay alter Session abgelehnt | Security |
| T-039 | Security | Path Traversal nicht ausgeführt | Security |
| T-040 | Security | Logs ohne Token/Text/Path | Inspection |
| T-041 | Threading | Hook Callback wartet nicht auf Transport | Performance |
| T-042 | Threading | Shutdown Race ohne Use-after-free | Stress |
| T-043 | Threading | Kein Callback unter Adapter Lock | Unit |
| T-044 | SKSE | Plugin Bootstrap auf Steam AE | In-game |
| T-045 | SKSE | Plugin Bootstrap auf Steam SE 1.5.97 | In-game |
| T-046 | SKSE | Plugin Bootstrap auf GOG AE | In-game |
| T-047 | SKSE | Lifecycle Hooks registriert/entfernt | In-game |
| T-048 | SKSE | Dialogue Start/End Semantik | In-game |
| T-049 | SKSE | Subtitle Show/Clear | In-game |
| T-050 | SKSE | Main-Thread-Scheduling Deadline | In-game |
| T-051 | Plugin Evidence | Kein zweiter Filesystem Scan | Architecture |
| T-052 | Plugin Evidence | Atomic Profile Snapshot Replace | Integration |
| T-053 | Save Boundary | Keine Restore-Entscheidung in MOD-002 | Architecture |
| T-054 | Packaging | Saubere MO2 Installation/Deinstallation | Deployment |
| T-055 | Packaging | Saubere Vortex Installation/Deinstallation | Deployment |
| T-056 | Upgrade | Mixed-Version Package abgelehnt | Deployment |
| T-057 | Rollback | Voriges Package wiederhergestellt | Deployment |
| T-058 | Performance | Capture/Queue/RTT Baselines erfasst | Release |
| T-059 | Stability | Zweistündiger Dialogue/Location Stress | Release |
| T-060 | Shutdown | Keine Hooks/Handles nach Exit | Release |

## 56 Verfahren der Kompatibilitätsmatrix

Jeder Runtime Target wird separat mit Clean Profile und repräsentativem Minimal-Mod-Profil geprüft. Der Record enthält Executable Hash, Skyrim Runtime, SKSE, CommonLibSSE Build, Mod Manager, Windows Version, Package Hash und T-044 bis T-060.

`validated` ist erst erlaubt, wenn Plugin lädt, Required Capabilities Available sind, Smoke/Integration/Security Tests bestehen und kein ungeklärter Crash vorliegt. Fehler eines Targets löscht Nachweise eines anderen nicht; der Gesamtstatus bleibt jedoch auf explizit aufgeführte Targets begrenzt.

Skyrim VR ist kein Target des ersten Release. Das ist Scope Control und kein verstecktes Kompatibilitätsversprechen. Spätere VR-Arbeit braucht Architecture Delta, eigenen Build Target und Tests.

## 57 Betriebshandbuch und Diagnose

Diagnose beginnt mit Status Evidence, nicht mit Neuinstallation aller Mods. Erfasst werden Runtime Target, SKSE Version, Adapter/Profile Version, Lifecycle State und erster stabiler Reason Code.

| Symptom | Wahrscheinliche Ursache | Sichere Maßnahme |
|---|---|---|
| Plugin lädt nicht | SKSE/Runtime Mismatch oder Dependency fehlt | Exakte Versionen und Manifest vergleichen |
| Host verbindet nicht | Pipe fehlt, ACL oder Startup Order | Bridge Status und User Context prüfen |
| Adapter Unavailable | Required Capability fehlt | Evidence lesen, nicht force-enable |
| Untertitel fehlt | UI State, Handler fehlt oder Timeout | Action Result und Smoke Test prüfen |
| Doppelte Events | Fehler in Session/Sequence | Begrenzten Trace sichern, Generation neu starten |
| Queue Overflow | Event Storm oder Consumer Stall | Degraded setzen, Queue Metrics erfassen |
| Fehler nach Game Update | Runtime nicht validiert | Game/Plugin zurückrollen oder Target abwarten |

Issue-Logs werden redigiert: keine User Paths, Tokens oder Dialogtexte. Crash Dumps nur mit Zustimmung und eigener Privacy Policy. Validierung oder ACL abzuschalten ist keine zulässige Reparatur.

## 58 Implementation Backlog und Gates

| Gate | Deliverable | Exit Evidence | Zustand |
|---|---|---|---|
| M2.1 | Architecture, Contracts, Prototype Core | ADR-009, Source, Static QA | Im Branch erfüllt |
| M2.2 | Portable Build/Test | Clean CMake Build und Test Report | Wartet auf Windows C++ Toolchain |
| M2.3 | Named Pipe Protocol | Decoder, ACL, Transport Tests | Planned |
| M2.4 | SKSE Event Bridge | Runtime Identity und Lifecycle Hooks | Scaffold only |
| M2.5 | Subtitle Action | Handler, Authorization, In-game Test | Planned |
| M2.6 | Plugin Evidence Mapping | Immutable Profile Integration | Planned |
| M2.7 | Compatibility Matrix | Drei validierte Target Records | Planned |
| M2.8 | Release Package | MO2/Vortex Package, Rollback, Manifest | Planned |

Die Arbeit folgt vertikalen Slices: Identity/Handshake, Lifecycle Observation, Subtitle Action, danach Dialogue Context. Jeder Slice umfasst Contract, Implementation, negative Tests, Metriken und Dokumentation. Ein neuer Slice ändert den Support Status eines früheren nicht ohne Evidence.

## 59 Traceability der Implementierungsanforderungen

| Anforderung | Komponente | Prüfung | Architekturquelle |
|---|---|---|---|
| Einzige Skyrim Boundary | Game Adapter/Bridge | Dependency Scan | ARCH-002, ARCH-005 |
| SDK Types isoliert | Skyrim Bridge | Include Scan/Build | ADR-009 |
| Host besitzt Readiness | Status Sink | Lifecycle Tests | ARCH-008, ADR-008 |
| Plugin Detection besitzt Discovery | Evidence Mapper | T-051 | ARCH-010, ADR-006 |
| Save Manager besitzt Restore | Nur Evidence Contract | T-053 | ARCH-011, ADR-005 |
| Kein Event-Engine-RPC | Ingress/Coordination | Contract Review | ADR-002 |
| Explicit Unsupported | Capability Registry | T-026 | ADR-007 |
| Atomic Candidate Config | Configuration Provider Boundary | Config Tests | ARCH-009 |
| Bounded Queues/Deadlines | Transport/Processors | Stress/Timeout Tests | ARCH-004 |
| SE/AE ohne VR | Profile/Build Flags | Schema/Build Inspection | ADR-009 |

## 60 Glossar, Grenzen und Historie v2.0

| Begriff | Definition |
|---|---|
| Adapter | Portable Orchestrierung und Validierung in MOD-002 |
| Bridge | Skyrim-Prozess-Teil mit SKSE/CommonLibSSE |
| Capability | Explizit erkannte und geprüfte Integrationsfähigkeit |
| Evidence | Scoped Fakt, aus dem der Policy Owner entscheidet |
| Game Profile | Versionierte Runtime-, Capability- und Limitationsbeschreibung |
| Generation | Initialization/Recovery-Versuch, der alte Ergebnisse isoliert |
| Observation | Zu normalisierender Eingangsfakt aus Skyrim |
| Action | Autorisierte Bitte um Whitelist-Game-Operation |
| Supported Target | Exakte Runtime/SKSE/Package-Kombination mit erfüllten Gates |
| Prototype | Entwicklung ohne Support Claim |

Offene Grenzen: kein bestätigter Portable Build in der aktuellen Umgebung, Named Pipe nicht implementiert, Event Hooks fehlen, Subtitle Handler fehlt, numerische Performance Budgets sind ungemessen, Runtime Targets haben keine In-game-Validierung. Diese Punkte bleiben auf Cover, Status Matrix und Acceptance Gates sichtbar.

| Version | Datum | Änderung |
|---|---|---|
| 2.0 | 2026-09-05 | Basis um vollständige Runtime-, Protocol-, Security-, Testing-, Deployment- und Operations-Verträge sowie reale Source-Anhänge erweitert |
