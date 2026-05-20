#pragma once

#include <string>

struct TTF_Text;
struct TTF_TextEngine;

namespace engine::platform {

class Font;

class Text {
 public:
  Text(TTF_TextEngine* text_engine, const std::string& text, const Font& font);
  ~Text();

  Text(const Text&) = delete;
  Text& operator=(const Text&) = delete;

  Text(Text&&) = delete;
  Text& operator=(Text&&) = delete;

  void SetText(const std::string& text);
  void SetFont(const Font& font);

  TTF_Text* NativeText() const { return text_; }

 private:
  TTF_Text* text_ = nullptr;
};

}  // namespace engine::platform
