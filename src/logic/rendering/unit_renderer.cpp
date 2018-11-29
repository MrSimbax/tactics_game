#include "unit_renderer.h"
#include <utility>
#include <plog/Log.h>
#include "../../misc/custom_log.h"
#include <glm/glm.hpp>

using namespace tactics_game;

unit_renderer::unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model, const model& grid_model)
    : unit_{std::move(unit)},
      graphics_object_{new graphics_object{unit_model}},
      buffered_graphics_object_{graphics_object_},
      grid_object_{grid_model}
{
}

void unit_renderer::render(shader_program& program) const
{
    if (!unit_->is_visible() || unit_->is_dead())
        return;
    const auto game_position = unit_->get_position();
    graphics_object_->set_position(glm::vec3{
        game_position.x,
        game_position.y + 0.5f,
        game_position.z
    });
    buffered_graphics_object_.render(program);
}

void unit_renderer::render_outline(shader_program& program) const
{
    if (outline_)
        program.set_vec4("u_color", outline_color_);
    else
        return;

    const auto game_position = unit_->get_position();
    graphics_object_->set_position(glm::vec3{
        game_position.x,
        game_position.y + 0.5f,
        game_position.z
    });
    graphics_object_->set_scale(glm::vec3(1.0f) + outline_width);
    buffered_graphics_object_.render(program);
    graphics_object_->set_scale(glm::vec3(1.0f));
}

void unit_renderer::render_grid(shader_program& program, const int y_max) const
{
    for (size_t y = 0; y < static_cast<size_t>(y_max); ++y)
    {
        const auto& buffered_object = movable_grid_[y];
        if (buffered_object)
        {
            buffered_object->render(program);
        }
    }
}

bool unit_renderer::get_outline() const
{
    return outline_;
}

void unit_renderer::set_outline(const bool selected)
{
    outline_ = selected;
}

glm::vec4 unit_renderer::get_outline_color() const
{
    return outline_color_;
}

void unit_renderer::set_outline_color(const glm::vec4 color)
{
    outline_color_ = color;
}

void unit_renderer::update_movable_grid(const int y_size)
{
    std::vector<model> grid_models;
    grid_models.resize(y_size);
    const auto& movable_tiles = path_finder::get_movable_tiles(unit_->get_position(), unit_->get_movable_tiles());
    for (const auto& movable_tile : movable_tiles)
    {
        grid_object_.set_position(glm::vec3(movable_tile.x, movable_tile.y + 0.1f, movable_tile.z));
        grid_object_.set_scale(glm::vec3(0.8f, 1.0f, 0.8f));
        grid_models[movable_tile.y].add_mesh(grid_object_.transformed().get_model().merged());
    }

    movable_grid_.clear();
    for (const auto& grid_model : grid_models)
    {
        if (!grid_model.get_meshes().empty())
        {
            auto color_obj = std::make_shared<simple_color_object>(grid_model.merged());
            color_obj->set_color(movable_grid_color);
            movable_grid_.emplace_back(std::make_unique<buffered_simple_color_object>(color_obj));
        }
        else
        {
            movable_grid_.emplace_back(nullptr);
        }
    }
}

std::shared_ptr<game_unit> unit_renderer::get_unit() const
{
    return unit_;
}
