#pragma once
#include <string>
#include <assimp/scene.h>
#include <nlohmann/json.hpp>
#include "../graphics/model/object/model.h"
#include "../logic/game_map.h"
#include "../logic/game_scene.h"
#include "../graphics/model/lights.h"
#include "../graphics/model/camera/top_camera.h"

using json = nlohmann::json;

namespace tactics_game
{

struct game_scene_with_other_data
{
    game_scene scene;
    std::vector<std::vector<point_light>> point_lights;
    glm::vec3 world_ambient;
    std::vector<top_camera> player_cameras;
};

class assets_manager
{
public:
    assets_manager();
    explicit assets_manager(std::string root);

    const std::string& get_root() const;
    void set_root(const std::string& root);

    std::string get_shader_source(const std::string& name) const;

    model get_model(const std::string& name) const;

    game_scene_with_other_data get_scene(const std::string& name) const;

private:
    game_map get_map(const std::string& directory_path, const glm::ivec3 size) const;

    static std::string load_text_file(const std::string& path);
    static json load_json(const std::string& path);

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
