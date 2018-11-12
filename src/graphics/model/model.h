#pragma once
#include "mesh.h"

namespace tactics_game
{
class model
{
public:
    explicit model(std::vector<mesh> meshes);

    void render(shader_program& program) const;

private:
    std::vector<mesh> meshes_;
};
}
