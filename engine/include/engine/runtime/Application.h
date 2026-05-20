#pragma once

#include <memory>

#include "engine/ecs/World.h"

namespace engine {
class Renderer;

class Application {
 public:
  Application(const char* title, int width, int height, float fixed_time_step = 1.0f / 120.0f);
  virtual ~Application();

  void Run();

 protected:
  World world_;

  virtual void OnInit() = 0;
  virtual void OnShutdown() = 0;

 private:
  struct PlatformContext;
  std::unique_ptr<PlatformContext> context_;

  std::unique_ptr<Renderer> renderer_;
};

}  // namespace engine