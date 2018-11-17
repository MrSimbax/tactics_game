#pragma once

#include <glm/glm.hpp>
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"
#include "glm/detail/_noise.hpp"

namespace tactics_game
{
enum class top_camera_orientation
{
    top_left,
    top_right,
    bottom_left,
    bottom_right
};

class top_camera
{
public:
    top_camera();

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;

    void process_keyboard(glm::vec3 direction, float delta_time);
    //void process_mouse(glm::ivec2 offset);
    //void process_scroll(int offset);
    glm::vec3 mouse_to_xz_plane(glm::ivec2 mouse_pos, glm::ivec2 frame_size, float y) const;

    glm::vec3 get_offset() const;
    void set_offset(glm::vec3 position);

    glm::vec3 get_position() const;

    glm::vec3 get_target() const;
    void set_target(glm::vec3 target);

    top_camera_orientation get_orientation() const;
    void set_orientation(top_camera_orientation orientation);

    void rotate_left();
    void rotate_right();

    float get_speed() const;
    void set_speed(float speed);

    //float get_mouse_sensitivity() const;
    //void set_mouse_sensitivity(float mouse_sensitivity);

    float get_fov() const;
    void set_fov(float fov);

    float get_ratio() const;
    void set_ratio(float ratio);

    float get_near() const;
    void set_near(float fnear);

    float get_far() const;
    void set_far(float ffar);

private:
    void update();

    top_camera_orientation orientation_{top_camera_orientation::top_left};

    glm::vec3 offset_{0};
    glm::vec3 position_{0};
    glm::vec3 look_at_{0};

    const glm::vec3 world_up_{0.0f, 1.0f, 0.0f};

    float ratio_{1.0f};
    float near_{0.1f};
    float far_{100.0f};

    float speed_{2.5f};
    float fov_{45.0f};
};
}
