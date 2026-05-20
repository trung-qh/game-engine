#pragma once

#include <string>

struct TTF_Font;

namespace engine::platform {

class Font {
 public:
  Font(const std::string& path, float size);
  ~Font();

  Font(const Font&) = delete;
  Font& operator=(const Font&) = delete;

  Font(Font&&) = delete;
  Font& operator=(Font&&) = delete;

  void SetSize(float size);

  TTF_Font* NativeFont() const { return font_; }

 private:
  TTF_Font* font_ = nullptr;
};

}  // namespace engine::platform
