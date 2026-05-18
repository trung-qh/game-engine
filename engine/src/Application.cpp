#include "engine/Application.h"

#include "SDL3/SDL.h"

namespace engine {

Application::Application(const char* title, int width, int height, float fixed_time_step) {}

Application::~Application() = default;

void Application::Run() {
  OnInit();

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }
  }

  OnShutdown();
}

}  // namespace engine