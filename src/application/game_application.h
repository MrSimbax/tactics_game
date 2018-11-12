#pragma once

#include "../graphics/window/window.h"
#include "../graphics/shader/shader_program.h"
#include "../graphics/model/graphics_object.h"

#include <memory>

#include <SDL2/SDL.h>
#include "../graphics/model/mesh.h"
#include "glm/mat4x2.hpp"

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
    bool init();
    void init_services() const;
    void init_logger() const;
    void init_window();
    void init_graphics();

    void handle_event(SDL_Event* event);
    void update() const;
    void render() const;

private:
    bool is_running_;
    std::unique_ptr<window> window_{};

    std::unique_ptr<shader_program> shader_program_{};
    std::unique_ptr<graphics_object> object_{};

    glm::mat4 projection_{1.0f};
    glm::mat4 view_{1.0f};
    glm::mat4 model_{1.0f};
};
} // namespace tactics_game
