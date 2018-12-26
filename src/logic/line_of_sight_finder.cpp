#include "line_of_sight_finder.h"

#include <glm/geometric.hpp>
#include <array>
#include <vector>
#include <plog/Log.h>
#include "../misc/custom_log.h"

using namespace tactics_game;

line_of_sight_finder::los_t line_of_sight_finder::find_los(const glm::ivec3 start_pos, const game_map& map)
{
    auto los = create_los(start_pos);

    los[center][center][center].color = node_color::visible;

    for (auto y = 0u; y < size; ++y)
    {
        if (!is_tile_on_map(from_array_to_map_pos(start_pos, glm::ivec3{center, y, center}), map))
            continue;

        for (auto x = 0u; x < size; ++x)
        {
            for (auto z = 0u; z < size; ++z)
            {
                const auto array_pos = glm::ivec3{x, y, z};
                const auto node = &los[y][x][z];
                if (is_tile_in_range(array_pos) &&
                    is_tile_on_map(node->position, map) &&
                    static_cast<unsigned>(length(glm::vec3(start_pos - node->position))) <= range)
                {
                    node->color = node_color::visible;
                }
            }
        }
    }
    return los;
}

bool line_of_sight_finder::can_see_tile(const glm::ivec3 from, const glm::ivec3 to, const game_map& map, const los_t& los)
{
    const auto to_array = from_map_to_array_pos(from, to);
    return is_tile_on_map(to, map) &&
           is_tile_in_range(to_array) &&
           los[to_array.y][to_array.x][to_array.z].color == node_color::visible;
}

std::vector<glm::ivec3> line_of_sight_finder::get_visible_tiles(const glm::ivec3 from, los_t los)
{
    std::vector<glm::ivec3> tiles;
    for (auto y = 0u; y < size; ++y)
    {
        for (auto x = 0u; x < size; ++x)
        {
            for (auto z = 0u; z < size; ++z)
            {
                const auto node = &los[y][x][z];
                if (node->color == node_color::visible)
                {
                    tiles.push_back(node->position);
                }
            }
        }
    }
    return tiles;
}

line_of_sight_finder::los_t line_of_sight_finder::create_los(const glm::ivec3 start_pos)
{
    los_t paths;
    paths.resize(size);

    for (auto y = 0u; y < size; ++y)
    {
        for (auto x = 0u; x < size; ++x)
        {
            for (auto z = 0u; z < size; ++z)
            {
                const glm::ivec3 array_pos{x, y, z};
                auto node = &paths[y][x][z];
                node->position = from_array_to_map_pos(start_pos, array_pos);
            }
        }
    }
    return paths;
}

bool line_of_sight_finder::is_tile_in_range(const glm::ivec3 pos)
{
    return (pos.x >= 0 && pos.x < static_cast<int>(size) &&
        pos.y >= 0 && pos.y < static_cast<int>(size) &&
        pos.z >= 0 && pos.z < static_cast<int>(size));
}

bool line_of_sight_finder::is_tile_on_map(const glm::ivec3 pos, const game_map& map)
{
    const auto size = map.get_size();
    return (pos.x >= 0 && pos.x < size.x &&
        pos.y >= 0 && pos.y < size.y &&
        pos.z >= 0 && pos.z < size.z);
}

bool line_of_sight_finder::is_tile_blocking_vision(const glm::ivec3 pos, const game_map& map)
{
    //const auto tile = map.get_tile(pos);
    //return (tile != tile_type::wall && tile != tile_type::empty && tile != tile_type::blocked) || tile == tile_type::climber;
    return false;
}

glm::ivec3 line_of_sight_finder::from_map_to_array_pos(const glm::ivec3 start_pos, const glm::ivec3 map_pos)
{
    return (map_pos - start_pos) + static_cast<int>(center);
}

glm::ivec3 line_of_sight_finder::from_map_to_array_pos(const glm::ivec3 start_pos, node* u)
{
    return (u->position - start_pos) + static_cast<int>(center);
}

glm::ivec3 line_of_sight_finder::from_array_to_map_pos(const glm::ivec3 start_pos, const glm::ivec3 array_pos)
{
    return (array_pos - static_cast<int>(center)) + start_pos;
}
