#include "ui_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace tactics_game;


ui_renderer::ui_renderer(const simple_color_object& no_moves_left, const simple_color_object& moves_left)
    : no_moves_left_{no_moves_left},
      moves_left_{moves_left},
      state_{ui_moves_state::moves_left}
{
}

void ui_renderer::render(shader_program& program, const glm::vec4 color)
{
    glm::vec3 scale{0.1f, 0.1f, 0.1f};
    program.set_mat4("u_projection", glm::mat4(1));
    program.set_mat4("u_view", glm::mat4(1));
    buffered_simple_color_object* indicator_to_render = nullptr;
    switch (state_)
    {
    case moves_left:
        indicator_to_render = &moves_left_;
        break;
    case no_moves_left:
        indicator_to_render = &no_moves_left_;
        break;
    }
    indicator_to_render->get_object().set_position({-0.9f, 0.9f, 0.0f});
    indicator_to_render->get_object().set_scale({0.1f, 0.1f, 0.1f});
    indicator_to_render->get_object().set_color(color);
    indicator_to_render->render(program);
}

void ui_renderer::set_state(const ui_moves_state state)
{
    this->state_ = state;
}
