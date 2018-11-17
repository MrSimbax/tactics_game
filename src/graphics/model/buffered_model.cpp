#include <utility>
#include "buffered_graphics_object.h"
#include <algorithm>

using namespace tactics_game;

buffered_model::buffered_model(const model& model)
{
    meshes_.reserve(model.get_meshes().size());
    std::transform(model.get_meshes().begin(), model.get_meshes().end(), std::back_inserter(meshes_),
                   [](const std::shared_ptr<mesh> mesh)
                   {
                       return buffered_mesh{mesh};
                   });
}

buffered_model::buffered_model(std::vector<buffered_mesh>&& meshes)
    : meshes_{std::move(meshes)}
{
}

void buffered_model::render(shader_program& program) const
{
    for (const auto& mesh : meshes_)
    {
        mesh.render(program);
    }
}
