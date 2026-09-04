# MOD-002 Game Adapter

MOD-002 is the only approved boundary between World Engine and a concrete game runtime. The first implementation target is Skyrim Special Edition and Anniversary Edition on Windows. Skyrim VR is outside the first release.

The current implementation increment contains:

- a game-agnostic adapter lifecycle and capability model;
- typed inbound and outbound contracts;
- validation, bounded payloads, sequence deduplication and controlled failures;
- an explicit runtime bridge that contains every Skyrim and SKSE type;
- a CommonLibSSE NG plugin entry point behind an optional build flag;
- a versioned Skyrim SE/AE profile and JSON Schema;
- dependency-free contract tests for the portable adapter layer.

The plugin remains `prototype` until it is compiled with Visual Studio 2022, CommonLibSSE NG and SKSE, then tested against each declared runtime target.

## Build the portable layer

```text
cmake -S . -B build -DWORLD_ENGINE_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## Build the Skyrim plugin

Provide CommonLibSSE NG through its supported package integration, then enable:

```text
-DWORLD_ENGINE_BUILD_SKSE_PLUGIN=ON
```

Use the `commonlibsse-ng-flatrim` package for the first SE/AE-only implementation. Do not enable the VR target in the first release.

## Architecture boundaries

- Game Adapter calls the application-owned Game Ingress Port.
- Application coordination calls the Game Output or Capability Port implemented by Game Adapter.
- Application Host selects the Game Profile and owns process-level lifecycle wiring.
- Plugin Detection owns discovery and validated plugin evidence.
- Game Adapter maps validated evidence to capabilities.
- Save Manager owns restore compatibility.
- No domain module imports Skyrim, SKSE or Game Adapter implementation types.
