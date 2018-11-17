#pragma once
#include "../game_scene.h"

#include "game_map_renderer.h"
#include "player_renderer.h"
#include "light_objects_renderer.h"
#include "../../graphics/model/lights.h"

namespace tactics_game
{
class game_scene_renderer
{
public:
    game_scene_renderer(std::shared_ptr<game_scene> scene, std::shared_ptr<game_map_renderer> map_renderer,
                        std::vector<std::shared_ptr<player_renderer>> player_renderers,
                        std::vector<point_light> point_lights,
                        std::shared_ptr<light_objects_renderer> light_objects_renderer = {},
                        glm::vec3 world_ambient = {});

    void render(shader_program& program, shader_program& light_objects_program);

    void set_lights(shader_program& program);

private:
    std::shared_ptr<game_scene> scene_;

    std::shared_ptr<light_objects_renderer> light_objects_renderer_;
    std::shared_ptr<game_map_renderer> map_renderer_;
    std::vector<std::shared_ptr<player_renderer>> player_renderers_;

    std::vector<point_light> point_lights_;
    glm::vec3 world_ambient_;
};
}
