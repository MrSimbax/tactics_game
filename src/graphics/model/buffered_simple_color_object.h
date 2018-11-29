#pragma once
#include "buffered_model.h"
#include "simple_color_object.h"

namespace tactics_game
{
class buffered_simple_color_object
{
public:
    explicit buffered_simple_color_object(const std::shared_ptr<simple_color_object>& object);
    buffered_simple_color_object(const buffered_simple_color_object& other) = delete;
    buffered_simple_color_object& operator=(const buffered_simple_color_object& other) = delete;
    buffered_simple_color_object(buffered_simple_color_object&& other) noexcept = default;
    buffered_simple_color_object& operator=(buffered_simple_color_object&& other) noexcept = default;
    ~buffered_simple_color_object() = default;

    void render(shader_program& program) const;

private:
    std::shared_ptr<simple_color_object> object_;
    buffered_model model_;
};
}
