#include "buffered_light_object.h"

using namespace tactics_game;

buffered_light_object::buffered_light_object(const std::shared_ptr<light_object>& object)
    : object_{object},
      model_{object->get_model()}
{
}

void buffered_light_object::render(shader_program& program) const
{
    program.set_mat4("u_model", object_->get_model_matrix());
    program.set_vec4("u_color", object_->get_color());
    model_.render(program);
}
