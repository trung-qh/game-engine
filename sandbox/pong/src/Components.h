#pragma once

#include "engine/Engine.h"

enum class PlayerSide {
  Left,
  Right,
};

struct Velocity {
  float x_ = 0.0f;
  float y_ = 0.0f;
};

struct Ball {
  float spawn_x_ = 0.0f;
  float spawn_y_ = 0.0f;
};

struct Paddle {
  PlayerSide side_ = PlayerSide::Left;
};

struct PlayerController {
  engine::core::Key up_;
  engine::core::Key down_;

  PlayerController(engine::core::Key up, engine::core::Key down) : up_(up), down_(down) {}
};

struct Score {
  int value_ = 0;
  engine::entity::Entity text_;

  explicit Score(engine::entity::Entity text) : text_(text) {}
};
