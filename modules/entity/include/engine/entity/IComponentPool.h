#pragma once

#include "engine/entity/Entity.h"

namespace engine::entity {

class IComponentPool {
 public:
  IComponentPool() = default;
  virtual ~IComponentPool() = default;

  virtual void Remove(Entity entity) = 0;
};

}  // namespace engine::entity
