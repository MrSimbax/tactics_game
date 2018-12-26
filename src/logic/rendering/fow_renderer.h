#pragma once
#include "../../graphics/shader/shader_program.h"
#include "../../graphics/model/texture/quad_xz.h"

namespace tactics_game
{
class fow_renderer
{
public:
    explicit fow_renderer(glm::ivec3 map_size);

    void render(shader_program& program, size_t layer_index) const;

private:
    void create_fogs();

    glm::ivec3 map_size_;

    std::vector<quad_xz> fogs_;
};
}
