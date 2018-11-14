#pragma once

#include <stdexcept>

namespace tactics_game
{
class vertex_array_object
{
public:
    vertex_array_object();
    vertex_array_object(const vertex_array_object& other) = delete;
    vertex_array_object& operator=(const vertex_array_object& other) = delete;
    vertex_array_object(vertex_array_object&& other) noexcept;
    vertex_array_object& operator=(vertex_array_object&& other) noexcept;
    ~vertex_array_object();

    unsigned int get_id() const;
    void bind() const;
    static void unbind();

private:
    void create_array();

    unsigned int id_;
};

class vertex_array_object_not_created_error : public std::runtime_error
{
public:
    explicit vertex_array_object_not_created_error(const char* what)
        : runtime_error(what)
    {
    }
};
}
