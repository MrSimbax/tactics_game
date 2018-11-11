#include "shader_program.h"

#include <glad/glad.h>
#include <plog/Log.h>
#include <utility>

using namespace tactics_game;

shader_program::shader_program(std::string vertex_shader_source, std::string fragment_shader_source)
    : shader_program_id_{0},
      vertex_shader_source_{std::move(vertex_shader_source)},
      fragment_shader_source_{std::move(fragment_shader_source)},
      success_{0},
      error_str_{""}
{
    compile();
}

shader_program::shader_program(const shader_program& other)
{
}

shader_program::shader_program(shader_program&& other) noexcept
{
}

shader_program& shader_program::operator=(const shader_program& other)
{
}

shader_program& shader_program::operator=(shader_program&& other) noexcept
{
}


shader_program::~shader_program()
{
    if (!shader_program_id_)
    {
        glDeleteProgram(shader_program_id_);
    }
}

void shader_program::compile()
{
    const unsigned int vertex_shader_id{compile_shader(vertex_shader_source_, GL_VERTEX_SHADER)};
    const unsigned int fragment_shader_id{compile_shader(fragment_shader_source_, GL_FRAGMENT_SHADER)};

    shader_program_id_ = glCreateProgram();
    glAttachShader(shader_program_id_, vertex_shader_id);
    glAttachShader(shader_program_id_, fragment_shader_id);
    glLinkProgram(shader_program_id_);

    glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &success_);
    if (!success_)
    {
        glGetProgramInfoLog(shader_program_id_, static_cast<GLsizei>(error_str_.size()), nullptr,
                            &error_str_[0]);
        LOG_FATAL << "Shader program compilation failed: " << &error_str_[0];

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        glDeleteProgram(shader_program_id_);
        shader_program_id_ = 0;

        return false;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return true;
}

void shader_program::use() const
{
    if (!shader_program_id_)
    {
        LOG_WARNING << "Attempting to use program which was not compiled correctly.";
    }
    else
    {
        glUseProgram(shader_program_id_);
    }
}

unsigned int shader_program::compile_shader(const std::string& source, const unsigned int type)
{
    const auto shader_id{glCreateShader(type)};
    auto source_str{source.c_str()};
    glShaderSource(shader_id, 1, &source_str, nullptr);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success_);
    if (!success_)
    {
        glGetShaderInfoLog(shader_id, static_cast<GLsizei>(error_str_.size()), nullptr, &error_str_[0]);
        std::string message{"Shader compilation failed: "};
        message += &error_str_[0];
        glDeleteShader(shader_id);
        throw shader_compilation_error{message.c_str()};
    }

    return shader_id;
}

void shader_program::set_bool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader_program_id_, name.c_str()), static_cast<int>(value));
}

void shader_program::set_int(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shader_program_id_, name.c_str()), value);
}

void shader_program::set_float(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shader_program_id_, name.c_str()), value);
}
