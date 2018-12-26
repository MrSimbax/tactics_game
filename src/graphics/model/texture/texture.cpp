#include "texture.h"
#include <utility>
#include <glad/glad.h>

using namespace tactics_game;

texture::texture()
    : data_{{0, 0, 0, 127}},
      width_{1},
      height_{1}
{
    set_up_texture();
}

texture::texture(data_t data, const unsigned width, const unsigned height)
    : data_{std::move(data)},
      width_{width},
      height_{height}
{
    set_up_texture();
}

texture::texture(texture&& other) noexcept
    : id_{other.id_},
      data_{std::move(other.data_)},
      width_{other.width_},
      height_{other.height_}
{
    other.id_ = 0;
}

texture& texture::operator=(texture&& other) noexcept
{
    this->id_ = other.id_;
    this->data_ = std::move(other.data_),
    this->width_ = other.width_;
    this->height_ = other.height_;
    other.id_ = 0;
    return *this;
}

texture::~texture()
{
    if (id_)
    {
        glDeleteTextures(1, &id_);
    }
}

void texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::set_up_texture()
{
    glGenTextures(1, &id_);
    if (!id_)
    {
        throw texture_not_created_error("glGenTextures failed");
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data_[0]);
}


