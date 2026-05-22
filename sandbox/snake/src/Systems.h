#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "Components.h"
#include "GameObjects.h"
#include "engine/Engine.h"

class SnakeControlSystem : public engine::ISystem {
 public:
  SnakeControlSystem() = default;
  ~SnakeControlSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<SnakeGame>()) {
      auto& game = context.world.Get<SnakeGame>(entity);

      if (context.input_state.IsKeyPressed(engine::core::KeyCode::Up) ||
          context.input_state.IsKeyPressed(engine::core::KeyCode::W)) {
        QueueDirection(game, Direction::Up);
      } else if (context.input_state.IsKeyPressed(engine::core::KeyCode::Down) ||
                 context.input_state.IsKeyPressed(engine::core::KeyCode::S)) {
        QueueDirection(game, Direction::Down);
      } else if (context.input_state.IsKeyPressed(engine::core::KeyCode::Left) ||
                 context.input_state.IsKeyPressed(engine::core::KeyCode::A)) {
        QueueDirection(game, Direction::Left);
      } else if (context.input_state.IsKeyPressed(engine::core::KeyCode::Right) ||
                 context.input_state.IsKeyPressed(engine::core::KeyCode::D)) {
        QueueDirection(game, Direction::Right);
      }
    }
  }

 private:
  static bool Opposite(Direction a, Direction b) {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
  }

  static void QueueDirection(SnakeGame& game, Direction direction) {
    if (game.segments_.size() > 1 && Opposite(game.direction_, direction)) {
      return;
    }
    game.next_direction_ = direction;
  }
};

class SnakeMovementSystem : public engine::ISystem {
 public:
  SnakeMovementSystem(float origin_x, float origin_y, float cell_size)
      : origin_x_(origin_x), origin_y_(origin_y), cell_size_(cell_size) {}

  ~SnakeMovementSystem() = default;

  void OnUpdate(const engine::UpdateContext& context) override {
    for (auto entity : context.world.View<SnakeGame>()) {
      auto& game = context.world.Get<SnakeGame>(entity);

      if (game.game_over_) {
        if (context.input_state.IsKeyPressed(engine::core::KeyCode::Space)) {
          ResetGame(context.world, game);
        }
        continue;
      }

      game.step_timer_ += context.fixed_time_step;
      if (game.step_timer_ < game.step_interval_) {
        continue;
      }
      game.step_timer_ = 0.0f;

      Step(context.world, game);
    }
  }

 private:
  GridPosition NextPosition(GridPosition position, Direction direction) const {
    switch (direction) {
      case Direction::Up:
        --position.y_;
        break;
      case Direction::Down:
        ++position.y_;
        break;
      case Direction::Left:
        --position.x_;
        break;
      case Direction::Right:
        ++position.x_;
        break;
    }
    return position;
  }

  bool IsOccupied(engine::World& world, const SnakeGame& game, GridPosition position,
                  bool ignore_tail) const {
    const size_t size = game.segments_.size();
    const size_t limit = ignore_tail && size > 0 ? size - 1 : size;
    for (size_t i = 0; i < limit; ++i) {
      const auto* segment_position = world.TryGet<GridPosition>(game.segments_[i]);
      if (segment_position != nullptr && *segment_position == position) {
        return true;
      }
    }
    return false;
  }

  GridPosition RandomFreeCell(engine::World& world, SnakeGame& game,
                              GridPosition pending_segment_position) const {
    std::vector<GridPosition> candidates;
    candidates.reserve(static_cast<size_t>(game.columns_ * game.rows_));

    for (int y = 0; y < game.rows_; ++y) {
      for (int x = 0; x < game.columns_; ++x) {
        GridPosition position{x, y};
        if (position != pending_segment_position && !IsOccupied(world, game, position, false)) {
          candidates.emplace_back(position);
        }
      }
    }

    if (candidates.empty()) {
      return {0, 0};
    }

    std::uniform_int_distribution<size_t> distribution(0, candidates.size() - 1);
    return candidates[distribution(game.rng_)];
  }

  void UpdateScoreText(engine::World& world, const SnakeGame& game) const {
    if (auto* text = world.TryGet<engine::RenderableText>(game.score_text_)) {
      text->text_ = "Score: " + std::to_string(game.score_);
    }
  }

  void UpdateStatusText(engine::World& world, const SnakeGame& game) const {
    if (auto* text = world.TryGet<engine::RenderableText>(game.status_text_)) {
      text->text_ = game.game_over_ ? "Game over - Space to restart" : "Arrow keys or WASD";
      text->color_ = game.game_over_ ? snake::kFoodColor : snake::kMutedTextColor;
    }
  }

  void MoveSegmentVisual(engine::World& world, engine::entity::Entity entity,
                         GridPosition position) const {
    if (auto* grid_position = world.TryGet<GridPosition>(entity)) {
      *grid_position = position;
    }
    if (auto* shape = world.TryGet<engine::RenderableShape>(entity)) {
      snake::MoveShapeToCell(*shape, position, origin_x_, origin_y_, cell_size_);
    }
  }

  void Step(engine::World& world, SnakeGame& game) const {
    if (game.segments_.empty()) {
      return;
    }

    game.direction_ = game.next_direction_;

    const auto* head_position = world.TryGet<GridPosition>(game.segments_.front());
    if (head_position == nullptr) {
      return;
    }

    const auto next_head = NextPosition(*head_position, game.direction_);
    const auto* food_position = world.TryGet<GridPosition>(game.food_);
    const bool ate_food = food_position != nullptr && *food_position == next_head;

    if (next_head.x_ < 0 || next_head.y_ < 0 || next_head.x_ >= game.columns_ ||
        next_head.y_ >= game.rows_ || IsOccupied(world, game, next_head, !ate_food)) {
      game.game_over_ = true;
      UpdateStatusText(world, game);
      return;
    }

    std::vector<GridPosition> old_positions;
    old_positions.reserve(game.segments_.size());
    for (auto segment : game.segments_) {
      if (auto* position = world.TryGet<GridPosition>(segment)) {
        old_positions.emplace_back(*position);
      }
    }

    MoveSegmentVisual(world, game.segments_.front(), next_head);
    for (size_t i = 1; i < game.segments_.size() && i <= old_positions.size(); ++i) {
      MoveSegmentVisual(world, game.segments_[i], old_positions[i - 1]);
    }

    if (ate_food) {
      const GridPosition tail_position = old_positions.empty() ? next_head : old_positions.back();
      const int index = static_cast<int>(game.segments_.size());
      game.segments_.emplace_back(
          snake::CreateSnakeSegment(world, tail_position, index, false, origin_x_, origin_y_,
                                    cell_size_));

      ++game.score_;
      UpdateScoreText(world, game);

      world.DestroyEntity(game.food_);
      game.food_ = snake::CreateFood(world, RandomFreeCell(world, game, tail_position), origin_x_,
                                     origin_y_, cell_size_);
    }
  }

  void ResetGame(engine::World& world, SnakeGame& game) const {
    for (auto segment : game.segments_) {
      world.DestroyEntity(segment);
    }
    world.DestroyEntity(game.food_);

    const int center_x = game.columns_ / 2;
    const int center_y = game.rows_ / 2;

    game.segments_.clear();
    game.segments_.emplace_back(
        snake::CreateSnakeSegment(world, {center_x, center_y}, 0, true, origin_x_, origin_y_,
                                  cell_size_));
    game.segments_.emplace_back(
        snake::CreateSnakeSegment(world, {center_x - 1, center_y}, 1, false, origin_x_, origin_y_,
                                  cell_size_));
    game.segments_.emplace_back(
        snake::CreateSnakeSegment(world, {center_x - 2, center_y}, 2, false, origin_x_, origin_y_,
                                  cell_size_));

    game.food_ =
        snake::CreateFood(world, {center_x + 5, center_y}, origin_x_, origin_y_, cell_size_);
    game.direction_ = Direction::Right;
    game.next_direction_ = Direction::Right;
    game.score_ = 0;
    game.step_timer_ = 0.0f;
    game.game_over_ = false;

    UpdateScoreText(world, game);
    UpdateStatusText(world, game);
  }

  float origin_x_ = 0.0f;
  float origin_y_ = 0.0f;
  float cell_size_ = 0.0f;
};
