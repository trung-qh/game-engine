#include "engine/ecs/World.h"

#include <stdexcept>

namespace engine {

World::World(float fixed_time_step) {
  if (fixed_time_step <= 0.0f) {
    throw std::runtime_error("Fixed time step must be greater than 0.0f");
  }

  registry_ = std::make_unique<entity::Registry>();

  fixed_time_step_ = fixed_time_step;
  accumulator_ = 0.0f;
}

World::~World() {}

void World::Update(core::InputState& input_state, float delta_time) {
  accumulator_ += delta_time;

  while (accumulator_ >= fixed_time_step_) {
    UpdateContext context{*this, input_state, fixed_time_step_};

    for (auto& system : systems_) {
      system->OnUpdate(context);
    }

    FlushCommands();

    input_state.AdvanceFrame();
    accumulator_ -= fixed_time_step_;
  }
}

entity::Entity World::CreateEntity() { return registry_->CreateEntity(); }

void World::DestroyEntity(entity::Entity entity) {
  commands_.push(std::make_unique<DestroyEntityCommand>(entity));
}

void World::FlushCommands() {
  while (!commands_.empty()) {
    commands_.front()->Execute(*registry_);
    commands_.pop();
  }
}

}  // namespace engine
