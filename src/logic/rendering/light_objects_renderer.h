#pragma once
#include <vector>
#include "../../graphics/model/simple_color_object.h"
#include "../../graphics/model/buffered_simple_color_object.h"

namespace tactics_game
{
class light_objects_renderer
{
public:
    explicit light_objects_renderer(std::vector<std::vector<simple_color_object>> light_objects);

    void render(shader_program& program, int layer) const;

private:
    std::vector<std::vector<simple_color_object>> light_objects_{};
    std::vector<std::vector<buffered_simple_color_object>> buffered_light_objects_{};
};
}
