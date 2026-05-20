#pragma once

#include <cstdint>

#include "SDL3/SDL.h"
#include "engine/core/Core.h"
#include "platform/window/Window.h"

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

  void DrawRect(float x, float y, float w, float h, float thickness, uint8_t r, uint8_t g,
                uint8_t b, uint8_t a);
  void DrawFilledRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t a);

  void DrawCircle(float x, float y, float radius, float thickness, uint8_t red, uint8_t green,
                  uint8_t blue, uint8_t alpha);
  void DrawFilledCircle(float x, float y, float radius, uint8_t red, uint8_t green, uint8_t blue,
                        uint8_t alpha, int segments);

 private:
  SDL_Renderer* renderer_ = nullptr;
};

}  // namespace engine::platform
