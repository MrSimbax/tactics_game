#include "buffered_graphics_object.h"

#include "../../../misc/custom_log.h"
#include "glm/gtc/matrix_inverse.hpp"

using namespace tactics_game;

buffered_graphics_object::buffered_graphics_object(graphics_object object)
    : object_{std::move(object)},
      model_{object_.model_}
{
}

void buffered_graphics_object::render(shader_program& program) const
{
    const auto model_matrix = object_.get_model_matrix();
    program.set_mat4("u_model", model_matrix);
    program.set_mat3("u_normal_model", inverseTranspose(glm::mat3(model_matrix)));
    model_.render(program);
}

graphics_object& buffered_graphics_object::get_graphics_object()
{
    return object_;
}
