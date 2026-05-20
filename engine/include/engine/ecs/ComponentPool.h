#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "engine/ecs/Entity.h"
#include "engine/ecs/IComponentPool.h"

namespace engine {

namespace internal {

constexpr uint32_t kInvalidIndex = std::numeric_limits<uint32_t>::max();

template <typename T>
class ComponentPool : public IComponentPool {
 public:
  ComponentPool() = default;
  ~ComponentPool() = default;

  void Add(Entity entity, T component) {
    if (entity >= entity_to_index_.size()) {
      entity_to_index_.resize(entity + 1, kInvalidIndex);
    }

    if (Has(entity)) {
      components_[entity_to_index_[entity]] = component;
      return;
    }

    entity_to_index_[entity] = static_cast<uint32_t>(components_.size());
    components_.emplace_back(component);
    entities_.emplace_back(entity);
  }

  void Remove(Entity entity) override {
    if (!Has(entity)) return;

    auto index = entity_to_index_[entity];
    auto last_index = components_.size() - 1;

    if (index != last_index) {
      entity_to_index_[entities_[last_index].id] = index;
      std::swap(components_[index], components_[last_index]);
      std::swap(entities_[index], entities_[last_index]);
    }

    components_.pop_back();
    entities_.pop_back();
    entity_to_index_[entity] = kInvalidIndex;
  }

  bool Has(Entity entity) const {
    return entity < entity_to_index_.size() && entity_to_index_[entity] != kInvalidIndex &&
           entities_[entity_to_index_[entity]].generation == entity.generation;
  }

  const T* TryGet(Entity entity) const {
    if (!Has(entity)) return nullptr;

    const auto index = entity_to_index_[entity];
    return &components_[index];
  }

  T* TryGet(Entity entity) {
    return const_cast<T*>(static_cast<const ComponentPool&>(*this).TryGet(entity));
  }

  const std::vector<Entity>& GetEntities() const { return entities_; }

  size_t GetSize() const { return components_.size(); }

 private:
  std::vector<T> components_;
  std::vector<Entity> entities_;

  std::vector<uint32_t> entity_to_index_;
};

}  // namespace internal

}  // namespace engine