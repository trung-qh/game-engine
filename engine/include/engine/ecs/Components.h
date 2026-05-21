#pragma once

#include <string>
#include <vector>

#include "core/Color.h"
#include "core/Font.h"
#include "core/Shapes.h"
#include "entity/Entity.h"

namespace engine {

struct RenderableShape {
  core::Shape shape_;
  core::Color color_;
  bool visible_ = true;
  bool is_solid_ = true;
  float thickness_ = 1.0f;
};

struct RenderableText {
  std::string text_;
  core::Font* font_ = nullptr;
  core::Color color_;
  bool visible_ = true;
  float thickness_ = 1.0f;

  float x_ = 0.0f;
  float y_ = 0.0f;
};

struct Collider {
  core::Shape shape_;
  std::vector<entity::Entity> others_;
};

}  // namespace engine
