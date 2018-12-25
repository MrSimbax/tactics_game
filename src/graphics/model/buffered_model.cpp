#include <utility>
#include "buffered_graphics_object.h"
#include <algorithm>

using namespace tactics_game;

buffered_model::buffered_model(const model& model)
{
    meshes_.reserve(model.get_meshes().size());
    for (const auto& mesh : model.get_meshes())
    {
        meshes_.push_back(buffered_mesh{mesh});
    }
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
