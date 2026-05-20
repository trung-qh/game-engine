#pragma once

#include "engine/core/InputState.h"

namespace engine {
class World;

struct UpdateContext {
  World& world;
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
