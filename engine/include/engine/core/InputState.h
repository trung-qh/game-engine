#pragma once

#include <array>

#include "engine/core/Keyboard.h"
#include "engine/core/Mouse.h"

namespace engine {

namespace platform {
class Window;
}

class InputState {
  friend class platform::Window;

 public:
  InputState() = default;
  ~InputState() = default;

  bool IsKeyPressed(Key key) const {
    return IsValidKey(key) && keyboard_[key] == KeyStates::Pressed;
  }
  bool IsKeyReleased(Key key) const {
    return IsValidKey(key) && keyboard_[key] == KeyStates::Released;
  }
  bool IsKeyUp(Key key) const { return IsValidKey(key) && keyboard_[key] == KeyStates::Up; }
  bool IsKeyDown(Key key) const {
    return IsValidKey(key) &&
           (keyboard_[key] == KeyStates::Pressed || keyboard_[key] == KeyStates::Down);
  }

  bool IsMouseButtonPressed(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonStates::Pressed;
  }
  bool IsMouseButtonReleased(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonStates::Released;
  }
  bool IsMouseButtonUp(MouseButton button) const {
    return IsValidMouseButton(button) && mouse_[button] == MouseButtonStates::Up;
  }
  bool IsMouseButtonDown(MouseButton button) const {
    return IsValidMouseButton(button) && (mouse_[button] == MouseButtonStates::Pressed ||
                                          mouse_[button] == MouseButtonStates::Down);
  }

  const MouseMoveInput& GetMousePosition() const { return mouse_pos_; }

  void AdvanceFrame();

 private:
  std::array<KeyState, KeyCodes::Count> keyboard_{};

  std::array<MouseButtonState, MouseButtons::Count> mouse_{};
  MouseMoveInput mouse_pos_{};

  bool IsValidKey(Key key) const { return key < KeyCodes::Count; }
  bool IsValidMouseButton(MouseButton button) const { return button < MouseButtons::Count; }
};

}  // namespace engine
