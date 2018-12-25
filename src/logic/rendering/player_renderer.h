#pragma once

#include "../player.h"
#include "unit_renderer.h"
#include "../../graphics/model/top_camera.h"

namespace tactics_game
{
class player_renderer
{
public:
    player_renderer(std::shared_ptr<player> player, top_camera camera, const model& unit_model,
                    const model& grid_model, glm::vec4 color);

    void render(shader_program& program, int layer);
    void render_outline(shader_program& program, int layer);

    top_camera& get_camera();

    std::vector<unit_renderer>& get_unit_renderers();

    glm::vec4 get_color() const;

private:
    void create_unit_renderers(const model& unit_model, const model& grid_model);

    std::shared_ptr<player> player_;

    std::vector<unit_renderer> unit_renderers_;
    top_camera camera_;

    glm::vec4 color_{};
};
}
