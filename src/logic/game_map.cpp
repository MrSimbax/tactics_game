#include "game_map.h"
#include <utility>
#include <plog/Log.h>

#include "../misc/custom_log.h"

using namespace tactics_game;

game_map::game_map(std::vector<fields_t> layers)
    : layers_{std::move(layers)}
{
}

const std::vector<game_map::fields_t>& game_map::get_layers() const
{
    return layers_;
}

game_map_renderer::game_map_renderer(std::shared_ptr<game_map> map,
                                     std::unique_ptr<graphics_object> floor,
                                     std::unique_ptr<graphics_object> wall,
                                     std::unique_ptr<graphics_object> climber)
    : map_{map},
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
        const auto& layer = buffered_static_layers_[y];
        layer.render(program);
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

void game_map_renderer::create_static_layer(const game_map::fields_t& layer, const size_t y)
{
    model static_layer_model{};

    for (size_t x = 0; x < layer.size(); ++x)
    {
        for (size_t z = 0; z < layer[x].size(); ++z)
        {
            const auto field = layer[x][z];
            switch (field)
            {
            case field_type::empty:
                break;
            case field_type::floor:
                {
                    floor_->set_position(glm::vec3(x, y, z));
                    static_layer_model.add_mesh(floor_->transformed().get_model().merged());
                    break;
                }
            case field_type::wall:
                {
                    wall_->set_position(glm::vec3(x, y, z));
                    const auto wall = wall_->transformed().get_model().merged();
                    static_layer_model.add_mesh(wall_->transformed().get_model().merged());
                    break;
                }
            default: ;
            }
        }
    }
    /*
        for (const auto& vertex : vertices)
        {
            LOG_DEBUG << vertex.position;
        }*/

    static_layers_.emplace_back(static_layer_model.merged());
    buffered_static_layers_.emplace_back(&static_layers_[y]);
}
