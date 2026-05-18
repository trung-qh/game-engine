#include "engine/Application.h"

#include "platform/renderer/Renderer.h"
#include "platform/time/Clock.h"
#include "platform/window/Window.h"

namespace engine {

struct Application::PlatformContext {
  platform::Window window;
  platform::Renderer renderer;
  platform::Clock clock;

  PlatformContext(const char* title, int width, int height)
      : window(title, width, height), renderer(window) {}
};

Application::Application(const char* title, int width, int height, float fixed_time_step)
    : context_(std::make_unique<PlatformContext>(title, width, height)),
      fixed_time_step_(fixed_time_step) {}

Application::~Application() = default;

void Application::Run() {
  OnInit();

  while (context_->window.PollEvents()) {
    float delta_time = context_->clock.Tick().GetSeconds();
    accumulator_ += delta_time;

    while (accumulator_ >= fixed_time_step_) {
      // Update world state
      accumulator_ -= fixed_time_step_;
    }

    context_->renderer.BeginFrame();
    // Render world
    context_->renderer.EndFrame();
  }

  OnShutdown();
}

}  // namespace engine