#pragma once

#include "../graphics/window/window.h"
#include "../graphics/shader/shader_program.h"
#include "../graphics/model/graphics_object.h"
#include "../graphics/model/free_roam_camera.h"

#include <memory>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "input_manager.h"

namespace tactics_game
{
class game_application
{
public:
    game_application();
    game_application(const game_application&) = delete;
    game_application& operator=(const game_application&) = delete;
    game_application(game_application&& other) = delete;
    game_application& operator=(game_application&& other) = delete;
    ~game_application();

    int execute(int argc, char* argv[]);

private:
    void init_input();
    bool init();
    void init_services() const;
    void init_window();
    void init_graphics();

    void handle_event(SDL_Event* event);
    void update(float delta_time);
    void render() const;

    void update_perspective_matrix();

    input_manager input_manager_{};

    bool is_running_;
    std::unique_ptr<window> window_{};

    std::unique_ptr<shader_program> shader_program_{};
    std::unique_ptr<graphics_object> object_{};

    free_roam_camera camera_{};
    glm::vec3 camera_direction_{};

    float last_frame_time_{0.0f};
};
} // namespace tactics_game
