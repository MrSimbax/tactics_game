#include "fow_renderer.h"
#include <glad/glad.h>

using namespace tactics_game;

fow_renderer::fow_renderer(glm::ivec3 map_size)
    : map_size_{map_size}
{
    create_fogs();
}

void fow_renderer::render(shader_program& program, const size_t layer_index) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto y = 0u; y <= layer_index; ++y)
        fogs_[y].render(program);
    glDisable(GL_BLEND);
}

void fow_renderer::update_textures(std::vector<texture::data_t> textures)
{
    for (auto y = 0; y < map_size_.y; ++y)
    {
        fogs_[y].set_texture(texture{
            textures[y], static_cast<unsigned>(map_size_.x), static_cast<unsigned>(map_size_.z)
        });
    }
}

void fow_renderer::create_fogs()
{
    fogs_.reserve(map_size_.y);
    for (auto y = 0; y < map_size_.y; ++y)
    {
        fogs_.emplace_back(glm::vec2{-0.5f, -0.5f}, glm::vec2{map_size_.x - 0.5f, map_size_.z - 0.5f},
                           y + 0.95f);
    }
}
