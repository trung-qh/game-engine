#pragma once

#include <array>

#include "core/Keyboard.h"
#include "core/Mouse.h"

namespace engine::core {

class InputState {
 public:
  InputState() = default;
  ~InputState() = default;

  bool IsKeyPressed(Key key) const {
    return IsValidKey(key) && keyboard_[key] == KeyState::Pressed;
  }
  bool IsKeyReleased(Key key) const {
    return IsValidKey(key) && keyboard_[key] == KeyState::Released;
  }
  bool IsKeyUp(Key key) const { return IsValidKey(key) && keyboard_[key] == KeyState::Up; }
  bool IsKeyDown(Key key) const {
    return IsValidKey(key) &&
           (keyboard_[key] == KeyState::Pressed || keyboard_[key] == KeyState::Down);
  }

  bool IsMouseButtonPressed(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonState::Pressed;
  }
  bool IsMouseButtonReleased(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonState::Released;
  }
  bool IsMouseButtonUp(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonState::Up;
  }

  const MouseMoveInput& GetMousePosition() const { return mouse_pos_; }

  void AdvanceFrame() {
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

  bool IsValidKey(Key key) const { return key >= 0 && key < KeyCode::Count; }
  bool IsValidMouseButton(MouseButton button) const {
    return button >= 0 && button < MouseButtons::Count;
  }

  void SetKeyState(Key key, KeyState state) { keyboard_[key] = state; }
  void SetMouseButtonState(MouseButton button, MouseButtonState state) { mouse_[button] = state; }
  void SetMousePosition(MouseMoveInput pos) { mouse_pos_ = pos; }

 private:
  std::array<KeyState, KeyCode::Count> keyboard_{};

  std::array<MouseButtonState, MouseButtons::Count> mouse_{};
  MouseMoveInput mouse_pos_{};
};

}  // namespace engine::core
