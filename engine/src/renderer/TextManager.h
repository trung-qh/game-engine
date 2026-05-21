#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/Font.h"

namespace engine::platform {
class Renderer;
class Font;
class Text;
}  // namespace engine::platform

namespace engine {

class TextManager {
 public:
  TextManager(platform::Renderer& renderer);
  ~TextManager();

  void BeginFrame();

  platform::Font& GetOrCreateNativeFont(core::Font* font);
  platform::Text& GetNextText(const char* text, const platform::Font& font);

 private:
  platform::Renderer& renderer_;

  std::unordered_map<core::Font*, std::unique_ptr<platform::Font>> fonts_;
  std::vector<std::unique_ptr<platform::Text>> texts_;
  size_t next_text_index_ = 0;
};

}  // namespace engine
