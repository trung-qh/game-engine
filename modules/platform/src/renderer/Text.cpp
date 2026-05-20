#include "platform/renderer/Text.h"

#include "SDL3_ttf/SDL_ttf.h"
#include "platform/errors/SdlError.h"
#include "platform/renderer/Font.h"

namespace engine::platform {

Text::Text(TTF_TextEngine* text_engine, const std::string& text, const Font& font) {
  text_ = TTF_CreateText(text_engine, font.NativeFont(), text.c_str(), 0);

  if (text_ == nullptr) {
    ThrowSdlError("TTF_CreateText failed");
  }
}

Text::~Text() {
  if (text_ != nullptr) {
    TTF_DestroyText(text_);
    text_ = nullptr;
  }
}

void Text::SetText(const std::string& text) {
  CheckSdlResult(TTF_SetTextString(text_, text.c_str(), 0), "TTF_SetTextString failed");
}

void Text::SetFont(const Font& font) {
  CheckSdlResult(TTF_SetTextFont(text_, font.NativeFont()), "TTF_SetTextFont failed");
}

}  // namespace engine::platform
