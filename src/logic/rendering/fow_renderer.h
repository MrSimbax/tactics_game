#pragma once
#include "../../graphics/shader/shader_program.h"
#include "../../graphics/model/texture/quad_xz.h"

namespace tactics_game
{
// fog of war renderer
class fow_renderer
{
public:
    explicit fow_renderer(glm::ivec3 map_size);

    void render(shader_program& program, size_t layer_index) const;

    void update_textures(std::vector<texture::data_t> textures);

private:
    void create_fogs();

    glm::ivec3 map_size_;

    std::vector<quad_xz> fogs_;
};
}
