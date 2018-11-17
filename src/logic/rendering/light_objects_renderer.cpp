#include "light_objects_renderer.h"

using namespace tactics_game;

light_objects_renderer::light_objects_renderer(std::vector<std::shared_ptr<light_object>> light_objects)
    : light_objects_{std::move(light_objects)}
{
    for (const auto& light : light_objects_)
    {
        buffered_light_objects_.emplace_back(light);
    }
}

void light_objects_renderer::render(shader_program& program) const
{
    program.use();
    for (const auto& light : buffered_light_objects_)
    {
        light.render(program);
    }
}
