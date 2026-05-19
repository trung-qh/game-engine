#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "engine/ecs/Registry.h"
#include "engine/ecs/interfaces/ISystem.h"

namespace engine {

class World {
 public:
  World();
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

  Registry& Reg() { return registry_; }

 private:
  Registry registry_;
  std::vector<std::unique_ptr<ISystem>> systems_;
};

}  // namespace engine
