#include "shader.h"

using namespace tactics_game;

#include <glad/glad.h>
#include <utility>

shader::shader(std::string source, const unsigned int type)
    : id_{0},
      type_{type},
      source_{std::move(source)}
{
    id_ = compile();
}

shader::shader(const shader& other)
    : id_{0},
      type_{other.type_},
      source_{other.source_}
{
    id_ = compile();
}

shader::shader(shader&& other) noexcept
    : id_{other.id_},
      type_{other.type_},
      source_{std::move(other.source_)}
{
    other.id_ = 0;
}

shader& shader::operator=(const shader& other)
{
    type_ = other.type_;
    source_ = other.source_;
    id_ = compile();
    return *this;
}

shader& shader::operator=(shader&& other) noexcept
{
    id_ = other.id_;
    type_ = other.type_;
    source_ = std::move(other.source_);

    other.id_ = 0;
    return *this;
}

shader::~shader()
{
    if (!id_)
    {
        glDeleteShader(id_);;
    }
}

void shader::attach_to_program(const unsigned program_id) const
{
    glAttachShader(program_id, id_);
}

unsigned shader::get_id() const
{
    return id_;
}

unsigned int shader::compile() const
{
    const auto shader_id{glCreateShader(type_)};
    auto source_str{source_.c_str()};
    glShaderSource(shader_id, 1, &source_str, nullptr);
    glCompileShader(shader_id);

    int success{};
    std::array<char, 512> error_str{};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, static_cast<GLsizei>(error_str.size()), nullptr, &error_str[0]);
        glDeleteShader(shader_id);
        throw shader_compilation_error{&error_str[0]};
    }

    return shader_id;
}
