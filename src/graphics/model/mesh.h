#pragma once

#include "../shader/shader_program.h"

#include <vector>

#include <glm/vec3.hpp>

namespace tactics_game
{
struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class mesh
{
public:
    mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices);

    mesh transformed(glm::mat4 transform) const;

    const std::vector<vertex>& get_vertices() const;
    const std::vector<unsigned int>& get_indices() const;

private:
    std::vector<vertex> vertices_;
    std::vector<unsigned int> indices_;
};
}
