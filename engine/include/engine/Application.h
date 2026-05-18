#pragma once

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
};

}  // namespace engine