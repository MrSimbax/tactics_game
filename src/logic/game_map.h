#pragma once
#include <vector>
#include "../graphics/model/buffered_graphics_object.h"

namespace tactics_game
{
enum class field_type
{
    empty = 0,
    floor = 1,
    wall = 2,
    climber = 3
};

class game_map
{
public:
    typedef std::vector<std::vector<field_type>> fields_t;

    explicit game_map(std::vector<fields_t> layers);

    const std::vector<fields_t>& get_layers() const;

private:
    std::vector<fields_t> layers_;
    //std::vector<game_unit> units_;
    //std::vector<light> lights_;
};

class game_map_renderer
{
public:
    game_map_renderer(std::shared_ptr<game_map> map,
                      std::unique_ptr<graphics_object> floor,
                      std::unique_ptr<graphics_object> wall,
                      std::unique_ptr<graphics_object> climber);

    void render(shader_program& program, size_t layer_index) const;

private:
    void create_static_buffers();

    void create_static_layer(const game_map::fields_t& layer, size_t y);

    std::shared_ptr<game_map> map_;

    std::unique_ptr<graphics_object> floor_;
    std::unique_ptr<graphics_object> wall_;
    std::unique_ptr<graphics_object> climber_;

    std::vector<graphics_object> static_layers_;
    std::vector<buffered_graphics_object> buffered_static_layers_;
};
}
