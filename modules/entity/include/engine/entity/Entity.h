#pragma once

#include <cstdint>
#include <limits>

namespace engine::entity {

constexpr uint32_t kInvalidEntityId = std::numeric_limits<uint32_t>::max();

struct Entity {
  uint32_t id = kInvalidEntityId;
  uint32_t generation = 0;

  bool operator==(const Entity&) const = default;
  operator uint32_t() const { return id; }
};

struct EntityMetadata {
  uint32_t generation = 0;

  bool is_alive = true;
};

}  // namespace engine::entity
