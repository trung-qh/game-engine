#pragma once

#include <cstdint>

namespace engine {

using MouseButtonState = uint32_t;
namespace MouseButtonStates {

inline constexpr MouseButtonState Up = 0;
inline constexpr MouseButtonState Pressed = 1;
inline constexpr MouseButtonState Down = 2;
inline constexpr MouseButtonState Released = 3;

}  // namespace MouseButtonStates

using MouseButton = uint32_t;
namespace MouseButtons {

inline constexpr MouseButton Left = 1;
inline constexpr MouseButton Middle = 2;
inline constexpr MouseButton Right = 3;

inline constexpr MouseButton Count = 4;

}  // namespace MouseButtons

struct MouseMoveInput {
  int x = 0;
  int y = 0;
};

}  // namespace engine
