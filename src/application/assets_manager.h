#pragma once
#include <string>
#include <assimp/scene.h>
#include "../graphics/model/model.h"
#include "../logic/game_map.h"

namespace tactics_game
{
class assets_manager
{
public:
    assets_manager();
    explicit assets_manager(std::string root);

    const std::string& get_root() const;
    void set_root(const std::string& root);

    std::string get_shader_source(const std::string& name) const;

    model get_model(const std::string& name) const;

    game_map get_map(const std::string& name) const;

private:
    static std::string load_text_file(const std::string& path);

    static std::vector<mesh> process_model_node(aiNode* ai_node, const aiScene* scene);
    static mesh process_model_mesh(aiMesh* ai_mesh, const aiScene* scene);

    std::string root_;
    const std::string shaders_path_{"shaders/"};
    const std::string images_path_{"images/"};
    const std::string models_path_{"models/"};
    const std::string maps_path_{"maps/"};

    static const std::string game_map_layer_extension;
    static const std::string game_map_main_file_name;
};
}
