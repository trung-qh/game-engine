#pragma once

#include <random>
#include <vector>

#include "engine/Engine.h"

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

struct GridPosition {
  int x_ = 0;
  int y_ = 0;

  bool operator==(const GridPosition&) const = default;
};

struct SnakeSegment {
  int index_ = 0;
  bool is_head_ = false;
};

struct Food {};

struct SnakeGame {
  std::vector<engine::entity::Entity> segments_;
  engine::entity::Entity food_;
  engine::entity::Entity score_text_;
  engine::entity::Entity status_text_;

  int columns_ = 0;
  int rows_ = 0;
  int score_ = 0;

  Direction direction_ = Direction::Right;
  Direction next_direction_ = Direction::Right;

  float step_interval_ = 0.12f;
  float step_timer_ = 0.0f;

  bool game_over_ = false;

  std::mt19937 rng_{std::random_device{}()};

  SnakeGame(std::vector<engine::entity::Entity> segments, engine::entity::Entity food,
            engine::entity::Entity score_text, engine::entity::Entity status_text, int columns,
            int rows, float step_interval)
      : segments_(std::move(segments)),
        food_(food),
        score_text_(score_text),
        status_text_(status_text),
        columns_(columns),
        rows_(rows),
        step_interval_(step_interval) {}
};
