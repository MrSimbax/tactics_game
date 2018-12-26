#include "quad_xz.h"

#include "../../shader/shader_program.h"

#include <glad/glad.h>

using namespace tactics_game;

quad_xz::quad_xz(const glm::vec2 min, const glm::vec2 max, const unsigned y)
{
    generate_vertices(min, max, y);
    set_up_buffers();
}

void quad_xz::render(const shader_program& program) const
{
    vao_.bind();
    texture_.bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
    texture::unbind();
    vertex_array_object::unbind();
}

void quad_xz::set_texture(texture texture)
{
    texture_ = std::move(texture);
}

void quad_xz::generate_vertices(const glm::vec2 min, const glm::vec2 max, const unsigned y)
{
    vertices_.push_back({{min.x, y, min.y}, {0.0f, 0.0f}});
    vertices_.push_back({{min.x, y, max.y}, {0.0f, 1.0f}});
    vertices_.push_back({{max.x, y, min.y}, {1.0f, 0.0f}});
    vertices_.push_back({{max.x, y, max.y}, {1.0f, 1.0f}});
    indices_.push_back(0);
    indices_.push_back(1);
    indices_.push_back(2);
    indices_.push_back(1);
    indices_.push_back(2);
    indices_.push_back(3);
}

void quad_xz::set_up_buffers() const
{
    vao_.bind();

    vbo_.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(quad_vertex), &vertices_[0], GL_STATIC_DRAW);

    ebo_.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned), &indices_[0], GL_STATIC_DRAW);

    shader_program::set_vertex_attrib(shader_program::position_location, 3, GL_FLOAT, sizeof(quad_vertex),
                                      offsetof(quad_vertex, position));
    shader_program::set_vertex_attrib(shader_program::tex_coords_location, 3, GL_FLOAT, sizeof(quad_vertex),
                                      offsetof(quad_vertex, tex_coords));

    vertex_array_object::unbind();
    buffer_object::unbind(GL_ARRAY_BUFFER);
    buffer_object::unbind(GL_ELEMENT_ARRAY_BUFFER);
}
