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
    void set_up_opengl_debug_output() const;
    sdl_window(const std::string& title, window_size size, window_settings settings);
    sdl_window(const sdl_window&) = delete;
    sdl_window& operator=(const sdl_window&) = delete;
    sdl_window(sdl_window&&) noexcept;
    sdl_window& operator=(sdl_window&&) noexcept;
    ~sdl_window() override;

    window_size get_size() const override;

    void clear_screen() override;
    void resize() override;
    void swap_buffers() override;

    glm::vec4 get_clear_color() override;
    void set_clear_color(glm::vec4 color) override;

    bool is_wireframe_mode_on() override;
    void set_wireframe_mode(bool mode) override;

private:
    static void init_sdl();
    static SDL_Window* create_sdl_window(const std::string& title, window_size size,
                                         window_settings settings);

    static bool is_sdl_initialized_;

    SDL_Window* window_;
    SDL_GLContext context_;

    window_size size_;
    window_settings settings_;

    glm::vec4 clear_color_{};
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