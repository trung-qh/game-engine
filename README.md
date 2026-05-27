# game-engine

A small C++20 game engine built around an entity-component-system runtime, simple 2D rendering, input, collision helpers, and SDL-backed platform services.

The repository is currently Apple-only and builds the engine as a shared library (`libengine.dylib`). The build also creates a distributable bundle at `build/engine` containing the public headers and dynamic library that sandbox games can link against.

## Repository Layout

```text
engine/              Public engine runtime, ECS facade, and renderer integration.
modules/core/        Header-only shared types: colors, shapes, fonts, keyboard, mouse, input state.
modules/entity/      Standalone ECS registry and component storage.
modules/platform/    SDL3/SDL_ttf platform layer for windows, rendering, fonts, text, and time.
res/                 Shared assets used by examples.
sandbox/             Example games and experiments. Docs for these will come later.
```

## Requirements

- macOS or another Apple platform
- CMake 3.28+
- A C++20 compiler
- Git submodules for vendored SDL dependencies

The `platform` module vendors SDL3 and SDL_ttf as submodules. CMake will try to fetch missing submodules automatically during configure, but you can also do it explicitly:

```sh
git submodule update --init --recursive
```

## Build

Configure and build from the repository root:

```sh
cmake -S . -B build
cmake --build build
```

The default build includes the `engine_dist` target, which refreshes:

```text
build/engine/include/
build/engine/lib/libengine.dylib
```

You can also run that bundle step directly:

```sh
cmake --build build --target engine_dist
```

## Engine Model

User applications derive from `engine::Application`, create entities and components in `OnInit`, register systems with `world_.AddSystem<T>()`, and let `Application::Run()` drive the loop.

At runtime, the engine:

- polls platform events into `engine::core::InputState`
- advances systems on a fixed timestep
- flushes queued entity/component commands
- checks collisions between `engine::Collider` components
- renders `engine::RenderableShape` and `engine::RenderableText` components

The umbrella include for users is:

```cpp
#include "engine/Engine.h"
```

Applications that want the provided `main` function include:

```cpp
#include "engine/runtime/EntryPoint.h"
```

and define:

```cpp
std::unique_ptr<engine::Application> CreateApplication();
```

## Modules

- `engine` depends publicly on `engine::core` and `engine::entity`, and privately on `engine::platform`.
- `modules/core` is header-only and contains data types shared across the engine.
- `modules/entity` owns entity lifetimes, component pools, views, and iteration.
- `modules/platform` owns SDL initialization, windows, input translation, drawing, text, fonts, and timing.

See the README in each directory for more focused notes.
