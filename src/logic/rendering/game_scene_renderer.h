#pragma once
#include "../game_scene.h"

#include "game_map_renderer.h"
#include "player_renderer.h"
#include "light_objects_renderer.h"
#include "../../graphics/model/lights.h"
#include "ui_renderer.h"
#include "fow_renderer.h"

namespace tactics_game
{
class game_scene_renderer
{
public:
    game_scene_renderer(game_scene scene,
                        game_map_renderer map_renderer,
                        std::vector<player_renderer> player_renderers,
                        std::vector<std::vector<point_light>> point_lights,
                        buffered_graphics_object grid_object,
                        ui_renderer ui_renderer,
                        light_objects_renderer light_objects_renderer,
                        glm::vec3 world_ambient = {});

    // Init lights in program
    void set_lights(shader_program& program);

    void render(shader_program& program, shader_program& simple_color_program, shader_program& fow_program);

    // Input handlers
    void start_new_turn();
    void select_next_unit();
    void on_mouse_motion(glm::vec3 ray);
    void on_mouse_click(glm::vec3 ray, int button);

    // Getters
    int get_current_layer();
    void set_current_layer(int layer);
    top_camera& get_current_camera();
    std::vector<top_camera*> get_all_cameras();
    int get_current_player_id() const;
    game_scene* get_scene();

    bool did_game_end() const;

private:
    // Input handlers
    bool hovered_position_changed(glm::vec3 ray);
    void handle_left_mouse_button(glm::ivec3 position);
    void handle_right_mouse_button(glm::ivec3 position);

    // Rendering logic
    void count_point_lights();
    void try_to_find_hovered_unit();

    void init_new_turn();
    void update_movable_grids();
    void update_fow();
    void end_turn();
    void update_ui();

    void select_unit(unit_renderer* unit);
    bool try_select_and_move_to_unit(unit_renderer* unit);
    void move_camera_to_unit(const unit_renderer* unit);

    // Helpers
    bool is_unit_selected() const;
    bool is_unit_hovered() const;
    bool is_hovered_unit_selected() const;
    bool is_unit_from_current_player(const unit_renderer* unit) const;
    bool is_hovered_position_on_map() const;
    
    void turn_off_outline(unit_renderer* unit) const;
    static void turn_on_outline(unit_renderer* unit, glm::vec4 color);

    // Map
    glm::ivec3 get_map_position_from_camera_ray(glm::vec3 ray);
    static glm::vec3 raycast_to_xz_plane(glm::vec3 from, glm::vec3 ray, float y);

    // Game logic
    game_scene scene_;

    // Renderers
    ui_renderer ui_renderer_;
    light_objects_renderer light_objects_renderer_;
    game_map_renderer map_renderer_;
    std::vector<player_renderer> player_renderers_;
    fow_renderer fow_renderer_;

    // Lighting
    int point_lights_count_{0};
    std::vector<std::vector<point_light>> point_lights_;
    glm::vec3 world_ambient_;

    // Cursor
    buffered_graphics_object grid_cursor_object_;
    bool should_render_grid_cursor_{false};

    // Hover/selection
    unit_renderer* currently_selected_unit_{};
    unit_renderer* currently_hovered_unit_{};
    glm::ivec3 currently_hovered_position_{0};

    // should probably use observer pattern but this is simpler
    bool did_game_end_ = false;
};
}
