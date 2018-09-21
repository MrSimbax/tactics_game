#include "application.h"

#include "../graphics/sdl_window.h"

#include <iostream>

#include <glad/glad.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/DebugOutputAppender.h>

using namespace TacticsGame;

Application::Application() :
    isRunning{ false },
    window{ nullptr }
{
}

int Application::execute(int argc, char *argv[])
{
    if (!init())
    {
        LOG_ERROR << "Application could not be started due to initialization error.";
        return 1;
    }

    LOG_INFO << "Application is running.";
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
    if (!this->initLogger()) return false;
    if (!this->initWindow()) return false;
    this->isRunning = true;
    LOG_INFO << "Application initialized successfully.";
    return true;
}

bool TacticsGame::Application::initLogger()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::verbose, &consoleAppender).addAppender(&debugOutputAppender);
    LOG_INFO << "Logger is working.";
    return true;
}

bool TacticsGame::Application::initWindow()
{
    const std::string title = "Tactics Game";

    Graphics::WindowSize size = {};
    size.width = 800;
    size.height = 600;

    Graphics::WindowSettings settings = {};
    settings.fullscreen = false;
    settings.resizable = true;
    settings.vsync = false;

    return !!(this->window = Graphics::SDLWindow::create(title, size, settings));
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
            this->window->resize();
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
    this->window->swapBuffers();
}

void Application::cleanUp()
{
    LOG_INFO << "Cleaned up.";
}

