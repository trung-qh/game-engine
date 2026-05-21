#pragma once

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

#include "Components.h"
#include "engine/Engine.h"

class BoundingSystem : public engine::ISystem {
 public:
  BoundingSystem(float top_bound, float bottom_bound)
      : top_bound_(top_bound), bottom_bound_(bottom_bound) {}
  ~BoundingSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<Paddle, engine::RenderableShape, engine::Collider>()) {
      auto& shape = context.world.Get<engine::RenderableShape>(entity);
      auto& collider = context.world.Get<engine::Collider>(entity);

      auto* rect = std::get_if<engine::core::Rect>(&shape.body_);
      if (rect == nullptr) {
        continue;
      }

      if (rect->y < top_bound_) {
        engine::core::SetPosition(shape.body_, rect->x, top_bound_);
        engine::core::SetPosition(collider.body_, rect->x, top_bound_);
      } else if (rect->y + rect->h > bottom_bound_) {
        engine::core::SetPosition(shape.body_, rect->x, bottom_bound_ - rect->h);
        engine::core::SetPosition(collider.body_, rect->x, bottom_bound_ - rect->h);
      }
    }

    for (auto entity :
         context.world.View<Ball, engine::RenderableShape, engine::Collider, Velocity>()) {
      auto& shape = context.world.Get<engine::RenderableShape>(entity);
      auto& collider = context.world.Get<engine::Collider>(entity);
      auto& velocity = context.world.Get<Velocity>(entity);
      const auto* circle = std::get_if<engine::core::Circle>(&collider.body_);
      if (circle == nullptr) {
        continue;
      }

      if (circle->y - circle->r < top_bound_) {
        velocity.y_ = std::abs(velocity.y_);
        engine::core::SetPosition(shape.body_, circle->x, top_bound_ + circle->r);
        engine::core::SetPosition(collider.body_, circle->x, top_bound_ + circle->r);
      } else if (circle->y + circle->r > bottom_bound_) {
        velocity.y_ = -std::abs(velocity.y_);
        engine::core::SetPosition(shape.body_, circle->x, bottom_bound_ - circle->r);
        engine::core::SetPosition(collider.body_, circle->x, bottom_bound_ - circle->r);
      }
    }
  }

 private:
  float top_bound_ = 0.0f;
  float bottom_bound_ = 0.0f;
};

class MovementSystem : public engine::ISystem {
 public:
  MovementSystem() = default;
  ~MovementSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<engine::RenderableShape, engine::Collider, Velocity>()) {
      auto& shape = context.world.Get<engine::RenderableShape>(entity);
      auto& collider = context.world.Get<engine::Collider>(entity);
      auto& velocity = context.world.Get<Velocity>(entity);

      float dx = velocity.x_ * context.fixed_time_step;
      float dy = velocity.y_ * context.fixed_time_step;

      if (context.world.Has<PlayerController>(entity)) {
        auto& controller = context.world.Get<PlayerController>(entity);
        dx = 0.0f;
        dy = 0.0f;

        if (context.input_state.IsKeyDown(controller.up_)) {
          dy -= velocity.y_ * context.fixed_time_step;
        }
        if (context.input_state.IsKeyDown(controller.down_)) {
          dy += velocity.y_ * context.fixed_time_step;
        }
      }

      engine::core::Translate(shape.body_, dx, dy);
      engine::core::Translate(collider.body_, dx, dy);
    }
  }
};

class PaddleCollisionSystem : public engine::ISystem {
 public:
  PaddleCollisionSystem() = default;
  ~PaddleCollisionSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<Ball, engine::Collider, Velocity>()) {
      auto& collider = context.world.Get<engine::Collider>(entity);
      auto& velocity = context.world.Get<Velocity>(entity);
      const auto* ball = std::get_if<engine::core::Circle>(&collider.body_);
      if (ball == nullptr) {
        continue;
      }

      for (auto other : collider.others_) {
        if (!context.world.Has<Paddle, engine::Collider>(other)) {
          continue;
        }

        const auto& paddle_collider = context.world.Get<engine::Collider>(other);
        const auto* paddle = std::get_if<engine::core::Rect>(&paddle_collider.body_);
        if (paddle == nullptr) {
          continue;
        }

        velocity.x_ = ball->x < paddle->x ? -std::abs(velocity.x_) : std::abs(velocity.x_);
        break;
      }
    }
  }
};

class ScoringSystem : public engine::ISystem {
 public:
  ScoringSystem(float left_bound, float right_bound)
      : left_bound_(left_bound), right_bound_(right_bound) {}

  ~ScoringSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity :
         context.world.View<Ball, engine::RenderableShape, engine::Collider, Velocity>()) {
      auto& ball = context.world.Get<Ball>(entity);
      auto& shape = context.world.Get<engine::RenderableShape>(entity);
      auto& collider = context.world.Get<engine::Collider>(entity);
      auto& velocity = context.world.Get<Velocity>(entity);
      const auto* circle = std::get_if<engine::core::Circle>(&collider.body_);
      if (circle == nullptr) {
        continue;
      }

      PlayerSide scoring_side;
      if (circle->x - circle->r < left_bound_) {
        scoring_side = PlayerSide::Right;
        velocity.x_ = std::abs(velocity.x_);
      } else if (circle->x + circle->r > right_bound_) {
        scoring_side = PlayerSide::Left;
        velocity.x_ = -std::abs(velocity.x_);
      } else {
        continue;
      }

      engine::core::SetPosition(shape.body_, ball.spawn_x_, ball.spawn_y_);
      engine::core::SetPosition(collider.body_, ball.spawn_x_, ball.spawn_y_);

      for (auto player : context.world.View<Paddle, Score>()) {
        auto& paddle = context.world.Get<Paddle>(player);
        if (paddle.side_ != scoring_side) {
          continue;
        }

        auto& score = context.world.Get<Score>(player);
        ++score.value_;
        if (auto* text = context.world.TryGet<engine::RenderableText>(score.text_)) {
          text->text_ = std::to_string(score.value_);
        }
        break;
      }
    }
  }

 private:
  float left_bound_ = 0.0f;
  float right_bound_ = 0.0f;
};

class FpsSystem : public engine::ISystem {
 public:
  FpsSystem() = default;
  ~FpsSystem() = default;

  void OnUpdate(const engine::UpdateContext&) override {
    ++frames_;

    const auto now = Clock::now();
    const std::chrono::duration<float> elapsed = now - last_report_;
    if (elapsed.count() < 1.0f) {
      return;
    }

    const float fps = static_cast<float>(frames_) / elapsed.count();
    std::cout << "FPS: " << fps << '\n';

    frames_ = 0;
    last_report_ = now;
  }

 private:
  using Clock = std::chrono::steady_clock;

  int frames_ = 0;
  Clock::time_point last_report_ = Clock::now();
};
