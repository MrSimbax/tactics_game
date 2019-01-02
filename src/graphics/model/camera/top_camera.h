#pragma once

#include <glm/glm.hpp>
#include "camera_observer.h"

namespace tactics_game
{
enum class top_camera_orientation
{
    top_left,
    top_right,
    bottom_left,
    bottom_right
};

struct top_camera_bounds
{
    glm::vec3 xz_min{0.0f};
    glm::vec3 xz_max{100.0f};
    float zoom_min = 0.25f;
    float zoom_max = 2.0f;
};

class top_camera
{
public:
    top_camera();

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;

    void process_keyboard(glm::vec3 direction, float delta_time);
    void process_scroll(int offset);
    glm::vec3 mouse_to_ray(glm::ivec2 mouse_pos, glm::ivec2 frame_size) const;

    glm::vec3 get_offset() const;
    void set_offset(glm::vec3 position);

    glm::vec3 get_position() const;
    glm::vec3 get_render_position() const;

    glm::vec3 get_target() const;
    void set_target(glm::vec3 target);

    top_camera_orientation get_orientation() const;
    void set_orientation(top_camera_orientation orientation);

    void rotate_left();
    void rotate_right();

    top_camera_bounds& get_bounds();

    float get_speed() const;
    void set_speed(float speed);

    float get_zoom_speed() const;
    void set_zoom_speed(float speed);

    float get_fov() const;
    void set_fov(float fov);

    float get_ratio() const;
    void set_ratio(float ratio);

    float get_near() const;
    void set_near(float fnear);

    float get_far() const;
    void set_far(float ffar);

    float get_zoom() const;

    int get_current_layer() const;
    void set_current_layer(int layer);

    void set_observer(camera_observer* observer);

private:
    void update();

    top_camera_orientation orientation_{top_camera_orientation::top_left};

    glm::vec3 offset_{0.0f};
    glm::vec3 position_{0.0f};
    glm::vec3 look_at_{0.0f};

    glm::vec3 position_render_{0.0f};
    glm::vec3 look_at_render_{0.0f};

    const glm::vec3 world_up_{0.0f, 1.0f, 0.0f};

    float ratio_{1.0f};
    float near_{5.0f};
    float far_{25.0f};

    float zoom_{1.0f};

    float speed_{8.0f};
    float zoom_speed_{0.1f};
    float fov_{45.0f};

    int current_layer_{0};

    top_camera_bounds bounds_{};

    camera_observer* observer_{nullptr};
};
}
