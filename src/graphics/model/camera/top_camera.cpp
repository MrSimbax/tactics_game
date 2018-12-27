#include "top_camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "../../../misc/custom_log.h"

using namespace tactics_game;

top_camera::top_camera()
{
    update();
}

glm::mat4 top_camera::get_view_matrix() const
{
    return lookAt(position_render_, look_at_render_, world_up_) * scale(glm::mat4(1), glm::vec3(zoom_));
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

glm::vec3 top_camera::get_render_position() const
{
    return position_render_;
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

top_camera_bounds& top_camera::get_bounds()
{
    return bounds_;
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
    if (epsilonEqual(direction, glm::vec3{0.0f, 0.0f, 0.0f}, 0.1f) == glm::bvec3{true, true, true})
    {
        return;
    }

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
    
    look_at_.x = glm::clamp(look_at_.x, bounds_.xz_min.x, bounds_.xz_max.x);
    look_at_.z = glm::clamp(look_at_.z, bounds_.xz_min.z, bounds_.xz_max.z);

    update();
}

void top_camera::process_scroll(const int offset)
{
    zoom_ = glm::clamp(zoom_ + offset * zoom_speed_, bounds_.zoom_min, bounds_.zoom_max);
    update();
}

glm::vec3 top_camera::mouse_to_ray(const glm::ivec2 mouse_pos, const glm::ivec2 frame_size) const
{
    // The ray has origin in camera position and is directed out of the screen

    // Convert mouse pos to NDC coordinates
    glm::vec3 ray;
    ray.x = ((mouse_pos.x + 0.5f) / frame_size.x) * 2.0f - 1.0f;
    ray.y = 1.0f - ((mouse_pos.y + 0.5f) / frame_size.y) * 2.0f;
    ray.z = -1.0f;

    // Calculate direction in world coordinates
    ray = inverse(get_projection_matrix()) * glm::vec4(ray, 0.0f);
    ray = inverse(get_view_matrix()) * glm::vec4(ray.x, ray.y, -1.0f, 0.0f);
    ray = normalize(ray);

    return ray;
}

float top_camera::get_speed() const
{
    return speed_;
}

void top_camera::set_speed(const float speed)
{
    speed_ = speed;
}

float top_camera::get_zoom_speed() const
{
    return zoom_speed_;
}

void top_camera::set_zoom_speed(const float speed)
{
    this->zoom_speed_ = speed;
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
    update();
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

float top_camera::get_zoom() const
{
    return zoom_;
}

int top_camera::get_current_layer() const
{
    return current_layer_;
}

void top_camera::set_current_layer(const int layer)
{
    current_layer_ = layer;
}

void top_camera::set_observer(camera_observer* observer)
{
    this->observer_ = observer;
}

void top_camera::update()
{
    look_at_render_ = look_at_ * zoom_;
    
    switch (orientation_)
    {
    case top_camera_orientation::top_right:
        position_render_.x = look_at_render_.x + offset_.x;
        position_render_.z = look_at_render_.z + offset_.z;
        position_.x = look_at_.x + offset_.x;
        position_.z = look_at_.z + offset_.z;
        break;
    case top_camera_orientation::top_left:
        position_render_.x = look_at_render_.x + offset_.x;
        position_render_.z = look_at_render_.z - offset_.z;
        position_.x = look_at_.x + offset_.x;
        position_.z = look_at_.z - offset_.z;
        break;
    case top_camera_orientation::bottom_left:
        position_render_.x = look_at_render_.x - offset_.x;
        position_render_.z = look_at_render_.z - offset_.z;
        position_.x = look_at_.x - offset_.x;
        position_.z = look_at_.z - offset_.z;
        break;
    case top_camera_orientation::bottom_right:
        position_render_.x = look_at_render_.x - offset_.x;
        position_render_.z = look_at_render_.z + offset_.z;
        position_.x = look_at_.x - offset_.x;
        position_.z = look_at_.z + offset_.z;
        break;
    }
    position_render_.y = look_at_render_.y + offset_.y;
    position_.y = look_at_.y + offset_.y;

    if (observer_)
    {
        observer_->notify_on_camera_update();
    }
}
