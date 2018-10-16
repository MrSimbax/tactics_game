#pragma once

#include "../graphics/window.h"
#include "../graphics/shader_program.h"
#include "../graphics/objects/graphics_object.h"

#include <memory>

#include <SDL2/SDL.h>

namespace TacticsGame
{

class Application
{
public:
    Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int execute(int argc, char* argv[]);

private:
    bool init();
    bool initServices();
    bool initLogger();
    bool initWindow();
    bool initGraphics();

    void handleEvent(SDL_Event* event);
    void update();
    void render();
    void cleanUp();

private:
    bool isRunning;
    std::unique_ptr<Graphics::Window> window;

    std::unique_ptr<Graphics::ShaderProgram> shaderProgram;
    std::unique_ptr<Graphics::Objects::AbstractObject> object;
};

}  // namespace TacticsGame
