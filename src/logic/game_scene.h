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
    bool can_unit_move(const game_unit& unit, glm::ivec3 position) const;
    void shoot_unit(const game_unit& shooter, game_unit& target);

private:
    std::string name_;
    size_t current_player_id_{0};

    std::shared_ptr<game_map> map_;
    std::vector<std::shared_ptr<player>> players_;
};
}
