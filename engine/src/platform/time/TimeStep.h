#pragma once

namespace engine::platform {

class TimeStep {
 public:
  TimeStep(float time_seconds) : time_seconds_(time_seconds) {}
  ~TimeStep() = default;

  float GetSeconds() const { return time_seconds_; }
  float GetMilliseconds() const { return time_seconds_ * 1000.0f; }

 private:
  float time_seconds_ = 0.0;
};

}  // namespace engine::platform
