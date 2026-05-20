#include "renderer/Renderer.h"

namespace engine {

Renderer::Renderer(platform::Renderer& renderer) : native_renderer_(renderer) {}

Renderer::~Renderer() {}

void Renderer::Render(World& world) {}

}  // namespace engine