#pragma once

#include <glm/glm.hpp>

#include "../graphics/model/buffered_graphics_object.h"

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

    bool is_visible() const;
    void set_visible(bool is_visible);

    unsigned get_hit_points() const;
    void set_hit_points(unsigned hit_points);
    
    glm::ivec3 get_position() const;
    void set_position(const glm::ivec3 position);

private:
    size_t id_;
    size_t player_id_;
    unsigned int hit_points_{1};
    bool is_visible_{true};
    glm::ivec3 position_;
};
};
