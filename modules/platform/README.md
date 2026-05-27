# platform

`modules/platform` is the SDL-backed platform layer. It owns native initialization, windows, input event translation, renderer calls, text rendering, font loading, timing, and SDL error handling.

Game code normally talks to this module through `engine::Application` and the public engine components, not directly.

## Target

```cmake
engine::platform
```

The module builds as a static library and links:

- `engine::core`
- vendored `SDL3::SDL3-static`
- vendored `SDL3_ttf::SDL3_ttf-static`

## Vendored Dependencies

SDL3 and SDL_ttf live under:

```text
vendor/SDL
vendor/SDL_ttf
```

If those directories are missing required CMake files, this module tries to run:

```sh
git submodule update --init --recursive
```

during configure. If that still fails, configure stops with a message naming the missing dependency.

## Headers

```text
include/platform/Context.h             SDL and SDL_ttf lifetime management.
include/platform/window/Window.h       SDL window wrapper and event polling.
include/platform/renderer/Renderer.h   Drawing API for rectangles, circles, and text.
include/platform/renderer/Font.h       SDL_ttf font wrapper.
include/platform/renderer/Text.h       SDL_ttf text wrapper.
include/platform/time/Clock.h          Per-frame timing.
include/platform/time/TimeStep.h       Seconds and milliseconds duration helper.
include/platform/errors/SdlError.h     SDL error formatting and checking helpers.
```

## Responsibilities

- Initialize and shut down SDL and SDL_ttf through `platform::Context`.
- Create and destroy an SDL window through `platform::Window`.
- Translate SDL keyboard, mouse, and quit events into `core::InputState`.
- Draw filled and outlined rectangles and circles.
- Cache and render text through SDL_ttf.
- Provide frame delta time through `platform::Clock`.

## Boundary

This module is intentionally private to the high-level engine target. Public game-facing types should stay in `engine` or `core` unless a feature specifically needs native SDL access.
