#include "player.h"
#include <utility>

using namespace tactics_game;

player::player(const size_t id, std::vector<std::shared_ptr<game_unit>> units)
    : id_(id),
      units_(std::move(units))
{
}

size_t player::get_id() const
{
    return id_;
}

void player::set_id(const size_t id)
{
    id_ = id;
}

std::vector<std::shared_ptr<game_unit>>& player::get_units()
{
    return units_;
}
