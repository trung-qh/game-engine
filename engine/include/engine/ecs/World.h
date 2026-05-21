#pragma once

#include <memory>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>

#include "core/InputState.h"
#include "engine/ecs/Commands.h"
#include "engine/ecs/ISystem.h"

namespace engine::entity {
struct Entity;
class Registry;
}  // namespace engine::entity

namespace engine {

class World {
 public:
  World(float fixed_time_step);
  ~World();

  World(const World&) = delete;
  World& operator=(const World&) = delete;

  World(World&&) = delete;
  World& operator=(World&&) = delete;

  template <typename System>
  void AddSystem() {
    static_assert(std::is_base_of_v<ISystem, System>);
    systems_.emplace_back(std::make_unique<System>());
  }

  void Update(core::InputState& input_state, float delta_time);

  entity::Entity CreateEntity();
  void DestroyEntity(entity::Entity entity);

  template <typename T, typename... Args>
  void AddComponent(entity::Entity entity, Args&&... args) {
    commands_.push(
        std::make_unique<AddComponentCommand<T>>(entity, T{std::forward<Args>(args)...}));
  }

  template <typename T>
  void RemoveComponent(entity::Entity entity) {
    commands_.push(std::make_unique<RemoveComponentCommand<T>>(entity));
  }

  template <typename First, typename... Rest>
  auto View() const {
    return registry_->View<First, Rest...>();
  }

  template <typename First, typename... Rest, typename Func>
  void Each(Func&& func) {
    registry_->Each<First, Rest...>(std::forward<Func>(func));
  }

 private:
  void FlushCommands();

  float fixed_time_step_ = 1.0f / 120.0f;
  float accumulator_ = 0.0f;

  std::unique_ptr<entity::Registry> registry_;
  std::queue<std::unique_ptr<ICommand>> commands_;

  std::vector<std::unique_ptr<ISystem>> systems_;
};

}  // namespace engine
