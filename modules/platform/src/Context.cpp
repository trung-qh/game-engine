#include "platform/Context.h"

#include <stdexcept>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "platform/errors/SdlError.h"

namespace engine::platform {

Context::Context() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    ThrowSdlError("SDL_Init failed");
  }

  if (!TTF_Init()) {
    const std::string error = FormatSdlError("TTF_Init failed");
    SDL_Quit();
    throw std::runtime_error(error);
  }

  is_initialized_ = true;
}

Context::~Context() {
  if (is_initialized_) {
    TTF_Quit();
    SDL_Quit();
  }
}

}  // namespace engine::platform
