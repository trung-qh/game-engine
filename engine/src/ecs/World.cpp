#include "engine/ecs/World.h"

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "engine/ecs/Components.h"

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
    CheckCollisions();

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

void World::CheckCollisions() {
  std::vector<entity::Entity> entities;
  for (auto entity : registry_->View<Collider>()) {
    entities.emplace_back(entity);

    if (auto* collider = registry_->TryGetComponent<Collider>(entity)) {
      collider->others_.clear();
    }
  }

  for (size_t i = 0; i < entities.size(); ++i) {
    auto* collider = registry_->TryGetComponent<Collider>(entities[i]);
    if (collider == nullptr) {
      continue;
    }

    for (size_t j = i + 1; j < entities.size(); ++j) {
      auto* other_collider = registry_->TryGetComponent<Collider>(entities[j]);
      if (other_collider == nullptr || !Intersecting(collider->body_, other_collider->body_)) {
        continue;
      }

      collider->others_.emplace_back(entities[j]);
      other_collider->others_.emplace_back(entities[i]);
    }
  }
}

}  // namespace engine
