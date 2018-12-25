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
    explicit graphics_object(const mesh& m);

    graphics_object(const graphics_object&) = default;
    graphics_object(graphics_object&&) = default;

    graphics_object& operator=(const graphics_object&) = default;
    graphics_object& operator=(graphics_object&&) = default;
    
    virtual ~graphics_object() = default;

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
    model model_;

    glm::vec3 position_{0};
    glm::vec3 rotation_{0};
    glm::vec3 scale_{1};
};
}
