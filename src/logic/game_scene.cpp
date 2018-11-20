#include "game_scene.h"
#include <utility>
#include "../misc/utils.h"

using namespace tactics_game;

game_scene::game_scene()
    : map_{{}}
{
}

game_scene::game_scene(std::string name, std::shared_ptr<game_map> map, std::vector<std::shared_ptr<player>> players)
    : name_{std::move(name)},
      map_{std::move(map)},
      players_{std::move(players)}
{
}

std::shared_ptr<game_map> game_scene::get_game_map() const
{
    return map_;
}

std::vector<std::shared_ptr<player>>& game_scene::get_players()
{
    return players_;
}

const std::string& game_scene::get_name() const
{
    return name_;
}

size_t game_scene::get_current_player_id() const
{
    return current_player_id_;
}

void game_scene::set_current_player_id(const size_t i)
{
    current_player_id_ = i;
}

float game_scene::calculate_shooting_probability(const game_unit& shooter, const game_unit& target) const
{
    auto shooter_pos = shooter.get_position();
    auto target_pos = target.get_position();

    // Start with 100%
    auto prob{1.0f};

    // Decrease by Manhattan distance
    const float dist = glm::abs(shooter_pos[0] - target_pos[0]) + glm::abs(shooter_pos[1] - target_pos[1]) + glm::abs(
        shooter_pos[2] - target_pos[2]);
    if (dist > 3)
    {
        prob -= (dist - 3) * 0.05f;
    }

    // Decrease by target's hover
    const auto cover_penalty = 0.4f;

    if (
            // left
        (shooter_pos[0] < target_pos[0] - 1 &&
            map_->get_field(glm::ivec3(target_pos[0] - 1, target_pos[1], target_pos[2])) == field_type::wall) ||
        // right
        (shooter_pos[0] > target_pos[0] + 1 &&
            map_->get_field(glm::ivec3(target_pos[0] + 1, target_pos[1], target_pos[2])) == field_type::wall) ||
        // up
        (shooter_pos[2] > target_pos[2] + 1 &&
            map_->get_field(glm::ivec3(target_pos[0], target_pos[1], target_pos[2] + 1)) == field_type::wall) ||
        // down
        (shooter_pos[2] < target_pos[2] - 1 &&
            map_->get_field(glm::ivec3(target_pos[0], target_pos[1], target_pos[2] - 1)) == field_type::wall)
    )
    {
        prob -= 0.4f;
    }

    // Decrease by target's height
    if (shooter_pos[1] < target_pos[1])
        prob -= 0.15f;

    // Increase by shooter's height
    if (shooter_pos[1] > target_pos[1])
        prob += 0.15f;

    return glm::clamp(prob, 0.01f, 1.0f);
}

bool game_scene::can_unit_move(const game_unit& unit, const glm::ivec3 position) const
{
    const auto field = map_->get_field(position);
    return (field != field_type::wall && field != field_type::empty);
}

void game_scene::shoot_unit(const game_unit& shooter, game_unit& target)
{
    const auto prob = calculate_shooting_probability(shooter, target);
    if (utils::random_float() < prob)
    {
        target.set_hit_points(target.get_hit_points() - 1);
    }
}

//std::vector<game_unit*> game_scene::get_visible_units_for_player(const size_t player_id)
//{
//    // My units are always visible
//    std::vector<game_unit*> visible_units;
//    auto& my_units{players_[player_id].get_units()};
//    visible_units.reserve(my_units.size());
//    for (size_t i = 0; i < my_units.size(); ++i)
//    {
//        visible_units.push_back(&my_units[i]);
//    }
//
//    // For now, just return all units
//    for (const auto& player : players_)
//    {
//        auto& other_units{players_[player_id].get_units()};
//        visible_units.reserve(other_units.size());
//        for (size_t i = 0; i < other_units.size(); ++i)
//        {
//            visible_units.push_back(&other_units[i]);
//        }
//    }
//
//    return visible_units;
//}
