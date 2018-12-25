#pragma once
#include "../../graphics/model/buffered_graphics_object.h"
#include "../game_map.h"

namespace tactics_game
{
class game_map;

class game_map_renderer
{
public:
    game_map_renderer(std::shared_ptr<game_map> map,
                      graphics_object floor,
                      graphics_object wall,
                      graphics_object climber);

    void render(shader_program& program, size_t layer_index) const;

private:
    void create_static_buffers();

    void create_static_layer(const game_map::tiles_t& layer, size_t y);

    std::vector<std::vector<bool>> visited_;

    std::shared_ptr<game_map> map_;

    graphics_object floor_;
    graphics_object wall_;
    graphics_object climber_;

    std::vector<buffered_graphics_object> buffered_static_layers_;

    std::vector<buffered_graphics_object> climbers_;
};
}
