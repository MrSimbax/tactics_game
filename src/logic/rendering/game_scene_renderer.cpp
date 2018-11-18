#include "game_scene_renderer.h"
#include <utility>
#include "../../graphics/model/lights.h"
#include <plog/Log.h>
#include "../../misc/custom_log.h"

using namespace tactics_game;

game_scene_renderer::game_scene_renderer(std::shared_ptr<game_scene> scene,
                                         std::shared_ptr<game_map_renderer> map_renderer,
                                         std::vector<std::shared_ptr<player_renderer>> player_renderers,
                                         std::vector<std::vector<point_light>> point_lights,
                                         std::unique_ptr<buffered_graphics_object> grid_object,
                                         std::shared_ptr<light_objects_renderer> light_objects_renderer,
                                         glm::vec3 world_ambient)
    : scene_{std::move(scene)},
      light_objects_renderer_{std::move(light_objects_renderer)},
      map_renderer_{std::move(map_renderer)},
      player_renderers_{std::move(player_renderers)},
      point_lights_{std::move(point_lights)},
      world_ambient_{world_ambient},
      grid_cursor_object_{std::move(grid_object)}
{
}

void game_scene_renderer::render(shader_program& program, shader_program& light_objects_program)
{
    program.use();
    program.set_int("u_point_lights_count", point_lights_count_);
    map_renderer_->render(program, get_current_layer() + 1);
    for (auto& player_renderer : player_renderers_)
        player_renderer->render(program);

    if (should_render_grid_cursor_)
        grid_cursor_object_->render(program);

    light_objects_program.use();
    if (!!light_objects_renderer_)
        light_objects_renderer_->render(light_objects_program, get_current_layer() + 1);
}

void game_scene_renderer::set_lights(shader_program& program)
{
    program.use();
    program.set_vec3("u_world_ambient", world_ambient_);
    size_t count = 0;
    for (auto& layer : point_lights_)
    {
        for (size_t j = 0; j < layer.size(); ++j)
            set_point_light(program, layer[j], count + j);
        count += layer.size();
    }
    count_point_lights();
}

void game_scene_renderer::count_point_lights()
{
    point_lights_count_ = 0;
    for (auto y = 0; y < get_current_layer() + 1; ++y)
        point_lights_count_ += static_cast<int>(point_lights_[y].size());
}

void game_scene_renderer::on_mouse_motion(const glm::vec3 ray)
{
    const auto position = get_map_position_from_camera_ray(ray);
    if (position.x == -1)
    {
        should_render_grid_cursor_ = false;
        return;
    }
    const auto field = scene_->get_game_map()->get_field(position);
    should_render_grid_cursor_ = !(field == field_type::wall);
    grid_cursor_object_->get_graphics_object()->set_position(position);
}

glm::ivec3 game_scene_renderer::get_map_position_from_camera_ray(const glm::vec3 ray)
{
    auto layer_id = get_current_layer();
    glm::vec3 position{-1};
    while (true)
    {
        position = raycast_to_xz_plane(get_current_camera()->get_position(), ray, layer_id);
        position.x = std::lroundf(position.x);
        position.y = layer_id;
        position.z = std::lroundf(position.z);
        if (position.x >= scene_->get_game_map()->get_size().x ||
            position.x < 0 ||
            position.z >= scene_->get_game_map()->get_size().z ||
            position.z < 0)
        {
            position = glm::vec3{-1};
            break;
        }
        const auto field = scene_->get_game_map()->get_field(position);

        if (field == field_type::empty)
        {
            if (layer_id == 0)
            {
                position = glm::vec3{-1};
                break;
            }
            --layer_id;
        }
        else
        {
            // ok!
            break;
        }
    }
    return position;
}

void game_scene_renderer::on_mouse_click(glm::ivec3 position, int button)
{
}

int game_scene_renderer::get_current_layer()
{
    return get_current_camera()->get_current_layer();
}

void game_scene_renderer::set_current_layer(const int layer)
{
    const auto max_layer = scene_->get_game_map()->get_size().y - 1;
    auto camera = get_current_camera();
    camera->set_current_layer(glm::clamp(layer, 0, max_layer));
    camera->set_target(glm::vec3(camera->get_target().x, camera->get_current_layer(), camera->get_target().z));
    count_point_lights();
}

std::shared_ptr<top_camera> game_scene_renderer::get_current_camera()
{
    return player_renderers_[scene_->get_current_player_id()]->get_camera();
}

glm::vec3 game_scene_renderer::raycast_to_xz_plane(const glm::vec3 from, const glm::vec3 ray, const float y)
{
    // Intersect with plane
    // It's just solving the equation for t
    // (cam_pos + ray * t) * plane_normal + d = 0
    // plane_normal = [0, 1, 0]
    // d = y
    const auto t = -(from.y - y) / ray.y;
    return from + ray * t;
}
