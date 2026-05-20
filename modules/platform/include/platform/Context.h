#pragma once

namespace engine::platform {

class Context {
 public:
  Context();
  ~Context();

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

 private:
  bool is_initialized_ = false;
};

}  // namespace engine::platform