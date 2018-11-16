#include "unit_renderer.h"
#include <utility>
#include "plog/Log.h"
#include "../../misc/custom_log.h"

using namespace tactics_game;

unit_renderer::unit_renderer(std::shared_ptr<game_unit> unit, const model& unit_model)
    : unit_{std::move(unit)},
      graphics_object_{new graphics_object{unit_model}},
      buffered_graphics_object_{graphics_object_}
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
