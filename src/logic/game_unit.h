#pragma once

#include <glm/glm.hpp>

#include "path_finder.h"
#include "line_of_sight_finder.h"
#include <array>

namespace tactics_game
{
class player;

class game_unit
{
public:
    game_unit(size_t id, size_t player_id, glm::ivec3 position);

    size_t get_id() const;
    void set_id(size_t id);

    size_t get_player_id() const;

    bool is_dead() const;

    bool turn_done() const;

    bool can_shoot() const;
    bool can_move() const;

    bool is_visible() const;
    void set_visible(bool is_visible);

    int get_hit_points() const;
    void set_hit_points(int hit_points);

    int get_action_points() const;
    void set_action_points(int action_points);
    
    glm::ivec3 get_position() const;
    void set_position(glm::ivec3 position);

    const path_finder::paths_t& get_movable_tiles() const;
    void set_movable_tiles(path_finder::paths_t paths);

    const line_of_sight_finder::los_t& get_visible_tiles() const;
    void set_visible_tiles(line_of_sight_finder::los_t los);

private:
    size_t id_;
    size_t player_id_;
    int hit_points_{1};
    int action_points_{2};
    bool is_visible_{true};
    line_of_sight_finder::los_t visible_tiles_{};
    path_finder::paths_t movable_tiles_{};
    glm::ivec3 position_;
};
};
