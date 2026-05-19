#pragma once

#include "engine/ecs/Entity.h"

namespace engine {

class IComponentPool {
 public:
  IComponentPool() = default;
  virtual ~IComponentPool() = default;

  virtual void Remove(Entity entity) = 0;
};

}  // namespace engine
