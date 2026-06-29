# C++ ECS

The purpose of this project is to evolve a simple ECS through guided optimisations. 
To that end it includes an experiment harness created via C++ Concepts, allowing any ECS World that fits the parameters. 
The idea is to start from ecsSimple.h, analysing that theoretically with Big O analysis, as well as empirically through tools such as VTune or Valgrind,
and finding opportunities for optimisations. 
Using the experiment harness we can then verify that the intended optimisation actually worked. 

## Building

> **Windows only.** Linux support has not been tested.

The project uses CMake with vcpkg for dependency management.

Required tools:

- CMake (3.20+)
- Ninja
- vcpkg
- MSVC toolchain + Windows SDK (Visual Studio Build Tools is sufficient; full Visual Studio IDE is optional)

1. Clone the repository
2. Ensure `VCPKG_ROOT` is set to your vcpkg installation
3. Open an x64 MSVC developer shell (required for configure, or any time the build cache is recreated)
4. Run:

```
cmake --preset debug
cmake --build --preset debug
```

> Note: A plain command prompt is not guaranteed to work unless your environment already resolves to x64 `cl.exe`.

The executable is output to `build/debug/ECSExperiments.exe`.

Subsequent rebuilds after code changes only need:

```
cmake --build --preset debug
```

### clang-tidy

`clang-tidy` is enabled during normal builds by default (`ENABLE_CLANG_TIDY_ON_BUILD=ON`).

To disable it for faster local builds:

```
cmake --preset debug -DENABLE_CLANG_TIDY_ON_BUILD=OFF
```

You can also run it manually via the dedicated target:

```
cmake --build --preset debug --target tidy
```

### Opening in Visual Studio

Open the repo folder via **File → Open → CMake Project** and select `CMakeLists.txt`. Visual Studio will configure automatically.

### Dependencies

The project uses a `vcpkg.json` manifest for dependency resolution. Dependencies are restored automatically during the configure step.
