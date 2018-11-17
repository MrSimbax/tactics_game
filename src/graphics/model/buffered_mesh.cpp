#include "buffered_mesh.h"
#include <utility>
#include <glad/glad.h>
#include <algorithm>
#include "glm/gtc/matrix_inverse.inl"

using namespace tactics_game;

buffered_mesh::buffered_mesh(std::shared_ptr<mesh> m)
    : mesh_{std::move(m)}
{
    set_up_buffers();
}

buffered_mesh::buffered_mesh(buffered_mesh&& other) noexcept
    : mesh_{std::move(other.mesh_)},
      vbo_{std::move(other.vbo_)},
      ebo_{std::move(other.ebo_)},
      vao_{std::move(other.vao_)}
{
}

buffered_mesh& buffered_mesh::operator=(buffered_mesh&& other) noexcept
{
    mesh_ = other.mesh_;
    vbo_ = std::move(other.vbo_);
    ebo_ = std::move(other.ebo_);
    vao_ = std::move(other.vao_);
    return *this;
}

buffered_mesh::~buffered_mesh() = default;

void buffered_mesh::render(const shader_program& program) const
{
    vao_.bind();
    set_material(program);
    glDrawElements(GL_TRIANGLES, static_cast<int>(mesh_->get_indices().size()), GL_UNSIGNED_INT,
                   static_cast<void*>(nullptr));
    vertex_array_object::unbind();
}

void buffered_mesh::set_up_buffers() const
{
    vao_.bind();

    vbo_.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, mesh_->get_vertices().size() * sizeof(vertex), &mesh_->get_vertices()[0],
                 GL_STATIC_DRAW);

    ebo_.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->get_indices().size() * sizeof(unsigned), &mesh_->get_indices()[0],
                 GL_STATIC_DRAW);

    shader_program::set_vertex_attrib(shader_program::position_location, 3, GL_FLOAT, sizeof(vertex),
                                      offsetof(vertex, position));
    shader_program::set_vertex_attrib(shader_program::normal_location, 3, GL_FLOAT, sizeof(vertex),
                                      offsetof(vertex, normal));

    vertex_array_object::unbind();
    buffer_object::unbind(GL_ARRAY_BUFFER);
    buffer_object::unbind(GL_ELEMENT_ARRAY_BUFFER);
}

void buffered_mesh::set_material(const shader_program& program) const
{
    program.set_vec3("mat.ambient", mesh_->get_material().ambient);
    program.set_vec3("mat.diffuse", mesh_->get_material().diffuse);
    program.set_vec3("mat.specular", mesh_->get_material().specular);
    program.set_float("mat.shininess", mesh_->get_material().shininess);
}
