#include "engine/ecs/Registry.h"

namespace engine {

Registry::Registry() {}

Registry::~Registry() {}

Entity Registry::CreateEntity() {
  if (!available_entity_ids_.empty()) {
    auto id = available_entity_ids_.front();
    available_entity_ids_.pop();

    entity_metadata_[id].is_alive = true;
    return Entity{id, entity_metadata_[id].generation};
  }

  auto id = static_cast<uint32_t>(entity_metadata_.size());
  entity_metadata_.emplace_back(EntityMetadata{0, true});
  return Entity{id, 0};
}

void Registry::DestroyEntity(Entity entity) {
  if (IsAlive(entity)) {
    entity_metadata_[entity].is_alive = false;
    ++entity_metadata_[entity].generation;

    entities_to_flush_.push_back(entity);
  }
}

void Registry::Flush() {
  for (auto entity : entities_to_flush_) {
    for (auto& [type, pool] : component_pools_) {
      pool->Remove(entity);
    }

    available_entity_ids_.push(entity);
  }

  entities_to_flush_.clear();
}

bool Registry::IsAlive(Entity entity) const {
  return entity.id < entity_metadata_.size() &&
         entity.generation == entity_metadata_[entity].generation &&
         entity_metadata_[entity].is_alive;
}

}  // namespace engine
