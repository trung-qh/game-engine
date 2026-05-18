#include "engine/core/InputState.h"

namespace engine::core {

void InputState::AdvanceFrame() {
  for (auto& key_state : keyboard_) {
    if (key_state == KeyState::Pressed) {
      key_state = KeyState::Down;
    } else if (key_state == KeyState::Released) {
      key_state = KeyState::Up;
    }
  }

  for (auto& mouse_state : mouse_) {
    if (mouse_state == MouseButtonState::Pressed) {
      mouse_state = MouseButtonState::Down;
    } else if (mouse_state == MouseButtonState::Released) {
      mouse_state = MouseButtonState::Up;
    }
  }
}

}  // namespace engine::core
