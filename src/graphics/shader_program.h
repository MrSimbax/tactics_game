#pragma once

#include <array>
#include <string>

namespace TacticsGame::Graphics
{

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~ShaderProgram();

    bool compile();
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    unsigned int compileShader(const std::string& source, unsigned int type);

private:
    unsigned int shaderProgramId;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    int success;
    std::array<char, 512> errorStr;
};

}
