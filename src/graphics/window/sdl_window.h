#pragma once

#include "window.h"

#include <string>
#include <memory>

#include <SDL2/SDL.h>

namespace tactics_game
{
class sdl_window : public window
{
public:
    sdl_window(const std::string& title, window_size size, window_settings settings);
    ~sdl_window() override;

    window_size get_size() const override;

    void clear_screen() override;
    void resize() override;
    void swap_buffers() override;

    color4_f get_clear_color() override;
    void set_clear_color(color4_f color) override;

    bool is_wireframe_mode_on() override;
    void set_wireframe_mode(bool mode) override;

private:
    static void init_sdl();
    static SDL_Window* create_sdl_window(const std::string& title, window_size size,
                                         window_settings settings);

    SDL_Window* window_;
    SDL_GLContext context_;

    window_size size_;
    window_settings settings_;

    color4_f clear_color_{};
    bool wireframe_mode_{};
};

class sdl_initialization_error : public std::runtime_error
{
public:
    explicit sdl_initialization_error(const char* what)
        : runtime_error(what)
    {
    }
};

class sdl_window_not_created_error : public std::runtime_error
{
public:
    explicit sdl_window_not_created_error(const char* what)
        : runtime_error(what)
    {
    }
};

class opengl_initialization_error : public std::runtime_error
{
public:
    explicit opengl_initialization_error(const char* what)
        : runtime_error(what)
    {
    }
};
}
