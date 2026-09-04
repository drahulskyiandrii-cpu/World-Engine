# ADR-009 Реализация MOD-002 для Skyrim SE и AE

Версия 1.0  |  Статус Accepted  |  Дата 2026-09-04

## 1 Контекст

Архитектурный аудит завершён, а владелец проекта явно разрешил разработку MOD-002 для Skyrim Special Edition и Anniversary Edition. Для начала реализации необходимо выбрать технологическую границу, не нарушающую ADR-007 и ADR-008.

## 2 Решение

Принята двухчастная реализация:

1. portable Game Adapter library на C++20 внутри World Engine process;
2. тонкий SKSE64 plugin на C++23 и CommonLibSSE NG внутри Skyrim process;
3. SE/AE flat runtime target без Skyrim VR;
4. локальный Windows named pipe как process transport;
5. versioned length-prefixed UTF-8 JSON protocol;
6. Papyrus только как точечный fallback для отсутствующего стабильного native hook.

## 3 Ownership

Game Ingress Port и Game Output Capability Port принадлежат application contract boundary. MOD-002 вызывает ingress abstraction и реализует output abstraction. Host владеет lifecycle wiring. Plugin Detection владеет discovery evidence. Save Manager владеет restore compatibility.

## 4 Поддерживаемая область

Целевые линии: актуальная Steam AE, legacy Steam SE 1.5.97 и GOG AE. Точные версии фиксируются Game Profile и тестовым отчётом. Skyrim VR и платформы без поддержки SKSE исключены.

## 5 Статус совместимости

Начальный статус каждой runtime target — Prototype и Pending Game Test. Новая версия игры не получает совместимость автоматически. После обновления требуется новый SKSE build, проверка CommonLibSSE NG, сборка plugin и in-game regression.

## 6 Последствия

Положительные последствия: Skyrim SDK изолирован; portable logic тестируется без игры; SE/AE может использовать один flat target; runtime bidirectionality не создаёт source dependency cycle.

Стоимость решения: появляется локальный transport и versioned wire protocol; требуется отдельная матрица runtime tests; compatibility maintenance продолжается после обновлений Bethesda.

## 7 Отклонённые варианты

| Вариант | Причина отклонения |
|---|---|
| Skyrim SDK внутри Core | Нарушает game-agnostic boundary |
| Только Papyrus | Недостаточный контроль, производительность и тестируемость для системного bridge |
| Network listener | Лишняя поверхность атаки для локальной интеграции |
| Отдельный DLL для каждой SE и AE версии | Увеличивает packaging matrix; flat target уже решает эту задачу |
| Включить VR сразу | Расширяет ABI и test matrix до подтверждения SE/AE baseline |

## 8 Gates

Решение разрешает M2.1 и последующие этапы, но не объявляет релиз готовым. Support требует сборки, unit tests, transport tests, in-game validation, security review и точного compatibility report.

## 9 Источники

- ARCH-003, ARCH-005, ARCH-008
- ADR-006, ADR-007, ADR-008
- MOD-001 Core v2.0 full content
- https://skse.silverlock.org/
- https://github.com/ianpatt/skse64
- https://github.com/CharmedBaryon/CommonLibSSE-NG

## 10 История

| Версия | Дата | Изменение |
|---|---|---|
| 1.0 | 2026-09-04 | Первое принятое решение по реализации MOD-002 для Skyrim SE и AE |
