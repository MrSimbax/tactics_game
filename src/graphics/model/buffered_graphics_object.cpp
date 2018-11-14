#include "buffered_graphics_object.h"

using namespace tactics_game;

buffered_graphics_object::buffered_graphics_object(const graphics_object* object)
    : object_{object},
      model_{object->model_}
{
}

void buffered_graphics_object::render(shader_program& program) const
{
    program.set_mat4("model", object_->get_model_matrix());
    model_.render(program);
}
