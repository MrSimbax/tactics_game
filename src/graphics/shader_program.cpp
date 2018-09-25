#include "shader_program.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace TacticsGame::Graphics;

ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) :
    vertexShaderSource{vertexShaderSource},
    fragmentShaderSource{fragmentShaderSource},
    shaderProgramId{ 0 },
    success{0},
    errorStr{""}
{
}

ShaderProgram::~ShaderProgram()
{
    if (!this->shaderProgramId)
    {
        glDeleteProgram(this->shaderProgramId);
    }
}

bool ShaderProgram::compile()
{
    unsigned int vertexShaderId{ this->compileShader(this->vertexShaderSource, GL_VERTEX_SHADER) };
    if (!vertexShaderId)
    {
        LOG_ERROR << "Could not compile shader program: VERTEX shader compilation failed";
        return false;
    }

    unsigned int fragmentShaderId{ this->compileShader(this->fragmentShaderSource, GL_FRAGMENT_SHADER) };
    if (!fragmentShaderId)
    {
        LOG_ERROR << "Could not compile shader program: FRAGMENT shader compilation failed";
        return false;
    }

    this->shaderProgramId = glCreateProgram();
    glAttachShader(this->shaderProgramId, vertexShaderId);
    glAttachShader(this->shaderProgramId, fragmentShaderId);
    glLinkProgram(this->shaderProgramId);
    glGetProgramiv(this->shaderProgramId, GL_LINK_STATUS, &this->success);
    if (!this->success)
    {
        glGetProgramInfoLog(this->shaderProgramId, static_cast<GLsizei>(this->errorStr.size()), nullptr, &this->errorStr[0]);
        LOG_FATAL << "Shader program compilation failed: " << &this->errorStr[0];

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        glDeleteProgram(this->shaderProgramId);
        this->shaderProgramId = 0;

        return false;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return true;
}

void ShaderProgram::use() const
{
    if (!this->shaderProgramId)
    {
        LOG_WARNING << "Attempting to use program which was not compiled correctly.";
    }
    else
    {
        glUseProgram(this->shaderProgramId);
    }
}

unsigned int ShaderProgram::compileShader(const std::string& source, GLenum type)
{
    unsigned int shaderId{ glCreateShader(type) };
    const char* sourceStr{ source.c_str() };
    glShaderSource(shaderId, 1, &sourceStr, nullptr);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &this->success);
    if (!this->success)
    {
        glGetShaderInfoLog(shaderId, static_cast<GLsizei>(this->errorStr.size()), nullptr, &this->errorStr[0]);
        LOG_ERROR << "Shader compilation failed: " << &this->errorStr[0];
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}
