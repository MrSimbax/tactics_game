#include "game.h"

using namespace tactics_game;

game::game(const game_scene& scene)
    : scene_{std::move(scene)}
{
}

game_scene& game::get_scene()
{
    return scene_;
}
