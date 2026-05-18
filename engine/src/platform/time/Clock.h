#pragma once

#include "platform/time/TimeStep.h"

namespace engine::platform {

class Clock {
 public:
  Clock();
  ~Clock();

  Clock(const Clock&) = delete;
  Clock& operator=(const Clock&) = delete;

  Clock(Clock&&) = delete;
  Clock& operator=(Clock&&) = delete;

  TimeStep Tick();

 private:
  float last_time_seconds_ = 0.0f;
};

}  // namespace engine::platform
