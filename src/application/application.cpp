#include "application.h"

#include "../graphics/sdl_window.h"
#include "../graphics/graphics_object.h"
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
    window{ nullptr }
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

    // init graphics
    Graphics::Object triangle{ std::unique_ptr<std::vector<float>>{ new std::vector({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    })} };

    // buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle.getVertices()), &triangle.getVertices().front(), GL_STATIC_DRAW);

    // vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string source = Services::getResourcesManager().getShaderSource("main.vert");
    const char* sourceStr = source.c_str();
    glShaderSource(vertexShader, 1, &sourceStr, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LOG_FATAL << "Vertex shader compilation failed: " << infoLog;
        return false;
    }

    //// fragment shader
    //std::ifstream shader2{ "assets/shaders/main.frag" };
    //if (!shader2.is_open() || !shader2.good())
    //{
    //    LOG_FATAL << "Could not load fragment shader " << strerror(errno);
    //    return false;
    //}
    //shader2.close();
    //std::ostringstream ss2;
    //ss2 << shader2.rdbuf();
    //std::string shader2Str{ ss.str() };
    //const char * shader2Src{ shader2Str.c_str() };

    //unsigned int fragmentShader;
    //fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(fragmentShader, 1, &shader2Src, nullptr);
    //glCompileShader(fragmentShader);

    //int  success2;
    //char infoLog2[512];
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);
    //if (!success2)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
    //    LOG_FATAL << "Fragment shader compilation failed: " << infoLog2;
    //    return false;
    //}

    //// Program
    //unsigned int shaderProgram;
    //shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success)
    //{
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    LOG_FATAL << "Shader program compilation failed: " << infoLog;
    //    return false;
    //}

    //glUseProgram(shaderProgram);
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);


    this->isRunning = true;
    LOG_INFO << "Application initialized successfully.";
    return true;
}

bool TacticsGame::Application::initServices()
{
    Services::initialize();
    Services::provide(std::make_unique<Service::Resources::ResourcesManagerImpl>("assets/"));
    LOG_INFO << "Services initialized.";
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
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //glUseProgram(shaderProgram);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->window->swapBuffers();
}

void Application::cleanUp()
{
    LOG_INFO << "Cleaned up.";
}

