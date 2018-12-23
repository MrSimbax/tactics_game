#include "buffered_simple_color_object.h"

using namespace tactics_game;

buffered_simple_color_object::buffered_simple_color_object(const std::shared_ptr<simple_color_object>& object)
    : object_{object},
      model_{object->get_model()}
{
}

void buffered_simple_color_object::render(shader_program& program) const
{
    program.set_mat4("u_model", object_->get_model_matrix());
    program.set_vec4("u_color", object_->get_color());
    model_.render(program);
}

simple_color_object* buffered_simple_color_object::get_object() const
{
    return object_.get();
}
