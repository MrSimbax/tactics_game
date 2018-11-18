#include "game_map.h"
#include <utility>
#include <plog/Log.h>

#include "../misc/custom_log.h"

using namespace tactics_game;

game_map::game_map(std::vector<fields_t> layers)
    : layers_{std::move(layers)}
{
}

glm::ivec3 game_map::get_size() const
{
    if (layers_.empty()) return glm::ivec3(0);
    if (layers_[0].empty()) return glm::ivec3(layers_[0].size(), layers_.size(), 0);
    return glm::ivec3{layers_[0].size(), layers_.size(), layers_[0][0].size()};
}

const std::vector<game_map::fields_t>& game_map::get_layers() const
{
    return layers_;
}

field_type game_map::get_field(const glm::ivec3 pos) const
{
    return get_layers()[pos.y][pos.x][pos.z];
}
