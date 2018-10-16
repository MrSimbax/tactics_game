#pragma once

#include "color.h"

#include <memory>
#include <string>

namespace TacticsGame::Graphics
{

struct WindowSize
{
    int width = 0;
    int height = 0;
};

struct WindowSettings
{
    bool fullscreen = false;
    bool resizable = false;
    bool vsync = false;
};

class Window
{
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    virtual ~Window() = 0;

    virtual WindowSize getCurrentSize() const = 0;

    virtual void clearScreen() = 0;
    virtual void resize() = 0;
    virtual void swapBuffers() = 0;

    virtual bool isWireframeModeOn() = 0;
    virtual void setWireframeMode(bool mode) = 0;

    virtual Color4f getClearColor() = 0;
    virtual void setClearColor(Color4f color) = 0;

protected:
    Window() {};
};

inline Window::~Window() {}

class WindowFactory
{
public:
    static std::unique_ptr<Window> create(const std::string& title, WindowSize size, WindowSettings settings) { return nullptr; };
};

}
