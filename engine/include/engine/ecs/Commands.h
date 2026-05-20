#pragma once

#include <utility>

#include "engine/entity/Registry.h"

namespace engine {

class ICommand {
 public:
  virtual ~ICommand() = default;
  virtual void Execute(entity::Registry& registry) = 0;
};

struct DestroyEntityCommand : public ICommand {
  explicit DestroyEntityCommand(entity::Entity entity) : entity(entity) {}

  entity::Entity entity;

  void Execute(entity::Registry& registry) override { registry.DestroyEntity(entity); }
};

template <typename T>
struct AddComponentCommand : public ICommand {
  AddComponentCommand(entity::Entity entity, T component)
      : entity(entity), component(std::move(component)) {}

  entity::Entity entity;
  T component;

  void Execute(entity::Registry& registry) override {
    registry.AddComponent<T>(entity, std::move(component));
  }
};

template <typename T>
struct RemoveComponentCommand : public ICommand {
  explicit RemoveComponentCommand(entity::Entity entity) : entity(entity) {}

  entity::Entity entity;

  void Execute(entity::Registry& registry) override { registry.RemoveComponent<T>(entity); }
};

}  // namespace engine
