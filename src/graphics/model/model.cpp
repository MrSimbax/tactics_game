#include "model.h"

#include <utility>
#include <algorithm>
#include <iterator>

using namespace tactics_game;

model::model() = default;

model::model(std::vector<std::shared_ptr<mesh>> meshes)
    : meshes_{std::move(meshes)}
{
}

const std::vector<std::shared_ptr<mesh>>& model::get_meshes() const
{
    return meshes_;
}

model model::transformed(glm::mat4 transform) const
{
    std::vector<std::shared_ptr<mesh>> transformed_meshes{};
    transformed_meshes.reserve(meshes_.size());
    std::transform(meshes_.begin(), meshes_.end(), std::back_inserter(transformed_meshes),
                   [&](const std::shared_ptr<mesh>& m)
                   {
                       return std::make_shared<mesh>(m->transformed(transform));
                   });
    return model{transformed_meshes};
}

std::shared_ptr<mesh> model::merged() const
{
    std::vector<vertex> all_vertices;
    std::vector<unsigned> all_indices;
    for (const auto& mesh : meshes_)
    {
        const auto vertices_size = all_vertices.size();
        all_vertices.insert(all_vertices.end(), mesh->get_vertices().begin(), mesh->get_vertices().end());

        std::transform(mesh->get_indices().begin(), mesh->get_indices().end(), std::back_inserter(all_indices),
                       [&](const size_t index)
                       {
                           return static_cast<unsigned>(vertices_size + index);
                       });
    }
    return std::make_shared<mesh>(all_vertices, all_indices, meshes_[0]->get_material());
}

void model::add_mesh(std::shared_ptr<mesh> mesh)
{
    meshes_.push_back(std::move(mesh));
}
