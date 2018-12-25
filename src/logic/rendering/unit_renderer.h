#pragma once
#include "../game_unit.h"
#include "../../graphics/model/buffered_graphics_object.h"
#include "../../graphics/model/buffered_simple_color_object.h"

namespace tactics_game
{

const glm::vec4 outline_color_selected{0.101f, 0.662f, 1.0f, 1.0f};
const glm::vec4 outline_color_turn_ready{0.352f, 0.160f, 1.0f, 1.0f};

// interpolate between the two
const glm::vec4 outline_color_shoot_probability_low{0.1f, 0.0f, 0.1f, 1.0f};
const glm::vec4 outline_color_shoot_probability_high{0.1f, 1.0f, 0.1f, 1.0f};

const glm::vec4 movable_grid_color{0.0f, 0.474f, 0.678f, 1.0f};

const float outline_width = 0.2f;

class unit_renderer
{
public:
    unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model, const model& grid_model);

    unit_renderer(unit_renderer&&) = default;

    void render(shader_program& program);
    void render_outline(shader_program& program);
    void render_grid(shader_program& program, int y_max) const;

    bool get_outline() const;
    void set_outline(bool selected);

    glm::vec4 get_outline_color() const;
    void set_outline_color(glm::vec4 color);

    void update_movable_grid(int y_size);

    std::shared_ptr<game_unit> get_unit() const;

private:
    std::shared_ptr<game_unit> unit_;

    buffered_graphics_object buffered_graphics_object_;

    bool outline_{false};
    glm::vec4 outline_color_{false};

    graphics_object grid_object_;
    std::vector<std::unique_ptr<buffered_simple_color_object>> movable_grid_;
};
}
