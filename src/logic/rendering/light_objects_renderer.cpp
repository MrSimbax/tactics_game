#include "light_objects_renderer.h"

using namespace tactics_game;

light_objects_renderer::light_objects_renderer(std::vector<std::vector<std::shared_ptr<simple_color_object>>> light_objects)
    : light_objects_{std::move(light_objects)}
{
    buffered_light_objects_.resize(light_objects_.size());
    for (auto i = 0u; i < light_objects_.size(); ++i)
    {
        for (const auto& light : light_objects_[i])
        {
            buffered_light_objects_[i].emplace_back(light);
        }
    }
}

void light_objects_renderer::render(shader_program& program, const int layer) const
{
    for (auto i = 0; i < layer; ++i)
    {
        for (const auto& light : buffered_light_objects_[i])
        {
            light.render(program);
        }
    }
}
