#include "platform/renderer/Renderer.h"

#include <stdexcept>
#include <vector>

namespace engine::platform {

Renderer::Renderer(const Window& window) {
  renderer_ = SDL_CreateRenderer(window.NativeWindow(), nullptr);
  if (renderer_ == nullptr) {
    throw std::runtime_error("Failed to create SDL Renderer");
  }
}

Renderer::~Renderer() {
  if (renderer_ != nullptr) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }
}

void Renderer::BeginFrame(const core::Color& color) {
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer_);
}

void Renderer::EndFrame() { SDL_RenderPresent(renderer_); }

void Renderer::DrawRect(float x, float y, float w, float h, float thickness, uint8_t r, uint8_t g,
                        uint8_t b, uint8_t a) {
  SDL_FRect top{x, y, w, thickness};
  SDL_FRect bottom{x, y + h - thickness, w, thickness};
  SDL_FRect left{x, y, thickness, h};
  SDL_FRect right{x + w - thickness, y, thickness, h};

  SDL_SetRenderDrawColor(renderer_, r, g, b, a);
  SDL_RenderFillRect(renderer_, &top);
  SDL_RenderFillRect(renderer_, &bottom);
  SDL_RenderFillRect(renderer_, &left);
  SDL_RenderFillRect(renderer_, &right);
}

void Renderer::DrawFilledRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b,
                              uint8_t a) {
  SDL_FRect rect{x, y, w, h};

  SDL_SetRenderDrawColor(renderer_, r, g, b, a);
  SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::DrawCircle(float x, float y, float radius, float thickness, uint8_t red,
                          uint8_t green, uint8_t blue, uint8_t alpha) {
  const int segments = 64;  // Adjust for smoothness based on radius
  if (radius <= 0.0f || thickness <= 0.0f) {
    return;
  }

  const float inner_radius = std::max(0.0f, radius - thickness);
  const SDL_FColor sdl_color = {red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f};

  std::vector<SDL_Vertex> vertices;
  std::vector<int> indices;

  for (int i = 0; i <= segments; ++i) {
    float theta = 2.0f * 3.14159265f * float(i) / float(segments);
    float cos_t = cosf(theta);
    float sin_t = sinf(theta);

    // Outer vertex
    vertices.push_back({{x + radius * cos_t, y + radius * sin_t}, sdl_color, {0, 0}});
    // Inner vertex
    vertices.push_back({{x + inner_radius * cos_t, y + inner_radius * sin_t}, sdl_color, {0, 0}});
  }

  // Create triangles (Bridge the outer and inner loops)
  for (int i = 0; i < segments; ++i) {
    int outer = i * 2;
    int inner = i * 2 + 1;
    int next_outer = (i + 1) * 2;
    int next_inner = (i + 1) * 2 + 1;

    // Triangle 1
    indices.push_back(outer);
    indices.push_back(inner);
    indices.push_back(next_outer);

    // Triangle 2
    indices.push_back(inner);
    indices.push_back(next_inner);
    indices.push_back(next_outer);
  }

  SDL_RenderGeometry(renderer_, nullptr, vertices.data(), (int)vertices.size(), indices.data(),
                     (int)indices.size());
}

void Renderer::DrawFilledCircle(float x, float y, float radius, uint8_t red, uint8_t green,
                                uint8_t blue, uint8_t alpha, int segments) {
  if (radius <= 0.0f || segments < 3) {
    return;
  }

  std::vector<SDL_Vertex> vertices;
  const SDL_FColor sdl_color = {red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f};

  // Center vertex
  SDL_Vertex center = {{x, y}, sdl_color, {0, 0}};
  vertices.push_back(center);

  for (int i = 0; i <= segments; i++) {
    float theta = 2.0f * 3.1415926f * (float)i / (float)segments;
    float x_ = x + radius * static_cast<float>(std::cos(theta));
    float y_ = y + radius * static_cast<float>(std::sin(theta));

    vertices.push_back({{x_, y_}, sdl_color, {0, 0}});
  }

  // SDL_RenderGeometry creates a triangle fan if you provide vertices in order
  // In SDL3, we use indices to define the triangles explicitly
  std::vector<int> indices;
  for (int i = 1; i <= segments; i++) {
    indices.push_back(0);      // Center
    indices.push_back(i);      // Current point
    indices.push_back(i + 1);  // Next point
  }

  SDL_RenderGeometry(renderer_, nullptr, vertices.data(), (int)vertices.size(), indices.data(),
                     (int)indices.size());
}

}  // namespace engine::platform
