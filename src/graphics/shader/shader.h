#pragma once
#include <stdexcept>
#include <array>

namespace tactics_game
{
class shader
{
public:
    explicit shader(std::string source, unsigned int type);
    shader(const shader& other);
    shader(shader&& other) noexcept;
    shader& operator=(const shader& other);
    shader& operator=(shader&& other) noexcept;
    ~shader();

    void attach_to_program(unsigned int program_id) const;
    unsigned int get_id() const;

private:
    unsigned int compile() const;

    unsigned int id_;
    unsigned int type_{};
    std::string source_;
};

class shader_compilation_error : std::runtime_error
{
public:
    explicit shader_compilation_error(const char* what)
        : runtime_error{what}
    {
    }
};
}
