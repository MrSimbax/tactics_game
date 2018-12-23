#pragma once
#include "../../graphics/model/simple_color_object.h"
#include "../../graphics/model/buffered_simple_color_object.h"

namespace tactics_game
{
enum ui_moves_state
{
    no_moves_left,
    moves_left
};

class ui_renderer
{
public:
    ui_renderer(const std::shared_ptr<simple_color_object>& no_moves_left,
                const std::shared_ptr<simple_color_object>& moves_left);

    void render(shader_program& program, glm::vec4 color) const;

    void set_state(ui_moves_state state);

private:
    std::unique_ptr<buffered_simple_color_object> no_moves_left_;
    std::unique_ptr<buffered_simple_color_object> moves_left_;
    ui_moves_state state_;
};
}
