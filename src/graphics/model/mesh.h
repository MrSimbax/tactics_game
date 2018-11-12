#pragma once

#include "bo.h"
#include "vao.h"
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
    mesh(const mesh& other);
    mesh& operator=(const mesh& other);
    mesh(mesh&& other) noexcept;
    mesh& operator=(mesh&& other) noexcept;
    ~mesh();

    void render(const shader_program& program) const;

private:
    void set_up_buffers();

    std::vector<vertex> vertices_;
    std::vector<unsigned int> indices_;

    buffer_object vbo_{};
    buffer_object ebo_{};
    vertex_array_object vao_{};
};
}
