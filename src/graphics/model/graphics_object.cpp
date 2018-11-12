#include "graphics_object.h"
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace tactics_game;

graphics_object::graphics_object(model model)
    : model_{std::move(model)}
{
}

void graphics_object::render(shader_program& program) const
{
    model_.render(program);
}

glm::vec3 graphics_object::get_position() const
{
    return position_;
}

glm::vec3 graphics_object::get_rotation() const
{
    return rotation_;
}

glm::vec3 graphics_object::get_scale() const
{
    return scale_;
}

void graphics_object::set_position(const glm::vec3 position)
{
    position_ = position;
}

void graphics_object::set_rotation(const glm::vec3 rotation)
{
    rotation_ = rotation;
}

void graphics_object::set_scale(const glm::vec3 scale)
{
    scale_ = scale;
}

glm::mat4 graphics_object::get_model_matrix() const
{
    glm::mat4 model{1};
    model = glm::scale(model, scale_);
    model = glm::rotate(model, rotation_.x, {1.0f, 0.0f, 0.0f});
    model = glm::rotate(model, rotation_.y, {0.0f, 1.0f, 0.0f});
    model = glm::rotate(model, rotation_.z, {0.0f, 0.0f, 1.0f});
    model = glm::translate(model, position_);
    return model;
}
