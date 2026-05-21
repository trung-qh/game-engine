#pragma once

#include "Components.h"
#include "engine/Engine.h"

inline engine::entity::Entity CreatePaddle(engine::World& world, PlayerSide side,
                                           const engine::core::Rect& body, float speed,
                                           engine::core::Key up, engine::core::Key down,
                                           engine::core::Font* font, float score_x,
                                           float score_y) {
  auto score_text = world.CreateEntity();
  world.AddComponent<engine::RenderableText>(score_text, score_x, score_y, "0", font);

  auto paddle = world.CreateEntity();
  world.AddComponent<engine::RenderableShape>(paddle, body, engine::core::colors::White);
  world.AddComponent<engine::Collider>(paddle, body);
  world.AddComponent<Velocity>(paddle, Velocity{0.0f, speed});
  world.AddComponent<PlayerController>(paddle, up, down);
  world.AddComponent<Paddle>(paddle, side);
  world.AddComponent<Score>(paddle, score_text);

  return paddle;
}

inline engine::entity::Entity CreateBall(engine::World& world, const engine::core::Circle& body,
                                         float speed) {
  auto ball = world.CreateEntity();
  world.AddComponent<engine::RenderableShape>(ball, body, engine::core::colors::White);
  world.AddComponent<engine::Collider>(ball, body);
  world.AddComponent<Velocity>(ball, Velocity{speed, speed});
  world.AddComponent<Ball>(ball, body.x, body.y);

  return ball;
}

inline engine::entity::Entity CreateDivider(engine::World& world, const engine::core::Rect& body) {
  auto divider = world.CreateEntity();
  world.AddComponent<engine::RenderableShape>(divider, body, engine::core::colors::Gray);
  return divider;
}
