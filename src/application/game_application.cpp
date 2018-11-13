#include "game_application.h"

#include "../graphics/window/sdl_window.h"
#include "../graphics/shader/shader_program.h"
#include "../services/services.h"
#include "../services/assets//assets_service_impl.h"

#include <plog/Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace tactics_game;

game_application::game_application()
    : is_running_{false},
      window_{nullptr},
      shader_program_{nullptr},
      object_{nullptr}
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
        init_services();
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

void game_application::init_services() const
{
    services::initialize();
    services::provide(std::make_unique<assets_service_impl>("assets/"));
    LOG_INFO << "services initialized.";
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
    const auto cube_model = services::get_assets_service().get_model("cube.obj");
    object_.reset(new graphics_object{cube_model});

    shader_program_.reset(new shader_program{
        services::get_assets_service().get_shader_source("main.vert"),
        services::get_assets_service().get_shader_source("main.frag")
    });

    object_->set_rotation({glm::radians(-55.0f), 0.0f, 0.0f});
    object_->set_scale({0.5f, 0.5f, 0.5f});

    camera_.set_position({0.0f, 0.0f, 3.0f});
    camera_.set_rotation({0.0f, glm::radians(-90.0f)});
    camera_.set_mouse_sensitivity(input_manager_.get_mouse_sensitivity());

    update_perspective_matrix();
}

void game_application::init_input()
{
    input_manager_.bind_key_to_action(SDLK_a, input_action::camera_left);
    input_manager_.bind_key_to_action(SDLK_d, input_action::camera_right);
    input_manager_.bind_key_to_action(SDLK_w, input_action::camera_forward);
    input_manager_.bind_key_to_action(SDLK_s, input_action::camera_backward);
    input_manager_.bind_key_to_action(SDLK_SPACE, input_action::camera_up);
    input_manager_.bind_key_to_action(SDLK_LCTRL, input_action::camera_down);

    input_manager_.bind_action_down(input_action::camera_left, [this]{ camera_direction_.x -= 1.0f; });
    input_manager_.bind_action_up(input_action::camera_left, [this]{ camera_direction_.x += 1.0f; });

    input_manager_.bind_action_down(input_action::camera_right, [this]{ camera_direction_.x += 1.0f; });
    input_manager_.bind_action_up(input_action::camera_right, [this]{ camera_direction_.x -= 1.0f; });

    input_manager_.bind_action_down(input_action::camera_forward, [this]{ camera_direction_.z += 1.0f; });
    input_manager_.bind_action_up(input_action::camera_forward, [this]{ camera_direction_.z -= 1.0f; });

    input_manager_.bind_action_down(input_action::camera_backward, [this]{ camera_direction_.z -= 1.0f; });
    input_manager_.bind_action_up(input_action::camera_backward, [this]{ camera_direction_.z += 1.0f; });

    input_manager_.bind_action_down(input_action::camera_up, [this]{ camera_direction_.y += 1.0f; });
    input_manager_.bind_action_up(input_action::camera_up, [this]{ camera_direction_.y -= 1.0f; });

    input_manager_.bind_action_down(input_action::camera_down, [this]{ camera_direction_.y -= 1.0f; });
    input_manager_.bind_action_up(input_action::camera_down, [this]{ camera_direction_.y += 1.0f; });

    input_manager_.bind_mouse_motion([this](const glm::ivec2 offset){ camera_.process_mouse(offset); });
    input_manager_.bind_mouse_scroll([this](const int offset){ camera_.process_scroll(offset); });
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
    default:;
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

    shader_program_->set_mat4("model", object_->get_model_matrix());
    shader_program_->set_mat4("view", camera_.get_view_matrix());
    shader_program_->set_mat4("projection", camera_.get_projection_matrix());

    object_->render(*shader_program_);

    window_->swap_buffers();
}
