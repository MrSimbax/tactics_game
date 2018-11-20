#include "unit_renderer.h"
#include <utility>
#include <plog/Log.h>
#include "../../misc/custom_log.h"
#include <glm/glm.hpp>

using namespace tactics_game;

unit_renderer::unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model)
    : unit_{std::move(unit)},
      graphics_object_{new graphics_object{unit_model}},
      buffered_graphics_object_{graphics_object_}
{
}

void unit_renderer::render(shader_program& program) const
{
    if (!unit_->is_visible() || unit_->is_dead())
        return;
    const auto game_position = unit_->get_position();
    graphics_object_->set_position(glm::vec3{
        game_position.x,
        game_position.y + 0.5f,
        game_position.z
    });
    buffered_graphics_object_.render(program);
}

void unit_renderer::render_outline(shader_program& program) const
{
    if (outline_)
        program.set_vec4("u_color", outline_color_);
    else
        return;

    const auto game_position = unit_->get_position();
    graphics_object_->set_position(glm::vec3{
        game_position.x,
        game_position.y + 0.5f,
        game_position.z
    });
    graphics_object_->set_scale(glm::vec3(1.0f) + outline_width);
    buffered_graphics_object_.render(program);
    graphics_object_->set_scale(glm::vec3(1.0f));
}

bool unit_renderer::get_outline() const
{
    return outline_;
}

void unit_renderer::set_outline(const bool selected)
{
    outline_ = selected;
}

glm::vec4 unit_renderer::get_outline_color() const
{
    return outline_color_;
}

void unit_renderer::set_outline_color(glm::vec4 color)
{
    outline_color_ = color;
}

std::shared_ptr<game_unit> unit_renderer::get_unit() const
{
    return unit_;
}
