#pragma once

#include "engine/core/InputState.h"
#include "engine/ecs/Registry.h"

namespace engine {

struct UpdateContext {
  Registry& registry;
  const core::InputState& input_state;
  const float fixed_time_step;
};

class ISystem {
 public:
  ISystem() = default;
  virtual ~ISystem() = default;

  virtual void OnUpdate(const UpdateContext& context) = 0;

 private:
};

}  // namespace engine
