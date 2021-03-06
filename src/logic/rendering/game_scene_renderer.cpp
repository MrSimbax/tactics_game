#include "game_scene_renderer.h"
#include <utility>
#include "../../graphics/model/lights.h"
#include <plog/Log.h>
#include "../../misc/custom_log.h"
#include <glad/glad.h>
#include <SDL2/SDL_mouse.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

using namespace tactics_game;

game_scene_renderer::game_scene_renderer(game_scene scene,
                                         game_map_renderer map_renderer,
                                         std::vector<player_renderer> player_renderers,
                                         std::vector<std::vector<point_light>> point_lights,
                                         buffered_graphics_object grid_object,
                                         ui_renderer ui_renderer,
                                         light_objects_renderer light_objects_renderer,
                                         glm::vec3 world_ambient)
    : scene_{std::move(scene)},
      ui_renderer_{std::move(ui_renderer)},
      light_objects_renderer_{std::move(light_objects_renderer)},
      map_renderer_{std::move(map_renderer)},
      player_renderers_{std::move(player_renderers)},
      fow_renderer_{scene_.get_game_map()->get_size()},
      point_lights_{std::move(point_lights)},
      world_ambient_{world_ambient},
      grid_cursor_object_{std::move(grid_object)}
{
    init_new_turn();
}

void game_scene_renderer::render(shader_program& program, shader_program& simple_color_program,
                                 shader_program& fow_program)
{
    program.use();
    program.set_mat4("u_view", get_current_camera().get_view_matrix());
    program.set_mat4("u_projection", get_current_camera().get_projection_matrix());
    program.set_vec3("u_view_pos", get_current_camera().get_position());

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);

    // Static map
    program.set_int("u_point_lights_count", point_lights_count_);
    map_renderer_.render(program, get_current_layer() + 1);

    // Grid cursor
    if (should_render_grid_cursor_)
        grid_cursor_object_.render(program);

    simple_color_program.use();
    simple_color_program.set_mat4("u_view", get_current_camera().get_view_matrix());
    simple_color_program.set_mat4("u_projection", get_current_camera().get_projection_matrix());

    // Render light objects
    light_objects_renderer_.render(simple_color_program, get_current_layer() + 1);

    // Movable tiles grid
    if (is_unit_selected())
        currently_selected_unit_->render_grid(simple_color_program, get_current_layer() + 1);

    // Rendering units
    program.use();
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    for (auto& player_renderer : player_renderers_)
        player_renderer.render(program, get_current_layer());
    // Render outlines
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    simple_color_program.use();
    for (auto& player_renderer : player_renderers_)
        player_renderer.render_outline(simple_color_program, get_current_layer());


    // Back
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);

    // FOW
    fow_program.use();
    fow_program.set_mat4("u_view", get_current_camera().get_view_matrix());
    fow_program.set_mat4("u_projection", get_current_camera().get_projection_matrix());
    fow_renderer_.render(fow_program, get_current_layer());

    // UI
    simple_color_program.use();
    ui_renderer_.render(simple_color_program, player_renderers_[scene_.get_current_player_id()].get_color());

    glEnable(GL_STENCIL_TEST);
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

void game_scene_renderer::try_to_find_hovered_unit()
{
    // try to find a unit under cursor
    currently_hovered_unit_ = nullptr;
    for (auto& player : player_renderers_)
    {
        for (auto& unit_renderer : player.get_unit_renderers())
        {
            if (unit_renderer.get_unit()->get_position() == currently_hovered_position_)
            {
                if (unit_renderer.get_unit()->is_dead() ||
                    !unit_renderer.get_unit()->is_visible() ||
                    (unit_renderer.get_unit()->get_player_id() == get_current_player_id() &&
                        unit_renderer.get_unit()->turn_done()))
                    break;
                currently_hovered_unit_ = &unit_renderer;
                break;
            }
        }
    }
}

bool game_scene_renderer::hovered_position_changed(const glm::vec3 ray)
{
    // Update position
    const auto position = get_map_position_from_camera_ray(ray);

    if (currently_hovered_position_ == position)
        return false;

    currently_hovered_position_ = position;

    if (!is_hovered_position_on_map())
        return true;

    // Find hovered unit
    if (!is_unit_hovered() || currently_hovered_unit_->get_unit()->get_position() != currently_hovered_position_)
    {
        if (is_unit_hovered())
        {
            // turn off outline if not selected
            if (!is_hovered_unit_selected())
                turn_off_outline(currently_hovered_unit_);
        }

        try_to_find_hovered_unit();
    }

    return true;
}

void game_scene_renderer::on_mouse_motion(const glm::vec3 ray)
{
    if (!hovered_position_changed(ray))
        return;

    // Update hover grid under cursor
    if (is_hovered_position_on_map())
    {
        const auto tile = scene_.get_game_map()->get_tile(currently_hovered_position_);
        should_render_grid_cursor_ = !(tile == tile_type::wall);
        grid_cursor_object_.get_graphics_object().set_position(currently_hovered_position_);
    }
    else
    {
        should_render_grid_cursor_ = false;
    }

    // Hover enemy unit outline
    if (is_unit_hovered() && is_unit_selected() && !is_unit_from_current_player(currently_hovered_unit_) &&
        scene_.can_unit_shoot(*currently_selected_unit_->get_unit(), *currently_hovered_unit_->get_unit()))
    {
        turn_on_outline(currently_hovered_unit_,
                        lerp(outline_color_shoot_probability_low,
                             outline_color_shoot_probability_high,
                             scene_.calculate_shooting_probability(
                                 *currently_selected_unit_->get_unit(),
                                 *currently_hovered_unit_->get_unit()
                             )
                        )
        );
    }
}

glm::ivec3 game_scene_renderer::get_map_position_from_camera_ray(const glm::vec3 ray)
{
    auto layer_id = get_current_layer();
    glm::vec3 position{-1};
    while (true)
    {
        position = raycast_to_xz_plane(
            get_current_camera().get_render_position(),
            ray,
            static_cast<float>(layer_id) * get_current_camera().get_zoom()
        ) / get_current_camera().get_zoom();
        position.x = static_cast<float>(std::lroundf(position.x));
        position.y = static_cast<float>(layer_id);
        position.z = static_cast<float>(std::lroundf(position.z));
        if (position.x >= scene_.get_game_map()->get_size().x ||
            position.x < 0 ||
            position.z >= scene_.get_game_map()->get_size().z ||
            position.z < 0)
        {
            position = glm::vec3{-1};
            break;
        }
        const auto tile = scene_.get_game_map()->get_tile(position);

        if (tile == tile_type::empty)
        {
            if (layer_id == 0)
            {
                position = glm::vec3{-1};
                break;
            }
            --layer_id;
        }
        else
            // ok!
            break;
    }
    return position;
}

void game_scene_renderer::select_unit(unit_renderer* unit)
{
    currently_selected_unit_ = unit;
    turn_on_outline(currently_selected_unit_, outline_color_selected);
}

void game_scene_renderer::handle_left_mouse_button(const glm::ivec3 position)
{
    if (is_unit_selected() && !is_hovered_unit_selected())
    {
        turn_off_outline(currently_selected_unit_);
        currently_selected_unit_ = nullptr;
    }

    if (is_unit_hovered() && is_unit_from_current_player(currently_hovered_unit_))
        select_unit(currently_hovered_unit_);
}

void game_scene_renderer::handle_right_mouse_button(const glm::ivec3 position)
{
    if (!is_hovered_position_on_map())
        return;

    if (is_unit_selected() && is_unit_hovered() && !is_hovered_unit_selected() &&
        !is_unit_from_current_player(currently_hovered_unit_))
    {
        // shoot
        auto& target = *currently_hovered_unit_->get_unit();
        if (scene_.can_unit_shoot(*currently_selected_unit_->get_unit(), target))
            scene_.shoot_unit(*currently_selected_unit_->get_unit(), target);
        if (target.is_dead())
        {
            turn_off_outline(currently_hovered_unit_);
            currently_hovered_unit_ = nullptr;
            update_movable_grids();
        }
    }
    else if (is_unit_selected() && !is_unit_hovered() &&
        scene_.can_unit_move(*currently_selected_unit_->get_unit(), position))
    {
        // move
        scene_.move_unit(*currently_selected_unit_->get_unit(), position);
        update_movable_grids();
        update_fow();
    }

    if (is_unit_selected() && currently_selected_unit_->get_unit()->turn_done())
    {
        // turn done?
        turn_off_outline(currently_selected_unit_);
        currently_selected_unit_ = nullptr;
        update_ui();
    }
}

void game_scene_renderer::update_ui()
{
    if (scene_.has_any_unit_any_action_left())
    {
        ui_renderer_.set_state(moves_left);
    }
    else
    {
        ui_renderer_.set_state(no_moves_left);
    }
}

bool game_scene_renderer::is_unit_selected() const
{
    return !!currently_selected_unit_;
}

bool game_scene_renderer::is_unit_hovered() const
{
    return !!currently_hovered_unit_;
}

bool game_scene_renderer::is_hovered_unit_selected() const
{
    return currently_selected_unit_ == currently_hovered_unit_;
}

bool game_scene_renderer::is_unit_from_current_player(const unit_renderer* unit) const
{
    return unit->get_unit()->get_player_id() == scene_.get_current_player_id();
}

bool game_scene_renderer::is_hovered_position_on_map() const
{
    return currently_hovered_position_.x != -1;
}

void game_scene_renderer::turn_off_outline(unit_renderer* unit) const
{
    if (is_unit_from_current_player(unit) && !unit->get_unit()->turn_done())
        turn_on_outline(unit, outline_color_turn_ready);
    else
        unit->set_outline(false);
}

void game_scene_renderer::turn_on_outline(unit_renderer* unit, const glm::vec4 color)
{
    unit->set_outline(true);
    unit->set_outline_color(color);
}

void game_scene_renderer::on_mouse_click(const glm::vec3 ray, const int button)
{
    hovered_position_changed(ray);

    if (button == SDL_BUTTON_LEFT)
        handle_left_mouse_button(currently_hovered_position_);
    else if (button == SDL_BUTTON_RIGHT)
        handle_right_mouse_button(currently_hovered_position_);
}

int game_scene_renderer::get_current_layer()
{
    return get_current_camera().get_current_layer();
}

void game_scene_renderer::set_current_layer(const int layer)
{
    const auto max_layer = scene_.get_game_map()->get_size().y - 1;
    auto& camera = get_current_camera();
    camera.set_current_layer(glm::clamp(layer, 0, max_layer));
    camera.set_target(glm::vec3(camera.get_target().x, camera.get_current_layer(), camera.get_target().z));
    count_point_lights();
}

top_camera& game_scene_renderer::get_current_camera()
{
    return player_renderers_[scene_.get_current_player_id()].get_camera();
}

std::vector<top_camera*> game_scene_renderer::get_all_cameras()
{
    std::vector<top_camera*> v;
    for (auto& player : player_renderers_)
    {
        v.push_back(&player.get_camera());
    }
    return v;
}

int game_scene_renderer::get_current_player_id() const
{
    return static_cast<int>(scene_.get_current_player_id());
}

game_scene* game_scene_renderer::get_scene()
{
    return &scene_;
}

void game_scene_renderer::start_new_turn()
{
    end_turn();
    if (scene_.start_new_turn())
    {
        did_game_end_ = true;
    }
    init_new_turn();

    // trigger camera update
    set_current_layer(get_current_camera().get_current_layer());
}

void game_scene_renderer::move_camera_to_unit(const unit_renderer* unit)
{
    glm::vec3 pos = unit->get_unit()->get_position();
    pos.x += 0.5f;
    pos.z += 0.5f;
    get_current_camera().set_current_layer(static_cast<size_t>(pos.y));
    get_current_camera().set_target(pos);
    count_point_lights();
}

bool game_scene_renderer::try_select_and_move_to_unit(unit_renderer* unit)
{
    if (!unit->get_unit()->is_dead() && !unit->get_unit()->turn_done())
    {
        if (is_unit_selected())
        {
            turn_off_outline(currently_selected_unit_);
            currently_selected_unit_ = nullptr;
        }
        select_unit(unit);
        move_camera_to_unit(unit);
        return true;
    }
    return false;
}

void game_scene_renderer::select_next_unit()
{
    auto& units = player_renderers_[scene_.get_current_player_id()].get_unit_renderers();

    if (is_unit_selected())
    {
        auto selected = false;
        for (auto i = currently_selected_unit_->get_unit()->get_id() + 1; i < units.size(); ++i)
        {
            if (try_select_and_move_to_unit(&units[i]))
            {
                selected = true;
                break;
            }
        }

        if (!selected)
        {
            for (auto i = 0u; i <= currently_selected_unit_->get_unit()->get_id(); ++i)
            {
                if (try_select_and_move_to_unit(&units[i]))
                {
                    break;
                }
            }
        }
    }
    else
    {
        for (auto& unit : units)
        {
            if (try_select_and_move_to_unit(&unit))
                break;
        }
    }
}

bool game_scene_renderer::did_game_end() const
{
    return did_game_end_;
}

void game_scene_renderer::end_turn()
{
    for (auto& unit_renderer : player_renderers_[scene_.get_current_player_id()].get_unit_renderers())
    {
        unit_renderer.set_outline(false);
    }
    currently_selected_unit_ = nullptr;
}

void game_scene_renderer::init_new_turn()
{
    // turn on outlines
    for (auto& unit_renderer : player_renderers_[scene_.get_current_player_id()].get_unit_renderers())
    {
        if (unit_renderer.get_unit()->is_dead())
            continue;
        turn_on_outline(&unit_renderer, outline_color_turn_ready);
    }

    update_movable_grids();
    update_fow();
    update_ui();
}

void game_scene_renderer::update_movable_grids()
{
    for (auto& unit_renderer : player_renderers_[scene_.get_current_player_id()].get_unit_renderers())
    {
        if (unit_renderer.get_unit()->is_dead())
            continue;
        unit_renderer.update_movable_grid(scene_.get_game_map()->get_size().y);
    }
}

void game_scene_renderer::update_fow()
{
    const glm::vec<4, uint8_t> visible_color{0, 0, 0, 0};
    const glm::vec<4, uint8_t> not_visible_color{0, 0, 0, 127};
    std::vector<texture::data_t> layers;
    layers.reserve(scene_.get_game_map()->get_size().y);
    for (auto y = 0; y < scene_.get_game_map()->get_size().y; ++y)
    {
        texture::data_t data;
        for (auto z = 0; z < scene_.get_game_map()->get_size().z; ++z)
        {
            for (auto x = 0; x < scene_.get_game_map()->get_size().x; ++x)
            {
                auto is_visible = false;
                for (const auto& unit : player_renderers_[get_current_player_id()].get_unit_renderers())
                {
                    if (unit.get_unit()->is_dead())
                        continue;

                    if (line_of_sight_finder::can_see_tile(unit.get_unit()->get_position(), glm::ivec3{x, y, z},
                                                           *scene_.get_game_map(),
                                                           unit.get_unit()->get_visible_tiles()))
                    {
                        data.push_back(visible_color);
                        is_visible = true;
                        break;
                    }
                }
                if (!is_visible)
                    data.push_back(not_visible_color);
            }
        }
        layers.push_back(data);
    }

    fow_renderer_.update_textures(layers);
}

glm::vec3 game_scene_renderer::raycast_to_xz_plane(const glm::vec3 from, const glm::vec3 ray, const float y)
{
    // Intersect with plane
    // It's just solving the equation for t
    // (from + ray * t) * plane_normal + d = 0
    // plane_normal = [0, 1, 0]
    // d = - plane_normal * [0, y, 0] = -y
    const auto t = -(from.y - y) / ray.y;
    return from + ray * t;
}
