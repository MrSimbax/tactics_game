#pragma once

#include "window.h"

#include <string>
#include <memory>

#include <SDL2/SDL.h>

namespace TacticsGame::Graphics
{

class SDLWindow : public Window, public WindowFactory
{
public:
    static std::unique_ptr<SDLWindow> create(const std::string& title, WindowSize size, WindowSettings settings);
    ~SDLWindow() override;

    WindowSize getCurrentSize() const override;

    void resize() override;
    void swapBuffers() override;

private:
    SDLWindow(SDL_Window *window, SDL_GLContext context, WindowSettings settings);

    SDL_Window *window;
    SDL_GLContext context;

    WindowSize size;
    WindowSettings settings;
};

namespace Internal
{

extern bool initSDL();

}

}
