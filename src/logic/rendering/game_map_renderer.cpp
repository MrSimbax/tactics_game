#include "game_map_renderer.h"
#include <plog/Log.h>
#include "../../misc/custom_log.h"

using namespace tactics_game;

game_map_renderer::game_map_renderer(std::shared_ptr<game_map> map,
                                     std::unique_ptr<graphics_object> floor,
                                     std::unique_ptr<graphics_object> wall,
                                     std::unique_ptr<graphics_object> climber)
    : map_{std::move(map)},
      floor_{std::move(floor)},
      wall_{std::move(wall)},
      climber_{std::move(climber)}
{
    create_static_buffers();
}

void game_map_renderer::render(shader_program& program, const size_t layer_index) const
{
    for (size_t y = 0; y < layer_index; ++y)
    {
        buffered_static_layers_[y].render(program);
        climbers_[y].render(program);
    }
}

void game_map_renderer::create_static_buffers()
{
    static_layers_.reserve(map_->get_layers().size());
    buffered_static_layers_.reserve(map_->get_layers().size());
    const auto& layers = map_->get_layers();
    for (size_t y = 0; y < map_->get_layers().size(); ++y)
    {
        const auto& layer = layers[y];
        create_static_layer(layer, y);
    }
}

void game_map_renderer::create_static_layer(const game_map::tiles_t& layer, const size_t y)
{
    model static_layer_model{};
    model climbers_model{};

    for (size_t x = 0; x < layer.size(); ++x)
    {
        for (size_t z = 0; z < layer[x].size(); ++z)
        {
            const auto tile = layer[x][z];
            switch (tile)
            {
            case tile_type::empty:
                break;
            case tile_type::floor:
                {
                    floor_->set_position(glm::vec3(x, y, z));
                    static_layer_model.add_mesh(floor_->transformed().get_model().merged());
                    break;
                }
            case tile_type::wall:
                {

                    wall_->set_position(glm::vec3(x, y + 0.45f, z));
                    wall_->set_scale(glm::vec3(1.0f, 0.9f, 1.0f));
                    static_layer_model.add_mesh(wall_->transformed().get_model().merged());
                    break;
                }
            case tile_type::climber:
                {
                    climber_->set_position(glm::vec3(x, y + 0.2f, z));
                    climbers_model.add_mesh(climber_->transformed().get_model().merged());
                    if (y == 0 || map_->get_layers()[y-1][x][z] != tile_type::climber)
                    {
                        floor_->set_position(glm::vec3(x, y, z));
                        static_layer_model.add_mesh(floor_->transformed().get_model().merged());
                    }
                    break;
                }
            default: ;
            }
        }
    }

    static_layers_.emplace_back(new graphics_object{static_layer_model.merged()});
    buffered_static_layers_.emplace_back(static_layers_[y]);
    climbers_.emplace_back(std::make_shared<graphics_object>(climbers_model));
}
