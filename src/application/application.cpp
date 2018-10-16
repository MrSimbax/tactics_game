#include "application.h"

#include "../graphics/sdl_window.h"
#include "../graphics/objects/triangle.h"
#include "../graphics/objects/rectangle.h"
#include "../services/services.h"
#include "../services/resources/resources_manager_impl.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/DebugOutputAppender.h>

using namespace TacticsGame;

Application::Application() :
    isRunning{ false },
    window{ nullptr },
    shaderProgram{ nullptr },
    object{ nullptr }
{
}

int Application::execute(int argc, char *argv[])
{
    if (!init())
    {
        LOG_FATAL << "Application could not be started due to initialization error.";
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
    if (!this->initServices()) return false;
    if (!this->initLogger()) return false;
    if (!this->initWindow()) return false;
    if (!this->initGraphics()) return false;

    this->isRunning = true;
    LOG_INFO << "Application initialized successfully.";
    return true;
}

bool Application::initServices()
{
    Services::initialize();
    Services::provide(std::make_unique<Service::Resources::ResourcesManagerImpl>("assets/"));
    LOG_INFO << "Services initialized.";
    return true;
}

bool Application::initLogger()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::verbose, &consoleAppender).addAppender(&debugOutputAppender);
    LOG_INFO << "Logger is working.";
    return true;
}

bool Application::initWindow()
{
    const std::string title = "Tactics Game";

    Graphics::WindowSize size{};
    size.width = 800;
    size.height = 600;

    Graphics::WindowSettings settings{};
    settings.fullscreen = false;
    settings.resizable = true;
    settings.vsync = false;

    return !!(this->window = Graphics::SDLWindow::create(title, size, settings));
}

bool Application::initGraphics()
{
    /*this->object.reset(new Graphics::Objects::Triangle{ {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    } });*/

    this->object.reset(new Graphics::Objects::Rectangle{ {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    }});

    this->shaderProgram.reset(new Graphics::ShaderProgram{
        Services::getResourcesManager().getShaderSource("main.vert"),
        Services::getResourcesManager().getShaderSource("main.frag")
    });
    if (!this->shaderProgram->compile())
    {
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
    this->window->clearScreen();
    this->shaderProgram->use();
    this->object->render();
    this->window->swapBuffers();
}

void Application::cleanUp()
{
    LOG_INFO << "Cleaned up.";
}

