#include "game_application.h"

#include "../graphics/window/sdl_window.h"
#include "../graphics/objects/triangle.h"
#include "../services/services.h"
#include "../services/assets//assets_service_impl.h"

#include <glad/glad.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include "../graphics/shader/shader_program.h"

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
        return 1;
    }

    LOG_INFO << "game_application is running.";
    SDL_Event event{};
    while (is_running_)
    {
        while (SDL_PollEvent(&event))
        {
            handle_event(&event);
        }

        update();
        render();
    }

    return 0;
}

bool game_application::init()
{
    try
    {
        init_services();
        init_logger();
        init_window();
        init_graphics();
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

void game_application::init_logger() const
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
    static plog::DebugOutputAppender<plog::TxtFormatter> debug_output_appender;
    plog::init(plog::verbose, &console_appender).addAppender(&debug_output_appender);
    LOG_INFO << "Logger is working.";
}

void game_application::init_window()
{
    const std::string title = "Tactics Game";

    window_size size{};
    size.width = 800;
    size.height = 600;

    window_settings settings{};
    settings.fullscreen = false;
    settings.resizable = true;
    settings.vsync = false;

    window_.reset(new sdl_window(title, size, settings));
}

void game_application::init_graphics()
{
    this->object_.reset(new triangle{
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        }
    });

    /*this->object.reset(new Graphics::Objects::Rectangle{
        {
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        }
    });*/

    shader_program_.reset(new shader_program{
        services::get_assets_service().get_shader_source("main.vert"),
        services::get_assets_service().get_shader_source("main.frag")
    });
}

void game_application::handle_event(SDL_Event* event)
{
    if (event->type == SDL_QUIT)
    {
        is_running_ = false;
    }
    else if (event->type == SDL_WINDOWEVENT)
    {
        switch (event->window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window_->resize();
            break;
        default: ;
        }
    }
}

void game_application::update() const
{
}

void game_application::render() const
{
    window_->clear_screen();
    shader_program_->use();
    object_->render();
    window_->swap_buffers();
}
