#pragma once

#include "../color.h"

#include <memory>
#include <string>

namespace tactics_game
{
struct window_size
{
    int width = 0;
    int height = 0;
};

struct window_settings
{
    bool fullscreen = false;
    bool resizable = false;
    bool vsync = false;
};

class window
{
public:
    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(const window&&) = delete;
    window& operator=(const window&&) = delete;
    virtual ~window() = default;

    virtual window_size get_size() const = 0;

    virtual void clear_screen() = 0;
    virtual void resize() = 0;
    virtual void swap_buffers() = 0;

    virtual bool is_wireframe_mode_on() = 0;
    virtual void set_wireframe_mode(bool mode) = 0;

    virtual color4_f get_clear_color() = 0;
    virtual void set_clear_color(color4_f color) = 0;

protected:
    window() = default;
};
}
