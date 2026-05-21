#include "renderer/TextManager.h"

#include "platform/renderer/Font.h"
#include "platform/renderer/Renderer.h"
#include "platform/renderer/Text.h"

namespace engine {

TextManager::TextManager(platform::Renderer& renderer) : renderer_(renderer) {}

TextManager::~TextManager() {}

void TextManager::BeginFrame() { next_text_index_ = 0; }

platform::Font& TextManager::GetOrCreateNativeFont(core::Font* font) {
  auto it = fonts_.find(font);
  if (it != fonts_.end()) {
    return *it->second;
  }

  auto native_font = std::make_unique<platform::Font>(font->path, font->size);
  auto& font_ref = *native_font;
  fonts_[font] = std::move(native_font);

  return font_ref;
}

platform::Text& TextManager::GetNextText(const char* text, const platform::Font& font) {
  if (next_text_index_ >= texts_.size()) {
    texts_.resize(next_text_index_ + 1);
  }

  if (texts_[next_text_index_] == nullptr) {
    texts_[next_text_index_] =
        std::make_unique<platform::Text>(renderer_.NativeTextEngine(), text, font);
  } else {
    texts_[next_text_index_]->SetText(text);
    texts_[next_text_index_]->SetFont(font);
  }

  return *texts_[next_text_index_++];
}

}  // namespace engine
