#pragma once
#include "../game_unit.h"

namespace tactics_game
{

const glm::vec4 outline_color_selected{0.101f, 0.662f, 1.0f, 1.0f};
const glm::vec4 outline_color_turn_done{0.3f, 0.3f, 0.3f, 1.0f};

// interpolate between the two
const glm::vec4 outline_color_shoot_probability_low{0.1f, 0.0f, 0.1f, 1.0f};
const glm::vec4 outline_color_shoot_probability_high{0.1f, 1.0f, 0.1f, 1.0f};

const float outline_width = 0.2f;

class unit_renderer
{
public:
    unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model);

    void render(shader_program& program) const;
    void render_outline(shader_program& program) const;

    bool get_outline() const;
    void set_outline(bool selected);

    glm::vec4 get_outline_color() const;
    void set_outline_color(glm::vec4 color);

    std::shared_ptr<game_unit> get_unit() const;

private:
    std::shared_ptr<game_unit> unit_;

    std::shared_ptr<graphics_object> graphics_object_;
    buffered_graphics_object buffered_graphics_object_;

    bool outline_{false};
    glm::vec4 outline_color_{false};
};
}
