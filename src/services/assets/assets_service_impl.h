#pragma once

#include "assets_service.h"

#include <string>
#include <assimp/scene.h>

namespace tactics_game
{
class assets_service_impl : public assets_service
{
public:
    assets_service_impl();
    explicit assets_service_impl(std::string root);

    const std::string& get_root() const override;
    void set_root(const std::string& root) override;

    std::string get_shader_source(const std::string& name) const override;

    model get_model(const std::string& name) const override;

private:
    static std::vector<mesh> process_model_node(aiNode* const ai_node, const aiScene* const scene);
    static mesh process_model_mesh(aiMesh* ai_mesh, const aiScene* const scene);

    std::string root_;
    const std::string shaders_path_{"shaders/"};
    const std::string images_path_{"images/"};
    const std::string models_path_{"models/"};
};
}
