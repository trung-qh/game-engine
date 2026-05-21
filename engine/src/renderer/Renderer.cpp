#include "renderer/Renderer.h"

#include <variant>

#include "core/Shapes.h"
#include "engine/ecs/Components.h"
#include "entity/Entity.h"

namespace engine {

Renderer::Renderer(platform::Renderer& renderer)
    : native_renderer_(renderer), text_manager_(renderer) {}

Renderer::~Renderer() {}

void Renderer::Render(World& world) {
  text_manager_.BeginFrame();

  world.Each<RenderableShape>([&](entity::Entity, const RenderableShape& renderable) {
    if (!renderable.visible_) {
      return;
    }

    if (const auto* rect = std::get_if<core::Rect>(&renderable.shape_)) {
      if (renderable.is_solid_) {
        native_renderer_.DrawFilledRect(rect->x, rect->y, rect->w, rect->h, renderable.color_);
      } else {
        native_renderer_.DrawRect(rect->x, rect->y, rect->w, rect->h, renderable.thickness_,
                                  renderable.color_);
      }
    } else if (const auto* circle = std::get_if<core::Circle>(&renderable.shape_)) {
      if (renderable.is_solid_) {
        native_renderer_.DrawFilledCircle(circle->x, circle->y, circle->r, renderable.color_);
      } else {
        native_renderer_.DrawCircle(circle->x, circle->y, circle->r, renderable.thickness_,
                                    renderable.color_);
      }
    }
  });

  world.Each<RenderableText>([&](entity::Entity, const RenderableText& renderable) {
    if (!renderable.visible_ || renderable.text_.empty() || renderable.font_ == nullptr ||
        renderable.font_->path == nullptr) {
      return;
    }

    platform::Font& native_font = text_manager_.GetOrCreateNativeFont(renderable.font_);
    platform::Text& native_text = text_manager_.GetNextText(renderable.text_.c_str(), native_font);

    native_renderer_.DrawText(native_text, renderable.x_, renderable.y_, renderable.thickness_,
                              renderable.color_);
  });
}

}  // namespace engine
