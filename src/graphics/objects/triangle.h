#pragma once

#include "../model/graphics_object.h"
#include "../model/bo.h"
#include "../model/vao.h"

#include <array>

namespace tactics_game
{
class triangle : public graphics_object
{
public:
    typedef std::array<float, 3 * 3> vertices_container;

    explicit triangle(vertices_container&& vertices);
    triangle(const triangle& other);
    triangle& operator=(const triangle& other);
    triangle(triangle&& other) noexcept;
    triangle& operator=(triangle&& other) noexcept;
    ~triangle() override;

    void render() const override;

private:
    void set_up_buffers();

private:
    vertices_container vertices_;

    buffer_object vbo_;
    vertex_array_object vao_;
};
}
