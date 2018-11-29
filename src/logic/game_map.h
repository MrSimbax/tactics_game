#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "glm/vec3.hpp"
#include "glm/detail/_noise.hpp"

namespace tactics_game
{
enum class tile_type
{
    empty = 0,
    floor = 1,
    wall = 2,
    climber = 3,

    // e.g. unit
    blocked = 999
};

class game_map
{
public:
    typedef std::vector<std::vector<tile_type>> tiles_t;

    explicit game_map(std::vector<tiles_t> layers);

    glm::ivec3 get_size() const;

    const std::vector<tiles_t>& get_layers() const;
    tile_type get_tile(glm::ivec3 pos) const;

    tile_type get_static_tile(glm::ivec3 pos) const;

    void set_blocked(glm::ivec3 pos, bool blocked);

private:
    std::vector<tiles_t> layers_;
    std::vector<tiles_t> blocked_tiles_;
};
}
