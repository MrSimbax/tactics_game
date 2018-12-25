#pragma once
#include "graphics_object.h"

namespace tactics_game
{
class simple_color_object : public graphics_object
{
public:
    explicit simple_color_object(const model& model);
    explicit simple_color_object(const mesh& m);

    glm::vec4 get_color() const;
    void set_color(glm::vec4 color);

private:
    glm::vec4 color_{1};
};
}
