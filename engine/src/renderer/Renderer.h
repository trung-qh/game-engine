#pragma once

#include <memory>

#include "engine/ecs/World.h"
#include "platform/renderer/Renderer.h"

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
};

}  // namespace engine