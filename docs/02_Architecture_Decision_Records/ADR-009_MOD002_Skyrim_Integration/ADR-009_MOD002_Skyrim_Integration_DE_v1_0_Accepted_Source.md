# ADR-009 Implementierung von MOD-002 für Skyrim SE und AE

Version 1.0  |  Status Accepted  |  Datum 2026-09-04

## 1 Kontext

Das Architekturaudit ist abgeschlossen. Der Projekteigentümer hat die Entwicklung von MOD-002 für Skyrim Special Edition und Anniversary Edition ausdrücklich freigegeben. Vor der Implementierung ist eine technische Grenze erforderlich, die ADR-007 und ADR-008 einhält.

## 2 Entscheidung

Die Implementierung besteht aus:

1. einer portablen Game-Adapter-Bibliothek in C++20 im World Engine process;
2. einem dünnen SKSE64-Plugin in C++23 mit CommonLibSSE NG im Skyrim process;
3. einem SE/AE Flat Runtime Target ohne Skyrim VR;
4. einem lokalen Windows Named Pipe als Process Transport;
5. einem versionierten, längenpräfixierten UTF-8-JSON-Protokoll;
6. Papyrus nur als gezieltem Fallback für einen fehlenden stabilen Native Hook.

## 3 Ownership

Game Ingress Port und Game Output Capability Port gehören zur Application Contract Boundary. MOD-002 ruft die Ingress-Abstraktion auf und implementiert die Output-Abstraktion. Host besitzt Lifecycle Wiring. Plugin Detection besitzt Discovery Evidence. Save Manager besitzt Restore Compatibility.

## 4 Unterstützter Bereich

Ziel sind die aktuelle Steam AE, die ältere Steam SE 1.5.97 und GOG AE. Game Profile und Testbericht nennen die exakten Versionen. Skyrim VR und Plattformen ohne SKSE-Unterstützung sind ausgeschlossen.

## 5 Kompatibilitätsstatus

Jedes Runtime Target beginnt als Prototype und Pending Game Test. Eine neue Spielversion erbt keine Kompatibilität. Nach einem Update sind SKSE Build, CommonLibSSE NG, Plugin Build und In-Game Regression erneut zu prüfen.

## 6 Folgen

Vorteile: Skyrim SDK bleibt isoliert, portable Logik ist ohne Spiel testbar, SE und AE können ein Flat Target verwenden und die bidirektionale Laufzeitkommunikation erzeugt keinen Source Dependency Cycle.

Kosten: lokaler Transport und Wire Protocol müssen gepflegt werden, die Runtime-Testmatrix bleibt verpflichtend und Bethesda-Updates erzeugen neue Compatibility-Arbeit.

## 7 Verworfene Alternativen

| Alternative | Grund |
|---|---|
| Skyrim SDK im Core | Verletzt die game-agnostic boundary |
| Nur Papyrus | Zu wenig Kontrolle, Performance und Testbarkeit für den System Bridge |
| Network Listener | Unnötige Angriffsfläche für lokale Integration |
| Separate DLL je SE- und AE-Version | Vergrößert Packaging Matrix; Flat Target löst das Problem |
| VR sofort einschließen | Erweitert ABI und Testmatrix vor einer bestätigten SE/AE-Baseline |

## 8 Gates

Die Entscheidung erlaubt M2.1 und folgende Stufen, erklärt den Release aber nicht für fertig. Support setzt Build, Unit Tests, Transport Tests, In-Game Validation, Security Review und einen genauen Compatibility Report voraus.

## 9 Quellen

- ARCH-003, ARCH-005, ARCH-008
- ADR-006, ADR-007, ADR-008
- MOD-001 Core v2.0 full content
- https://skse.silverlock.org/
- https://github.com/ianpatt/skse64
- https://github.com/CharmedBaryon/CommonLibSSE-NG

## 10 Historie

| Version | Datum | Änderung |
|---|---|---|
| 1.0 | 2026-09-04 | Erste akzeptierte Implementierungsentscheidung für MOD-002 auf Skyrim SE und AE |
