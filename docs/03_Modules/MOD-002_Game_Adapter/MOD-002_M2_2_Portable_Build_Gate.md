# MOD-002 M2.2 Portable Build Gate

**Status:** Ready to execute; blocked by missing local C++ toolchain  
**Scope:** Portable Game Adapter library and dependency-free tests only  
**Not included:** SKSE plugin build, Skyrim runtime execution or Skyrim VR

## Required environment

- Visual Studio 2022 Build Tools with the MSVC x64 C++ workload;
- Windows SDK supported by that toolchain;
- CMake 3.25 or newer available on `PATH`;
- no CommonLibSSE NG or Skyrim installation is required for this gate.

## Deterministic commands

```text
cmake --preset msvc-debug
cmake --build --preset msvc-debug
ctest --preset msvc-debug

cmake --preset msvc-release
cmake --build --preset msvc-release
ctest --preset msvc-release
```

Both configurations compile with warnings treated as errors. The SKSE plugin is explicitly disabled in these presets so the portable contract layer can be verified independently.

## Pass criteria

1. Debug configure and build succeed.
2. Debug test preset reports every test passed.
3. Release configure and build succeed.
4. Release test preset reports every test passed.
5. No warning is suppressed to obtain a pass.
6. No Skyrim, SKSE or CommonLibSSE type appears in the portable include or source directories.
7. The exact compiler, CMake and Windows SDK versions are recorded in the gate result.

## Current evidence

The execution environment was inspected on 2026-09-05. `cl`, `clang-cl`, `clang++`, `g++`, `cmake`, `ninja`, `msbuild` and a Visual Studio installation containing `Microsoft.VisualStudio.Component.VC.Tools.x86.x64` were not available. Therefore this gate remains **Pending**, and no compile or test pass is claimed.

## Gate dependency

M2.3 may prepare the named-pipe transport design, but the portable baseline must pass M2.2 before a combined adapter and transport build can be promoted.
