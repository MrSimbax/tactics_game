#include "player_renderer.h"
#include <utility>

using namespace tactics_game;


player_renderer::player_renderer(std::shared_ptr<player> player, top_camera camera,
                                 const model& unit_model, const model& grid_model, glm::vec4 color)
    : player_{std::move(player)},
      camera_{std::move(camera)},
      color_{color}
{
    create_unit_renderers(unit_model, grid_model);
}

void player_renderer::render(shader_program& program, const int layer)
{
    for (auto& unit_renderer : unit_renderers_)
    {
        if (unit_renderer.get_unit()->get_position().y > layer)
            continue;
        unit_renderer.render(program);
    }
}

void player_renderer::render_outline(shader_program& program, const int layer)
{
    for (auto& unit_renderer : unit_renderers_)
    {
        if (unit_renderer.get_unit()->get_position().y > layer)
            continue;
        unit_renderer.render_outline(program);
    }
}

top_camera& player_renderer::get_camera()
{
    return camera_;
}

std::vector<unit_renderer>& player_renderer::get_unit_renderers()
{
    return unit_renderers_;
}

glm::vec4 player_renderer::get_color() const
{
    return color_;
}

void player_renderer::create_unit_renderers(const model& unit_model, const model& grid_model)
{
    for (auto& unit : player_->get_units())
    {
        unit_renderers_.emplace_back(unit, unit_model, grid_model);
    }
}
