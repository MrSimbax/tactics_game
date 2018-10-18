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

    void clearScreen() override;
    void resize() override;
    void swapBuffers() override;

    Color4f getClearColor() override;
    void setClearColor(Color4f color) override;

    bool isWireframeModeOn() override;
    void setWireframeMode(bool mode) override;

private:
    SDLWindow(SDL_Window *window, SDL_GLContext context, WindowSettings settings);

    SDL_Window *window;
    SDL_GLContext context;

    WindowSize size;
    WindowSettings settings;

    Color4f clearColor;
    bool wireframeMode;
};

namespace InternalSDLWindow
{

extern bool initSDL();

}

}
