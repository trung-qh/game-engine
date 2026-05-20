#pragma once

#include <array>

#include "engine/core/Keyboard.h"
#include "engine/core/Mouse.h"

namespace engine {

namespace core {

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

  void AdvanceFrame();

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

}  // namespace core
}  // namespace engine
