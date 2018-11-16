#include "game_scene_renderer.h"
#include <utility>

using namespace tactics_game;

game_scene_renderer::game_scene_renderer(std::shared_ptr<game_scene> scene,
                                         std::shared_ptr<game_map_renderer> map_renderer,
                                         std::vector<std::shared_ptr<player_renderer>> player_renderers)
    : scene_{std::move(scene)},
      map_renderer_{std::move(map_renderer)},
      player_renderers_{std::move(player_renderers)}
{
}

void game_scene_renderer::render(shader_program& program)
{
    map_renderer_->render(program, scene_->get_game_map()->get_layers().size());
    for (auto& player_renderer : player_renderers_)
    {
        player_renderer->render(program);
    }
}
