#pragma once

#include "../shader/shader_program.h"

#include <vector>

#include <glm/vec3.hpp>
#include "plog/Log.h"

namespace tactics_game
{
struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class mesh
{
public:
    mesh(std::vector<vertex> vertices, std::vector<unsigned> indices, material material);

    mesh transformed(glm::mat4 transform) const;

    const std::vector<vertex>& get_vertices() const;
    const std::vector<unsigned int>& get_indices() const;
    const material& get_material() const;

private:
    std::vector<vertex> vertices_;
    std::vector<unsigned int> indices_;
    material material_;
};
}
