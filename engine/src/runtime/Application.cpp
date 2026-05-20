#include "engine/runtime/Application.h"

#include "platform/Context.h"
#include "platform/renderer/Renderer.h"
#include "platform/time/Clock.h"
#include "platform/window/Window.h"
#include "renderer/Renderer.h"

namespace engine {

struct Application::PlatformContext {
  platform::Context context;
  platform::Window window;
  platform::Renderer renderer;
  platform::Clock clock;

  core::InputState input_state;

  PlatformContext(const char* title, int width, int height)
      : window(title, width, height), renderer(window) {}
};

Application::Application(const char* title, int width, int height, float fixed_time_step)
    : context_(std::make_unique<PlatformContext>(title, width, height)),
      renderer_(std::make_unique<Renderer>(context_->renderer)),
      world_(fixed_time_step) {}

Application::~Application() = default;

void Application::Run() {
  OnInit();

  while (context_->window.PollEvents(context_->input_state)) {
    float delta_time = context_->clock.Tick().GetSeconds();
    world_.Update(context_->input_state, delta_time);

    context_->renderer.BeginFrame();
    renderer_->Render(world_);
    context_->renderer.EndFrame();
  }

  OnShutdown();
}

}  // namespace engine
