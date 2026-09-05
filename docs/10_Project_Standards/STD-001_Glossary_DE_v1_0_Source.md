# STD-001 World Engine Glossary

Version 1.0 · Deutsch · 2026-09-05 · Current normative standard

## 1. Zweck

STD-001 legt das einheitliche Vokabular für Architektur, Dokumentation, Code, Tests und Betrieb der World Engine fest. Es verhindert, dass dasselbe Wort unterschiedliche Dinge bezeichnet oder unterschiedliche Wörter fälschlich als verschiedene Konzepte behandelt werden.

Der kanonische technische Token wird auf Englisch geführt und bleibt in RU- und DE-Dokumenten stabil. Die Übersetzung erklärt die Bedeutung, ändert jedoch niemals den Namen eines Typs, Events, APIs, Feldes, einer Metrik oder eines Konfigurationsschlüssels.

## 2. Normativität und Vorrang

Die Definitionen aus STD-001 sind für neue Dokumente und neue Implementierung verbindlich. Verwendet ein älteres Dokument einen Begriff anders, gilt die Bedeutung des spezifischeren angenommenen ARCH-, ADR- oder Modulvertrags; die Abweichung wird registriert und kontrolliert korrigiert. Das Glossar darf keine bereits angenommene Architekturentscheidung stillschweigend ändern.

| Ebene | Quelle | Regel bei Konflikt |
|---|---|---|
| 1 | Accepted ADR | Steuert die Entscheidung und ihre Begründung |
| 2 | Current ARCH | Steuert Systemgrenze und Abhängigkeiten |
| 3 | Current module specification | Steuert den Vertrag des konkreten Moduls |
| 4 | STD-001 | Steuert allgemeine Bedeutung und Übersetzung |
| 5 | Lokale Notiz oder Draft | Ändert die kanonische Bedeutung nicht |

## 3. Sprachmodell

RU- und DE-Versionen verweisen auf denselben englischen canonical token. Im Fließtext ist eine natürliche Übersetzung zulässig; bei der ersten wesentlichen Verwendung wird der Token genannt. In Code, Schemas, Logs, Events und Konfiguration gilt ausschließlich die angenommene englische Form.

| Element | RU-Dokument | DE-Dokument | Code und Daten |
|---|---|---|---|
| Konzept | Übersetzung plus canonical token bei erster Verwendung | Übersetzung plus canonical token bei erster Verwendung | Canonical English token |
| Identifier | Wird nicht übersetzt | Wird nicht übersetzt | Exact identifier |
| Status | Englischer Status kann russisch erklärt werden | Englischer Status kann deutsch erklärt werden | Exact status value |
| Zahlenlimit | Gleicher Wert und gleiche Einheit | Gleicher Wert und gleiche Einheit | Machine-readable unit suffix |

## 4. Dokument- und Implementierungsstatus

| Canonical token | Russische Bedeutung | Deutscher Begriff | Normative Definition |
|---|---|---|---|
| Planned | Запланировано | Geplant | Artefakt ist vorgesehen, aber keine kanonische Version vorhanden |
| Draft | Черновик | Entwurf | Inhalt existiert, jedoch sind nicht alle Gates bestanden |
| Design Baseline | Проектная база | Entwurfsbaseline | Verträge reichen nach Freigabe für den Implementierungsbeginn |
| Accepted | Принято | Angenommen | Entscheidung wurde vom zuständigen Owner formal angenommen |
| Complete | Документ завершён | Dokument vollständig | Dokument ist eigenständig und hat die verpflichtende QA bestanden |
| Not Started | Не начато | Nicht begonnen | Implementierung oder Verifikation fehlt |
| Prototype | Прототип | Prototyp | Untersuchungsimplementierung ohne behauptete Production Readiness |
| Implemented | Реализовано | Implementiert | Code existiert, baut erfolgreich und entspricht der benannten Baseline |
| Verified | Проверено | Verifiziert | Verhalten ist durch definierte Tests und gespeicherte Ergebnisse belegt |
| Operational | Эксплуатационно готово | Betriebsbereit | Deployment, Monitoring, Recovery und Betriebseignung sind nachgewiesen |
| Current | Действующее | Aktuell | Version steuert die gegenwärtige Arbeit |
| Superseded | Заменено | Ersetzt | Version bleibt historisch erhalten, steuert aber nicht mehr |
| Living | Ведущийся документ | Fortgeschrieben | Dokument wird nach definierter Cadence aktualisiert |

**Verbotene Verkürzung:** Wörter wie „fertig“, „erledigt“ oder „abgeschlossen“ ohne Objekt und Status. Korrekt ist: „Spezifikation Complete; Implementation Not Started“ oder „Implementation Verified in Skyrim SE 1.6.x“.

## 5. Dokumentation und Steuerung

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Canonical Document | Канонический документ | Kanonisches Dokument | Einzige wirksame Version, die ausdrücklich im Canonical Index steht |
| Canonical Index | Канонический индекс | Kanonischer Index | Register der implementation-facing Dokumente und ihrer aktuellen Versionen |
| Control Artifact | Контрольный артефакт | Kontrollartefakt | Plan, QA Report oder Register zur Evidence-Steuerung außerhalb des Delivery Denominator |
| Delivery Artifact | Поставляемый артефакт | Lieferartefakt | Eine gezählte Master-Plan-Position, üblicherweise ein zweisprachiges Dokumentpaar |
| Source Document | Исходный документ | Quelldokument | Editierbare inhaltliche Grundlage der veröffentlichten Version |
| Reference Document | Документ-образец | Referenzdokument | Unveränderte Quelle der visuellen oder strukturellen Gestaltung |
| Historical Snapshot | Исторический снимок | Historischer Snapshot | Gesicherter Stand einer ersetzten Version |
| Traceability | Прослеживаемость | Rückverfolgbarkeit | Verbindung von Anforderung, Quelle, Implementierung, Test und Ergebnis |
| Coverage Register | Реестр покрытия | Abdeckungsregister | Quellliste und Nachweis ihrer Aufnahme oder kontrollierten Ausnahme |
| Change Log | Журнал изменений | Änderungsjournal | Versionierte Aufzeichnung tatsächlicher inhaltlicher Änderungen |
| Gate | Контрольный допуск | Freigabestufe | Formale Entscheidungsschwelle, die die nächste Arbeitsklasse blockiert |
| Definition of Done | Критерий завершения | Fertigstellungsdefinition | Pflichtnachweise für den Abschluss eines bestimmten Ergebnisses |

## 6. System und Architektur

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| World Engine | World Engine | World Engine | Modulares System zur Simulation einer lebendigen Welt mit Spieleintegration über Adapter |
| System Boundary | Граница системы | Systemgrenze | Explizite Trennung zwischen World Engine und externer Umgebung |
| Architecture | Архитектура | Architektur | Struktur von Owners, Grenzen, Abhängigkeiten und Pflichtmerkmalen |
| Layer | Слой | Schicht | Gruppe von Komponenten mit gleicher Verantwortung und erlaubter Dependency Direction |
| Core Platform | Базовая платформа | Kernplattform | Infrastruktur für Lifecycle, Module Registry, Events, Scheduling und gemeinsame Contracts |
| Simulation Layer | Слой симуляции | Simulationsschicht | Game-unabhängige Domain Logic für Memory, Characters, Relationships und World State |
| Game Integration Layer | Слой интеграции с игрой | Spielintegrationsschicht | Grenze zwischen Game Runtime und internen World-Engine-Verträgen |
| AI Integration Layer | Слой AI-интеграции | AI-Integrationsschicht | Provider-neutrale Orchestrierung von AI Requests, Context und Fallbacks |
| Infrastructure | Инфраструктура | Infrastruktur | Technische Implementierungen für Storage, Filesystem, Transport, Process und Provider Access |
| Dependency Direction | Направление зависимости | Abhängigkeitsrichtung | Erlaubte statische Beziehung zwischen Layers und Contract Owners |
| Static Dependency | Статическая зависимость | Statische Abhängigkeit | Compile-time- oder Package-level-Verweis einer Komponente auf eine andere |
| Runtime Interaction | Runtime-взаимодействие | Laufzeitinteraktion | Nachrichten- oder Call-Austausch zur Laufzeit ohne Änderung des Ownership |
| Inversion of Control | Инверсия управления | Steuerungsumkehr | Abhängigkeit auf einen vom Consumer besessenen Contract mit externer Implementierung |

## 7. Module und Komponenten

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Module | Модуль | Modul | Versionierte Verantwortungseinheit mit Lifecycle, Contracts und Owner |
| Component | Компонент | Komponente | Interner Teil eines Moduls mit konkreter technischer Rolle |
| Module Owner | Владелец модуля | Modulverantwortlicher | Rolle für Spezifikation, Conformance und Evidence des Moduls |
| Module Registry | Реестр модулей | Modulregister | Core Service, der Descriptors annimmt und den zulässigen Lifecycle steuert |
| Module Descriptor | Дескриптор модуля | Moduldeskriptor | Metadaten zu ID, Version, Capabilities, Dependencies und Startup Requirements |
| Lifecycle | Жизненный цикл | Lebenszyklus | Definierte Folge aus Registration, Start, Betrieb, Stop und Failure |
| Capability | Возможность | Fähigkeit | Explizit deklarierte Funktion, die ein Consumer vor Nutzung prüfen kann |
| Contract | Контракт | Vertrag | Versionierte Zusage über Daten, Verhalten, Fehler und Compatibility |
| Port | Порт | Port | Schmales Interface an einer Use-Case-Grenze im Ownership des Verbrauchers |
| Adapter | Адаптер | Adapter | Implementierung, die externes Protocol oder Runtime in einen internen Contract überführt |
| Repository | Репозиторий данных | Repository | Schmales Persistence Interface für Domain- und Application-Use-Cases |
| Service | Сервис | Dienst | Verhaltenskomponente; Name benötigt konkrete Rolle und Boundary |
| Use Case | Сценарий применения | Anwendungsfall | Eine orchestrierte Zielhandlung von User, System oder Modul |

## 8. Events und Nachrichten

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Event | Событие | Ereignis | Unveränderbarer Fakt über etwas bereits Geschehenes |
| Domain Event | Доменное событие | Domänenereignis | Für game-unabhängige Domain Logic bedeutsamer Fakt |
| Integration Event | Интеграционное событие | Integrationsereignis | Versionierte Nachricht über eine Module Boundary |
| Command | Команда | Befehl | Aufforderung zu einer Handlung; kann abgelehnt werden |
| Query | Запрос чтения | Abfrage | Datenanfrage ohne Absicht, Domain State zu verändern |
| Result | Результат | Ergebnis | Typisierter Ausgang von Command, Query oder Operation |
| Event Envelope | Конверт события | Ereignisumschlag | Metadaten zu Identity, Version, Time, Correlation, Causation und Producer |
| Producer | Производитель | Erzeuger | Modul, das den Fakt bestätigt und veröffentlicht |
| Consumer | Потребитель | Verbraucher | Komponente, die einen Contract abonniert und Nachrichten verarbeitet |
| Correlation ID | Идентификатор корреляции | Korrelationskennung | ID einer logischen Operation über mehrere Komponenten |
| Causation ID | Идентификатор причины | Ursachenkennung | ID der unmittelbaren Nachricht oder Handlung, die die aktuelle ausgelöst hat |
| Idempotency Key | Ключ идемпотентности | Idempotenzschlüssel | Stabiler Schlüssel zum Erkennen einer wiederholten Operation |
| At-least-once Delivery | Доставка минимум один раз | Mindestens-einmal-Zustellung | Nachricht kann mehrfach eintreffen und benötigt idempotenten Consumer |
| Dead Letter | Необработанное сообщение | Unzustellbare Nachricht | Nach erlaubten Versuchen isolierte unverarbeitete Nachricht |

## 9. Daten und Persistence

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Domain State | Доменное состояние | Domänenzustand | State mit Spielbedeutung im Ownership eines Domain Module |
| Durable State | Долговременное состояние | Dauerhafter Zustand | Daten, die einen Process Restart überleben müssen |
| Transient State | Временное состояние | Flüchtiger Zustand | Rekonstruierbare oder einmalige Runtime-Daten |
| Entity | Сущность | Entität | Objekt mit stabiler Identity über die Zeit |
| Value Object | Объект-значение | Wertobjekt | Unveränderlicher Wert, der durch Inhalt statt Identity bestimmt ist |
| Aggregate | Агрегат | Aggregat | Consistency Boundary mit genau einem steuernden Root |
| Schema | Схема | Schema | Versionierte Datenstruktur und ihre Invariants |
| Logical Schema | Логическая схема | Logisches Schema | Datenmodell ohne Bindung an physische Layout eines konkreten Engines |
| Physical Schema | Физическая схема | Physisches Schema | Tabellen, Columns, Indexes und Constraints eines Storage Engines |
| Migration | Миграция | Migration | Kontrollierte Transformation zwischen Schema Versions |
| Transaction | Транзакция | Transaktion | Atomic Unit von Persistence Changes mit definierter Isolation Boundary |
| Repository Contract | Контракт репозитория | Repository-Vertrag | Domain-facing Persistence Operations ohne SQL- oder Connection-Leakage |
| Outbox | Исходящий журнал | Outbox | Transaktional gespeicherte Records für zuverlässige spätere Event Publication |
| Snapshot | Снимок состояния | Zustandsabbild | Versionierte Darstellung von State zu einem bestimmten Zeitpunkt |
| Backup | Резервная копия | Sicherung | Unabhängige wiederherstellbare Kopie von Durable Data |

## 10. Save und Restore

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Save Manager | Менеджер сохранения | Speicherstandsmanager | MOD-013 Coordinator des gemeinsamen Save/Restore Protocol |
| Save Participant | Участник сохранения | Speicherteilnehmer | Modulvertrag für Vorbereitung, Prüfung und Aktivierung des eigenen Snapshot |
| Checkpoint | Контрольная точка | Prüfpunkt | Koordinierter Satz kompatibler Participant Snapshots |
| Save Lineage | Линия сохранения | Speicherstandslinie | Stabile Herkunftslinie eines gespeicherten World State |
| Stage | Подготовительная стадия | Vorbereitungsstufe | Aufbau eines Non-live Candidate ohne Änderung des aktiven State |
| Validate | Проверка кандидата | Kandidatenprüfung | Deterministische Prüfung von Struktur und Cross-participant Invariants |
| Commit | Фиксация | Festschreibung | Innerhalb der Operation endgültiger Übergang einer vorbereiteten Änderung |
| Atomic Restore | Атомарное восстановление | Atomare Wiederherstellung | Gleichzeitige logische Aktivierung aller geprüften Participant States |
| Rollback | Откат операции | Rücknahme | Rückkehr zum letzten nachgewiesenen State nach fehlgeschlagenem Versuch |
| Recovery | Восстановление после сбоя | Wiederanlauf | Verfahren zur Rückkehr in korrekten Zustand nach Failure |
| Compatibility Fixture | Фикстура совместимости | Kompatibilitätsfixture | Gespeichertes älteres Testbeispiel für Load- und Migration-Prüfung |

## 11. Konfiguration und Deployment

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Configuration | Конфигурация | Konfiguration | Versionierte Eingabewerte zur Änderung erlaubten Verhaltens |
| Configuration Schema | Схема конфигурации | Konfigurationsschema | Types, Defaults, Ranges, Dependencies und Validation Rules von Settings |
| Profile | Профиль | Profil | Benannter Konfigurationssatz für Environment oder Mode |
| Safe Default | Безопасное значение по умолчанию | Sicherer Standardwert | Wert zur Minimierung von Data Damage und unerwarteter Scope Expansion |
| Secret | Секрет | Geheimnis | Credential oder Key, das nicht in Docs, Logs oder normaler Config stehen darf |
| Deployment | Развёртывание | Bereitstellung | Installation eines verifizierten Build samt Config in die Zielumgebung |
| Environment | Среда | Umgebung | Definierte Kombination aus OS, Game Runtime, Dependencies, Config und Data |
| Build Artifact | Артефакт сборки | Build-Artefakt | Versioniertes Ergebnis der Build Pipeline |
| Feature Flag | Флаг функции | Funktionsschalter | Kontrollierter Behavior Switch mit Owner, Default und Removal Plan |
| Health Check | Проверка работоспособности | Zustandsprüfung | Begrenzte Readiness- oder Liveness-Diagnose ohne Domain-State-Änderung |
| Degraded Mode | Ограниченный режим | Eingeschränkter Modus | Expliziter Modus mit reduzierter Funktion und erhaltenen Safety Guarantees |

## 12. Spiel und Adapter

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Game Adapter | Игровой адаптер | Spieladapter | MOD-002 Boundary zwischen Skyrim Runtime und World-Engine-Verträgen |
| Game Runtime | Исполняемая среда игры | Spiel-Laufzeit | Laufendes Spiel samt APIs, Hooks und Process Constraints |
| Skyrim SE | Skyrim Special Edition | Skyrim Special Edition | Unterstützte Skyrim-Linie im aktuellen Scope |
| Skyrim AE | Skyrim Anniversary Edition | Skyrim Anniversary Edition | Unterstützte Distribution- und Runtime-Linie im aktuellen Scope |
| Skyrim VR | Skyrim VR | Skyrim VR | Separate Plattform, bis zu neuem ADR und Tests ausdrücklich ausgeschlossen |
| SKSE | Skyrim Script Extender | Skyrim Script Extender | Externe Runtime Extension, kein internes World-Engine-API |
| Plugin | Игровой плагин | Spiel-Plugin | ESP, ESM, ESL oder anderes vom Spiel erkanntes Content Artifact |
| Native Plugin | Нативный плагин | Natives Plugin | Runtime Binary über einen erlaubten Skyrim Extension Mechanism |
| Load Order | Порядок загрузки | Ladereihenfolge | Reihenfolge der Game Plugins mit Einfluss auf resolved records |
| Form ID | Идентификатор формы | Form-ID | Skyrim-spezifische Identity mit Normalisierung und Load-order Context |
| Game Event | Игровое событие | Spielereignis | Raw Runtime Signal, noch nicht zwingend ein validiertes Domain Event |
| Normalized Event | Нормализованное событие | Normalisiertes Ereignis | Geprüfte game-unabhängige Nachricht nach Adapter Mapping |
| Capability Detection | Определение возможностей | Fähigkeitserkennung | Runtime-Prüfung verfügbarer Hooks, Versions und Optional Features |

## 13. Memory und Simulation

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Memory Engine | Движок памяти | Memory Engine | MOD-003 und Owner von Bedeutung, Prüfung, Speicherung und Retrieval von NPC Memory |
| Memory Record | Запись памяти | Memory-Datensatz | Durable Validated Fact mit Identity, Provenance, Time und Retention Metadata |
| Memory Candidate | Кандидат памяти | Memory-Kandidat | Ungeprüfter Vorschlag, der keine Durable Truth ist |
| Subject | Субъект памяти | Memory-Subjekt | Entity, deren Memory oder Perspektive dargestellt wird |
| Provenance | Происхождение | Herkunft | Prüfbare Information über Source, Producer und Entstehungspfad |
| Authority | Авторитет источника | Quellenautorität | Vertrauensklasse für Zulässigkeit und Konfliktauflösung |
| Confidence | Уверенность | Konfidenz | Begrenzte Unsicherheitsbewertung, kein Ersatz für Authority |
| Salience | Значимость | Bedeutsamkeit | Deterministisch berechnete Nützlichkeit eines Records für einen Purpose |
| Retention | Удержание | Aufbewahrung | Policy für Frist, Archive, Deletion oder Erhalt eines Records |
| Visibility | Видимость | Sichtbarkeit | Regel, wer Memory Content für welchen Purpose abrufen darf |
| Conflict Group | Группа конфликта | Konfliktgruppe | Satz inkompatibler Claims ohne falsche Zusammenführung |
| Supersession | Замещение записи | Ablösung | Explizite Verbindung eines neuen Records zum ersetzten, ohne History Rewrite |
| Retrieval | Извлечение памяти | Abruf | Begrenzter Query Path zu relevanten und erlaubten Records |
| Context Projection | Проекция контекста | Kontextprojektion | Bounded Darstellung von Memory Data für einen konkreten Consumer |

## 14. AI, Dialogue und Voice

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| AI Provider | AI-провайдер | AI-Anbieter | Externe oder lokale Model-Inference-Implementierung hinter einem Port |
| Local AI | Локальный AI | Lokale AI | Model Runtime auf einem vom User kontrollierten Gerät |
| LLM | Большая языковая модель | Großes Sprachmodell | Probabilistic Text Model und keine Source of Truth |
| Prompt | Инструкция модели | Modellanweisung | Versioniertes Input Template mit Owner und Injection Boundary |
| Context | Контекст модели | Modellkontext | Bounded Satz geprüfter Daten für genau eine Inference |
| Context Manager | Менеджер контекста | Kontextmanager | Komponente für Budget, Ordering, Provenance und Truncation |
| AI Candidate | AI-кандидат | AI-Kandidat | Ungeprüftes AI Result mit Pflicht zur deterministic validation |
| Deterministic Fallback | Детерминированный fallback | Deterministische Rückfalllogik | Vorhersagbares Non-AI Result bei Failure oder Timeout |
| Dialogue Engine | Диалоговый движок | Dialog-Engine | MOD-005 und Owner der Dialogue Orchestration, nicht der Memory Persistence |
| Voice Engine | Голосовой движок | Sprach-Engine | MOD-014 und Owner von TTS/STT Integration und Audio Lifecycle |
| Hallucination | Недостоверная генерация | Halluzination | Output ohne ausreichende Stütze durch erlaubte Daten; kein direkter Truth Input |
| Prompt Injection | Инъекция инструкции | Prompt-Injektion | Versuch untrusted Content, Model Policy oder Authority zu verändern |

## 15. Tests und Nachweise

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Test Case | Тестовый случай | Testfall | Identifizierter Input, Preconditions, Action und Expected Result |
| Unit Test | Модульный тест | Unit-Test | Isolierte Prüfung einer kleinen Verhaltenseinheit |
| Integration Test | Интеграционный тест | Integrationstest | Prüfung des Contract zwischen zwei oder mehr realen Boundaries |
| Acceptance Test | Приёмочный тест | Abnahmetest | Prüfung eines User- oder Systemergebnisses gegen eine Requirement |
| Regression Test | Регрессионный тест | Regressionstest | Schutz zuvor nachgewiesenen Verhaltens vor unbeabsichtigter Änderung |
| Fixture | Фикстура | Fixture | Versionierter reproduzierbarer Input oder Environment State |
| Test Harness | Тестовый стенд | Test-Harness | Werkzeuge zum Starten, Beobachten und Steuern eines Testszenarios |
| Golden Corpus | Эталонный корпус | Referenzkorpus | Stabiler Beispielsatz mit Expected Judgments für Retrieval oder AI Evaluation |
| Coverage | Покрытие | Abdeckung | Gemessene Beziehung von Tests zu Code Paths oder Requirements; kein Qualitätsersatz |
| Evidence | Доказательство | Nachweis | Gespeichertes Ergebnis zur unabhängigen Bestätigung eines Claim |
| Pass | Пройдено | Bestanden | Observed Result entspricht dem vorher festgelegten Expectation |
| Waiver | Временное исключение | Ausnahmegenehmigung | Begrenzte Annahme einer offenen Bedingung mit Owner, Reason und Expiry |
| Blocker | Блокирующая проблема | Blocker | Bedingung, die den Gate-Übergang bis zur Auflösung verbietet |

## 16. Sicherheit und Privacy

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Trust Boundary | Граница доверия | Vertrauensgrenze | Übergang von Data oder Control zwischen unterschiedlichen Trust Levels |
| Untrusted Input | Недоверенный ввод | Nicht vertrauenswürdige Eingabe | Alle externen Daten vor Validation und Authorization |
| Validation | Валидация | Validierung | Prüfung von Shape, Range, Invariants und Zulässigkeit vor Nutzung |
| Authorization | Разрешение действия | Autorisierung | Prüfung des Rechts auf eine Operation in einem bestimmten Scope |
| Authentication | Подтверждение субъекта | Authentifizierung | Prüfung einer behaupteten Identity, sofern relevant |
| Least Privilege | Минимальные полномочия | Minimale Rechte | Nur notwendige Capabilities für die notwendige Dauer |
| Sensitive Data | Чувствительные данные | Schutzbedürftige Daten | Data mit begrenzter Speicherung, Anzeige oder Übertragung |
| Credential | Учётный секрет | Zugangsdaten | Token, Password oder Key für eine geschützte Ressource |
| Redaction | Сокрытие данных | Schwärzung | Kontrolliertes Entfernen von Sensitive Content aus View oder Evidence |
| Audit Trail | Аудиторский след | Prüfpfad | Unveränderbare Folge bedeutsamer Actions und Decisions |
| Data Retention Policy | Политика хранения | Datenaufbewahrungsrichtlinie | Regeln für Frist, Deletion, Archive und rechtliche oder project constraints |

## 17. Fehler, Observability und Betrieb

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Error | Ошибка результата | Fehler | Erwarteter typisierter negativer Ausgang einer Operation |
| Fault | Дефект выполнения | Störung | Falscher interner State oder verletztes Invariant |
| Failure | Наблюдаемый сбой | Ausfall | Beobachtete Nichterfüllung des zugesagten Verhaltens |
| Exception | Исключение runtime | Ausnahme | Mechanismus für abnormal control flow, nicht die Domain-Error-Taxonomie |
| Error Code | Код ошибки | Fehlercode | Stabiler machine-readable Identifier einer Failure Class |
| Retryable | Допускает повтор | Wiederholbar | Operation darf unter definierten Bedingungen sicher wiederholt werden |
| Timeout | Превышение времени | Zeitüberschreitung | Operation endet nicht im Budget; Outcome kann Reconciliation benötigen |
| Log | Журнал выполнения | Laufzeitprotokoll | Strukturierter Record eines diagnostischen Fakts |
| Metric | Метрика | Metrik | Numerisches Time-series Signal mit Unit und Semantics |
| Trace | Трасса | Ablaufspur | Verbundener Pfad einer Operation durch Components |
| Observability | Наблюдаемость | Beobachtbarkeit | Erklärbarkeit des internen State aus Logs, Metrics und Traces |
| Runbook | Эксплуатационная инструкция | Betriebsanleitung | Geprüfte Folge für Diagnose, Recovery und Escalation |
| Incident | Инцидент | Vorfall | Failure oder Risk mit gesteuerter Response und Lessons Learned |

## 18. Versionen, Compatibility und Release

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Version | Версия | Version | Identifier eines Contract-, Document- oder Artifact-State |
| Semantic Versioning | Семантическое версионирование | Semantische Versionierung | MAJOR.MINOR.PATCH mit erklärten Compatibility Semantics |
| Contract Version | Версия контракта | Vertragsversion | Version von Public Schema oder Behavior unabhängig vom Package Build |
| Schema Version | Версия схемы | Schemaversion | Version einer serialisierten oder persistierten Struktur |
| Backward Compatible | Обратно совместимо | Rückwärtskompatibel | Neues System akzeptiert weiterhin erlaubte ältere Inputs |
| Forward Compatible | Прямо совместимо | Vorwärtskompatibel | Altes System verarbeitet oder verwirft erweiterte neue Inputs korrekt |
| Breaking Change | Несовместимое изменение | Inkompatible Änderung | Änderung verletzt einen zuvor zugesagten Contract |
| Deprecation | Объявление устаревания | Abkündigung | Kontrollierte Auslaufphase eines Contract vor Removal |
| Release | Релиз | Veröffentlichung | Identifizierter verifizierter Artifact-Satz für definierten Scope |
| Release Candidate | Кандидат релиза | Veröffentlichungskandidat | Immutable Build in finalen Gates |
| Rollback Plan | План отката | Rücknahmeplan | Geprüftes Verfahren zur Rückkehr zum vorherigen Release |

## 19. Git und Arbeitsablauf

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Repository | Git-репозиторий | Git-Repository | Versionierte Projektgeschichte; nicht mit Data Repository Contract verwechseln |
| Branch | Ветка | Zweig | Benannter Verweis auf eine Commit-Folge |
| Worktree | Рабочее дерево | Arbeitsbaum | Separater Checkout eines Branch im gemeinsamen Repository |
| Commit | Коммит | Commit | Atomic Version-control Record mit einem erklärbaren Intent |
| Merge | Слияние | Zusammenführung | Integration verifizierter Branch History in einen anderen Branch |
| Rebase | Перенос истории | Umbasierung | Umschreiben der Commit Base; für publizierte Shared History nur nach Regel |
| Pull Request | Запрос интеграции | Integrationsanfrage | Review Boundary für Changes, Evidence und Merge Decision |
| Main Branch | Основная ветка | Hauptzweig | Geschützte Integrationsgeschichte und kein Ort unfertiger Arbeit |
| Audit Branch | Аудиторская ветка | Auditzweig | Isolierter Branch für Audit und Korrektur von Documentation oder Architecture |
| Feature Branch | Ветка изменения | Änderungszweig | Begrenzter Branch eines Implementation- oder Documentation-Pakets |
| Dirty Worktree | Рабочее дерево с изменениями | Geänderter Arbeitsbaum | Checkout mit uncommitted tracked oder untracked files |

## 20. Risiken und Entscheidungen

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Risk | Риск | Risiko | Unsicheres Ereignis mit möglichem Einfluss auf Ziel, Zeit, Qualität oder Safety |
| Issue | Текущая проблема | Aktuelles Problem | Bereits eingetretener Zustand mit Handlungsbedarf |
| Assumption | Допущение | Annahme | Bis zur Prüfung als wahr behandelte Bedingung |
| Constraint | Ограничение | Randbedingung | Bedingung, die eine Lösung einhalten muss |
| Probability | Вероятность | Wahrscheinlichkeit | Bewertung der Eintrittsmöglichkeit eines Risk |
| Impact | Влияние | Auswirkung | Bewertung der Folgen eines Risk |
| Mitigation | Снижение риска | Risikominderung | Maßnahme zur Senkung von Probability oder Impact |
| Contingency | План на случай риска | Notfallmaßnahme | Handlung nach Eintreten eines Trigger |
| Residual Risk | Остаточный риск | Restrisiko | Nach Controls verbleibendes Risk |
| Decision Record | Запись решения | Entscheidungsprotokoll | Gespeicherter Context, Options, Decision und Consequences |
| Open Decision | Открытое решение | Offene Entscheidung | Frage mit Owner und Deadline, die die Baseline noch nicht ändert |

## 21. Zeit, Reihenfolge und Concurrency

| Canonical token | Russische Bedeutung | Deutscher Begriff | Definition |
|---|---|---|---|
| Instant UTC | Момент UTC | UTC-Zeitpunkt | Absoluter Machine Timestamp in UTC |
| Domain Time | Доменное время | Domänenzeit | Game- oder Simulation-Time-Skala mit eigenen Regeln |
| Monotonic Time | Монотонное время | Monotone Zeit | Duration-Quelle, die bei Wall-clock-Änderungen nicht rückwärts läuft |
| Tick | Такт | Takt | Diskreter Scheduler Step mit deklarierter Größe und Semantics |
| Sequence Number | Порядковый номер | Sequenznummer | Monotone Nummer innerhalb eines ausdrücklich benannten Scope |
| Deterministic Order | Детерминированный порядок | Deterministische Reihenfolge | Gleiche Reihenfolge bei gleichen Inputs und State |
| Concurrency | Конкурентное выполнение | Nebenläufigkeit | Überlappende Operations mit expliziter Consistency Policy |
| Race Condition | Состояние гонки | Race Condition | Outcome hängt von unkontrolliertem Interleaving ab |
| Cancellation | Отмена | Abbruch | Kooperatives Beenden einer Operation mit definiertem Outcome |
| Deadline | Предельный момент | Fristzeitpunkt | Absoluter Endzeitpunkt einer Operation |
| Duration | Длительность | Dauer | Intervall mit verpflichtender Unit |

## 22. Abkürzungen

| Token | Vollständige Form | Bedeutung im Projekt |
|---|---|---|
| ADR | Architecture Decision Record | Architekturentscheidungsprotokoll |
| API | Application Programming Interface | Versionierte Programmschnittstelle |
| AI | Artificial Intelligence | Allgemeine Klasse der AI Integrations |
| LLM | Large Language Model | Sprachmodell probabilistischer Generation |
| NPC | Non-Player Character | Nicht vom Spieler gesteuerter Game Character |
| SE | Special Edition | Unterstützte Skyrim Distribution-Linie |
| AE | Anniversary Edition | Unterstützte Skyrim Distribution- und Runtime-Linie |
| VR | Virtual Reality | Aktuell ausgeschlossene Skyrim Platform |
| SKSE | Skyrim Script Extender | Externe Extension Runtime |
| ESP | Elder Scrolls Plugin | Game-Plugin-Format |
| ESM | Elder Scrolls Master | Master-Plugin-Format |
| ESL | Elder Scrolls Light | Light-Plugin-Format |
| TTS | Text to Speech | Sprachsynthese aus Text |
| STT | Speech to Text | Spracherkennung zu Text |
| FTS | Full Text Search | Volltextsuche mit separatem Entscheidungsbedarf |
| CI | Continuous Integration | Automatisierte Integrationsprüfung |
| QA | Quality Assurance | Qualitätsprozess und Nachweise |
| UTC | Coordinated Universal Time | Basiszeitzone absoluter Timestamps |

## 23. Verbotene Mehrdeutigkeiten

| Nicht ohne Präzisierung verwenden | Grund | Erforderliche Formulierung |
|---|---|---|
| Fertig | Dokument oder Code unklar | `Document Complete`, `Implementation Verified` oder exakter Status |
| Service | Rolle und Boundary unklar | `Application Service`, `Repository`, `Adapter` oder konkreter Name |
| Manager | Kann überbreite Verantwortung verdecken | Owned Lifecycle nennen, zum Beispiel `Save Manager` |
| Daten | Owner, Durability und Schema unklar | `Domain State`, `Durable State`, `Configuration` oder konkreter Record |
| Event | Kann Raw Signal, Command oder Fact bedeuten | `Game Event`, `Domain Event`, `Integration Event` oder `Command` |
| ID | Scope und Stabilität unklar | `WorldId`, `ModuleId`, `CorrelationId` oder anderer Typed Identifier |
| Version | Objekt unklar | `Document Version`, `Contract Version`, `Schema Version` oder `Build Version` |
| Save | Game Save und World Engine Checkpoint unklar | `Game Save`, `Checkpoint` oder `Save Participant Snapshot` |
| Memory | Process Memory und NPC Memory unklar | `Process Memory`, `Memory Record` oder `Memory Engine` |
| Fehler | Expected Error und Fault vermischt | `Error`, `Fault`, `Failure` oder konkreter Error Code |
| Unterstützt | Evidence-Level fehlt | Platform Version, Capability und Test Evidence nennen |
| Kompatibel | Richtung fehlt | `Backward Compatible` oder `Forward Compatible` plus Versions |

## 24. Übersetzungsregeln

1. Code-Identifier, Events, Fields, Metrics und Config Keys werden nicht übersetzt.
2. World Engine und Dokument-IDs bleiben unverändert.
3. Beim ersten spezialisierten Begriff steht die Übersetzung mit canonical token.
4. RU und DE erzeugen keine unabhängigen Synonyme für einen technischen Token.
5. Modale Wörter `MUST`, `SHOULD` und `MAY` behalten in allen Sprachen ihre Stärke.
6. Zahlen, Units, Versions, Enum Values und Error Codes müssen exakt übereinstimmen.
7. Verändert eine natürliche Übersetzung die Boundary, bleibt der englische Token mit Erklärung stehen.

## 25. Einführung eines neuen Begriffs

Ein neuer Public Term erhält zuerst proposed canonical token, Kurzdefinition, Owner, Scope, Abgrenzung und mindestens ein Beispiel. Ein Begriff mit Architecture Semantics benötigt Review durch den Architecture Owner; ändert er eine angenommene Entscheidung, ist ein ADR nötig. Nach Annahme werden STD-001, bei Bedarf STD-002, betroffene Contracts und Tests aktualisiert.

| Gate | Frage | Nachweis |
|---|---|---|
| T1 Need | Ist es ein neues Konzept statt eines Synonyms | Suche im Canonical Corpus und Konfliktkarte |
| T2 Meaning | Existiert genau eine prüfbare Definition | Definition, Scope, Owner und Non-example |
| T3 Naming | Entspricht der Token STD-002 | Naming Review |
| T4 Compatibility | Bricht die Änderung bestehende Contracts | Impact und Migration Plan |
| T5 Publication | Sind beide Sprachversionen aktualisiert | RU/DE Parity und Canonical Index |

## 26. Acceptance Criteria

- Jedes Public Document nutzt das kanonische Statusvokabular.
- Jeder neue Public Identifier verweist auf ein definiertes Konzept.
- RU und DE bewahren dieselben Tokens, Zahlenwerte und die gleiche modale Stärke.
- Eine Suche nach verbotenen Mehrdeutigkeiten findet keine unerklärten normativen Verwendungen.
- Module, Port, Adapter, Repository, Event, Command, Query und Result sind nicht austauschbar.
- AI Output wird vor Deterministic Validation nirgends Truth genannt.
- Complete eines Dokuments wird nicht als Implemented oder Verified einer Softwarekomponente behandelt.
- Skyrim VR gilt nirgends als Teil des aktuellen Supported Scope.

## Anhang A. Kurze Statusformel

Empfohlene Zustandsangabe: `Document Status; Implementation Status; Verification Scope; Evidence`. Beispiel: `Complete; Prototype; build and in-game validation pending; MOD-002 specification v2.0`.

## Anhang B. Begriffe mit mehreren Bedeutungen

| Token | Erlaubte Bedeutungen | Pflichtpräzisierung |
|---|---|---|
| Repository | Git Repository oder Repository Contract | Immer `Git` oder Domain Name nennen |
| Runtime | Game Runtime, AI Runtime oder Process Runtime | Immer Owner nennen |
| Plugin | Game Plugin oder Native Plugin | Artifact Kind nennen |
| Save | Game Save oder World Engine Checkpoint | Einzelwort in Contracts vermeiden |
| Context | Operation Context, Permission Context oder AI Context | Purpose und Boundary nennen |
| Model | Domain Model, Data Model oder AI Model | Modellklasse nennen |

## Anhang C. Traceability

| Quelle | Festgelegter Einfluss |
|---|---|
| PLAN-001 | Statuswerte, Delivery Denominator und Foundation Gates |
| ARCH-002/004/005 | Layers, Module Boundaries und Dependency Direction |
| ARCH-007 | Runtime Interactions und Sequence Terminology |
| ARCH-009 | Configuration Vocabulary |
| ARCH-011 | Save Participant und Atomic Restore Vocabulary |
| ARCH-012 | AI Boundary und Source-of-truth Rules |
| ADR-002/003/005/008/009 | Events, Persistence, Save, Contract Ownership und Skyrim Scope |
| MOD-001/002/003 | Core-, Game-Adapter- und Memory-Engine-Domain-Vokabular |

## Anhang D. Änderungsjournal

| Version | Datum | Änderung |
|---|---|---|
| 1.0 | 2026-09-05 | Vollständiges RU/DE/EN Glossary für Governance, Architecture, Modules, Events, Persistence, Save, AI, Testing, Operations, Git und Risk Terminology erstellt. |
