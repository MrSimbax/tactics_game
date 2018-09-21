#pragma once

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

    virtual void resize() = 0;
    virtual void swapBuffers() = 0;

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
