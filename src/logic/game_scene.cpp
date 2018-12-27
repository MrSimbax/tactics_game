#include "game_scene.h"
#include <utility>
#include "../misc/utils.h"
#include <plog/Log.h>

using namespace tactics_game;

game_scene::game_scene()
    : current_player_id_(0),
      map_{{}}
{
}

game_scene::game_scene(std::string name, std::shared_ptr<game_map> map, std::vector<std::shared_ptr<player>> players)
    : name_{std::move(name)},
      map_{std::move(map)},
      players_{std::move(players)}
{
    current_player_id_ = players_.size();
    start_new_turn();
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
    const auto dist = glm::abs(shooter_pos[0] - target_pos[0]) + glm::abs(shooter_pos[1] - target_pos[1]) + glm::abs(
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
            map_->get_tile(glm::ivec3(target_pos[0] - 1, target_pos[1], target_pos[2])) == tile_type::wall) ||
        // right
        (shooter_pos[0] > target_pos[0] + 1 &&
            map_->get_tile(glm::ivec3(target_pos[0] + 1, target_pos[1], target_pos[2])) == tile_type::wall) ||
        // up
        (shooter_pos[2] > target_pos[2] + 1 &&
            map_->get_tile(glm::ivec3(target_pos[0], target_pos[1], target_pos[2] + 1)) == tile_type::wall) ||
        // down
        (shooter_pos[2] < target_pos[2] - 1 &&
            map_->get_tile(glm::ivec3(target_pos[0], target_pos[1], target_pos[2] - 1)) == tile_type::wall)
    )
    {
        prob -= cover_penalty;
    }

    // Decrease by target's height
    if (shooter_pos[1] < target_pos[1])
        prob -= 0.15f;

    // Increase by shooter's height
    if (shooter_pos[1] > target_pos[1])
        prob += 0.15f;

    return glm::clamp(prob, 0.01f, 1.0f);
}

bool game_scene::can_unit_shoot(const game_unit& shooter, const game_unit& target) const
{
    return shooter.can_shoot() && !target.is_dead() && can_unit_see(shooter, target);
}

bool game_scene::can_unit_move(const game_unit& unit, const glm::ivec3 position) const
{
    return path_finder::can_move_to_tile(unit.get_position(), position, *map_, unit.get_movable_tiles());
}

void game_scene::shoot_unit(game_unit& shooter, game_unit& target)
{
    if (shooter.can_shoot())
    {
        shooter.set_action_points(shooter.get_action_points() - 2);
        const auto prob = calculate_shooting_probability(shooter, target);
        if (utils::random_float() < prob)
        {
            target.set_hit_points(target.get_hit_points() - 1);
            if (target.is_dead())
            {
                map_->set_blocked(target.get_position(), false);
                update_movable_tiles();
            }
        }
    }
}

void game_scene::move_unit(game_unit& unit, const glm::ivec3 position)
{
    if (unit.can_move())
    {
        map_->set_blocked(unit.get_position(), false);
        unit.set_position(position);
        map_->set_blocked(unit.get_position(), true);
        unit.set_action_points(unit.get_action_points() - 1);
        update_movable_tiles();
        update_visible_tiles();
    }
}

bool game_scene::start_new_turn()
{
    auto first_turn = false;
    if (current_player_id_ >= players_.size())
    {
        first_turn = true;
        current_player_id_ = 0;
    }

    auto game_ended = false;

    // Check winning condition
    for (const auto& player : players_)
    {
        if (player->get_id() == current_player_id_)
            continue;

        auto dead_count = 0;
        for (const auto& unit : player->get_units())
        {
            if (unit->is_dead())
            {
                ++dead_count;
            }
        }
        if (dead_count == static_cast<int>(player->get_units().size()))
        {
            game_ended = true;
            break;
        }
    }

    if (game_ended)
    {
        LOG_INFO << "Game won by player " << current_player_id_;
        return true;
    }

    // Change player if not first turn
    if (!first_turn)
    {
        ++current_player_id_;
        if (current_player_id_ >= players_.size())
            current_player_id_ = 0;
    }

    for (const auto& unit : players_[current_player_id_]->get_units())
    {
        unit->set_action_points(2);
    }

    update_movable_tiles();
    update_visible_tiles();

    return false;
}

bool game_scene::has_any_unit_any_action_left() const
{
    for (const auto& unit : players_[current_player_id_]->get_units())
    {
        if (!unit->turn_done())
            return true;
    }
    return false;
}

void game_scene::update_movable_tiles()
{
    for (const auto& unit : players_[current_player_id_]->get_units())
    {
        calculate_movable_tiles_for(*unit);
    }
}

void game_scene::calculate_movable_tiles_for(game_unit& unit) const
{
    unit.set_movable_tiles(path_finder::find_paths(unit.get_position(), *map_));
}

void game_scene::update_visible_tiles()
{
    for (const auto& unit : players_[current_player_id_]->get_units())
    {
        unit->set_visible(true);
        calculate_visible_tiles_for(*unit);
    }

    for (const auto& player : players_)
    {
        if (player->get_id() == current_player_id_)
            continue;

        for (const auto& enemy_unit : player->get_units())
        {
            enemy_unit->set_visible(false);
            for (const auto& unit : players_[current_player_id_]->get_units())
                if (can_unit_see(*unit, *enemy_unit))
                    enemy_unit->set_visible(true);
        }
    }
}


void game_scene::calculate_visible_tiles_for(game_unit& unit) const
{
    unit.set_visible_tiles(line_of_sight_finder::find_los(unit.get_position(), *map_));
}

bool game_scene::can_unit_see(const game_unit& unit, const game_unit& other) const
{
    return !unit.is_dead() && !other.is_dead() &&
        line_of_sight_finder::can_see_tile(unit.get_position(), other.get_position(), *map_,
                                              unit.get_visible_tiles());
}
