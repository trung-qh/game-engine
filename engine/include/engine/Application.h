#pragma once

#include <memory>

namespace engine {

class Application {
 public:
  Application(const char* title, int width, int height, float fixed_time_Step = 1.0f / 120.0f);
  virtual ~Application();

  void Run();

 protected:
  virtual void OnInit() = 0;
  virtual void OnShutdown() = 0;

 private:
  struct PlatformContext;
  std::unique_ptr<PlatformContext> context_;

  float fixed_time_step_;
  float accumulator_ = 0.0f;
};

}  // namespace engine