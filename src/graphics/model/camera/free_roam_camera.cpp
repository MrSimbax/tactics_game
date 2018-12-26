#include "free_roam_camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../../../misc/custom_log.h"
#include "glm/gtx/fast_trigonometry.inl"

using namespace tactics_game;

free_roam_camera::free_roam_camera()
{
    update();
}

glm::mat4 free_roam_camera::get_view_matrix() const
{
    return lookAt(position_, position_ + front_, up_);
}

glm::mat4 free_roam_camera::get_projection_matrix() const
{
    return glm::perspective(glm::radians(fov_), ratio_, 0.1f, 100.0f);
}

void free_roam_camera::process_keyboard(const glm::vec3 direction, const float delta_time)
{
    const auto speed = speed_ * delta_time;
    position_ += front_ * direction.z * speed;
    position_ += right_ * direction.x * speed;
    position_ += world_up_ * direction.y * speed;
    update();
}

void free_roam_camera::process_mouse(const glm::ivec2 offset)
{
    rotation_ += glm::vec2(-offset.y, offset.x) * mouse_sensitivity_;
    rotation_.x = glm::clamp(rotation_.x, glm::radians(-89.0f), glm::radians(89.0f));
    rotation_.y = glm::wrapAngle(rotation_.y);
    update();
}

void free_roam_camera::process_scroll(const int offset)
{
    fov_ = glm::clamp(fov_ - offset, 1.0f, 45.0f);
}

glm::vec3 free_roam_camera::get_position() const
{
    return position_;
}

void free_roam_camera::set_position(const glm::vec3 position)
{
    position_ = position;
    update();
}

glm::vec2 free_roam_camera::get_rotation() const
{
    return rotation_;
}

void free_roam_camera::set_rotation(const glm::vec2 rotation)
{
    rotation_ = rotation;
    update();
}

float free_roam_camera::get_speed() const
{
    return speed_;
}

void free_roam_camera::set_speed(const float speed)
{
    speed_ = speed;
}

float free_roam_camera::get_mouse_sensitivity() const
{
    return mouse_sensitivity_;
}

void free_roam_camera::set_mouse_sensitivity(const float mouse_sensitivity)
{
    mouse_sensitivity_ = mouse_sensitivity;
}

float free_roam_camera::get_fov() const
{
    return fov_;
}

void free_roam_camera::set_fov(const float fov)
{
    fov_ = fov;
}

float free_roam_camera::get_ratio() const
{
    return ratio_;
}

void free_roam_camera::set_ratio(const float ratio)
{
    ratio_ = ratio;
}

float free_roam_camera::get_near() const
{
    return near_;
}

void free_roam_camera::set_near(const float near)
{
    near_ = near;
}

float free_roam_camera::get_far() const
{
    return far_;
}

void free_roam_camera::set_far(const float far)
{
    far_ = far;
}

void free_roam_camera::update()
{
    front_.x = glm::cos(rotation_.y) * glm::cos(rotation_.x);
    front_.y = glm::sin(rotation_.x);
    front_.z = glm::sin(rotation_.y) * glm::cos(rotation_.x);
    front_ = normalize(front_);
    right_ = normalize(cross(front_, world_up_));
    up_ = normalize(cross(right_, front_));
}
