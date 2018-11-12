#include "mesh.h"
#include <utility>
#include <glad/glad.h>

using namespace tactics_game;

mesh::mesh(std::vector<vertex> vertices, std::vector<unsigned> indices)
    : vertices_{std::move(vertices)},
      indices_{std::move(indices)}
{
    set_up_buffers();
}

mesh::mesh(const mesh& other)
    : vertices_{other.vertices_},
      indices_{other.indices_}
{
    set_up_buffers();
}

mesh& mesh::operator=(const mesh& other)
{
    this->vertices_ = other.vertices_;
    this->indices_ = other.indices_;
    set_up_buffers();
    return *this;
}

mesh::mesh(mesh&& other) noexcept
    : vertices_{std::move(other.vertices_)},
      indices_{std::move(other.indices_)},
      vbo_{std::move(other.vbo_)},
      ebo_{std::move(other.ebo_)},
      vao_{std::move(other.vao_)}
{
}

mesh& mesh::operator=(mesh&& other) noexcept
{
    vertices_ = std::move(other.vertices_);
    indices_ = std::move(other.indices_);
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    vao_ = std::move(other.vao_);
    return *this;
}

mesh::~mesh() = default;

void mesh::render(const shader_program& program) const
{
    vao_.bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
    vertex_array_object::unbind();
}

void mesh::set_up_buffers()
{
    vao_.bind();

    vbo_.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), &vertices_[0], GL_STATIC_DRAW);

    ebo_.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned), &indices_[0], GL_STATIC_DRAW);

    shader_program::set_vertex_attrib(shader_program::position_location, 3, GL_FLOAT, sizeof(vertex), offsetof(vertex, position));
    shader_program::set_vertex_attrib(shader_program::normal_location, 3, GL_FLOAT, sizeof(vertex), offsetof(vertex, normal));

    vertex_array_object::unbind();
    buffer_object::unbind(GL_ARRAY_BUFFER);
    buffer_object::unbind(GL_ELEMENT_ARRAY_BUFFER);
}
