#pragma once

namespace engine {

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

inline bool Intersecting(const Rect& a, const Rect& b) {
  return a.x <= b.x + b.w && a.x + a.w >= b.x && a.y <= b.y + b.h && a.y + a.h >= b.y;
}

}  // namespace engine
