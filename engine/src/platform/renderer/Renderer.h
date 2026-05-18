#pragma once

#include "SDL3/SDL.h"
#include "engine/core/Color.h"
#include "platform/window/Window.h"

namespace platform {

class Renderer {
 public:
  Renderer(const Window& window);
  ~Renderer();

  void BeginFrame(const engine::Color& color = engine::colors::Black);
  void EndFrame();

  void DrawRect(float x, float y, float w, float h, float thickness, uint8_t r, uint8_t g,
                uint8_t b, uint8_t a);
  void DrawFilledRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t a);

  void DrawCircle(float x, float y, float radius, float thickness, uint8_t red, uint8_t green,
                  uint8_t blue, uint8_t alpha);
  void DrawFilledCircle(float x, float y, float radius, uint8_t red, uint8_t green, uint8_t blue,
                        uint8_t alpha, int segments);

 private:
  SDL_Renderer* renderer_;
};

}  // namespace platform