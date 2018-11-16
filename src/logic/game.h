#pragma once
#include "game_scene.h"

namespace tactics_game
{
// Game logic
class game
{
public:
    explicit game(const game_scene& scene);

    game_scene& get_scene();

private:
    game_scene scene_;
};
}
