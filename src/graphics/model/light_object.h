#pragma once
#include "graphics_object.h"

namespace tactics_game
{
class light_object : public graphics_object
{
public:
    explicit light_object(const model& model);
    explicit light_object(std::shared_ptr<mesh> m);

    glm::vec4 get_color() const;
    void set_color(glm::vec4 color);

private:
    glm::vec4 color_{1};
};
}
