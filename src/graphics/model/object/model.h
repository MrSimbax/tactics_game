#pragma once
#include "../buffered_object/buffered_mesh.h"

namespace tactics_game
{
class model
{
public:
    model();
    explicit model(std::vector<mesh> meshes);

    const std::vector<mesh>& get_meshes() const;
    void add_mesh(mesh mesh);

    model transformed(glm::mat4 transform) const;
    mesh merged() const;

private:
    std::vector<mesh> meshes_;
};
}
