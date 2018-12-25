#include "model.h"

#include <utility>
#include <algorithm>
#include <iterator>

using namespace tactics_game;

model::model() = default;

model::model(std::vector<mesh> meshes)
    : meshes_{std::move(meshes)}
{
}

const std::vector<mesh>& model::get_meshes() const
{
    return meshes_;
}

model model::transformed(glm::mat4 transform) const
{
    std::vector<mesh> transformed_meshes{};
    transformed_meshes.reserve(meshes_.size());
    for (const auto& mesh : meshes_)
    {
        transformed_meshes.push_back(mesh.transformed(transform));
    }
    return model{transformed_meshes};
}

mesh model::merged() const
{
    if (meshes_.size() == 1)
        return meshes_[0];
    std::vector<vertex> all_vertices;
    std::vector<unsigned> all_indices;
    for (const auto& mesh : meshes_)
    {
        const auto vertices_size = all_vertices.size();
        all_vertices.insert(all_vertices.end(), mesh.get_vertices().begin(), mesh.get_vertices().end());
        for (const auto& index : mesh.get_indices())
        {
            all_indices.push_back(static_cast<unsigned>(vertices_size + index));
        }
    }
    return mesh{all_vertices, all_indices, meshes_[0].get_material()};
}

void model::add_mesh(mesh mesh)
{
    meshes_.push_back(std::move(mesh));
}
