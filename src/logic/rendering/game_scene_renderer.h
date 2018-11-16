#pragma once
#include "../game_scene.h"

#include "game_map_renderer.h"
#include "player_renderer.h"

namespace tactics_game
{
class game_scene_renderer
{
public:
    game_scene_renderer(std::shared_ptr<game_scene> scene, std::shared_ptr<game_map_renderer> map_renderer,
                        std::vector<std::shared_ptr<player_renderer>> player_renderers);

    void render(shader_program& program);

private:
    std::shared_ptr<game_scene> scene_;

    std::shared_ptr<game_map_renderer> map_renderer_;
    std::vector<std::shared_ptr<player_renderer>> player_renderers_;
};
}
