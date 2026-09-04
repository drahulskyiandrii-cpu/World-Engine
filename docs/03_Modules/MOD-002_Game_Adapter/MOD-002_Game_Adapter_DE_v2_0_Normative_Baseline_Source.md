# MOD-002 Game Adapter

Vollständige Implementierungsspezifikation für Skyrim SE und AE

Version 1.0  |  Status In Development  |  Datum 2026-09-04

Projektverantwortlicher: Andrii Drahulskyi

## 1 Entscheidung und Zweck

MOD-002 ist die einzige freigegebene Grenze zwischen World Engine und Skyrim. Die erste Implementierung unterstützt Skyrim Special Edition und Anniversary Edition unter Windows. Skyrim VR, Classic Skyrim, Windows Store, Game Pass und Epic Games Store gehören nicht zum ersten Release.

Das Modul übersetzt Beobachtungen aus dem Spiel in normalisierte Eingangskontrakte und führt ausschließlich freigegebene Spielaktionen über den Output Capability Port aus. Es besitzt weder den World-Engine-Weltzustand noch Dialoglogik, AI, Saves, Mod-Erkennung oder die globale Anwendungsbereitschaft.

Die Entwicklung beginnt mit dem Status Prototype. Keine Skyrim-Version erhält den Status Supported, bevor das SKSE-Plugin gebaut und die vollständige Laufzeitmatrix im Spiel geprüft wurde.

## 2 Architekturposition

MOD-002 gehört zum Integration Layer. Der Laufzeitfluss ist bidirektional, während der statische Abhängigkeitsgraph azyklisch bleibt.

| Beziehung | Richtung | Bedeutung |
|---|---|---|
| Eingang aus dem Spiel | Skyrim Runtime zu Skyrim Bridge zu Game Adapter zu Game Ingress Port | Runtime call und normalisierter Datenfluss |
| Ausgang in das Spiel | Application Coordination zu Game Output Capability Port zu Game Adapter zu Skyrim Bridge | Runtime call nach Capability- und Berechtigungsprüfung |
| Statische Abhängigkeit | Game Adapter zu application-owned port abstractions | Nur Abhängigkeit von Verträgen |
| Bereitschaft | Game Adapter zu Application Host | Evidence flow ohne Ownership-Übertragung |
| Mod-Erkennung | Plugin Detection zu Game Adapter | Versionierte Evidence, kein zweiter Dateisystemscan |

Der Application Host wählt das aktive Game Profile, erzeugt den Adapter und seine Abhängigkeiten, steuert Start und Stopp und bestimmt Ready, Degraded, Startup Failed oder Recovery Required auf Anwendungsebene. MOD-002 veröffentlicht nur eigenes Integration Evidence.

## 3 Verantwortungsbereich

MOD-002 verantwortet:

1. die Identifikation der Skyrim-Laufzeit über einen vertrauenswürdigen SDK-Mechanismus;
2. die Prüfung des aktiven Game Profile und des Runtime Target;
3. die Ermittlung und Veröffentlichung des tatsächlichen Capability-Satzes;
4. die Registrierung sicherer Game Event Hooks;
5. die Normalisierung von Spielereignissen zu Application Contracts;
6. die Prüfung von Schema, Größe, Reihenfolge und Session-Identität;
7. die Ausführung freigegebener Spielaktionen;
8. die Normalisierung von Skyrim- und SKSE-Fehlern;
9. die Veröffentlichung von Health-, Readiness- und Capability-Evidence;
10. kontrolliertes Quiesce, Reconnect, Revalidation und Shutdown.

## 4 Ausgeschlossene Verantwortung

MOD-002 darf nicht:

- Implementierungen von Dialogue Engine, Memory Engine, Character Engine, Save Manager, Repository, Database oder AI Adapter importieren oder aufrufen;
- Event Engine als RPC für Befehle mit unmittelbarem Ergebnis verwenden;
- SQLite lesen oder World-Engine-Domainzustand direkt verändern;
- Plugins, Load Order oder Mod-Verzeichnisse eigenständig scannen;
- Restore-Kompatibilität entscheiden;
- nicht vertrauenswürdigen Plugin- oder Papyrus-Inhalt ausführen;
- externem Inhalt Zugriff auf Prozesse, Netzwerk, Secrets oder beliebige Dateien geben;
- Capabilities allein aus Spielname oder Dateivorhandensein ableiten;
- Aktionen automatisch wiederholen, wenn ein zweiter Effekt möglich ist.

## 5 Technologieentscheidung

Die erste Implementierung besteht aus zwei Teilen.

| Teil | Umgebung | Technologie | Verantwortung |
|---|---|---|---|
| Portable Game Adapter | World Engine process | C++20 ohne Skyrim SDK types | Lifecycle, Profile, Capabilities, Validierung, Verträge und Fehlernormalisierung |
| Skyrim Bridge | Skyrim process | C++23, SKSE64, CommonLibSSE NG | Game Hooks, SDK-Übersetzung und minimale Ausführung von Game Actions |

Für SE und AE wird das flache Runtime Target von CommonLibSSE NG verwendet. VR-Buildflags bleiben deaktiviert. Papyrus ist für den ersten Vertical Slice nicht erforderlich. Es wird nur für einen konkreten Hook ergänzt, der mit SKSE oder CommonLibSSE nicht stabil abgedeckt werden kann, und enthält keine Domainlogik.

Die Prozessgrenze zwischen Skyrim Bridge und World Engine ist als lokaler Windows Named Pipe mit Zugriff für den aktuellen Benutzer geplant. Das Protokoll verwendet versionierte, längenpräfixierte UTF-8-JSON-Frames. Es gibt keinen Netzwerk-Listener.

## 6 Externe Abhängigkeiten

| Abhängigkeit | Erforderlichkeit | Regel |
|---|---|---|
| Application-owned Game Ingress Port | Required | Einziger normalisierter Eingang in den Coordination Layer |
| Application-owned Game Output Capability Port | Required | MOD-002 implementiert den Port, die Anwendung nutzt die Abstraktion |
| Application Host status sink | Required | Empfängt scoped evidence und bestimmt das Anwendungsergebnis |
| SKSE64 runtime | Required for Skyrim integration | Muss zur konkreten Skyrim-Laufzeit passen |
| CommonLibSSE NG flat target | Required for plugin build | SDK-Typen bleiben vollständig im Skyrim Bridge |
| Validated Plugin Profile evidence | Conditional | Quelle gehört Plugin Detection; MOD-002 bildet daraus Capabilities |
| Save compatibility contract | Not owned | MOD-002 liefert Evidence, Save Manager entscheidet |

## 7 Game Profile

Eine aktive Session besitzt genau ein Game Profile. Es enthält `game_id`, Runtime-Version, Distribution, Adapter-Identität, Adapter-Version, Profile-Version, Capability Evidence und bekannte Einschränkungen.

| Runtime target | Ziel des ersten Releases | Aktuelles Gate |
|---|---|---|
| Steam Anniversary Edition 1.7.104, SKSE 2.3.1 | Ja | Pending build and in-game validation |
| Steam legacy Special Edition 1.5.97, SKSE 2.0.20 | Ja | Pending build and in-game validation |
| GOG Anniversary Edition 1.6.1179, SKSE 2.2.6 | Ja | Pending build and in-game validation |
| Skyrim VR | Nein | Explicitly excluded |
| Windows Store Game Pass | Nein | SKSE unsupported |
| Epic Games Store | Nein | SKSE unsupported |

Die Versionsangaben entsprechen der offiziellen SKSE-Seite am 2026-09-04. Ein Bethesda-Update gilt als Compatibility Event. Eine neue Version übernimmt niemals automatisch den Status Supported.

## 8 Capability Model

Jede Capability besitzt den Zustand `Unknown`, `Available`, `Degraded`, `Unavailable` oder `Unsupported`. Eine fehlende Capability erzeugt weder einen leeren Erfolg noch einen stillen Fallback.

| Capability ID | Zweck | MVP | Besitzer des Fakts |
|---|---|---:|---|
| runtime.identity | Identität von Spiel, Version und Distribution | Required | Skyrim Bridge und validiertes Profile |
| session.lifecycle | New Game, Load Game, Revert und Shutdown | Required | Skyrim Bridge |
| dialogue.context.observe | Beginn und Ende eines Gesprächs | Required | Skyrim Bridge, Normalisierung durch MOD-002 |
| ui.subtitle.show | Freigegebenen Text über die Spiel-UI anzeigen | Required | Game Adapter action path |
| safe_point.evidence | Evidence für einen sicheren Zeitpunkt | Required | Skyrim Bridge evidence only |
| world.time.observe | Normalisierte Spielzeit | Optional | Skyrim Runtime |
| player.location.observe | Location- und Cell-Kontext des Spielers | Optional | Skyrim Runtime |
| actor.basic.observe | Freigegebene Basisdaten eines Actors | Optional | Skyrim Runtime |
| voice.playback.trigger | Vorbereitetes Voice Asset starten | Deferred | Game Adapter action path |

Jede Outbound Action benötigt ein Capability Token oder einen gleichwertigen validierten Authorization Context. Ein Token erzeugt keine Capability und umgeht das aktive Profile nicht.

## 9 Lifecycle

Die semantischen Zustände lauten `Created`, `Initializing`, `Ready`, `Degraded`, `Unavailable`, `Recovering`, `Stopping`, `Stopped` und `Failed`.

| Von | Nach | Bedingung |
|---|---|---|
| Created oder Stopped | Initializing | Host startet eine neue Generation |
| Initializing | Ready | Profile gültig und alle required capabilities Available |
| Initializing | Degraded | Required capabilities verfügbar, optionale Capability eingeschränkt |
| Initializing | Unavailable | Required capability fehlt oder Runtime inkompatibel |
| Degraded, Unavailable oder Failed | Recovering | Host erlaubt bounded recovery |
| Recovering | Initializing | Alter Bridge quiesced, neue Generation beginnt |
| Operational state | Stopping | Host startet Shutdown |
| Stopping | Stopped | Hooks entfernt und Ressourcen freigegeben |
| Beliebig | Failed | Fatale Inkompatibilität oder ausgeschöpfte Recovery Policy |

Jede Initialisierung erhält eine neue Generation. Ein Ergebnis einer alten Generation darf den aktuellen Zustand nicht verändern.

## 10 Startup Sequence

1. Der Host lädt validierte typisierte Konfiguration und wählt das Game Profile.
2. Der Host erzeugt Game Adapter und explizite Port-Abhängigkeiten.
3. Der Adapter wechselt zu Initializing und publiziert scoped evidence.
4. Skyrim Bridge prüft SKSE interface und Runtime-Identität.
5. Der Adapter vergleicht `game_id`, Distribution und Runtime Target mit dem Profile.
6. Bei Bedarf erhält der Adapter ein validiertes Plugin Profile über den freigegebenen Vertrag.
7. Der Bridge registriert Hooks und liefert Capability Evidence.
8. Der Adapter prüft required capabilities.
9. Der Adapter publiziert Ready, Degraded oder Unavailable.
10. Der Host bestimmt den Anwendungszustand.

Kein externer Callback wird ausgeführt, während MOD-002 einen internen Lock hält.

## 11 Inbound Pipeline

`Skyrim event -> capture -> bounded queue -> schema validation -> session and sequence validation -> normalization -> Game Ingress Port -> typed outcome`

Jede `RuntimeObservation` enthält Schema-Version, Event-Identität, monotonen Sequence-Wert innerhalb der aktiven Session, Zeitstempel, Game-Identität, Event-Typ und begrenzte Payload. Nullsequenzen, Duplikate, alte Sessions, unbekannte Schemas und zu große Payloads werden vor jedem Core- oder Domain-Effekt abgewiesen.

MOD-002 publiziert kein Domain Event, solange der Contract Owner MOD-002 nicht ausdrücklich als Producer festlegt. Standardmäßig ruft der Adapter den Game Ingress Port auf. Application Coordination entscheidet danach über synchronen Input oder ein semantisches Event.

## 12 Outbound Pipeline

`Application command -> schema validation -> capability lookup -> authorization context -> state guard -> runtime translation -> Skyrim execution -> normalized result`

Ein Befehl enthält Action ID, Correlation ID, Capability ID, Capability Token, Action Type und begrenzte Payload. Die Spiel-API wird erst nach erfolgreichen Gates aufgerufen. `Unsupported`, `Unavailable`, `InvalidInput`, `TimedOut`, `Busy` und `Failed` bleiben getrennte Ergebnisse.

Ein Retry ist nur erlaubt, wenn der Vertrag Idempotenz ausdrücklich definiert. Nach unbekanntem Ergebnis ist automatischer Retry standardmäßig verboten.

## 13 Safe Point Evidence

MOD-002 darf melden, dass Skyrim einen für bestimmte Operationen geeigneten Zustand besitzt, zum Beispiel keine Loading Screen, antwortender Prozess und stabile Session. Dies bleibt Evidence.

Scheduler besitzt die Autosave Policy. Save Manager besitzt Save- und Restore-Semantik. Host besitzt die Application Recovery Policy. MOD-002 startet keinen Autosave und entscheidet keinen Atomic Restore.

## 14 Plugin Evidence und Mods

Plugin Detection bleibt alleiniger Besitzer von Discovery, Parsing, Normalisierung und Fingerprint. MOD-002 liest weder Load Order noch Mod-Verzeichnisse selbst.

Game Adapter erhält ein unveränderliches validiertes Plugin Profile und bildet daraus Capability Evidence. Ein unbekanntes Plugin gewährt keine Capability. Ein fehlgeschlagener Rescan wird nicht als leeres gültiges Profile dargestellt.

## 15 Protokoll und Daten

Protocol Version, Adapter Version und Game Profile Version sind getrennt. Die erste Transportversion verwendet:

- lokalen Named Pipe pro Benutzer;
- 32-bit little-endian frame length;
- UTF-8 JSON body;
- maximal 1 MiB Payload als Ausgangswert;
- Pflichtfelder `schema_version`, `message_id`, `session_id`, `generation`, `message_type`;
- Correlation ID für Requests und Ergebnisse;
- monotonen Sequence-Wert für Inbound Observations;
- keine Raw Pointer, Memory Addresses oder Skyrim SDK Object Layouts.

Raw Form IDs sind keine globalen World-Engine-IDs. Bei Bedarf werden sie als game-scoped identity mit Profile Fingerprint und Load-Order-Kontext übertragen.

## 16 Threading und Backpressure

Skyrim Callbacks dürfen auf Game Threads eintreffen. Der Callback führt nur bounded capture aus und wartet weder auf World Engine, AI, Disk noch Netzwerk. Die Weiterverarbeitung nutzt eine begrenzte Queue.

| Regel | Anforderung |
|---|---|
| Game thread blocking | Kein externes Warten |
| Queue capacity | In Configuration begrenzt |
| Overflow | Typisierte Drop- oder Backpressure-Policy je Eventklasse |
| Locks | Nicht während Port-, Bridge- oder Status-Callback halten |
| Cancellation | An lange Operationen weitergeben |
| Shutdown | Eingang vor Drain oder bounded cancel schließen |

Critical lifecycle evidence nutzt eine strengere Overflow Policy als Background Observations.

## 17 Fehlermodell

| Kategorie | Beispiel | Ergebnis |
|---|---|---|
| Invalid input | Leere Event ID, falsches Schema, zu große Payload | Reject before downstream effect |
| Unsupported | Capability nicht implementiert | Typed Unsupported |
| Unavailable | Spiel oder required hook vorübergehend nicht verfügbar | Unavailable oder State Transition |
| Incompatible | Runtime-Version nicht unterstützt | Unavailable oder Failed, kein Support-Claim |
| Timeout | Bridge-Operation überschreitet Deadline | TimedOut und bounded recovery decision |
| Duplicate | Sequence bereits angenommen | Rejected ohne zweiten Effekt |
| SDK failure | Exception oder API-Fehler | Normalized Failed |
| Security rejection | Authorization Context fehlt | Rejected and audited |

Benutzertext, vollständige Pfade und Dialoginhalte werden nicht als Metric Labels verwendet.

## 18 Recovery

Recovery folgt Detect, Quiesce, Disconnect, Reconnect, Revalidate Profile, Rediscover Capabilities und Resume. Jeder Versuch ist durch Count und Deadline begrenzt. Ready oder Degraded folgt nur nach erneuter Validierung.

World-Engine-Domainzustand bleibt authoritative. Ein unbekanntes Ergebnis einer Outbound Action gilt nicht als Erfolg. Nach Load oder Revert erzeugt der Adapter eine neue Session Identity und verwirft alte Callbacks.

## 19 Configuration

MOD-002 erhält einen unveränderlichen typisierten Configuration View. Das Modul liest kein Raw YAML und sucht keine globale Konfiguration zur Laufzeit.

| Feld | Zweck | Default |
|---|---|---|
| expected_game_id | Erwartetes Profile | skyrim-se-ae |
| max_payload_bytes | Obergrenze der Payload | 1048576 |
| inbound_queue_capacity | Maximale wartende Observations | Nach Messungen festzulegen |
| connect_timeout_ms | Deadline der Bridge-Verbindung | Nach Prototype festzulegen |
| operation_timeout_ms | Standarddeadline je Actionklasse | Nach Messungen festzulegen |
| recovery_attempt_limit | Begrenzte Wiederholungszahl | Operational Policy |
| pipe_name | Logischer lokaler Transportname | world-engine.mod002.v1 |

Ein Wechsel des aktiven Profiles ist ein kontrollierter Restart und kein Hot Toggle.

## 20 Sicherheit

1. Named Pipe erlaubt nur den aktuellen Benutzer und erwarteten Process Context.
2. Network Exposure bleibt standardmäßig aus.
3. Message- und Stringgrößen werden vor dem Parsing begrenzt.
4. Capability Token wird vor jedem Game API Call geprüft.
5. Plugin Content und Papyrus Text gelten als untrusted.
6. MOD-002 erhält keine AI Provider Secrets.
7. Diagnostics entfernen Paths, Tokens und Benutzerinhalt.
8. Skyrim Bridge führt keine beliebigen Commands, Scripts oder Dateipfade aus Payload aus.
9. Authorization Errors werden nicht wiederholt.
10. Eine neue Prozess- oder Netzwerkgrenze benötigt ein Security Review.

## 21 Observability

Pflichtsignale sind Adapter State und Generation, Profile-Identität, Capability-Änderungen, Initialisierungsdauer, Ingress-Ergebnisse, Queue Overflow, Outbound Outcomes, Reconnect Attempts und Incompatible Runtime Counts.

Event IDs, Actor IDs, Dialogtext und freie Error Strings sind keine unbeschränkten Metric Labels.

## 22 Performance

Vor Messungen werden keine erfundenen Latenz-SLO festgelegt. Der Prototype erfasst Callback Capture Time, Queue Delay, Normalisierungsdauer, Bridge Round Trip, Initialisierung, Speicher und CPU.

Der Release Gate verlangt bounded Game-Thread-Arbeit, begrenzte Queues, wiederverwendete Capability Discovery und keine aktiven Ressourcen für einen inaktiven Adapter.

## 23 Save und Restore Boundary

MOD-002 darf Game Identity, Runtime Version, Profile Version, Capability Evidence und Safe Point Evidence liefern. Save Manager vergleicht den Save Baseline, erzeugt einen Restore Candidate, validiert alle Teilnehmer und aktiviert atomar.

Game Adapter ruft weder Commit, Abort noch Rollback der SaveParticipants auf. Game-spezifische Restore-Aktionen benötigen einen eigenen freigegebenen Vertrag.

## 24 Teststrategie

| Ebene | Mindestabdeckung |
|---|---|
| Portable unit tests | Lifecycle, Capabilities, Validierung, Deduplication und typed failures |
| Contract tests | Game Ingress und Game Output Capability Port |
| Profile schema tests | Pflichtfelder, Enums, Versionen und Ausschlüsse |
| Transport tests | Framing, Limits, Disconnect, Partial Frame, ungültiges UTF-8 und JSON |
| Concurrency tests | Callback Capture, Overflow, Cancellation, Shutdown Race und stale generation |
| SKSE bridge tests | Runtime Identity, Hook Registration, Event Translation und Actions |
| Integration tests | Skyrim zu Adapter zu Fake Application und zurück |
| Compatibility matrix | Steam AE aktuell, Steam SE 1.5.97 und GOG AE 1.6.1179 |
| Failure tests | Fehlendes SKSE, inkompatible Runtime, Connection Loss und erschöpfte Recovery |
| Security tests | ACL, Oversized Frame, Unknown Action, Missing Token und untrusted content |

## 25 Acceptance Criteria

Der Wechsel von Prototype zu Supported erfordert:

1. Portable Library baut mit Warnings as Errors.
2. Unit- und Contract Tests bestehen.
3. SKSE Plugin baut für SE/AE Flat Target ohne VR.
4. Profile Schema ist gültig.
5. Required MVP Capabilities sind in jeder behaupteten Runtime bestätigt.
6. Unsupported Capability liefert ein typisiertes Ergebnis.
7. Ungültiger Input erreicht weder Core noch Domain Effect.
8. Core und Domain Packages importieren keine Skyrim- oder SKSE-Typen.
9. Plugin Detection bleibt alleiniger Discovery Owner.
10. Eine neue Game-Version erbt keinen Support-Status.
11. Shutdown und Recovery lassen keine Hooks oder unbounded retries zurück.
12. Diagnostics enthalten keine Tokens oder Benutzerinhalte.
13. Dokumentation, Profile, Tests und Dependency Graph ändern sich gemeinsam.
14. Der Testbericht nennt genaue Versionen von Skyrim, SKSE, CommonLibSSE NG und Mod Manager.

## 26 Implementierungsplan

| Stufe | Ergebnis | Status am 2026-09-04 |
|---|---|---|
| M2.1 Architecture and contract baseline | ADR, vollständige Spezifikation, Port Types und Lifecycle Core | Implemented in branch |
| M2.2 Portable adapter verification | CMake Build und dependency-free unit tests | Code ready, Windows toolchain required |
| M2.3 Local transport | Named Pipe Framing, ACL und Host Bridge | Planned |
| M2.4 SKSE event bridge | Plugin Lifecycle und erforderliche Hooks | Entry point scaffolded |
| M2.5 Outbound MVP | Subtitle und freigegebene UI Action | Planned |
| M2.6 Profile and plugin evidence mapping | Capabilities aus validiertem Plugin Profile | Planned |
| M2.7 In-game compatibility validation | Steam AE, Steam SE legacy und GOG AE | Requires test installation |
| M2.8 Release gate | Packaging, Diagnostics, Installer und Support Declaration | Blocked until prior gates pass |

## 27 Offene Entscheidungen

- genaue CommonLibSSE-NG-Version und Package Baseline;
- endgültige Named-Pipe-ACL und Process Authentication;
- Wire Schema für Observations, Actions und Results;
- konkrete SKSE Hooks für Dialogue Context und Safe Point Evidence;
- Packaging für MO2 und Vortex;
- Retry Policy pro Actionklasse;
- numerische Performance Budgets nach Messungen;
- Bedarf für einen minimalen Papyrus Bridge.

## 28 Traceability

| MOD-002-Anforderung | Quelle |
|---|---|
| Einzige Spielgrenze | ARCH-001, ARCH-002, ARCH-005 |
| Ingress und Output über owned ports | ARCH-003, ARCH-005, ADR-007, ADR-008 |
| Host besitzt Lifecycle und Readiness | ARCH-002, ARCH-008, ADR-008 |
| Capability States und explizites Unsupported | ADR-007 |
| Ownership von Plugin Evidence | ARCH-010, ADR-006 |
| Ownership der Save Compatibility | ARCH-011, ADR-005 |
| Configuration Candidate Validation | ARCH-009 |
| Event Semantics und kein RPC | ADR-002 |
| SE/AE-Implementierungstechnologie | ADR-009 |

## 29 Externe technische Quellen

- Offizielle SKSE-Seite: https://skse.silverlock.org/
- SKSE Source Repository: https://github.com/ianpatt/skse64
- CommonLibSSE NG Repository: https://github.com/CharmedBaryon/CommonLibSSE-NG

## 30 Änderungshistorie

| Version | Datum | Änderung |
|---|---|---|
| 1.0 | 2026-09-04 | Autorisierung von MOD-002, Skyrim-SE/AE-Scope, Portgrenzen, Lifecycle, Capability Model, C++- und SKSE-Aufteilung, Prototype Code und Release Gates festgelegt |
