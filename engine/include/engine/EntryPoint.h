#pragma once

#include <memory>

#include "engine/Application.h"

extern std::unique_ptr<engine::Application> CreateApplication();

int main() {
  auto app = CreateApplication();
  app->Run();

  return 0;
}