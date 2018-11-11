#pragma once

namespace tactics_game
{
class graphics_object
{
public:
    virtual ~graphics_object() = default;
    virtual void render() const = 0;
};
}
