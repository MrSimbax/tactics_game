#include "game_application.h"

#include "../graphics/window/sdl_window.h"
#include "../graphics/shader/shader_program.h"

#include <plog/Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../misc/custom_log.h"

using namespace tactics_game;

game_application::game_application()
    : is_running_{false}
{
}

game_application::~game_application() = default;

int game_application::execute(int argc, char* argv[])
{
    if (!init())
    {
        LOG_FATAL << "game_application could not be started due to initialization error.";
        std::getchar();
        return 1;
    }

    LOG_INFO << "game_application is running.";
    SDL_Event event{};
    while (is_running_)
    {
        const auto time = SDL_GetTicks() / 1000.0f;
        const auto delta_time = time - last_frame_time_;
        last_frame_time_ = time;

        while (SDL_PollEvent(&event))
        {
            handle_event(&event);
        }

        update(delta_time);
        render();
    }

    return 0;
}

bool game_application::init()
{
    try
    {
        init_window();
        init_graphics();
        init_input();
    }
    catch (const std::runtime_error& e)
    {
        LOG_ERROR << e.what();
        return false;
    }

    is_running_ = true;
    LOG_INFO << "game_application initialized successfully.";
    return true;
}

void game_application::init_window()
{
    const std::string title = "Tactics Game";

    window_size size;
    size.width = 800;
    size.height = 600;

    window_settings settings;
    settings.fullscreen = false;
    settings.resizable = true;
    settings.vsync = false;

    window_.reset(new sdl_window(title, size, settings));

    window_->set_wireframe_mode(true);
    window_->set_mouse_trapped(true);
}

void game_application::update_perspective_matrix()
{
    const auto aspect_ratio =
        static_cast<float>(window_->get_size().width)
        /
        static_cast<float>(window_->get_size().height);
    camera_.set_ratio(aspect_ratio);
}

void game_application::init_graphics()
{
    shader_program_.reset(new shader_program{
        assets_manager_.get_shader_source("main.vert"),
        assets_manager_.get_shader_source("main.frag")
    });

    camera_.set_position({0.0f, 0.0f, 3.0f});
    camera_.set_rotation({0.0f, glm::radians(-90.0f)});
    camera_.set_mouse_sensitivity(input_manager_.get_mouse_sensitivity());

    update_perspective_matrix();

    //current_map_.reset(new game_map{assets_manager_.get_map("demo")});

    /*map_renderer_.reset(new game_map_renderer{
        current_map_,
        std::make_unique<graphics_object>(assets_manager_.get_model("floor.obj")),
        std::make_unique<graphics_object>(assets_manager_.get_model("wall.obj")),
        std::make_unique<graphics_object>(assets_manager_.get_model("floor.obj"))
    });*/

    current_scene_.reset(new game_scene{assets_manager_.get_scene("demo")});

    std::shared_ptr<game_map_renderer> map_renderer{
        new game_map_renderer{
            std::shared_ptr<game_map>{current_scene_->get_game_map()},
            std::make_unique<graphics_object>(assets_manager_.get_model("floor.obj")),
            std::make_unique<graphics_object>(assets_manager_.get_model("wall.obj")),
            std::make_unique<graphics_object>(assets_manager_.get_model("floor.obj"))
        }
    };

    std::vector<std::shared_ptr<player_renderer>> player_renderers;

    player_renderers.emplace_back(new player_renderer{
        current_scene_->get_players()[0], assets_manager_.get_model("player1.obj")
    });
    player_renderers.emplace_back(new player_renderer{
        current_scene_->get_players()[1], assets_manager_.get_model("player2.obj")
    });

    scene_renderer_.reset(new game_scene_renderer(current_scene_, std::move(map_renderer),
                                                  std::move(player_renderers)));

    LOG_INFO << "Loaded map \"" << current_scene_->get_name();
}

void game_application::init_input()
{
    input_manager_.bind_key_to_action(SDLK_a, input_action::camera_left);
    input_manager_.bind_key_to_action(SDLK_d, input_action::camera_right);
    input_manager_.bind_key_to_action(SDLK_w, input_action::camera_forward);
    input_manager_.bind_key_to_action(SDLK_s, input_action::camera_backward);
    input_manager_.bind_key_to_action(SDLK_SPACE, input_action::camera_up);
    input_manager_.bind_key_to_action(SDLK_LCTRL, input_action::camera_down);
    input_manager_.bind_key_to_action(SDLK_SEMICOLON, input_action::debug);

    input_manager_.bind_action_down(input_action::camera_left, [this]
    {
        camera_direction_.x -= 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_left, [this]
    {
        camera_direction_.x += 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_right, [this]
    {
        camera_direction_.x += 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_right, [this]
    {
        camera_direction_.x -= 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_forward, [this]
    {
        camera_direction_.z += 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_forward, [this]
    {
        camera_direction_.z -= 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_backward, [this]
    {
        camera_direction_.z -= 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_backward, [this]
    {
        camera_direction_.z += 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_up, [this]
    {
        camera_direction_.y += 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_up, [this]
    {
        camera_direction_.y -= 1.0f;
    });

    input_manager_.bind_action_down(input_action::camera_down, [this]
    {
        camera_direction_.y -= 1.0f;
    });
    input_manager_.bind_action_up(input_action::camera_down, [this]
    {
        camera_direction_.y += 1.0f;
    });

    input_manager_.bind_mouse_motion([this](const glm::ivec2 offset)
    {
        camera_.process_mouse(offset);
    });
    input_manager_.bind_mouse_scroll([this](const int offset)
    {
        camera_.process_scroll(offset);
    });

    input_manager_.bind_action_down(input_action::debug, [this]
    {
        LOG_DEBUG << camera_.get_position();
    });
}

void game_application::handle_event(SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        is_running_ = false;
        break;
    case SDL_WINDOWEVENT:
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window_->resize();
            update_perspective_matrix();
            break;
        default: ;
        }
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    case SDL_MOUSEMOTION:
    case SDL_MOUSEWHEEL:
        input_manager_.handle_event(event);
        break;
    default: ;
    }
}

void game_application::update(const float delta_time)
{
    camera_.process_keyboard(camera_direction_, delta_time);
}

void game_application::render() const
{
    window_->clear_screen();
    shader_program_->use();

    shader_program_->set_mat4("view", camera_.get_view_matrix());
    shader_program_->set_mat4("projection", camera_.get_projection_matrix());

    //map_renderer_->render(*shader_program_, current_map_->get_layers().size());
    scene_renderer_->render(*shader_program_);

    window_->swap_buffers();
}
