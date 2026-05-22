#include <memory>

#include "GameObjects.h"
#include "Systems.h"
#include "engine/Engine.h"
#include "engine/runtime/EntryPoint.h"

constexpr static const char* kTitle = "Snake";
constexpr static int kWindowWidth = 720;
constexpr static int kWindowHeight = 720;
constexpr static float kFixedTimeStep = 1.0f / 120.0f;

constexpr static int kColumns = 24;
constexpr static int kRows = 24;
constexpr static float kCellSize = 24.0f;
constexpr static float kBoardOriginX = (kWindowWidth - kColumns * kCellSize) / 2.0f;
constexpr static float kBoardOriginY = 96.0f;
constexpr static float kStepInterval = 0.11f;

class Snake : public engine::Application {
 public:
  Snake() : engine::Application(kTitle, kWindowWidth, kWindowHeight, kFixedTimeStep) {}
  ~Snake() = default;

 protected:
  void OnInit() override {
    snake::CreateSnakeGame(world_, font_.get(), kColumns, kRows, kBoardOriginX, kBoardOriginY,
                           kCellSize, kStepInterval);

    world_.AddSystem<SnakeControlSystem>();
    world_.AddSystem<SnakeMovementSystem>(kBoardOriginX, kBoardOriginY, kCellSize);
  }

  void OnShutdown() override { font_.reset(); }

 private:
  std::unique_ptr<engine::core::Font> font_ =
      std::make_unique<engine::core::Font>(SNAKE_RES_DIR "/fonts/Arial.ttf", 28);
};

std::unique_ptr<engine::Application> CreateApplication() { return std::make_unique<Snake>(); }
