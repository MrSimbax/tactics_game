#pragma once

#include "../player.h"
#include "unit_renderer.h"

namespace tactics_game
{

class player_renderer
{
public:
    player_renderer(std::shared_ptr<player> player, const model& unit_model);

    void render(shader_program& program);

private:
    void create_unit_renderers(const model& unit_model);

    std::shared_ptr<player> player_;

    std::vector<unit_renderer> unit_renderers_;
};
}
