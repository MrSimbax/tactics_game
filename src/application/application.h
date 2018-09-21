#pragma once

#include "../graphics/window.h"

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
    bool initLogger();
    bool initWindow();

    void handleEvent(SDL_Event* event);
    void update();
    void render();
    void cleanUp();

private:
    bool isRunning;
    std::unique_ptr<Graphics::Window> window;
};

}  // namespace TacticsGame
