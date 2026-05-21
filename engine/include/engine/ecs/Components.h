#pragma once

#include <string>
#include <vector>

#include "core/Color.h"
#include "core/Font.h"
#include "core/Shapes.h"
#include "entity/Entity.h"

namespace engine {

struct RenderableShape {
  core::Shape body_;
  core::Color color_ = core::colors::White;
  bool visible_ = true;
  bool is_solid_ = true;
  float thickness_ = 1.0f;
};

struct RenderableText {
  float x_ = 0.0f;
  float y_ = 0.0f;

  std::string text_;
  core::Font* font_ = nullptr;
  core::Color color_ = core::colors::White;
  bool visible_ = true;
  float thickness_ = 0.0f;
};

struct Collider {
  core::Shape body_;
  std::vector<entity::Entity> others_;
};

}  // namespace engine
