#pragma once

#include <string>
#include "shader.h"

namespace tactics_game
{
class shader_program
{
public:
    shader_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
    shader_program(const shader_program& other);
    shader_program(shader_program&& other) noexcept;
    shader_program& operator=(const shader_program& other);
    shader_program& operator=(shader_program&& other) noexcept;
    ~shader_program();

    void use() const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;

private:
    void link();

    unsigned int id_;

    shader vertex_shader_;
    shader fragment_shader_;
};

class shader_program_linking_error : std::runtime_error
{
public:
    explicit shader_program_linking_error(const char* what)
        : runtime_error{what}
    {
    }
};
}
