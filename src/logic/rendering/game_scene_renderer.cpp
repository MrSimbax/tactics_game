#include "game_scene_renderer.h"
#include <utility>
#include "../../graphics/model/lights.h"

using namespace tactics_game;

game_scene_renderer::game_scene_renderer(std::shared_ptr<game_scene> scene,
                                         std::shared_ptr<game_map_renderer> map_renderer,
                                         std::vector<std::shared_ptr<player_renderer>> player_renderers,
                                         std::vector<point_light> point_lights,
                                         std::shared_ptr<light_objects_renderer> light_objects_renderer,
                                         glm::vec3 world_ambient)
    : scene_{std::move(scene)},
      light_objects_renderer_{std::move(light_objects_renderer)},
      map_renderer_{std::move(map_renderer)},
      player_renderers_{std::move(player_renderers)},
      point_lights_{std::move(point_lights)},
      world_ambient_{world_ambient}
{
}

void game_scene_renderer::render(shader_program& program, shader_program& light_objects_program)
{
    program.use();
    //TODO set matrices from players
    map_renderer_->render(program, scene_->get_game_map()->get_layers().size());
    for (auto& player_renderer : player_renderers_)
    {
        player_renderer->render(program);
    }
    if (!!light_objects_renderer_)
        light_objects_renderer_->render(light_objects_program);
}

void game_scene_renderer::set_lights(shader_program& program)
{
    program.use();
    program.set_vec3("u_world_ambient", world_ambient_);
    program.set_int("u_point_lights_count", point_lights_.size());
    for (size_t i = 0; i < point_lights_.size(); ++i)
    {
        set_point_light(program, point_lights_[i], i);
    }
}
