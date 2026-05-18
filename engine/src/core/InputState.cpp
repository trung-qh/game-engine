#include "engine/core/InputState.h"

namespace engine {

void InputState::AdvanceFrame() {
  for (auto& key_state : keyboard_) {
    if (key_state == KeyStates::Pressed) {
      key_state = KeyStates::Down;
    } else if (key_state == KeyStates::Released) {
      key_state = KeyStates::Up;
    }
  }

  for (auto& mouse_state : mouse_) {
    if (mouse_state == MouseButtonStates::Pressed) {
      mouse_state = MouseButtonStates::Down;
    } else if (mouse_state == MouseButtonStates::Released) {
      mouse_state = MouseButtonStates::Up;
    }
  }
}

}  // namespace engine