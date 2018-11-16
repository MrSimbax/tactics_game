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

bool game_unit::is_visible() const
{
    return is_visible_;
}

void game_unit::set_visible(const bool is_visible)
{
    is_visible_ = is_visible;
}

unsigned game_unit::get_hit_points() const
{
    return hit_points_;
}

void game_unit::set_hit_points(const unsigned hit_points)
{
    hit_points_ = hit_points;
}

glm::ivec3 game_unit::get_position() const
{
    return position_;
}

void game_unit::set_position(const glm::ivec3 position)
{
    position_ = position;
}
