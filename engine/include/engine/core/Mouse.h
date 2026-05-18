#pragma once

#include <cstdint>

namespace engine::core {

enum class MouseButtonState : uint32_t {
  Up = 0,
  Pressed = 1,
  Down = 2,
  Released = 3,
};

using MouseButton = uint32_t;
namespace MouseButtons {

inline constexpr MouseButton Left = 1;
inline constexpr MouseButton Middle = 2;
inline constexpr MouseButton Right = 3;

inline constexpr MouseButton Count = 4;

}  // namespace MouseButtons

struct MouseMoveInput {
  float x = 0;
  float y = 0;
};

}  // namespace engine::core
