#pragma once

#include "platform/time/TimeStep.h"

namespace platform {

class Clock {
 public:
  Clock();
  ~Clock();

  TimeStep Tick();

 private:
  float last_time_seconds_ = 0.0f;
};

}  // namespace platform
