#include "line_of_sight_finder.h"

#include <array>
#include <vector>
#include <plog/Log.h>
#include "../misc/custom_log.h"
#include <cstdlib>
#include "../misc/utils.h"
#include <vector>

using namespace tactics_game;

line_of_sight_finder::los_t line_of_sight_finder::find_los(const glm::ivec3 start_pos, const game_map& map)
{
    auto los = create_los(start_pos);

    los[center][center][center].color = node_color::visible;

    auto sidesteps = find_all_sidesteps(start_pos, map);
    for (auto& sidestep : sidesteps)
        sidestep = from_map_to_array_pos(start_pos, sidestep);

    // Cast line to each wall of the virtual cube imitating range of sight
    for (auto y = 0; y < static_cast<int>(size); y += size - 1)
        for (auto x = 0; x < static_cast<int>(size); ++x)
            for (auto z = 0; z < static_cast<int>(size); ++z)
            {
                find_line(glm::ivec3{center, center, center}, glm::ivec3{x, y, z}, los, start_pos, map);
                for (const auto& sidestep : sidesteps)
                    find_line(sidestep, glm::ivec3{x, y, z}, los, start_pos, map);
            }
    for (auto x = 0; x < static_cast<int>(size); x += size - 1)
        for (auto y = 1; y < static_cast<int>(size) - 1; ++y)
            for (auto z = 0; z < static_cast<int>(size); ++z)
            {
                find_line(glm::ivec3{center, center, center}, glm::ivec3{x, y, z}, los, start_pos, map);
                for (const auto& sidestep : sidesteps)
                    find_line(sidestep, glm::ivec3{x, y, z}, los, start_pos, map);
            }
    for (auto z = 0; z < static_cast<int>(size); z += size - 1)
        for (auto x = 1; x < static_cast<int>(size) - 1; ++x)
            for (auto y = 1; y < static_cast<int>(size) - 1; ++y)
            {
                find_line(glm::ivec3{center, center, center}, glm::ivec3{x, y, z}, los, start_pos, map);
                for (const auto& sidestep : sidesteps)
                    find_line(sidestep, glm::ivec3{x, y, z}, los, start_pos, map);
            }

    return los;
}

bool line_of_sight_finder::can_see_tile(const glm::ivec3 from, const glm::ivec3 to, const game_map& map,
                                        const los_t& los)
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

bool line_of_sight_finder::is_tile_blocking_vision(const glm::ivec3 start_pos, const glm::ivec3 prev,
                                                   const glm::ivec3 pos, const game_map& map)
{
    if (!is_tile_on_map(pos, map))
        return true;
    const auto tile = map.get_static_tile(pos);
    const auto prev_tile = map.get_static_tile(prev);
    return (utils::distance2(start_pos, pos) > static_cast<int>(range2) ||
        prev_tile == tile_type::wall ||
        (prev.y > pos.y && prev_tile != tile_type::empty && prev_tile != tile_type::climber) ||
        (prev.y < pos.y && tile != tile_type::empty && tile != tile_type::climber));
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

void line_of_sight_finder::find_line(
    const glm::ivec3 from,
    const glm::ivec3 to,
    los_t& los,
    const glm::ivec3 start_pos,
    const game_map& map
)
{
    // Using Bresenham algorithm generalized to 3D space
    auto prev = from;
    los[from.y][from.x][from.z].color = node_color::visible;

    const auto dx = abs(from.x - to.x);
    const auto dy = abs(from.y - to.y);
    const auto dz = abs(from.z - to.z);

    // Determine the direction of the line
    int xs;
    if (to.x > from.x)
        xs = 1;
    else
        xs = -1;

    int ys;
    if (to.y > from.y)
        ys = 1;
    else
        ys = -1;

    int zs;
    if (to.z > from.z)
        zs = 1;
    else
        zs = -1;

    auto p{from};
    if (dx >= dy && dx >= dz)
    {
        // x axis is driving
        auto error1 = 2 * dy - dx;
        auto error2 = 2 * dz - dx;
        while (p.x != to.x)
        {
            p.x += xs;
            if (error1 >= 0)
            {
                p.y += ys;
                error1 -= 2 * dx;
            }
            if (error2 >= 0)
            {
                p.z += zs;
                error2 -= 2 * dx;
            }
            error1 += 2 * dy;
            error2 += 2 * dz;

            if (is_tile_blocking_vision(
                start_pos,
                from_array_to_map_pos(start_pos, prev),
                from_array_to_map_pos(start_pos, p),
                map
            ))
            {
                return;
            }
            prev = p;
            los[p.y][p.x][p.z].color = node_color::visible;
            reveal_covers_from_sidestep(los, start_pos, map, p);
        }
    }
    else if (dy >= dx && dy >= dz)
    {
        // y axis is driving
        auto error1 = 2 * dx - dy;
        auto error2 = 2 * dz - dy;
        while (p.y != to.y)
        {
            p.y += ys;
            if (error1 >= 0)
            {
                p.x += xs;
                error1 -= 2 * dy;
            }
            if (error2 >= 0)
            {
                p.z += zs;
                error2 -= 2 * dy;
            }
            error1 += 2 * dx;
            error2 += 2 * dz;

            if (is_tile_blocking_vision(
                start_pos,
                from_array_to_map_pos(start_pos, prev),
                from_array_to_map_pos(start_pos, p),
                map
            ))
            {
                return;
            }
            prev = p;
            los[p.y][p.x][p.z].color = node_color::visible;
            reveal_covers_from_sidestep(los, start_pos, map, p);
        }
    }
    else
    {
        // z axis is driving
        auto error1 = 2 * dx - dz;
        auto error2 = 2 * dy - dz;
        while (p.z != to.z)
        {
            p.z += zs;
            if (error1 >= 0)
            {
                p.x += xs;
                error1 -= 2 * dz;
            }
            if (error2 >= 0)
            {
                p.y += ys;
                error2 -= 2 * dy;
            }
            error1 += 2 * dx;
            error2 += 2 * dy;

            if (is_tile_blocking_vision(
                start_pos,
                from_array_to_map_pos(start_pos, prev),
                from_array_to_map_pos(start_pos, p),
                map
            ))
            {
                return;
            }
            prev = p;
            los[p.y][p.x][p.z].color = node_color::visible;
            reveal_covers_from_sidestep(los, start_pos, map, p);
        }
    }
}

void line_of_sight_finder::reveal_covers_from_sidestep(los_t& los, const glm::ivec3 start_pos, const game_map& map,
                                                       const glm::ivec3 p)
{
    auto covers = find_covers_from_sidestep(from_array_to_map_pos(start_pos, p), map);
    for (const auto& cover : covers)
    {
        const auto array_cover = from_map_to_array_pos(start_pos, cover);
        if (is_tile_in_range(array_cover))
        {
            los[array_cover.y][array_cover.x][array_cover.z].color = node_color::visible;
        }
    }
}

std::vector<glm::ivec3> line_of_sight_finder::find_all_sidesteps(const glm::ivec3 pos, const game_map& map)
{
    std::vector<glm::ivec3> sidesteps;

    // 4 cases (u is unit):
    // a 1 b
    // 4 u 2
    // c 3 d
    // f.e. 4 is sidestep if 1 is wall and a is not a wall

    const glm::ivec3 a{pos.x - 1, pos.y, pos.z + 1};
    const glm::ivec3 b{pos.x + 1, pos.y, pos.z + 1};
    const glm::ivec3 c{pos.x - 1, pos.y, pos.z - 1};
    const glm::ivec3 d{pos.x + 1, pos.y, pos.z - 1};
    const glm::ivec3 one{pos.x, pos.y, pos.z + 1};
    const glm::ivec3 two{pos.x + 1, pos.y, pos.z};
    const glm::ivec3 three{pos.x, pos.y, pos.z - 1};
    const glm::ivec3 four{pos.x - 1, pos.y, pos.z};

    if (is_sidestep(map, one, a, four, b, two))
        sidesteps.push_back(one);
    if (is_sidestep(map, two, b, one, d, three))
        sidesteps.push_back(two);
    if (is_sidestep(map, three, c, four, d, two))
        sidesteps.push_back(three);
    if (is_sidestep(map, four, a, one, c, three))
        sidesteps.push_back(four);

    return sidesteps;
}

bool line_of_sight_finder::is_sidestep(const game_map& map, const glm::ivec3 candidate,
                                       const glm::ivec3 candidate_front_1, const glm::ivec3 candidate_cover_1,
                                       const glm::ivec3 candidate_front_2, const glm::ivec3 candidate_cover_2)
{
    return (is_tile_on_map(candidate, map) && map.get_static_tile(candidate) != tile_type::wall)
        &&
        (
            (is_tile_on_map(candidate_front_1, map) && map.get_static_tile(candidate_front_1) != tile_type::wall &&
                is_tile_on_map(candidate_cover_1, map) && map.get_static_tile(candidate_cover_1) == tile_type::wall)
            ||
            (is_tile_on_map(candidate_front_2, map) && map.get_static_tile(candidate_front_2) != tile_type::wall &&
                is_tile_on_map(candidate_cover_2, map) && map.get_static_tile(candidate_cover_2) == tile_type::wall)
        );
}

std::vector<glm::ivec3> line_of_sight_finder::find_covers_from_sidestep(const glm::ivec3 pos, const game_map& map)
{
    std::vector<glm::ivec3> covers;

    // 4 cases (s is sidestep):
    // a 1 b
    // 4 s 2
    // c 3 d
    // f.e. 4 is cover if a is wall and 1 is not a wall

    const glm::ivec3 a{pos.x - 1, pos.y, pos.z + 1};
    const glm::ivec3 b{pos.x + 1, pos.y, pos.z + 1};
    const glm::ivec3 c{pos.x - 1, pos.y, pos.z - 1};
    const glm::ivec3 d{pos.x + 1, pos.y, pos.z - 1};
    const glm::ivec3 one{pos.x, pos.y, pos.z + 1};
    const glm::ivec3 two{pos.x + 1, pos.y, pos.z};
    const glm::ivec3 three{pos.x, pos.y, pos.z - 1};
    const glm::ivec3 four{pos.x - 1, pos.y, pos.z};

    if (is_sidestep(map, pos, four, a, two, b))
        covers.push_back(one);
    if (is_sidestep(map, pos, one, b, three, d))
        covers.push_back(two);
    if (is_sidestep(map, pos, four, c, two, d))
        covers.push_back(three);
    if (is_sidestep(map, pos, one, a, three, c))
        covers.push_back(four);

    return covers;
}
