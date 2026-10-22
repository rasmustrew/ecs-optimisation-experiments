# C++ ECS

The purpose of this project is to evolve a simple ECS through guided optimisations. 
To that end it includes an experiment harness created via C++ Concepts, allowing any ECS World that fits the parameters. 
The idea is to start from ecsSimple.h, analysing that theoretically with Big O analysis, as well as empirically through tools such as VTune or Valgrind,
and finding opportunities for optimisations. 
Using the experiment harness we can then verify that the intended optimisation actually worked. 

# Analysis

Here we analyse our implementations and attempt to iteratively improve upon them. 
Our starting point is World0, and we will be focusing mainly on the Query function, 
as that is what would run every frame multiple times in a real scenario. 

## World0

The complexity of Query for World0 is O(n * t_q * t), 
where n is the number of entities, t_q is the number of queried components, and t is the total number of components an entity might have.
This is a slight oversimplification, as likely few, and certainly not all entities have the full range of components. 

### Hypothesis
By storing the components in a Hashmap instead of a vector, 
we can reduce the expected complexity to O(n * t_q), as we can avoid searching through the t components. 

### Testing
Here we see the average time in ms for a query, 
one scenario being a query with a single component after a fresh world creation, 
another scenario being a more coomplex query, after some random additions and deletions.


![World0 + World1 Query Single](./images/Query_Single.png)

![World0 + World1 Query Complex Messy](./images/Query_Complex_Messy.png)

Both of these graphs look linear for both Worlds, which is expected, 
as the variable we are varying is the number of Entities, which both are linear in.
World1 is much faster than World0, which is what we expected.
There are 0-10 components on each Entity, so about a 8-15x speedup is about right, 
especially considering we were doing the loop over all components twice.


# Building

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

## clang-tidy

`clang-tidy` is enabled during normal builds by default (`ENABLE_CLANG_TIDY_ON_BUILD=ON`).

To disable it for faster local builds:

```
cmake --preset debug -DENABLE_CLANG_TIDY_ON_BUILD=OFF
```

You can also run it manually via the dedicated target:

```
cmake --build --preset debug --target tidy
```

## Opening in Visual Studio

Open the repo folder via **File → Open → CMake Project** and select `CMakeLists.txt`. Visual Studio will configure automatically.

## Dependencies

The project uses a `vcpkg.json` manifest for dependency resolution. Dependencies are restored automatically during the configure step.


