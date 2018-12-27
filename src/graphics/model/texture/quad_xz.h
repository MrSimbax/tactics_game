#pragma once
#include <glm/glm.hpp>
#include "../buffers/bo.h"
#include "../buffers/vao.h"
#include <vector>
#include "texture.h"

namespace tactics_game
{
class shader_program;

struct quad_vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
};

class quad_xz
{
public:
    quad_xz(glm::vec2 min, glm::vec2 max, float y);

    void render(const shader_program& program) const;

    void set_texture(texture texture);

private:
    void generate_vertices(glm::vec2 min, glm::vec2 max, float y);
    void set_up_buffers() const;

    std::vector<quad_vertex> vertices_;
    std::vector<unsigned> indices_;
    buffer_object vbo_{};
    buffer_object ebo_{};
    vertex_array_object vao_{};
    texture texture_;
};

}
