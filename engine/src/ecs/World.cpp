#include "engine/ecs/World.h"

namespace engine {

World::World(float fixed_time_step) {
  if (fixed_time_step <= 0.0f) {
    throw std::runtime_error("Fixed time step must be greater than 0.0f");
  }

  fixed_time_step_ = fixed_time_step;
  accumulator_ = 0.0f;
}

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

  registry_.Flush();
}

}  // namespace engine
