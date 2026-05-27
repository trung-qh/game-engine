# entity

`modules/entity` is the standalone ECS storage module. It owns entity creation, entity recycling, component pools, filtered views, and component iteration.

## Target

```cmake
engine::entity
```

The module builds as a static library from `src/entity/Registry.cpp` and exposes `modules/entity/include` publicly.

## Headers

```text
include/entity/Entity.h          Entity ids, generations, and metadata.
include/entity/Registry.h        Registry API for entities, components, views, and iteration.
include/entity/ComponentPool.h   Dense typed component storage.
include/entity/IComponentPool.h  Type-erased component pool interface.
```

## Registry API

`engine::entity::Registry` supports:

- `CreateEntity()` and `DestroyEntity()`
- `IsAlive(entity)`
- `AddComponent<T>(entity, args...)`
- `RemoveComponent<T>(entity)`
- `Has<T...>(entity)`
- `TryGetComponent<T>(entity)`
- `View<T...>()`
- `Each<T...>(func)`

Views pick the smallest available component pool as the base range, then filter entities that do not have the full requested component set.

## Entity Lifetime

Entities contain an id and generation. Destroying an entity removes it from every component pool, marks the id as available, increments the generation, and allows the id to be reused later without making stale entity handles valid again.

## Component Storage

Each component type is stored in a dense `ComponentPool<T>`. Removing a component swaps the last component into the removed slot, which keeps iteration compact but does not preserve component order.
