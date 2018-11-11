#include "sdl_window.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace tactics_game;
// ReSharper disable once CppParameterMayBeConst
sdl_window::sdl_window(const std::string& title, window_size size, const window_settings settings)
    : settings_{settings}
{
    init_sdl();

    window_ = create_sdl_window(title, size, settings);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    context_ = SDL_GL_CreateContext(window_);
    if (!context_)
    {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw opengl_initialization_error{
            (std::string{"Failed to initialize OpenGL context: "} + SDL_GetError()).c_str()
        };
    }

    if (SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0) < 0)
    {
        LOG_WARNING << "VSync is not supported on this system";
    }

    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        SDL_DestroyWindow(window_);
        SDL_GL_DeleteContext(context_);
        SDL_Quit();
        throw opengl_initialization_error{(std::string{"Failed to initialize GLAD"} + SDL_GetError()).c_str()};
    }

    sdl_window::resize();
    sdl_window::set_wireframe_mode(false);
}

SDL_Window* sdl_window::create_sdl_window(const std::string& title, const window_size size,
                                          const window_settings settings)
{
    const auto window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        size.width, size.height,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL |
        (settings.resizable ? SDL_WINDOW_RESIZABLE : 0) |
        (settings.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (!window)
    {
        throw sdl_window_not_created_error{(std::string{"Failed to create window_: "} + SDL_GetError()).c_str()};
    }
    return window;
}

sdl_window::~sdl_window()
{
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void sdl_window::init_sdl()
{
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            throw sdl_initialization_error{(std::string{"Failed to initialize SDL2: "} + SDL_GetError()).c_str()};
        }
    }
    else
    {
        throw sdl_initialization_error{"SDL is already initialized."};
    }
}

window_size sdl_window::get_size() const
{
    return size_;
}

void sdl_window::clear_screen()
{
    glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void sdl_window::resize()
{
    SDL_GetWindowSize(window_, &size_.width, &size_.height);
    glViewport(0, 0, size_.width, size_.height);
}

void sdl_window::swap_buffers()
{
    SDL_GL_SwapWindow(window_);
}

color4_f sdl_window::get_clear_color()
{
    return clear_color_;
}

void sdl_window::set_clear_color(const color4_f color)
{
    clear_color_ = color;
}

bool sdl_window::is_wireframe_mode_on()
{
    return wireframe_mode_;
}

void sdl_window::set_wireframe_mode(const bool mode)
{
    wireframe_mode_ = mode;
    if (mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
