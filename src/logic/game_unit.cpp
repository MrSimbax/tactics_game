#include "game_unit.h"
#include <utility>

using namespace tactics_game;

game_unit::game_unit(const size_t id, const size_t player_id, glm::ivec3 position)
    : id_{id},
      player_id_{player_id},
      position_{position}
{
}

size_t game_unit::get_id() const
{
    return id_;
}

void game_unit::set_id(const size_t id)
{
    id_ = id;
}

size_t game_unit::get_player_id() const
{
    return player_id_;
}

bool game_unit::is_dead() const
{
    return hit_points_ <= 0;
}

bool game_unit::turn_done() const
{
    return action_points_ <= 0;
}

bool game_unit::can_shoot() const
{
    return action_points_ > 0;
}

bool game_unit::can_move() const
{
    return action_points_ > 0;
}

bool game_unit::is_visible() const
{
    return is_visible_;
}

void game_unit::set_visible(const bool is_visible)
{
    is_visible_ = is_visible;
}

int game_unit::get_hit_points() const
{
    return hit_points_;
}

void game_unit::set_hit_points(const int hit_points)
{
    hit_points_ = hit_points;
}

int game_unit::get_action_points() const
{
    return action_points_;
}

void game_unit::set_action_points(const int action_points)
{
    action_points_ = action_points;
}

glm::ivec3 game_unit::get_position() const
{
    return position_;
}

void game_unit::set_position(const glm::ivec3 position)
{
    position_ = position;
}

const path_finder::paths_t& game_unit::get_movable_tiles() const
{
    return movable_tiles_;
}

void game_unit::set_movable_tiles(const path_finder::paths_t paths)
{
    this->movable_tiles_ = paths;
}
