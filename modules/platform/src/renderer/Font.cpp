#include "platform/renderer/Font.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "platform/errors/SdlError.h"

namespace engine::platform {

Font::Font(const char* path, float size) {
  font_ = TTF_OpenFont(path, size);
  if (font_ == nullptr) {
    ThrowSdlError("TTF_OpenFont failed");
  }
}

Font::~Font() {
  if (font_ != nullptr) {
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
}

void Font::SetSize(float size) {
  CheckSdlResult(TTF_SetFontSize(font_, size), "TTF_SetFontSize failed");
}

}  // namespace engine::platform
