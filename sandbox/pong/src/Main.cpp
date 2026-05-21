#include <memory>

#include "GameObjects.h"
#include "Systems.h"
#include "engine/Engine.h"
#include "engine/runtime/EntryPoint.h"

constexpr static const char* kTitle = "Pong";
constexpr static int kWindowWidth = 900;
constexpr static int kWindowHeight = 600;
constexpr static float kFixedTimeStep = 1.0f / 120.0f;

constexpr static float kPaddleMargin = 30.0f;
constexpr static float kPaddleWidth = 10.0f;
constexpr static float kPaddleHeight = 120.0f;
constexpr static float kPaddleSpeed = 800.0f;

constexpr static float kBallRadius = 10.0f;
constexpr static float kBallSpeed = 600.0f;

constexpr static float kDividerWidth = 3.0f;

class Pong : public engine::Application {
 public:
  Pong() : engine::Application(kTitle, kWindowWidth, kWindowHeight, kFixedTimeStep) {}
  ~Pong() = default;

 protected:
  void OnInit() override {
    constexpr float kPaddleY = kWindowHeight / 2 - kPaddleHeight / 2;
    constexpr float kBallX = kWindowWidth / 2;
    constexpr float kBallY = kWindowHeight / 2;

    CreatePaddle(world_, PlayerSide::Left,
                 engine::core::Rect{kPaddleMargin, kPaddleY, kPaddleWidth, kPaddleHeight},
                 kPaddleSpeed, engine::core::KeyCode::W, engine::core::KeyCode::S, font_.get(),
                 kWindowWidth / 4, kPaddleMargin * 2);
    CreatePaddle(world_, PlayerSide::Right,
                 engine::core::Rect{kWindowWidth - kPaddleMargin - kPaddleWidth, kPaddleY,
                                    kPaddleWidth, kPaddleHeight},
                 kPaddleSpeed, engine::core::KeyCode::Up, engine::core::KeyCode::Down, font_.get(),
                 kWindowWidth / 4 * 3, kPaddleMargin * 2);

    CreateDivider(world_, engine::core::Rect{kWindowWidth / 2 - kDividerWidth / 2, 0.0f,
                                             kDividerWidth, kWindowHeight});
    CreateBall(world_, engine::core::Circle{kBallX, kBallY, kBallRadius}, kBallSpeed);

    world_.AddSystem<FpsSystem>();
    world_.AddSystem<PaddleCollisionSystem>();
    world_.AddSystem<MovementSystem>();
    world_.AddSystem<ScoringSystem>(0.0f, kWindowWidth);
    world_.AddSystem<BoundingSystem>(0.0f, kWindowHeight);
  }

  void OnShutdown() override { font_.reset(); }

 private:
  std::unique_ptr<engine::core::Font> font_ =
      std::make_unique<engine::core::Font>(PONG_RES_DIR "/fonts/Arial.ttf", 32);
};

std::unique_ptr<engine::Application> CreateApplication() { return std::make_unique<Pong>(); }
