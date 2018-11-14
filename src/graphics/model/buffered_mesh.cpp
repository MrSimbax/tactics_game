#include "buffered_mesh.h"
#include <utility>
#include <glad/glad.h>
#include <algorithm>
#include "glm/gtc/matrix_inverse.inl"

using namespace tactics_game;

buffered_mesh::buffered_mesh(const mesh& m)
    : indices_size_{static_cast<int>(m.get_indices().size())}
{
    set_up_buffers(m);
}

buffered_mesh::buffered_mesh(buffered_mesh&& other) noexcept
    : indices_size_{other.indices_size_},
      vbo_{std::move(other.vbo_)},
      ebo_{std::move(other.ebo_)},
      vao_{std::move(other.vao_)}
{
}

buffered_mesh& buffered_mesh::operator=(buffered_mesh&& other) noexcept
{
    indices_size_ = other.indices_size_;
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    vao_ = std::move(other.vao_);
    return *this;
}

buffered_mesh::~buffered_mesh() = default;

void buffered_mesh::render(const shader_program& program) const
{
    vao_.bind();
    glDrawElements(GL_TRIANGLES, indices_size_, GL_UNSIGNED_INT,
                   static_cast<void*>(nullptr));
    vertex_array_object::unbind();
}

void buffered_mesh::set_up_buffers(const mesh& mesh) const
{
    vao_.bind();

    vbo_.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, mesh.get_vertices().size() * sizeof(vertex), &mesh.get_vertices()[0],
                 GL_STATIC_DRAW);

    ebo_.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_indices().size() * sizeof(unsigned), &mesh.get_indices()[0],
                 GL_STATIC_DRAW);

    shader_program::set_vertex_attrib(shader_program::position_location, 3, GL_FLOAT, sizeof(vertex),
                                      offsetof(vertex, position));
    shader_program::set_vertex_attrib(shader_program::normal_location, 3, GL_FLOAT, sizeof(vertex),
                                      offsetof(vertex, normal));

    vertex_array_object::unbind();
    buffer_object::unbind(GL_ARRAY_BUFFER);
    buffer_object::unbind(GL_ELEMENT_ARRAY_BUFFER);
}
