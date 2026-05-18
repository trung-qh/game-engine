#include "platform/time/Clock.h"

#include <SDL3/SDL.h>

namespace platform {

namespace {

double GetCurrentTimeSeconds() {
  double ticks = static_cast<double>(SDL_GetPerformanceCounter());
  double frequency = static_cast<double>(SDL_GetPerformanceFrequency());

  return ticks / frequency;
}

}  // namespace

Clock::Clock() : last_time_seconds_(GetCurrentTimeSeconds()) {}

Clock::~Clock() {}

TimeStep Clock::Tick() {
  double current_time_seconds = GetCurrentTimeSeconds();
  double delta_time_seconds = current_time_seconds - last_time_seconds_;

  last_time_seconds_ = current_time_seconds;

  return TimeStep(delta_time_seconds);
}

}  // namespace platform
