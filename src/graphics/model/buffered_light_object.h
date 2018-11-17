#pragma once
#include "buffered_model.h"
#include "light_object.h"

namespace tactics_game
{
class buffered_light_object
{
public:
    explicit buffered_light_object(const std::shared_ptr<light_object>& object);
    buffered_light_object(const buffered_light_object& other) = delete;
    buffered_light_object& operator=(const buffered_light_object& other) = delete;
    buffered_light_object(buffered_light_object&& other) noexcept = default;
    buffered_light_object& operator=(buffered_light_object&& other) noexcept = default;
    ~buffered_light_object() = default;

    void render(shader_program& program) const;

private:
    std::shared_ptr<light_object> object_;
    buffered_model model_;
};
}
