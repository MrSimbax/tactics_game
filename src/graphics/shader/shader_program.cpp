#include "shader_program.h"

#include <utility>
#include <array>

#include "shader.h"

#include <glad/glad.h>
#include <plog/Log.h>
#include <glm/gtc/type_ptr.inl>

using namespace tactics_game;

shader_program::shader_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
    : id_{0},
      vertex_shader_{vertex_shader_source, GL_VERTEX_SHADER},
      fragment_shader_{fragment_shader_source, GL_FRAGMENT_SHADER}
{
    link();
}

shader_program::shader_program(const shader_program& other)
    : id_{0},
      vertex_shader_{other.vertex_shader_},
      fragment_shader_{other.fragment_shader_}
{
    link();
}

shader_program::shader_program(shader_program&& other) noexcept
    : id_{other.id_},
      vertex_shader_{std::move(other.vertex_shader_)},
      fragment_shader_{std::move(other.fragment_shader_)}
{
    other.id_ = 0;
}

shader_program& shader_program::operator=(const shader_program& other)
{
    id_ = 0;
    vertex_shader_ = other.vertex_shader_;
    fragment_shader_ = other.fragment_shader_;
    link();
    return *this;
}

shader_program& shader_program::operator=(shader_program&& other) noexcept
{
    id_ = other.id_;
    vertex_shader_ = std::move(other.vertex_shader_);
    fragment_shader_ = std::move(other.fragment_shader_);
    other.id_ = 0;
    return *this;
}

shader_program::~shader_program()
{
    if (!id_)
    {
        glDeleteProgram(id_);
    }
}

void shader_program::set_vertex_attrib(const int index, const int size, const unsigned type, const int stride,
                                       const size_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
}

void shader_program::link()
{
    id_ = glCreateProgram();
    vertex_shader_.attach_to_program(id_);
    fragment_shader_.attach_to_program(id_);
    glLinkProgram(id_);

    int success{};
    std::array<char, 512> error_str{};
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id_, static_cast<GLsizei>(error_str.size()), nullptr,
                            &error_str[0]);
        glDeleteProgram(id_);
        throw shader_program_linking_error{&error_str[0]};
    }
}

void shader_program::use() const
{
    if (!id_)
    {
        LOG_WARNING << "Attempting to use program which was not compiled correctly.";
    }
    else
    {
        glUseProgram(id_);
    }
}

void shader_program::set_bool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void shader_program::set_int(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void shader_program::set_float(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void shader_program::set_mat3(const std::string& name, glm::mat3 value) const
{
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void shader_program::set_mat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void shader_program::set_vec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value_ptr(value));
}

void shader_program::set_vec4(const std::string& name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, value_ptr(value));
}
