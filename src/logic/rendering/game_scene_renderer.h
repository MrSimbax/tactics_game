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
    game_scene_renderer(std::shared_ptr<game_scene> scene,
                        std::shared_ptr<game_map_renderer> map_renderer,
                        std::vector<std::shared_ptr<player_renderer>> player_renderers,
                        std::vector<std::vector<point_light>> point_lights,
                        std::unique_ptr<buffered_graphics_object> grid_object,
                        std::shared_ptr<light_objects_renderer> light_objects_renderer = {},
                        glm::vec3 world_ambient = {});

    void render(shader_program& program, shader_program& light_objects_program);

    void set_lights(shader_program& program);
    void count_point_lights();

    void on_mouse_motion(const glm::vec3 ray);
    void on_mouse_click(glm::ivec3 position, int button);

    int get_current_layer();
    void set_current_layer(int layer);
    std::shared_ptr<top_camera> get_current_camera();

private:
    glm::ivec3 get_map_position_from_camera_ray(glm::vec3 ray);
    static glm::vec3 raycast_to_xz_plane(glm::vec3 from, glm::vec3 ray, float y);

    std::shared_ptr<game_scene> scene_;

    std::shared_ptr<light_objects_renderer> light_objects_renderer_;
    std::shared_ptr<game_map_renderer> map_renderer_;
    std::vector<std::shared_ptr<player_renderer>> player_renderers_;

    std::vector<std::vector<point_light>> point_lights_;
    glm::vec3 world_ambient_;

    std::unique_ptr<buffered_graphics_object> grid_cursor_object_;
    bool should_render_grid_cursor_{false};

    int point_lights_count_{0};
};
}
