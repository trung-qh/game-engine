#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "engine/core/InputState.h"
#include "engine/ecs/ISystem.h"
#include "engine/entity/Registry.h"

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

  entity::Registry& Reg() { return registry_; }

 private:
  float fixed_time_step_ = 1.0f / 120.0f;
  float accumulator_ = 0.0f;

  entity::Registry registry_;

  std::vector<std::unique_ptr<ISystem>> systems_;
};

}  // namespace engine
