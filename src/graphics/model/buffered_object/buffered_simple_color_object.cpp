#include "buffered_simple_color_object.h"

using namespace tactics_game;

buffered_simple_color_object::buffered_simple_color_object(simple_color_object object)
    : object_{std::move(object)},
      model_{object_.get_model()}
{
}

void buffered_simple_color_object::render(shader_program& program) const
{
    program.set_mat4("u_model", object_.get_model_matrix());
    program.set_vec4("u_color", object_.get_color());
    model_.render(program);
}

simple_color_object& buffered_simple_color_object::get_object()
{
    return object_;
}
