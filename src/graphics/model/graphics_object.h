#pragma once
#include "model.h"

#include <glm/glm.hpp>

namespace tactics_game
{
class graphics_object
{
public:
    explicit graphics_object(model model);

    void render(shader_program& program) const;

    glm::vec3 get_position() const;
    glm::vec3 get_rotation() const;
    glm::vec3 get_scale() const;

    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_scale(glm::vec3 scale);

    glm::mat4 get_model_matrix() const;

private:
    model model_;

    glm::vec3 position_{0};
    glm::vec3 rotation_{0};
    glm::vec3 scale_{1};
};
}
