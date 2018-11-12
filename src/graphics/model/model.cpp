#include <utility>
#include "graphics_object.h"

using namespace tactics_game;

model::model(std::vector<mesh> meshes)
    : meshes_{std::move(meshes)}
{
}

void model::render(shader_program& program) const
{
    for (const auto& mesh : meshes_)
    {
        mesh.render(program);
    }
}
