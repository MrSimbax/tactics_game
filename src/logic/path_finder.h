#pragma once

#include "game_map.h"

#include <array>

#include <glm/glm.hpp>
#include <array>

namespace tactics_game
{
class path_finder
{
public:

    enum class node_color
    {
        visited,
        not_visited,
        neighbors_not_visited
    };

    struct node
    {
        glm::ivec3 position{-1};
        node_color color = node_color::not_visited;
        node* previous{nullptr};
        int dist = -1;
    };

    static const unsigned range{5};
    static const unsigned size{range * 2 + 1};
    static const unsigned center{range};
    using paths_t = std::array<std::array<std::array<node, size>, size>, size>;

    static paths_t find_paths(glm::ivec3 start_pos, const game_map& map);
    static bool can_move_to_tile(glm::ivec3 from, glm::ivec3 to, const game_map& map, const paths_t& paths);
    static std::vector<glm::ivec3> get_movable_tiles(glm::ivec3 from, paths_t paths);

private:
    static paths_t create_paths(glm::ivec3 start_pos);
    static bool is_tile_in_range(glm::ivec3 pos);
    static bool is_tile_on_map(glm::ivec3 pos, const game_map& map);
    static std::vector<node*> get_neighbors(paths_t& paths, glm::ivec3 start_pos, node* u, const game_map& map);
    static bool is_tile_movable(glm::ivec3 pos, const game_map& map);
    static glm::ivec3 from_map_to_array_pos(glm::ivec3 start_pos, glm::ivec3 map_pos);
    static glm::ivec3 from_map_to_array_pos(glm::ivec3 start_pos, node* u);
    static glm::ivec3 from_array_to_map_pos(glm::ivec3 start_pos, glm::ivec3 array_pos);
};
}
