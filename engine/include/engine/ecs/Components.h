#pragma once

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
  const char* text_;
  core::Font* font_ = nullptr;
  core::Color color_;
  bool visible_ = true;
  float thickness_ = 1.0f;

  float x_ = 0.0f;
  float y_ = 0.0f;
};

}  // namespace engine
