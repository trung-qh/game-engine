#pragma once

#include <cstdint>

namespace engine {

struct Entity {
  uint32_t id = 0;
  uint32_t generation = 0;

  bool operator==(const Entity&) const = default;
  operator uint32_t() const { return id; }
};

struct EntityMetadata {
  uint32_t generation = 0;

  bool is_alive = true;
};

}  // namespace engine
