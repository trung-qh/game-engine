#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

#include "engine/Engine.h"
#include "engine/runtime/EntryPoint.h"

constexpr static const char* kTitle = "Breakout";
constexpr static int kWindowWidth = 900;
constexpr static int kWindowHeight = 600;
constexpr static float kFixedTimeStep = 1.0f / 120.0f;

constexpr static float kPaddleWidth = 120.0f;
constexpr static float kPaddleHeight = 16.0f;
constexpr static float kPaddleY = 550.0f;
constexpr static float kPaddleSpeed = 700.0f;

constexpr static float kBallRadius = 8.0f;
constexpr static float kBallSpeedX = 320.0f;
constexpr static float kBallSpeedY = -420.0f;

constexpr static int kBrickRows = 5;
constexpr static int kBrickColumns = 10;
constexpr static float kBrickWidth = 72.0f;
constexpr static float kBrickHeight = 24.0f;
constexpr static float kBrickGap = 8.0f;
constexpr static float kBrickStartX = 51.0f;
constexpr static float kBrickStartY = 90.0f;

struct Velocity {
  float x = 0.0f;
  float y = 0.0f;
};

struct Paddle {};

struct Ball {
  float spawn_x = 0.0f;
  float spawn_y = 0.0f;
};

struct Brick {
  bool broken = false;
};

struct Score {
  int value = 0;
  engine::entity::Entity text;

  explicit Score(engine::entity::Entity score_text) : text(score_text) {}
};

struct GameState {
  bool launched = false;
  engine::entity::Entity score;
  engine::entity::Entity paddle;

  GameState(engine::entity::Entity score_entity, engine::entity::Entity paddle_entity)
      : score(score_entity), paddle(paddle_entity) {}
};

namespace {

engine::core::Rect* RectOf(engine::RenderableShape& renderable) {
  return std::get_if<engine::core::Rect>(&renderable.body_);
}

const engine::core::Rect* RectOf(const engine::RenderableShape& renderable) {
  return std::get_if<engine::core::Rect>(&renderable.body_);
}

engine::core::Circle* CircleOf(engine::RenderableShape& renderable) {
  return std::get_if<engine::core::Circle>(&renderable.body_);
}

const engine::core::Circle* CircleOf(const engine::RenderableShape& renderable) {
  return std::get_if<engine::core::Circle>(&renderable.body_);
}

void SetEntityPosition(engine::World& world, engine::entity::Entity entity, float x, float y) {
  auto& renderable = world.Get<engine::RenderableShape>(entity);
  auto& collider = world.Get<engine::Collider>(entity);

  engine::core::SetPosition(renderable.body_, x, y);
  engine::core::SetPosition(collider.body_, x, y);
}

void TranslateEntity(engine::World& world, engine::entity::Entity entity, float dx, float dy) {
  auto& renderable = world.Get<engine::RenderableShape>(entity);
  auto& collider = world.Get<engine::Collider>(entity);

  engine::core::Translate(renderable.body_, dx, dy);
  engine::core::Translate(collider.body_, dx, dy);
}

void ResetBall(engine::World& world, engine::entity::Entity entity) {
  auto& ball = world.Get<Ball>(entity);
  auto& velocity = world.Get<Velocity>(entity);

  SetEntityPosition(world, entity, ball.spawn_x, ball.spawn_y);
  velocity.x = kBallSpeedX;
  velocity.y = kBallSpeedY;
}

void ResetGame(engine::World& world, engine::entity::Entity game_entity) {
  auto* game_state = world.TryGet<GameState>(game_entity);
  if (game_state == nullptr) {
    return;
  }

  game_state->launched = false;

  if (auto* score = world.TryGet<Score>(game_state->score)) {
    score->value = 0;
    if (auto* text = world.TryGet<engine::RenderableText>(score->text)) {
      text->text_ = "0";
    }
  }

  if (world.Has<Paddle, engine::RenderableShape, engine::Collider>(game_state->paddle)) {
    SetEntityPosition(world, game_state->paddle, kWindowWidth / 2.0f - kPaddleWidth / 2.0f,
                      kPaddleY);
  }

  for (auto ball_entity : world.View<Ball, Velocity, engine::RenderableShape, engine::Collider>()) {
    ResetBall(world, ball_entity);
  }

  for (auto brick_entity : world.View<Brick, engine::RenderableShape>()) {
    auto& brick = world.Get<Brick>(brick_entity);
    auto& renderable = world.Get<engine::RenderableShape>(brick_entity);

    brick.broken = false;
    renderable.visible_ = true;
  }
}

bool IsLaunchInputDown(const engine::core::InputState& input_state) {
  return input_state.IsKeyDown(engine::core::KeyCode::Left) ||
         input_state.IsKeyDown(engine::core::KeyCode::Right) ||
         input_state.IsKeyDown(engine::core::KeyCode::A) ||
         input_state.IsKeyDown(engine::core::KeyCode::D) ||
         input_state.IsKeyDown(engine::core::KeyCode::Space);
}

float HorizontalHitBias(const engine::core::Circle& ball, const engine::core::Rect& paddle) {
  const float paddle_center = paddle.x + paddle.w / 2.0f;
  return std::clamp((ball.x - paddle_center) / (paddle.w / 2.0f), -1.0f, 1.0f);
}

void BounceOffRect(const engine::core::Circle& ball, const engine::core::Rect& rect,
                   Velocity& velocity) {
  const float overlap_left = (ball.x + ball.r) - rect.x;
  const float overlap_right = (rect.x + rect.w) - (ball.x - ball.r);
  const float overlap_top = (ball.y + ball.r) - rect.y;
  const float overlap_bottom = (rect.y + rect.h) - (ball.y - ball.r);

  const float min_horizontal = std::min(overlap_left, overlap_right);
  const float min_vertical = std::min(overlap_top, overlap_bottom);
  if (min_horizontal < min_vertical) {
    velocity.x = overlap_left < overlap_right ? -std::abs(velocity.x) : std::abs(velocity.x);
  } else {
    velocity.y = overlap_top < overlap_bottom ? -std::abs(velocity.y) : std::abs(velocity.y);
  }
}

}  // namespace

class PaddleControlSystem : public engine::ISystem {
 public:
  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<Paddle, engine::RenderableShape, engine::Collider>()) {
      float dx = 0.0f;
      if (context.input_state.IsKeyDown(engine::core::KeyCode::Left) ||
          context.input_state.IsKeyDown(engine::core::KeyCode::A)) {
        dx -= kPaddleSpeed * context.fixed_time_step;
      }
      if (context.input_state.IsKeyDown(engine::core::KeyCode::Right) ||
          context.input_state.IsKeyDown(engine::core::KeyCode::D)) {
        dx += kPaddleSpeed * context.fixed_time_step;
      }

      TranslateEntity(context.world, entity, dx, 0.0f);

      auto& renderable = context.world.Get<engine::RenderableShape>(entity);
      if (const auto* paddle = RectOf(renderable)) {
        const float clamped_x = std::clamp(paddle->x, 0.0f, kWindowWidth - paddle->w);
        SetEntityPosition(context.world, entity, clamped_x, paddle->y);
      }
    }
  }
};

class LaunchSystem : public engine::ISystem {
 public:
  explicit LaunchSystem(engine::entity::Entity game_entity) : game_entity_(game_entity) {}

  void OnUpdate(const engine::UpdateContext& context) override {
    auto* game_state = context.world.TryGet<GameState>(game_entity_);
    if (game_state == nullptr || game_state->launched || !IsLaunchInputDown(context.input_state)) {
      return;
    }

    game_state->launched = true;
  }

 private:
  engine::entity::Entity game_entity_;
};

class BallMovementSystem : public engine::ISystem {
 public:
  explicit BallMovementSystem(engine::entity::Entity game_entity) : game_entity_(game_entity) {}

  void OnUpdate(const engine::UpdateContext& context) override {
    const auto* game_state = context.world.TryGet<GameState>(game_entity_);
    if (game_state == nullptr || !game_state->launched) {
      return;
    }

    for (auto entity : context.world.View<Ball, Velocity, engine::RenderableShape, engine::Collider>()) {
      const auto& velocity = context.world.Get<Velocity>(entity);
      TranslateEntity(context.world, entity, velocity.x * context.fixed_time_step,
                      velocity.y * context.fixed_time_step);
    }
  }

 private:
  engine::entity::Entity game_entity_;
};

class WallBounceSystem : public engine::ISystem {
 public:
  explicit WallBounceSystem(engine::entity::Entity game_entity) : game_entity_(game_entity) {}

  void OnUpdate(const engine::UpdateContext& context) override {
    const auto* game_state = context.world.TryGet<GameState>(game_entity_);
    if (game_state == nullptr || !game_state->launched) {
      return;
    }

    for (auto entity : context.world.View<Ball, Velocity, engine::RenderableShape, engine::Collider>()) {
      auto& renderable = context.world.Get<engine::RenderableShape>(entity);
      auto& velocity = context.world.Get<Velocity>(entity);
      const auto* ball = CircleOf(renderable);
      if (ball == nullptr) {
        continue;
      }

      if (ball->x - ball->r < 0.0f) {
        velocity.x = std::abs(velocity.x);
        SetEntityPosition(context.world, entity, ball->r, ball->y);
      } else if (ball->x + ball->r > kWindowWidth) {
        velocity.x = -std::abs(velocity.x);
        SetEntityPosition(context.world, entity, kWindowWidth - ball->r, ball->y);
      }

      if (ball->y - ball->r < 0.0f) {
        velocity.y = std::abs(velocity.y);
        SetEntityPosition(context.world, entity, ball->x, ball->r);
      } else if (ball->y - ball->r > kWindowHeight) {
        ResetGame(context.world, game_entity_);
      }
    }
  }

 private:
  engine::entity::Entity game_entity_;
};

class PaddleBounceSystem : public engine::ISystem {
 public:
  explicit PaddleBounceSystem(engine::entity::Entity game_entity) : game_entity_(game_entity) {}

  void OnUpdate(const engine::UpdateContext& context) override {
    const auto* game_state = context.world.TryGet<GameState>(game_entity_);
    if (game_state == nullptr || !game_state->launched) {
      return;
    }

    for (auto ball_entity :
         context.world.View<Ball, Velocity, engine::RenderableShape, engine::Collider>()) {
      auto& ball_renderable = context.world.Get<engine::RenderableShape>(ball_entity);
      auto& velocity = context.world.Get<Velocity>(ball_entity);
      const auto* ball = CircleOf(ball_renderable);
      if (ball == nullptr || velocity.y <= 0.0f) {
        continue;
      }

      for (auto paddle_entity : context.world.View<Paddle, engine::RenderableShape>()) {
        const auto& paddle_renderable = context.world.Get<engine::RenderableShape>(paddle_entity);
        const auto* paddle = RectOf(paddle_renderable);
        if (paddle == nullptr || !engine::core::Intersecting(*paddle, *ball)) {
          continue;
        }

        const float bias = HorizontalHitBias(*ball, *paddle);
        velocity.x = bias * kPaddleSpeed * 0.75f;
        velocity.y = -std::abs(velocity.y);
        SetEntityPosition(context.world, ball_entity, ball->x, paddle->y - ball->r);
        break;
      }
    }
  }

 private:
  engine::entity::Entity game_entity_;
};

class BrickCollisionSystem : public engine::ISystem {
 public:
  BrickCollisionSystem(engine::entity::Entity game_entity, engine::entity::Entity score_entity)
      : game_entity_(game_entity), score_entity_(score_entity) {}

  void OnUpdate(const engine::UpdateContext& context) override {
    const auto* game_state = context.world.TryGet<GameState>(game_entity_);
    if (game_state == nullptr || !game_state->launched) {
      return;
    }

    auto* score = context.world.TryGet<Score>(score_entity_);
    if (score == nullptr) {
      return;
    }

    for (auto ball_entity :
         context.world.View<Ball, Velocity, engine::RenderableShape, engine::Collider>()) {
      auto& ball_renderable = context.world.Get<engine::RenderableShape>(ball_entity);
      auto& velocity = context.world.Get<Velocity>(ball_entity);
      const auto* ball = CircleOf(ball_renderable);
      if (ball == nullptr) {
        continue;
      }

      for (auto brick_entity : context.world.View<Brick, engine::RenderableShape>()) {
        auto& brick = context.world.Get<Brick>(brick_entity);
        if (brick.broken) {
          continue;
        }

        auto& brick_renderable = context.world.Get<engine::RenderableShape>(brick_entity);
        const auto* brick_rect = RectOf(brick_renderable);
        if (brick_rect == nullptr || !engine::core::Intersecting(*brick_rect, *ball)) {
          continue;
        }

        brick.broken = true;
        brick_renderable.visible_ = false;
        BounceOffRect(*ball, *brick_rect, velocity);
        ++score->value;

        if (auto* text = context.world.TryGet<engine::RenderableText>(score->text)) {
          text->text_ = std::to_string(score->value);
        }
        break;
      }
    }
  }

 private:
  engine::entity::Entity game_entity_;
  engine::entity::Entity score_entity_;
};

class Breakout : public engine::Application {
 public:
  Breakout() : engine::Application(kTitle, kWindowWidth, kWindowHeight, kFixedTimeStep) {}
  ~Breakout() override = default;

 protected:
  void OnInit() override {
    auto score_text = world_.CreateEntity();
    world_.AddComponent<engine::RenderableText>(score_text, kWindowWidth / 2.0f, 34.0f, "0",
                                                font_.get());

    auto score = world_.CreateEntity();
    world_.AddComponent<Score>(score, score_text);

    const engine::core::Rect paddle_body{kWindowWidth / 2.0f - kPaddleWidth / 2.0f, kPaddleY,
                                         kPaddleWidth, kPaddleHeight};
    auto paddle = world_.CreateEntity();
    world_.AddComponent<engine::RenderableShape>(paddle, paddle_body,
                                                 engine::core::colors::White);
    world_.AddComponent<engine::Collider>(paddle, paddle_body);
    world_.AddComponent<Paddle>(paddle);

    auto game_state = world_.CreateEntity();
    world_.AddComponent<GameState>(game_state, score, paddle);

    const engine::core::Circle ball_body{kWindowWidth / 2.0f, kPaddleY - kBallRadius - 2.0f,
                                         kBallRadius};
    auto ball = world_.CreateEntity();
    world_.AddComponent<engine::RenderableShape>(ball, ball_body, engine::core::colors::White);
    world_.AddComponent<engine::Collider>(ball, ball_body);
    world_.AddComponent<Velocity>(ball, kBallSpeedX, kBallSpeedY);
    world_.AddComponent<Ball>(ball, ball_body.x, ball_body.y);

    for (int row = 0; row < kBrickRows; ++row) {
      for (int column = 0; column < kBrickColumns; ++column) {
        const engine::core::Rect brick_body{
            kBrickStartX + column * (kBrickWidth + kBrickGap),
            kBrickStartY + row * (kBrickHeight + kBrickGap), kBrickWidth, kBrickHeight};

        auto brick = world_.CreateEntity();
        const auto color = row % 2 == 0 ? engine::core::colors::Gray : engine::core::colors::White;
        world_.AddComponent<engine::RenderableShape>(brick, brick_body, color);
        world_.AddComponent<engine::Collider>(brick, brick_body);
        world_.AddComponent<Brick>(brick);
      }
    }

    world_.AddSystem<PaddleControlSystem>();
    world_.AddSystem<LaunchSystem>(game_state);
    world_.AddSystem<BallMovementSystem>(game_state);
    world_.AddSystem<WallBounceSystem>(game_state);
    world_.AddSystem<PaddleBounceSystem>(game_state);
    world_.AddSystem<BrickCollisionSystem>(game_state, score);
  }

  void OnShutdown() override { font_.reset(); }

 private:
  std::unique_ptr<engine::core::Font> font_ =
      std::make_unique<engine::core::Font>(BREAKOUT_RES_DIR "/fonts/Arial.ttf", 32);
};

std::unique_ptr<engine::Application> CreateApplication() {
  return std::make_unique<Breakout>();
}
