#include "engine/Application.h"

#include "platform/renderer/Renderer.h"
#include "platform/window/Window.h"

namespace engine {

struct Application::PlatformContext {
  platform::Window window;
  platform::Renderer renderer;

  PlatformContext(const char* title, int width, int height, float fixed_time_step)
      : window(title, width, height), renderer(window) {}
};

Application::Application(const char* title, int width, int height, float fixed_time_step)
    : context_(std::make_unique<PlatformContext>(title, width, height, fixed_time_step)) {}

Application::~Application() = default;

void Application::Run() {
  OnInit();

  while (context_->window.PollEvents()) {
  }

  OnShutdown();
}

}  // namespace engine