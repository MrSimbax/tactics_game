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

void game_scene::set_current_player_id(size_t i)
{
    current_player_id_ = i;
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
