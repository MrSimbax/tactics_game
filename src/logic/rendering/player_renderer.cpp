#include "player_renderer.h"
#include <utility>

using namespace tactics_game;


player_renderer::player_renderer(std::shared_ptr<player> player, const model& unit_model)
    : player_{std::move(player)}
{
    create_unit_renderers(unit_model);
}

void player_renderer::render(shader_program& program)
{
    for (auto& unit_renderer : unit_renderers_)
    {
        unit_renderer.render(program);
    }
}

void player_renderer::create_unit_renderers(const model& unit_model)
{
    for (auto& unit : player_->get_units())
    {
        unit_renderers_.emplace_back(unit, unit_model);
    }
}
