#include "path_finder.h"

#include <queue>
#include <array>

using namespace tactics_game;

path_finder::paths_t path_finder::find_paths(const glm::ivec3 start_pos, const game_map& map)
{
    auto paths = create_paths(start_pos);
    std::queue<node*> queue{};

    auto* start = &paths[center][center][center];
    start->position = start_pos;
    start->color = node_color::neighbors_not_visited;
    start->dist = 0;
    start->previous = nullptr;
    queue.push(start);

    while (!queue.empty())
    {
        auto u = queue.front();
        queue.pop();
        if (u->dist + 1 <= static_cast<int>(range))
        {
            auto neighbors = get_neighbors(paths, start_pos, u, map);
            for (auto v : neighbors)
            {
                if (v->color == node_color::not_visited)
                {
                    v->color = node_color::neighbors_not_visited;
                    v->dist = u->dist + 1;
                    v->previous = u;
                    queue.push(v);
                }
            }
        }
        u->color = node_color::visited;
    }

    return paths;
}

bool path_finder::can_move_to_tile(const glm::ivec3 from, const glm::ivec3 to, const game_map& map, const paths_t& paths)
{
    const auto to_array = from_map_to_array_pos(from, to);
    return is_tile_on_map(to, map) && is_tile_in_range(to_array) && paths[to_array.y][to_array.x][to_array.z].dist > 0;
}

std::vector<glm::ivec3> path_finder::get_movable_tiles(const glm::ivec3 from, paths_t paths)
{
    std::vector<glm::ivec3> tiles;
    for (auto y = 0u; y < size; ++y)
    {
        for (auto x = 0u; x < size; ++x)
        {
            for (auto z = 0u; z < size; ++z)
            {
                const glm::ivec3 array_pos{x, y, z};
                const auto node = &paths[y][x][z];
                if (node->dist > 0)
                {
                    tiles.push_back(from_array_to_map_pos(from, array_pos));
                }
            }
        }
    }
    return tiles;
}

path_finder::paths_t path_finder::create_paths(const glm::ivec3 start_pos)
{
    paths_t paths;
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

bool path_finder::is_tile_in_range(const glm::ivec3 pos)
{
    return (pos.x >= 0 && pos.x < static_cast<int>(size) &&
        pos.y >= 0 && pos.y < static_cast<int>(size) &&
        pos.z >= 0 && pos.z < static_cast<int>(size));
}

bool path_finder::is_tile_on_map(const glm::ivec3 pos, const game_map& map)
{
    const auto size = map.get_size();
    return (pos.x >= 0 && pos.x < size.x &&
        pos.y >= 0 && pos.y < size.y &&
        pos.z >= 0 && pos.z < size.z);
}

std::vector<path_finder::node*> path_finder::get_neighbors(paths_t& paths, const glm::ivec3 start_pos, node* u,
                                                           const game_map& map)
{
    std::vector<node*> neighbors;
    const auto array_pos = from_map_to_array_pos(start_pos, u);

    // up
    auto pos = glm::ivec3(array_pos.x, array_pos.y, array_pos.z + 1);
    auto map_pos = from_array_to_map_pos(start_pos, pos);
    if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && is_tile_movable(map_pos, map))
    {
        neighbors.push_back(&paths[pos.y][pos.x][pos.z]);
    }

    // right
    pos = glm::ivec3(array_pos.x + 1, array_pos.y, array_pos.z);
    map_pos = from_array_to_map_pos(start_pos, pos);
    if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && is_tile_movable(map_pos, map))
    {
        neighbors.push_back(&paths[pos.y][pos.x][pos.z]);
    }

    // down
    pos = glm::ivec3(array_pos.x, array_pos.y, array_pos.z - 1);
    map_pos = from_array_to_map_pos(start_pos, pos);
    if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && is_tile_movable(map_pos, map))
    {
        neighbors.push_back(&paths[pos.y][pos.x][pos.z]);
    }

    // left
    pos = glm::ivec3(array_pos.x - 1, array_pos.y, array_pos.z);
    map_pos = from_array_to_map_pos(start_pos, pos);
    if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && is_tile_movable(map_pos, map))
    {
        neighbors.push_back(&paths[pos.y][pos.x][pos.z]);
    }

    if (map.get_tile(from_array_to_map_pos(start_pos, array_pos)) == tile_type::climber ||
        (glm::ivec3(center, center, center) == array_pos && map.get_static_tile(from_array_to_map_pos(start_pos, array_pos)) == tile_type::climber))
    {
        // top
        pos = glm::ivec3(array_pos.x, array_pos.y + 1, array_pos.z);
        map_pos = from_array_to_map_pos(start_pos, pos);
        if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && map.get_tile(map_pos) == tile_type::climber)
            neighbors.push_back(&paths[pos.y][pos.x][pos.z]);

        // bottom
        pos = glm::ivec3(array_pos.x, array_pos.y - 1, array_pos.z);
        map_pos = from_array_to_map_pos(start_pos, pos);
        if (is_tile_in_range(pos) && is_tile_on_map(map_pos, map) && map.get_tile(map_pos) == tile_type::climber)
            neighbors.push_back(&paths[pos.y][pos.x][pos.z]);
    }

    return neighbors;
}

bool path_finder::is_tile_movable(const glm::ivec3 pos, const game_map& map)
{
    const auto tile = map.get_tile(pos);
    return (tile != tile_type::wall && tile != tile_type::empty && tile != tile_type::blocked) || tile == tile_type::climber;
}

glm::ivec3 path_finder::from_map_to_array_pos(const glm::ivec3 start_pos, const glm::ivec3 map_pos)
{
    return (map_pos - start_pos) + static_cast<int>(center);
}

glm::ivec3 path_finder::from_map_to_array_pos(const glm::ivec3 start_pos, node* u)
{
    return (u->position - start_pos) + static_cast<int>(center);
}

glm::ivec3 path_finder::from_array_to_map_pos(const glm::ivec3 start_pos, const glm::ivec3 array_pos)
{
    return (array_pos - static_cast<int>(center)) + start_pos;
}
