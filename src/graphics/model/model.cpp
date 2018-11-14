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

void model::add_mesh(const mesh& mesh)
{
    meshes_.push_back(mesh);
}

model model::transformed(const glm::mat4 transform) const
{
    std::vector<mesh> transformed_meshes{};
    transformed_meshes.reserve(meshes_.size());
    std::transform(meshes_.begin(), meshes_.end(), std::back_inserter(transformed_meshes), [&](const mesh& m)
    {
        return m.transformed(transform);
    });
    return model{transformed_meshes};
}

mesh model::merged() const
{
    std::vector<vertex> all_vertices;
    std::vector<unsigned> all_indices;
    for (const auto& mesh : meshes_)
    {
        const auto vertices_size = all_vertices.size();
        all_vertices.insert(all_vertices.end(), mesh.get_vertices().begin(), mesh.get_vertices().end());

        std::transform(mesh.get_indices().begin(), mesh.get_indices().end(), std::back_inserter(all_indices),
                       [&](unsigned index)
                       {
                           return vertices_size + index;
                       });
    }
    return mesh{all_vertices, all_indices};
}
