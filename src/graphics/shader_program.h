#pragma once

#include <array>
#include <string>

#include <glad/glad.h>

namespace TacticsGame::Graphics
{

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~ShaderProgram();

    bool compile();
    void use() const;

private:
    unsigned int compileShader(const std::string& source, GLenum type);

private:
    unsigned int shaderProgramId;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    int success;
    std::array<char, 512> errorStr;
};

}
