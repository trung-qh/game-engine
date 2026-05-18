#pragma once

#include "SDL3/SDL.h"

namespace platform {

class Window {
 public:
  Window(const char* title, int width, int height);
  ~Window();

  bool PollEvents();

  SDL_Window* NativeWindow() const { return window_; }

 private:
  SDL_Window* window_ = nullptr;
};

}  // namespace platform