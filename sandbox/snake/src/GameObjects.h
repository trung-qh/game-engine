#pragma once

#include <string>
#include <vector>

#include "Components.h"
#include "engine/Engine.h"

namespace snake {

inline constexpr engine::core::Color kBoardColor{12, 18, 22, 255};
inline constexpr engine::core::Color kGridColor{35, 49, 55, 255};
inline constexpr engine::core::Color kSnakeHeadColor{126, 247, 139, 255};
inline constexpr engine::core::Color kSnakeBodyColor{46, 191, 92, 255};
inline constexpr engine::core::Color kFoodColor{242, 86, 74, 255};
inline constexpr engine::core::Color kTextColor{224, 241, 232, 255};
inline constexpr engine::core::Color kMutedTextColor{136, 155, 148, 255};

inline engine::core::Rect CellRect(GridPosition position, float origin_x, float origin_y,
                                   float cell_size) {
  constexpr float kInset = 2.0f;
  return engine::core::Rect{origin_x + position.x_ * cell_size + kInset,
                            origin_y + position.y_ * cell_size + kInset, cell_size - kInset * 2,
                            cell_size - kInset * 2};
}

inline void MoveShapeToCell(engine::RenderableShape& shape, GridPosition position, float origin_x,
                            float origin_y, float cell_size) {
  shape.body_ = CellRect(position, origin_x, origin_y, cell_size);
}

inline engine::entity::Entity CreateText(engine::World& world, const std::string& text,
                                         engine::core::Font* font, float x, float y,
                                         engine::core::Color color = kTextColor) {
  auto entity = world.CreateEntity();
  world.AddComponent<engine::RenderableText>(entity, x, y, text, font, color);
  return entity;
}

inline engine::entity::Entity CreateBoard(engine::World& world, float origin_x, float origin_y,
                                          int columns, int rows, float cell_size) {
  auto board = world.CreateEntity();
  world.AddComponent<engine::RenderableShape>(
      board, engine::core::Rect{origin_x, origin_y, columns * cell_size, rows * cell_size},
      kBoardColor);
  return board;
}

inline engine::entity::Entity CreateSnakeSegment(engine::World& world, GridPosition position,
                                                 int index, bool is_head, float origin_x,
                                                 float origin_y, float cell_size) {
  auto segment = world.CreateEntity();
  world.AddComponent<GridPosition>(segment, position);
  world.AddComponent<SnakeSegment>(segment, index, is_head);
  world.AddComponent<engine::RenderableShape>(
      segment, CellRect(position, origin_x, origin_y, cell_size),
      is_head ? kSnakeHeadColor : kSnakeBodyColor);
  return segment;
}

inline engine::entity::Entity CreateFood(engine::World& world, GridPosition position,
                                         float origin_x, float origin_y, float cell_size) {
  auto food = world.CreateEntity();
  world.AddComponent<GridPosition>(food, position);
  world.AddComponent<Food>(food);
  world.AddComponent<engine::RenderableShape>(food, CellRect(position, origin_x, origin_y, cell_size),
                                              kFoodColor);
  return food;
}

inline engine::entity::Entity CreateSnakeGame(engine::World& world, engine::core::Font* font,
                                              int columns, int rows, float origin_x,
                                              float origin_y, float cell_size,
                                              float step_interval) {
  CreateBoard(world, origin_x, origin_y, columns, rows, cell_size);

  for (int x = 0; x <= columns; ++x) {
    auto line = world.CreateEntity();
    world.AddComponent<engine::RenderableShape>(
        line, engine::core::Rect{origin_x + x * cell_size, origin_y, 1.0f, rows * cell_size},
        kGridColor);
  }

  for (int y = 0; y <= rows; ++y) {
    auto line = world.CreateEntity();
    world.AddComponent<engine::RenderableShape>(
        line, engine::core::Rect{origin_x, origin_y + y * cell_size, columns * cell_size, 1.0f},
        kGridColor);
  }

  auto score_text = CreateText(world, "Score: 0", font, origin_x, 24.0f);
  auto status_text =
      CreateText(world, "Arrow keys or WASD", font, origin_x + 300.0f, 24.0f, kMutedTextColor);

  const int center_x = columns / 2;
  const int center_y = rows / 2;
  std::vector<engine::entity::Entity> segments;
  segments.emplace_back(
      CreateSnakeSegment(world, {center_x, center_y}, 0, true, origin_x, origin_y, cell_size));
  segments.emplace_back(
      CreateSnakeSegment(world, {center_x - 1, center_y}, 1, false, origin_x, origin_y, cell_size));
  segments.emplace_back(
      CreateSnakeSegment(world, {center_x - 2, center_y}, 2, false, origin_x, origin_y, cell_size));

  auto food = CreateFood(world, {center_x + 5, center_y}, origin_x, origin_y, cell_size);

  auto state = world.CreateEntity();
  world.AddComponent<SnakeGame>(state, std::move(segments), food, score_text, status_text, columns,
                                rows, step_interval);
  return state;
}

}  // namespace snake
