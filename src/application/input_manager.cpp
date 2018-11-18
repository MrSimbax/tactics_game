#include "input_manager.h"
#include "plog/Log.h"

using namespace tactics_game;

void input_manager::handle_event(SDL_Event* event)
{
    const auto key = event->key.keysym.sym;
    switch (event->type)
    {
    case SDL_KEYDOWN:
        if (!keys_state_[key].is_pressed)
        {
            const auto callback = on_action_down_[bindings_[key]];
            if (callback)
                callback();
        }
        keys_state_[key].is_pressed = true;
        break;
    case SDL_KEYUP:
        if (keys_state_[key].is_pressed)
        {
            const auto callback = on_action_up_[bindings_[key]];
            if (callback)
                callback();
        }
        keys_state_[key].is_pressed = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (on_mouse_button_down_)
            on_mouse_button_down_(glm::ivec2{event->button.x, event->button.y}, event->button.button);
        break;
    case SDL_MOUSEMOTION:
        if (on_mouse_motion_)
            on_mouse_motion_(glm::ivec2{event->motion.x, event->motion.y},
                             glm::ivec2{event->motion.xrel, event->motion.yrel});
        break;
    case SDL_MOUSEWHEEL:
        if (on_mouse_scroll_)
            on_mouse_scroll_(event->wheel.y);
        break;
    default: ;
    }
}

void input_manager::bind_key_to_action(const key_t key, const input_action action)
{
    bindings_[key] = action;
}

void input_manager::bind_action_down(const input_action action, const callback_t& callback)
{
    on_action_down_[action] = callback;
}

void input_manager::bind_mouse_motion(const callback_mouse_motion_t& callback)
{
    on_mouse_motion_ = callback;
}

void input_manager::bind_mouse_scroll(const callback_mouse_scroll_t& callback)
{
    on_mouse_scroll_ = callback;
}

void input_manager::bind_mouse_button_down(const callback_mouse_button_down_t& callback)
{
    on_mouse_button_down_ = callback;
}

void input_manager::bind_action_up(const input_action action, const callback_t& callback)
{
    on_action_up_[action] = callback;
}

float input_manager::get_mouse_sensitivity() const
{
    return mouse_sensitivity_;
}

void input_manager::set_mouse_sensitivity(const float sensitivity)
{
    mouse_sensitivity_ = sensitivity;
}
