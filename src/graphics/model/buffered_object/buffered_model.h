#pragma once
#include "buffered_mesh.h"
#include "../object/model.h"

namespace tactics_game
{
class buffered_model
{
public:
    explicit buffered_model(const model& model);
    explicit buffered_model(std::vector<buffered_mesh>&& meshes);

    buffered_model(const buffered_model& other) = delete;
    buffered_model& operator=(const buffered_model& other) = delete;
    
    buffered_model(buffered_model&& other) noexcept = default;
    buffered_model& operator=(buffered_model&& other) noexcept = default;
    
    ~buffered_model() = default;

    void render(shader_program& program) const;

private:
    std::vector<buffered_mesh> meshes_;
};
}
