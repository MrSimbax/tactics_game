#include "graphics_object.h"
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace tactics_game;

graphics_object::graphics_object(model model)
    : model_{std::move(model)}
{
}

graphics_object::graphics_object(const mesh& m)
    : model_{model{std::vector<mesh>{m}}}
{
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
    model = scale(model, scale_);
    model = rotate(model, rotation_.x, {1.0f, 0.0f, 0.0f});
    model = rotate(model, rotation_.y, {0.0f, 1.0f, 0.0f});
    model = rotate(model, rotation_.z, {0.0f, 0.0f, 1.0f});
    model = translate(model, position_);
    return model;
}

graphics_object graphics_object::transformed() const
{
    return graphics_object{model_.transformed(get_model_matrix())};
}

const model& graphics_object::get_model() const
{
    return model_;
}
