#include "platform/errors/SdlError.h"

#include <stdexcept>
#include <string>

#include "SDL3/SDL.h"

namespace engine::platform {

std::string FormatSdlError(const char* operation) {
  return std::string(operation) + ": " + SDL_GetError();
}

[[noreturn]] void ThrowSdlError(const char* operation) {
  throw std::runtime_error(FormatSdlError(operation));
}

void CheckSdlResult(bool result, const char* operation) {
  if (!result) {
    ThrowSdlError(operation);
  }
}

}  // namespace engine::platform
