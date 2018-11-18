#pragma once

#include "../player.h"
#include "unit_renderer.h"
#include "../../graphics/model/top_camera.h"

namespace tactics_game
{
class player_renderer
{
public:
    player_renderer(std::shared_ptr<player> player, std::shared_ptr<top_camera> camera, const model& unit_model);

    void render(shader_program& program);

    std::shared_ptr<top_camera> get_camera();

private:
    void create_unit_renderers(const model& unit_model);

    std::shared_ptr<player> player_;

    std::vector<unit_renderer> unit_renderers_;
    std::shared_ptr<top_camera> camera_;
};
}
