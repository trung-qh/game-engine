#include "platform/Context.h"

#include <stdexcept>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

namespace engine::platform {

Context::Context() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("SDL_Init failed");
  }

  if (!TTF_Init()) {
    SDL_Quit();
    throw std::runtime_error("TTF_Init failed");
  }

  is_initialized_ = true;
}

Context::~Context() {
  if (is_initialized_) {
    TTF_Quit();
    SDL_Quit();
  }
}

}