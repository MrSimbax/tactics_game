#include "mesh.h"
#include <utility>
#include <algorithm>
#include "glm/gtc/matrix_inverse.inl"

using namespace tactics_game;

mesh::mesh(std::vector<vertex> vertices, std::vector<unsigned> indices, material material)
    : vertices_{std::move(vertices)},
      indices_{std::move(indices)},
      material_{material}
{
}

mesh mesh::transformed(glm::mat4 transform) const
{
    std::vector<vertex> transformed_vertices{};
    transformed_vertices.reserve(vertices_.size());
    auto inversed_transform = inverseTranspose(glm::mat3(transform));
    std::transform(vertices_.begin(), vertices_.end(), std::back_inserter(transformed_vertices), [&](const vertex& v)
    {
        vertex transformed_vertex{};
        transformed_vertex.position = transform * glm::vec4(v.position, 1.0f);
        transformed_vertex.normal = inversed_transform * v.normal;
        return transformed_vertex;
    });
    return mesh{transformed_vertices, indices_, material_};
}

const std::vector<vertex>& mesh::get_vertices() const
{
    return vertices_;
}

const std::vector<unsigned>& mesh::get_indices() const
{
    return indices_;
}

const material& mesh::get_material() const
{
    return material_;
}
