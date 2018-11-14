#pragma once
#include "graphics_object.h"
#include "buffered_model.h"

namespace tactics_game
{
class buffered_graphics_object
{
public:
    explicit buffered_graphics_object(const graphics_object* object);
    buffered_graphics_object(const buffered_graphics_object& other) = delete;
    buffered_graphics_object& operator=(const buffered_graphics_object& other) = delete;
    buffered_graphics_object(buffered_graphics_object&& other) noexcept = default;
    buffered_graphics_object& operator=(buffered_graphics_object&& other) noexcept = default;
    ~buffered_graphics_object() = default;

    void render(shader_program& program) const;

private:
    const graphics_object* object_;
    buffered_model model_;
};
}
