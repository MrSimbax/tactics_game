#include "sdl_window.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace TacticsGame::Graphics;

SDLWindow::SDLWindow(SDL_Window *window, SDL_GLContext context, WindowSettings settings) :
    window{ window },
    context{ context },
    size{},
    settings{settings}
{
    this->resize();
}

SDLWindow::~SDLWindow()
{
    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

std::unique_ptr<SDLWindow> SDLWindow::create(const std::string& title, WindowSize size, WindowSettings settings)
{
    if (!Internal::initSDL()) return nullptr;

    auto sdlWindow = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        size.width, size.height,
        SDL_WindowFlags::SDL_WINDOW_SHOWN |
        SDL_WindowFlags::SDL_WINDOW_OPENGL |
        (settings.resizable ? SDL_WindowFlags::SDL_WINDOW_RESIZABLE : 0) |
        (settings.fullscreen ? SDL_WindowFlags::SDL_WINDOW_FULLSCREEN : 0)
    );
    if (!sdlWindow)
    {
        LOG_ERROR << "Failed to create window: " << SDL_GetError();
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    auto context = SDL_GL_CreateContext(sdlWindow);
    if (!context)
    {
        LOG_ERROR << "Failed to initialize OpenGL context: " << SDL_GetError();
        return nullptr;
    }

    if (SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0) < 0)
    {
        LOG_WARNING << "VSync is not supported on this system";
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_ERROR << "Failed to initialize GLAD";
        return nullptr;
    }

    LOG_INFO << "Created a window successfully.";
    return std::unique_ptr<SDLWindow>{ new SDLWindow(sdlWindow, context, settings) };
}

bool Internal::initSDL()
{
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            LOG_ERROR << "Failed to initialize SDL2: " << SDL_GetError();
            return false;
        }
    }
    else
    {
        LOG_WARNING << "SDL is already initialized.";
    }
    LOG_INFO << "Initialized SDL2 successfully.";
    return true;
}

WindowSize SDLWindow::getCurrentSize() const
{
    return this->size;
}

void SDLWindow::resize()
{
    SDL_GetWindowSize(this->window, &this->size.width, &this->size.height);
    glViewport(0, 0, this->size.width, this->size.height);
}

void SDLWindow::swapBuffers()
{
    SDL_GL_SwapWindow(this->window);
}
