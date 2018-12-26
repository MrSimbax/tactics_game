#pragma once

#include <stdexcept>

namespace tactics_game
{
class buffer_object
{
public:
    buffer_object();
    buffer_object(const buffer_object& other) = delete;
    buffer_object& operator=(const buffer_object& other) = delete;
    buffer_object(buffer_object&& other) noexcept;
    buffer_object& operator=(buffer_object&& other) noexcept;
    ~buffer_object();

    unsigned int get_id() const;
    void bind(int target) const;
    static void unbind(int target);

private:
    void create_buffer();

    unsigned int id_;
};

class buffer_object_not_created_error : public std::runtime_error
{
public:
    explicit buffer_object_not_created_error(const char* what)
        : runtime_error(what)
    {
    }
};
}
