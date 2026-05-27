# engine

The `engine` directory contains the public runtime layer that game code uses directly. It combines the lower-level modules into an application loop, world update, built-in renderable components, collision pass, and renderer bridge.

## Public Headers

```text
include/engine/Engine.h                  Umbrella include for game code.
include/engine/runtime/Application.h     Base application and main loop owner.
include/engine/runtime/EntryPoint.h      Optional generated main function.
include/engine/ecs/World.h               Entity, component, system, and view facade.
include/engine/ecs/ISystem.h             System interface and update context.
include/engine/ecs/Components.h          Built-in renderable and collider components.
include/engine/ecs/Commands.h            Deferred world mutation commands.
```

Implementation files live in `src/ecs`, `src/runtime`, and `src/renderer`.

## Application Flow

Game code typically derives from `engine::Application`:

```cpp
class Game : public engine::Application {
 public:
  Game() : engine::Application("Game", 900, 600) {}

 protected:
  void OnInit() override {
    auto entity = world_.CreateEntity();
    world_.AddComponent<engine::RenderableShape>(
        entity,
        engine::core::Rect{100.0f, 100.0f, 64.0f, 64.0f},
        engine::core::colors::White);

    world_.AddSystem<MySystem>();
  }

  void OnShutdown() override {}
};
```

`Application::Run()` creates the platform context, polls events, ticks the world at a fixed timestep, renders the world, and calls the lifecycle hooks.

## World and Systems

`engine::World` wraps the entity registry and exposes the main ECS operations:

- `CreateEntity()` and `DestroyEntity()`
- `AddComponent<T>()` and `RemoveComponent<T>()`
- `View<T...>()` and `Each<T...>()`
- `Has<T...>()`, `Get<T>()`, and `TryGet<T>()`
- `AddSystem<T>()`

Systems inherit from `engine::ISystem` and implement:

```cpp
void OnUpdate(const engine::UpdateContext& context) override;
```

The update context gives systems access to the world, input state, and fixed timestep. World mutations are queued and flushed after all systems run for a tick.

## Built-in Components

- `RenderableShape` renders a `core::Rect` or `core::Circle`.
- `RenderableText` renders text with a `core::Font`.
- `Collider` stores a shape and the entities it intersected during the most recent collision pass.

Collision checks are broad and simple: every entity with `Collider` is tested against every other collider using the shape intersection helpers in `modules/core`.

## Dependencies

The engine target is declared at the repository root:

- public: `engine::core`, `engine::entity`
- private: `engine::platform`

The private platform dependency keeps SDL details out of normal game code while still allowing the runtime and renderer to use the SDL-backed services internally.
