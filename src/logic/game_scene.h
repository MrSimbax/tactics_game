#pragma once
#include "game_map.h"
#include "player.h"

namespace tactics_game
{
class game_scene
{
public:
    game_scene();
    game_scene(std::string name, std::shared_ptr<game_map> map, std::vector<std::shared_ptr<player>> players);

    std::shared_ptr<game_map> get_game_map() const;
    std::vector<std::shared_ptr<player>>& get_players();
    const std::string& get_name() const;

    size_t get_current_player_id() const;
    void set_current_player_id(size_t i);

    float calculate_shooting_probability(const game_unit& shooter, const game_unit& target) const;
    bool can_unit_shoot(const game_unit& shooter, const game_unit& target) const;
    void shoot_unit(game_unit& shooter, game_unit& target);

    bool can_unit_move(const game_unit& unit, glm::ivec3 position) const;
    void move_unit(game_unit& unit, glm::ivec3 position);

    bool start_new_turn();

    bool has_any_unit_any_action_left() const;
private:

    void update_movable_tiles();
    void calculate_movable_tiles_for(game_unit& unit) const;

    void update_visible_tiles();
    void calculate_visible_tiles_for(game_unit& unit) const;
    bool can_unit_see(const game_unit& unit, const game_unit& other) const;

    std::string name_;
    size_t current_player_id_;

    std::shared_ptr<game_map> map_;
    std::vector<std::shared_ptr<player>> players_;
};
}
