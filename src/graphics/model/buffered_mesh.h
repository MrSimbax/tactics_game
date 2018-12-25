#pragma once

#include <memory>

#include "bo.h"
#include "vao.h"
#include "../shader/shader_program.h"
#include "mesh.h"
#include "plog/Log.h"

namespace tactics_game
{
class buffered_mesh
{
public:
    explicit buffered_mesh(mesh m);

    void render(const shader_program& program) const;

private:
    void set_up_buffers() const;
    void set_material(const shader_program& program) const;

    mesh mesh_;

    buffer_object vbo_{};
    buffer_object ebo_{};
    vertex_array_object vao_{};
};
}
