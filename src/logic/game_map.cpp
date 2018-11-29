#include "game_map.h"
#include <utility>
#include <plog/Log.h>

#include "../misc/custom_log.h"

using namespace tactics_game;

game_map::game_map(std::vector<tiles_t> layers)
    : layers_{std::move(layers)},
      blocked_tiles_{layers_}
{
}

glm::ivec3 game_map::get_size() const
{
    if (layers_.empty()) return glm::ivec3(0);
    if (layers_[0].empty()) return glm::ivec3(layers_[0].size(), layers_.size(), 0);
    return glm::ivec3{layers_[0].size(), layers_.size(), layers_[0][0].size()};
}

const std::vector<game_map::tiles_t>& game_map::get_layers() const
{
    return layers_;
}

tile_type game_map::get_tile(const glm::ivec3 pos) const
{
    return blocked_tiles_[pos.y][pos.x][pos.z];
}

tile_type game_map::get_static_tile(const glm::ivec3 pos) const
{
    return layers_[pos.y][pos.x][pos.z];
}

void game_map::set_blocked(const glm::ivec3 pos, const bool blocked)
{
    if (!blocked)
    {
        blocked_tiles_[pos.y][pos.x][pos.z] = layers_[pos.y][pos.x][pos.z];
    }
    else
    {
        blocked_tiles_[pos.y][pos.x][pos.z] = tile_type::blocked;
    }
}
