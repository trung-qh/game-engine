#include "platform/window/Window.h"

#include <stdexcept>

namespace platform {

Window::Window(const char* title, int width, int height) {
  window_ = SDL_CreateWindow(title, width, height, 0);
  if (window_ == nullptr) {
    throw std::runtime_error("Failed to create SDL Window");
  }
}

Window::~Window() {
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
}

bool Window::PollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      return false;
    }
  }

  return true;
}

}