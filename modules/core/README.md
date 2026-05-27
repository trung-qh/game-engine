# core

`modules/core` is a header-only module for shared engine types. It has no source files and no platform dependencies.

## Target

```cmake
engine::core
```

The target exposes `modules/core/include` as an interface include directory.

## Headers

```text
include/core/Core.h        Convenience include for the module.
include/core/Color.h       RGBA color type and common color constants.
include/core/Font.h        Font path and size descriptor.
include/core/InputState.h  Keyboard, mouse button, and mouse position state.
include/core/Keyboard.h    Key states and SDL-compatible key codes.
include/core/Mouse.h       Mouse button states and mouse position data.
include/core/Shapes.h      Rect, Circle, Shape variant, transforms, and intersection tests.
```

## Responsibilities

- Keep common value types independent from the platform layer.
- Provide input state queries such as `IsKeyPressed`, `IsKeyDown`, and `IsMouseButtonReleased`.
- Provide basic 2D shape helpers used by rendering and collision systems.
- Provide lightweight descriptors, such as `core::Font`, that higher layers can resolve into native resources.

## Notes

`core::Shape` is a `std::variant<Rect, Circle>`. Use `core::Translate`, `core::SetPosition`, and `core::Intersecting` when code should work across both shape types.
