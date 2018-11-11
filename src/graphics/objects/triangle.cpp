#include "triangle.h"

#include "../../misc/custom_log.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace tactics_game;

triangle::triangle(vertices_container&& vertices)
    : vertices_{vertices}
{
    this->set_up_buffers();
}

triangle::triangle(const triangle& other)
    : vertices_{other.vertices_}
{
    this->set_up_buffers();
}

triangle& triangle::operator=(const triangle& other)
{
    this->vertices_ = other.vertices_;
    this->set_up_buffers();
    return *this;
}

triangle::triangle(triangle&& other) noexcept
    :
    vertices_{other.vertices_},
    vbo_{std::move(other.vbo_)},
    vao_{std::move(other.vao_)}
{
}

triangle& triangle::operator=(triangle&& other) noexcept
{
    this->vertices_ = other.vertices_;
    this->vao_ = std::move(other.vao_);
    this->vbo_ = std::move(other.vbo_);
    return *this;
}

void triangle::set_up_buffers()
{
    vao_.bind();
    vbo_.bind(GL_ARRAY_BUFFER);

    glBufferData(GL_ARRAY_BUFFER, this->vertices_.size() * sizeof(float), &this->vertices_[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    vertex_array_object::unbind();
    buffer_object::unbind(GL_ARRAY_BUFFER);
}

void triangle::render() const
{
    vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

triangle::~triangle() = default;
