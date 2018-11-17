#include "light_object.h"

using namespace tactics_game;


light_object::light_object(const model& model)
    : graphics_object{model}
{
}

light_object::light_object(std::shared_ptr<mesh> m)
    : graphics_object{std::move(m)}
{
}

glm::vec4 light_object::get_color() const
{
    return color_;
}

void light_object::set_color(const glm::vec4 color)
{
    color_ = color;
}
