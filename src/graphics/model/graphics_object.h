#pragma once
#include "model.h"

#include <glm/glm.hpp>

namespace tactics_game
{
class buffered_graphics_object;

class graphics_object
{
    friend class buffered_graphics_object;
public:
    explicit graphics_object(model model);
    explicit graphics_object(std::shared_ptr<mesh> m);

    glm::vec3 get_position() const;
    glm::vec3 get_rotation() const;
    glm::vec3 get_scale() const;

    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_scale(glm::vec3 scale);

    glm::mat4 get_model_matrix() const;

    graphics_object transformed() const;
    const model& get_model() const;

private:
    const model model_;

    glm::vec3 position_{0};
    glm::vec3 rotation_{0};
    glm::vec3 scale_{1};
};
}
