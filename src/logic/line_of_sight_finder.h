#pragma once

#include "game_map.h"

#include <array>

#include <glm/glm.hpp>
#include <array>

namespace tactics_game
{
class line_of_sight_finder
{
public:

    enum class node_color
    {
        visible,
        not_visible
    };

    struct node
    {
        glm::ivec3 position{-1};
        node_color color = node_color::not_visible;
    };

    static const unsigned range{10};
    static const unsigned size{range * 2 + 1};
    static const unsigned center{range};
    static const unsigned range2{range * range};
    using los_t = std::vector<std::array<std::array<node, size>, size>>;

    static los_t find_los(glm::ivec3 start_pos, const game_map& map);
    static bool can_see_tile(glm::ivec3 from, glm::ivec3 to, const game_map& map, const los_t& los);
    static std::vector<glm::ivec3> get_visible_tiles(glm::ivec3 from, los_t los);

private:
    static los_t create_los(glm::ivec3 start_pos);
    static bool is_tile_in_range(glm::ivec3 pos);
    static bool is_tile_on_map(glm::ivec3 pos, const game_map& map);
    static bool is_tile_blocking_vision(glm::ivec3 start_pos, glm::ivec3 prev, glm::ivec3 pos, const game_map& map);
    static glm::ivec3 from_map_to_array_pos(glm::ivec3 start_pos, glm::ivec3 map_pos);
    static glm::ivec3 from_map_to_array_pos(glm::ivec3 start_pos, node* u);
    static glm::ivec3 from_array_to_map_pos(glm::ivec3 start_pos, glm::ivec3 array_pos);

    static void find_line(
        glm::ivec3 from, glm::ivec3 to,
        los_t& los,
        glm::ivec3 start_pos,
        const game_map& map);

    static std::vector<glm::ivec3> find_all_sidesteps(glm::ivec3 pos, const game_map& map);
    static bool is_sidestep(const game_map& map, glm::ivec3 candidate, glm::ivec3 candidate_front_1,
                            glm::ivec3 candidate_cover_1, glm::ivec3 candidate_front_2, glm::ivec3 candidate_cover_2);
    static std::vector<glm::ivec3> find_covers_from_sidestep(glm::ivec3 pos, const game_map& map);
    static void reveal_covers_from_sidestep(los_t& los, glm::ivec3 start_pos, const game_map& map, glm::ivec3 p);
};
}
