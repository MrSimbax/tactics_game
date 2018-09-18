#pragma once

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
    bool initWindow();
    void handleEvent(SDL_Event* event);
    void update();
    void render();
    void cleanUp();

private:
    bool isRunning;

    SDL_Window *window;
    SDL_GLContext context;

    int windowWidth;
    int windowHeight;
};

}  // namespace TacticsGame
