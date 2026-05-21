#pragma once

#include "core/Color.h"
#include "platform/renderer/Text.h"
#include "platform/window/Window.h"

struct SDL_Renderer;
struct TTF_TextEngine;

namespace engine::platform {

class Renderer {
 public:
  Renderer(const Window& window);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void BeginFrame(const core::Color& color = core::colors::Black);
  void EndFrame();

  void DrawRect(float x, float y, float w, float h, float thickness, const core::Color& color);
  void DrawFilledRect(float x, float y, float w, float h, const core::Color& color);

  void DrawCircle(float x, float y, float radius, float thickness, const core::Color& color);
  void DrawFilledCircle(float x, float y, float radius, const core::Color& color,
                        int segments = 64);

  void DrawText(const Text& text, float x, float y, const core::Color& color);

  SDL_Renderer* NativeRenderer() const { return renderer_; }
  TTF_TextEngine* NativeTextEngine() const { return text_engine_; }

 private:
  SDL_Renderer* renderer_ = nullptr;
  TTF_TextEngine* text_engine_ = nullptr;
};

}  // namespace engine::platform
