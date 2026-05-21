#include "platform/renderer/Renderer.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "platform/errors/SdlError.h"

namespace engine::platform {

namespace {

void SetRenderDrawColor(SDL_Renderer* renderer, const core::Color& color) {
  CheckSdlResult(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a),
                 "SDL_SetRenderDrawColor failed");
}

SDL_FColor ToSdlFColor(const core::Color& color) {
  return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
}

}  // namespace

Renderer::Renderer(const Window& window) {
  renderer_ = SDL_CreateRenderer(window.NativeWindow(), nullptr);
  if (renderer_ == nullptr) {
    ThrowSdlError("SDL_CreateRenderer failed");
  }

  CheckSdlResult(SDL_SetRenderVSync(renderer_, 1), "SDL_SetRenderVSync failed");

  text_engine_ = TTF_CreateRendererTextEngine(renderer_);
  if (text_engine_ == nullptr) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    ThrowSdlError("TTF_CreateRendererTextEngine failed");
  }
}

Renderer::~Renderer() {
  if (text_engine_ != nullptr) {
    TTF_DestroyRendererTextEngine(text_engine_);
    text_engine_ = nullptr;
  }

  if (renderer_ != nullptr) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }
}

void Renderer::BeginFrame(const core::Color& color) {
  SetRenderDrawColor(renderer_, color);
  CheckSdlResult(SDL_RenderClear(renderer_), "SDL_RenderClear failed");
}

void Renderer::EndFrame() {
  CheckSdlResult(SDL_RenderPresent(renderer_), "SDL_RenderPresent failed");
}

void Renderer::DrawRect(float x, float y, float w, float h, float thickness,
                        const core::Color& color) {
  SDL_FRect top{x, y, w, thickness};
  SDL_FRect bottom{x, y + h - thickness, w, thickness};
  SDL_FRect left{x, y, thickness, h};
  SDL_FRect right{x + w - thickness, y, thickness, h};

  SetRenderDrawColor(renderer_, color);
  CheckSdlResult(SDL_RenderFillRect(renderer_, &top), "SDL_RenderFillRect failed");
  CheckSdlResult(SDL_RenderFillRect(renderer_, &bottom), "SDL_RenderFillRect failed");
  CheckSdlResult(SDL_RenderFillRect(renderer_, &left), "SDL_RenderFillRect failed");
  CheckSdlResult(SDL_RenderFillRect(renderer_, &right), "SDL_RenderFillRect failed");
}

void Renderer::DrawFilledRect(float x, float y, float w, float h, const core::Color& color) {
  SDL_FRect rect{x, y, w, h};

  SetRenderDrawColor(renderer_, color);
  CheckSdlResult(SDL_RenderFillRect(renderer_, &rect), "SDL_RenderFillRect failed");
}

void Renderer::DrawCircle(float x, float y, float radius, float thickness,
                          const core::Color& color) {
  const int segments = 64;  // Adjust for smoothness based on radius
  if (radius <= 0.0f || thickness <= 0.0f) {
    return;
  }

  const float inner_radius = std::max(0.0f, radius - thickness);
  const SDL_FColor sdl_color = ToSdlFColor(color);

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

  CheckSdlResult(SDL_RenderGeometry(renderer_, nullptr, vertices.data(), (int)vertices.size(),
                                    indices.data(), (int)indices.size()),
                 "SDL_RenderGeometry failed");
}

void Renderer::DrawFilledCircle(float x, float y, float radius, const core::Color& color,
                                int segments) {
  if (radius <= 0.0f || segments < 3) {
    return;
  }

  std::vector<SDL_Vertex> vertices;
  const SDL_FColor sdl_color = ToSdlFColor(color);

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

  CheckSdlResult(SDL_RenderGeometry(renderer_, nullptr, vertices.data(), (int)vertices.size(),
                                    indices.data(), (int)indices.size()),
                 "SDL_RenderGeometry failed");
}

void Renderer::DrawText(const Text& text, float x, float y, float thickness,
                        const core::Color& color) {
  int text_w = 0;
  int text_h = 0;
  TTF_Font* font = TTF_GetTextFont(text.NativeText());
  if (font == nullptr) {
    ThrowSdlError("TTF_GetTextFont failed");
  }

  CheckSdlResult(TTF_SetFontOutline(font, std::max(0, static_cast<int>(std::lround(thickness)))),
                 "TTF_SetFontOutline failed");
  CheckSdlResult(TTF_GetTextSize(text.NativeText(), &text_w, &text_h), "TTF_GetTextSize failed");

  CheckSdlResult(TTF_SetTextColor(text.NativeText(), color.r, color.g, color.b, color.a),
                 "TTF_SetTextColor failed");
  CheckSdlResult(TTF_DrawRendererText(text.NativeText(), x - text_w / 2.0f, y - text_h / 2.0f),
                 "TTF_DrawRendererText failed");
}

}  // namespace engine::platform
