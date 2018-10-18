#include "sdl_window.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace TacticsGame::Graphics;

SDLWindow::SDLWindow(SDL_Window *window, SDL_GLContext context, WindowSettings settings) :
    window{ window },
    context{ context },
    size{},
    settings{ settings },
    clearColor{0.0f, 0.0f, 0.0f, 1.0f}
{
    this->resize();
    this->setWireframeMode(false);
}

SDLWindow::~SDLWindow()
{
    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

std::unique_ptr<SDLWindow> SDLWindow::create(const std::string& title, WindowSize size, WindowSettings settings)
{
    if (!InternalSDLWindow::initSDL()) return nullptr;

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
        LOG_FATAL << "Failed to create window: " << SDL_GetError();
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    auto context = SDL_GL_CreateContext(sdlWindow);
    if (!context)
    {
        LOG_FATAL << "Failed to initialize OpenGL context: " << SDL_GetError();
        return nullptr;
    }

    if (SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0) < 0)
    {
        LOG_WARNING << "VSync is not supported on this system";
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_FATAL << "Failed to initialize GLAD";
        return nullptr;
    }

    LOG_INFO << "Created a window successfully.";
    return std::unique_ptr<SDLWindow>{ new SDLWindow(sdlWindow, context, settings) };
}

bool InternalSDLWindow::initSDL()
{
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            LOG_FATAL << "Failed to initialize SDL2: " << SDL_GetError();
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

void SDLWindow::clearScreen()
{
    glClearColor(this->clearColor.r, this->clearColor.g, this->clearColor.b, this->clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
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

Color4f SDLWindow::getClearColor()
{
    return this->clearColor;
}

void SDLWindow::setClearColor(Color4f color)
{
    this->clearColor = color;
}

bool SDLWindow::isWireframeModeOn()
{
    return this->wireframeMode;
}

void SDLWindow::setWireframeMode(bool mode)
{
    this->wireframeMode = mode;
    if (mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
