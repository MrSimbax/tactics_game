#include "player_renderer.h"
#include <utility>

using namespace tactics_game;


player_renderer::player_renderer(std::shared_ptr<player> player, std::shared_ptr<top_camera> camera,
                                 const model& unit_model)
    : player_{std::move(player)},
      camera_{std::move(camera)}
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

std::shared_ptr<top_camera> player_renderer::get_camera()
{
    return camera_;
}

void player_renderer::create_unit_renderers(const model& unit_model)
{
    for (auto& unit : player_->get_units())
    {
        unit_renderers_.emplace_back(unit, unit_model);
    }
}
