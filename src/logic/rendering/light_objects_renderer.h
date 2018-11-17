#pragma once
#include <vector>
#include "../../graphics/model/light_object.h"
#include "../../graphics/model/buffered_light_object.h"

namespace tactics_game
{
class light_objects_renderer
{
public:
    explicit light_objects_renderer(std::vector<std::shared_ptr<light_object>> light_objects);

    void render(shader_program& program) const;

private:
    std::vector<std::shared_ptr<light_object>> light_objects_{};
    std::vector<buffered_light_object> buffered_light_objects_{};
};
}
