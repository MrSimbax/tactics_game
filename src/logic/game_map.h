#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "glm/vec3.hpp"
#include "glm/detail/_noise.hpp"

namespace tactics_game
{
enum class field_type
{
    empty = 0,
    floor = 1,
    wall = 2,
    climber = 3
};

class game_map
{
public:
    typedef std::vector<std::vector<field_type>> fields_t;

    explicit game_map(std::vector<fields_t> layers);

    glm::ivec3 get_size() const;

    const std::vector<fields_t>& get_layers() const;
    field_type get_field(const glm::ivec3 pos) const;

private:
    std::vector<fields_t> layers_;
};
}
