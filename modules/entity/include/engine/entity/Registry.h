#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <ranges>
#include <span>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "engine/entity/ComponentPool.h"
#include "engine/entity/Entity.h"
#include "engine/entity/IComponentPool.h"

namespace engine::entity {

class Registry {
 public:
  Registry();
  ~Registry();

  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;

  Registry(Registry&&) = delete;
  Registry& operator=(Registry&&) = delete;

  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  bool IsAlive(Entity entity) const;

  template <typename T, typename... Args>
  void AddComponent(Entity entity, Args&&... args) {
    if (!IsAlive(entity)) return;

    auto& pool = GetOrCreateComponentPool<T>();
    pool.Add(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  void RemoveComponent(Entity entity) {
    auto* pool = TryGetComponentPool<T>();

    if (pool != nullptr) {
      pool->Remove(entity);
    }
  }

  template <typename First, typename... Rest>
  bool Has(Entity entity) const {
    auto* pool = TryGetComponentPool<First>();

    if constexpr (sizeof...(Rest) == 0) {
      return pool != nullptr && pool->Has(entity);
    } else {
      return pool != nullptr && pool->Has(entity) && (Has<Rest>(entity) && ...);
    }
  }

  template <typename T>
  const T* TryGetComponent(Entity entity) const {
    auto* pool = TryGetComponentPool<T>();
    return pool == nullptr ? nullptr : pool->TryGet(entity);
  }

  template <typename T>
  T* TryGetComponent(Entity entity) {
    const auto* component = static_cast<const Registry*>(this)->TryGetComponent<T>(entity);
    return const_cast<T*>(component);
  }

  template <typename First, typename... Rest>
  auto View() const {
    const std::vector<Entity>* entities = nullptr;
    size_t size = 0;

    if (!(UpdateSmallestPool<First>(entities, size) &&
          (UpdateSmallestPool<Rest>(entities, size) && ...))) {
      entities = nullptr;
    }

    auto base =
        entities == nullptr ? std::span<const Entity>{} : std::span<const Entity>{*entities};
    return base | std::views::filter([this](Entity entity) { return Has<First, Rest...>(entity); });
  }

  template <typename First, typename... Rest, typename Func>
  void Each(Func&& func) {
    for (auto entity : View<First, Rest...>()) {
      func(entity, *TryGetComponent<First>(entity), TryGetComponent<Rest>(entity)...);
    }
  }

 private:
  std::queue<uint32_t> available_entity_ids_;
  std::vector<EntityMetadata> entity_metadata_;

  std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> component_pools_;

  template <typename T>
  const internal::ComponentPool<T>* TryGetComponentPool() const {
    std::type_index type = std::type_index(typeid(T));

    auto it = component_pools_.find(type);
    return it == component_pools_.end()
               ? nullptr
               : static_cast<const internal::ComponentPool<T>*>(it->second.get());
  }

  template <typename T>
  internal::ComponentPool<T>* TryGetComponentPool() {
    const auto* pool = static_cast<const Registry*>(this)->TryGetComponentPool<T>();
    return const_cast<internal::ComponentPool<T>*>(pool);
  }

  template <typename T>
  internal::ComponentPool<T>& GetOrCreateComponentPool() {
    std::type_index type = std::type_index(typeid(T));

    if (component_pools_.find(type) == component_pools_.end()) {
      component_pools_[type] = std::make_unique<internal::ComponentPool<T>>();
    }

    return *static_cast<internal::ComponentPool<T>*>(component_pools_[type].get());
  }

  template <typename T>
  bool UpdateSmallestPool(const std::vector<Entity>*& entities, size_t& size) const {
    const auto* pool = TryGetComponentPool<T>();
    if (pool == nullptr) return false;

    if (entities == nullptr || size > pool->GetSize()) {
      entities = &pool->GetEntities();
      size = pool->GetSize();
    }

    return true;
  }
};

}  // namespace engine::entity
