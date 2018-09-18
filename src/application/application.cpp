#include "application.h"

#include <iostream>

#include <glad/glad.h>

using namespace TacticsGame;

Application::Application() :
    isRunning{ false },
    context{ nullptr },
    window{ nullptr },
    windowWidth{ 0 },
    windowHeight{ 0 }
{
}

int Application::execute(int argc, char *argv[])
{
    if (!init())
    {
        return 1;
    }

    SDL_Event event{};

    while (this->isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            handleEvent(&event);
        }

        update();
        render();
    }

    cleanUp();

    return 0;
}

bool Application::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }

    initWindow();

    // initGL
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    this->context = SDL_GL_CreateContext(this->window);
    if (!this->context)
    {
        std::cerr << "Failed to initialize context" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    // initGLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    this->isRunning = true;

    SDL_GetWindowSize(this->window, &this->windowWidth, &this->windowHeight);
    glViewport(0, 0, this->windowWidth, this->windowHeight);

    return true;
}

bool TacticsGame::Application::initWindow()
{
    // initWindow
    this->window = SDL_CreateWindow(
        "Tactics Game",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WindowFlags::SDL_WINDOW_SHOWN |
        SDL_WindowFlags::SDL_WINDOW_OPENGL |
        SDL_WindowFlags::SDL_WINDOW_RESIZABLE
    );
    if (!this->window)
    {
        std::cerr << "Failed to initialize window" << std::endl;
        return false;
    }
    return true;
}

void Application::handleEvent(SDL_Event* event)
{
    if (event->type == SDL_EventType::SDL_QUIT)
    {
        this->isRunning = false;
    }
    else if (event->type == SDL_EventType::SDL_WINDOWEVENT)
    {
        switch (event->window.event)
        {
        case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
            SDL_GetWindowSize(this->window, &this->windowWidth, &this->windowHeight);
            glViewport(0, 0, this->windowWidth, this->windowHeight);
            break;
        }
    }
}

void Application::update()
{
}

void Application::render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(this->window);
}

void Application::cleanUp()
{
    if (this->context)
    {
        SDL_GL_DeleteContext(this->context);
        this->context = nullptr;
    }

    if (this->window)
    {
        SDL_DestroyWindow(this->window);
        this->window = nullptr;
    }

    windowWidth = 0;
    windowHeight = 0;

    SDL_Quit();
}
