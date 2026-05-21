#pragma once

namespace engine::core {

struct Font {
  const char* path;
  float size;

  Font(const char* path, float size) : path(path), size(size) {}
};

}