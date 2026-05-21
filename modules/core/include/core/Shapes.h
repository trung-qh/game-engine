#pragma once

#include <algorithm>
#include <variant>

namespace engine::core {

struct Rect {
  float x = 0.0f;
  float y = 0.0f;
  float w = 0.0f;
  float h = 0.0f;
};

struct Circle {
  float x = 0.0f;
  float y = 0.0f;
  float r = 0.0f;
};

using Shape = std::variant<Rect, Circle>;

inline void Translate(Shape& shape, float dx, float dy) {
  std::visit(
      [dx, dy](auto& body) {
        body.x += dx;
        body.y += dy;
      },
      shape);
}

inline void SetPosition(Shape& shape, float new_x, float new_y) {
  std::visit(
      [new_x, new_y](auto& body) {
        body.x = new_x;
        body.y = new_y;
      },
      shape);
}

inline bool Intersecting(const Rect& a, const Rect& b) {
  return a.x <= b.x + b.w && a.x + a.w >= b.x && a.y <= b.y + b.h && a.y + a.h >= b.y;
}

inline bool Intersecting(const Circle& a, const Circle& b) {
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) <= (a.r + b.r) * (a.r + b.r);
}

inline bool Intersecting(const Rect& rect, const Circle& circle) {
  float closest_x = std::clamp(circle.x, rect.x, rect.x + rect.w);
  float closest_y = std::clamp(circle.y, rect.y, rect.y + rect.h);

  return (closest_x - circle.x) * (closest_x - circle.x) +
             (closest_y - circle.y) * (closest_y - circle.y) <=
         circle.r * circle.r;
}

inline bool Intersecting(const Circle& circle, const Rect& rect) {
  return Intersecting(rect, circle);
}

inline bool Intersecting(const Shape& a, const Shape& b) {
  return std::visit([](const auto& lhs, const auto& rhs) { return Intersecting(lhs, rhs); }, a, b);
}

}  // namespace engine::core
