#pragma once

#include "SDL3/SDL.h"
#include "engine/core/Core.h"

namespace engine::platform {

class Window {
 public:
  Window(const char* title, int width, int height);
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  bool PollEvents(core::InputState& input_state);

  SDL_Window* NativeWindow() const { return window_; }

 private:
  SDL_Window* window_ = nullptr;
};

}  // namespace engine::platform
