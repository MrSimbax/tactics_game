#pragma once

#include <glm/glm.hpp>

namespace tactics_game
{
class free_roam_camera
{
public:
    free_roam_camera();

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;

    void process_keyboard(glm::vec3 direction, float delta_time);
    void process_mouse(glm::ivec2 offset);
    void process_scroll(int offset);

    glm::vec3 get_position() const;
    void set_position(glm::vec3 position);

    glm::vec2 get_rotation() const;
    void set_rotation(glm::vec2 rotation);

    float get_speed() const;
    void set_speed(float speed);

    float get_mouse_sensitivity() const;
    void set_mouse_sensitivity(float mouse_sensitivity);

    float get_fov() const;
    void set_fov(float fov);

    float get_ratio() const;
    void set_ratio(float ratio);

    float get_near() const;
    void set_near(float near);

    float get_far() const;
    void set_far(float far);

private:
    void update();

    glm::vec3 position_{0};
    glm::vec2 rotation_{0};

    const glm::vec3 world_up_{0.0f, 1.0f, 0.0f};

    glm::vec3 up_{0};
    glm::vec3 right_{0};
    glm::vec3 front_{0};

    float ratio_{1.0f};
    float near_{0.1f};
    float far_{100.0f};

    float speed_{2.5f};
    float mouse_sensitivity_{0.001f};
    float fov_{45.0f};
};
}
