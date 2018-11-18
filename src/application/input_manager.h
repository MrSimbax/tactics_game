#pragma once
#include <map>
#include <functional>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace tactics_game
{
enum class input_action
{
    unbind,
    camera_forward,
    camera_backward,
    camera_left,
    camera_right,
    camera_up,
    camera_down,
    camera_rotate_left,
    camera_rotate_right,
    debug
};

class input_manager
{
    typedef std::function<void()> callback_t;
    typedef std::function<void(glm::ivec2, glm::ivec2)> callback_mouse_motion_t;
    typedef std::function<void(int)> callback_mouse_scroll_t;
    typedef std::function<void(glm::ivec2, int)> callback_mouse_button_down_t;
    typedef SDL_Keycode key_t;

    struct key_state_t
    {
        bool is_pressed;
    };

public:
    void handle_event(SDL_Event* event);

    void bind_key_to_action(key_t key, input_action action);
    void bind_mouse_motion(const callback_mouse_motion_t& callback);
    void bind_mouse_scroll(const callback_mouse_scroll_t& callback);
    void bind_mouse_button_down(const callback_mouse_button_down_t& callback);
    void bind_action_down(input_action action, const callback_t& callback);
    void bind_action_up(input_action action, const callback_t& callback);

    float get_mouse_sensitivity() const;
    void set_mouse_sensitivity(float sensitivity);

private:
    std::map<key_t, input_action> bindings_{};
    std::map<key_t, key_state_t> keys_state_{};

    std::map<input_action, callback_t> on_action_down_{};
    std::map<input_action, callback_t> on_action_up_{};

    callback_mouse_motion_t on_mouse_motion_{};
    callback_mouse_scroll_t on_mouse_scroll_{};
    callback_mouse_button_down_t on_mouse_button_down_{};

    float mouse_sensitivity_{0.01f};
};
}
