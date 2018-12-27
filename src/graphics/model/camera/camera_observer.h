#pragma once

namespace tactics_game
{
class camera_observer
{
public:
    camera_observer() = default;
    camera_observer(const camera_observer& other) = default;
    camera_observer& operator=(const camera_observer& other) = default;
    camera_observer(camera_observer&& other) = default;
    camera_observer& operator=(camera_observer&& other) = default;
    virtual ~camera_observer() = default;

    virtual void notify_on_camera_update() = 0;
};
}
