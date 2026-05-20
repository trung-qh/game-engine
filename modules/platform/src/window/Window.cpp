#include "platform/window/Window.h"

#include "platform/errors/SdlError.h"

namespace engine::platform {

Window::Window(const char* title, int width, int height) {
  window_ = SDL_CreateWindow(title, width, height, 0);
  if (window_ == nullptr) {
    ThrowSdlError("SDL_CreateWindow failed");
  }
}

Window::~Window() {
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
}

bool Window::PollEvents(core::InputState& input_state) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
        return false;
      case SDL_EVENT_MOUSE_MOTION:
        input_state.SetMousePosition({event.motion.x, event.motion.y});
        break;
      case SDL_EVENT_KEY_DOWN:
        if (!event.key.repeat && input_state.IsValidKey(event.key.scancode)) {
          input_state.SetKeyState(event.key.scancode, core::KeyState::Pressed);
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (input_state.IsValidKey(event.key.scancode)) {
          input_state.SetKeyState(event.key.scancode, core::KeyState::Released);
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (input_state.IsValidMouseButton(event.button.button)) {
          input_state.SetMouseButtonState(event.button.button, core::MouseButtonState::Pressed);
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        if (input_state.IsValidMouseButton(event.button.button)) {
          input_state.SetMouseButtonState(event.button.button, core::MouseButtonState::Released);
        }
        break;
      default:
        break;
    }
  }

  return true;
}

}  // namespace engine::platform
