#pragma once
#include "../game_unit.h"

namespace tactics_game
{
class unit_renderer
{
public:
    unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model);

    void render(shader_program& program) const;

private:
    std::shared_ptr<game_unit> unit_;

    std::shared_ptr<graphics_object> graphics_object_;
    buffered_graphics_object buffered_graphics_object_;
};
}
