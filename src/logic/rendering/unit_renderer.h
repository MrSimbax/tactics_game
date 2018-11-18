#pragma once
#include "../game_unit.h"

namespace tactics_game
{

const glm::vec4 outline_color_selected{0.101f, 0.662f, 1.0f, 1.0f};
const glm::vec4 outline_color_turn_done{0.3f, 0.3f, 0.3f, 1.0f};

// interpolate between the two
const glm::vec4 outline_color_shoot_probability_low{0.5f, 0.0f, 0.3f, 1.0f};
const glm::vec4 outline_color_shoot_probability_high{0.5f, 1.0f, 0.3f, 1.0f};

const float outline_width = 0.2f;

class unit_renderer
{
public:
    unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model);

    void render(shader_program& program) const;
    void render_outline(shader_program& program) const;

    bool get_selected() const;
    void set_selected(bool selected);

    std::shared_ptr<game_unit> get_unit() const;

private:
    std::shared_ptr<game_unit> unit_;

    std::shared_ptr<graphics_object> graphics_object_;
    buffered_graphics_object buffered_graphics_object_;

    bool selected_{false};
};
}
