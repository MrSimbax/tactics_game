#include "bo.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace tactics_game;

buffer_object::buffer_object()
    : id_{0}
{
    create_buffer();
}

buffer_object::buffer_object(buffer_object&& other) noexcept
    : id_{other.id_}
{
    other.id_ = 0;
}

buffer_object& buffer_object::operator=(buffer_object&& other) noexcept
{
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

buffer_object::~buffer_object()
{
    if (!id_)
    {
        glDeleteBuffers(1, &id_);
    }
}

void buffer_object::create_buffer()
{
    glGenBuffers(1, &this->id_);
    if (!this->id_)
    {
        throw buffer_object_not_created_error{"glGenBuffers failed"};
    }
}

unsigned int buffer_object::get_id() const
{
    return id_;
}

void buffer_object::bind(const int target) const
{
    glBindBuffer(target, this->id_);
}

void buffer_object::unbind(const int target)
{
    glBindBuffer(target, 0);
}
