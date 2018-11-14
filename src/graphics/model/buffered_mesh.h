#pragma once

#include "bo.h"
#include "vao.h"
#include "../shader/shader_program.h"
#include "mesh.h"

namespace tactics_game
{
class buffered_mesh
{
public:
    explicit buffered_mesh(const mesh& m);
    buffered_mesh(const buffered_mesh& other) = delete;
    buffered_mesh& operator=(const buffered_mesh& other) = delete;
    buffered_mesh(buffered_mesh&& other) noexcept;
    buffered_mesh& operator=(buffered_mesh&& other) noexcept;
    ~buffered_mesh();

    void render(const shader_program& program) const;

private:
    void set_up_buffers(const mesh& mesh) const;

    int indices_size_;

    buffer_object vbo_{};
    buffer_object ebo_{};
    vertex_array_object vao_{};
};
}
