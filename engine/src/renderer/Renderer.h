#pragma once

#include <memory>

#include "engine/ecs/World.h"
#include "platform/renderer/Renderer.h"
#include "renderer/TextManager.h"

namespace engine {

class Renderer {
 public:
  Renderer(platform::Renderer& renderer);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  void Render(World& world);

 private:
  platform::Renderer& native_renderer_;

  TextManager text_manager_;
};

}  // namespace engine