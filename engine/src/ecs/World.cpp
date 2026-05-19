#include "engine/ecs/World.h"

namespace engine {

World::World(float fixed_time_step) : fixed_time_step_(fixed_time_step) {}

World::~World() {}

void World::Update(const core::InputState& input_state, float delta_time) {
  accumulator_ += delta_time;

  UpdateContext context{registry_, input_state, fixed_time_step_};

  while (accumulator_ >= fixed_time_step_) {
    for (auto& system : systems_) {
      system->OnUpdate(context);
    }

    accumulator_ -= fixed_time_step_;
  }
}

}  // namespace engine
