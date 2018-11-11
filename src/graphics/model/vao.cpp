#include "vao.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace tactics_game;

vertex_array_object::vertex_array_object()
    : id_{0}
{
    create_array();
}

vertex_array_object::vertex_array_object(vertex_array_object&& other) noexcept
    :
    id_{other.id_}
{
    other.id_ = 0;
}

vertex_array_object& vertex_array_object::operator=(vertex_array_object&& other) noexcept
{
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

vertex_array_object::~vertex_array_object()
{
    if (!id_)
    {
        glDeleteVertexArrays(1, &id_);
    }
}

void vertex_array_object::create_array()
{
    glGenVertexArrays(1, &id_);
    if (!id_)
    {
        throw vertex_array_object_not_created_error("glGenVertexArrays failed");
    }
}

unsigned int vertex_array_object::get_id() const
{
    return id_;
}

void vertex_array_object::bind() const
{
    glBindVertexArray(this->id_);
}

void vertex_array_object::unbind()
{
    glBindVertexArray(0);
}
