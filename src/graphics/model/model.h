#pragma once
#include "buffered_mesh.h"

namespace tactics_game
{
class model
{
public:
    model();
    explicit model(std::vector<std::shared_ptr<mesh>> meshes);

    const std::vector<std::shared_ptr<mesh>>& get_meshes() const;
    void add_mesh(std::shared_ptr<mesh> mesh);

    model transformed(glm::mat4 transform) const;
    std::shared_ptr<mesh> merged() const;

private:
    std::vector<std::shared_ptr<mesh>> meshes_;
};
}
