#pragma once
#include "game_unit.h"

namespace tactics_game
{
class player
{
public:
    player(size_t id, std::vector<std::shared_ptr<game_unit>> units);

    size_t get_id() const;
    void set_id(size_t id);

    std::vector<std::shared_ptr<game_unit>>& get_units();

private:
    size_t id_;
    std::vector<std::shared_ptr<game_unit>> units_;
};
}
