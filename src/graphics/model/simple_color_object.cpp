#include "simple_color_object.h"

using namespace tactics_game;


simple_color_object::simple_color_object(const model& model)
    : graphics_object{model}
{
}

simple_color_object::simple_color_object(std::shared_ptr<mesh> m)
    : graphics_object{std::move(m)}
{
}

glm::vec4 simple_color_object::get_color() const
{
    return color_;
}

void simple_color_object::set_color(const glm::vec4 color)
{
    color_ = color;
}
