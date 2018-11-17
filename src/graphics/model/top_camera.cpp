#include "top_camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../../misc/custom_log.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

using namespace tactics_game;

top_camera::top_camera()
{
    update();
}

glm::mat4 top_camera::get_view_matrix() const
{
    return lookAt(position_, look_at_, world_up_);
}

glm::mat4 top_camera::get_projection_matrix() const
{
    return glm::perspective(glm::radians(fov_), ratio_, 0.1f, 100.0f);
}

glm::vec3 top_camera::get_offset() const
{
    return offset_;
}

void top_camera::set_offset(const glm::vec3 position)
{
    offset_ = position;
}

glm::vec3 top_camera::get_position() const
{
    return position_;
}

glm::vec3 top_camera::get_target() const
{
    return look_at_;
}

void top_camera::set_target(const glm::vec3 target)
{
    look_at_ = target;
    update();
}

top_camera_orientation top_camera::get_orientation() const
{
    return orientation_;
}

void top_camera::set_orientation(const top_camera_orientation orientation)
{
    orientation_ = orientation;
}

void top_camera::rotate_right()
{
    switch (orientation_)
    {
    case top_camera_orientation::top_left:
        orientation_ = top_camera_orientation::bottom_left;
        break;
    case top_camera_orientation::top_right:
        orientation_ = top_camera_orientation::top_left;
        break;
    case top_camera_orientation::bottom_left:
        orientation_ = top_camera_orientation::bottom_right;
        break;
    case top_camera_orientation::bottom_right:
        orientation_ = top_camera_orientation::top_right;
        break;
    }
    update();
}

void top_camera::rotate_left()
{
    switch (orientation_)
    {
    case top_camera_orientation::top_left:
        orientation_ = top_camera_orientation::top_right;
        break;
    case top_camera_orientation::top_right:
        orientation_ = top_camera_orientation::bottom_right;
        break;
    case top_camera_orientation::bottom_left:
        orientation_ = top_camera_orientation::top_left;
        break;
    case top_camera_orientation::bottom_right:
        orientation_ = top_camera_orientation::bottom_left;
        break;
    }
    update();
}

void top_camera::process_keyboard(const glm::vec3 direction, const float delta_time)
{
    const auto speed = speed_ * delta_time;
    const auto velocity = direction * speed;
    glm::vec3 front;
    switch (orientation_)
    {
    case top_camera_orientation::top_right:
        front = normalize(glm::vec3{-offset_.x, 0.0f, -offset_.z});
        break;
    case top_camera_orientation::top_left:
        front = normalize(glm::vec3{-offset_.x, 0.0f, offset_.z});
        break;
    case top_camera_orientation::bottom_left:
        front = normalize(glm::vec3{offset_.x, 0.0f, offset_.z});
        break;
    case top_camera_orientation::bottom_right:
        front = normalize(glm::vec3{offset_.x, 0.0f, -offset_.z});
        break;
    }
    const auto right = cross(front, world_up_);
    look_at_ += front * velocity.z;
    look_at_ += right * velocity.x;
    look_at_ += world_up_ * velocity.y;
    update();
}

glm::vec3 top_camera::mouse_to_xz_plane(const glm::ivec2 mouse_pos, const glm::ivec2 frame_size, const float y) const
{
    // Ray cast to plane xz on height y

    // The ray has origin in camera position and is directed out of the screen

    // Convert mouse pos to NDC coordinates
    glm::vec3 ray;
    ray.x = ((mouse_pos.x + 0.5f) / frame_size.x) * 2.0f - 1.0f;
    ray.y = 1.0f - ((mouse_pos.y + 0.5f) / frame_size.y) * 2.0f;
    ray.z = -1.0f;

    // Calculate direction in world coordinates
    ray = inverse(get_projection_matrix()) * glm::vec4(ray, 0.0f);
    ray = normalize(inverse(get_view_matrix()) * glm::vec4(ray.x, ray.y, -1.0f, 0.0f));

    // Intersect with plane
    // It's just solving the equation for t
    // (cam_pos + ray * t) * plane_normal + d = 0
    // plane_normal = [0, 1, 0]
    // d = y
    const auto t = -(position_.y + y) / ray.y;
    return position_ + ray * t;
}

float top_camera::get_speed() const
{
    return speed_;
}

void top_camera::set_speed(const float speed)
{
    speed_ = speed;
}

float top_camera::get_fov() const
{
    return fov_;
}

void top_camera::set_fov(const float fov)
{
    fov_ = fov;
}

float top_camera::get_ratio() const
{
    return ratio_;
}

void top_camera::set_ratio(const float ratio)
{
    ratio_ = ratio;
}

float top_camera::get_near() const
{
    return near_;
}

// ReSharper disable once IdentifierTypo
void top_camera::set_near(const float fnear)
{
    near_ = fnear;
}

float top_camera::get_far() const
{
    return far_;
}

// ReSharper disable once IdentifierTypo
void top_camera::set_far(const float ffar)
{
    far_ = ffar;
}

void top_camera::update()
{
    switch (orientation_)
    {
    case top_camera_orientation::top_right:
        position_.x = look_at_.x + offset_.x;
        position_.z = look_at_.z + offset_.z;
        break;
    case top_camera_orientation::top_left:
        position_.x = look_at_.x + offset_.x;
        position_.z = look_at_.z - offset_.z;
        break;
    case top_camera_orientation::bottom_left:
        position_.x = look_at_.x - offset_.x;
        position_.z = look_at_.z - offset_.z;
        break;
    case top_camera_orientation::bottom_right:
        position_.x = look_at_.x - offset_.x;
        position_.z = look_at_.z + offset_.z;
        break;
    }
    position_.y = look_at_.y + offset_.y;
}
