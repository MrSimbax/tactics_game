#include "assets_manager.h"

#include <array>
#include <fstream>

#ifdef __GNUG__
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include <utility>

#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../misc/custom_log.h"

using namespace tactics_game;


std::string get_fstream_error()
{
#ifdef __GNUG__
    return {strerror(errno)};
#else
    std::array<char, 512> error_str{};
    strerror_s(&error_str[0], error_str.size(), errno);
    return {error_str};
#endif
}

const std::string assets_manager::game_map_layer_extension{".layer"};
const std::string assets_manager::game_map_main_file_name{"map.json"};

assets_manager::assets_manager()
    : root_{""}
{
}

assets_manager::assets_manager(std::string root)
    : root_{std::move(root)}
{
}

const std::string& assets_manager::get_root() const
{
    return this->root_;
}

void assets_manager::set_root(const std::string& root)
{
    this->root_ = root;
}

std::string assets_manager::get_shader_source(const std::string& name) const
{
    const auto path = this->root_ + this->shaders_path_ + name;
    std::ifstream shader_file{path};
    
    if (!shader_file.is_open() || !shader_file.good())
    {
        LOG_ERROR << "Could not load shader \"" << path << "\": " << get_fstream_error();
        return "";
    }

    std::ostringstream ss;
    ss << shader_file.rdbuf();
    return ss.str();
}

model assets_manager::get_model(const std::string& name) const
{
    LOG_INFO << "Loading " << name;
    Assimp::Importer importer;
    const auto scene = importer.ReadFile(root_ + models_path_ + name, 0);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR << "Could not load model using Assimp: " << importer.GetErrorString();
        return model{{}};
    }
    const auto meshes = process_model_node(scene->mRootNode, scene);
    return model{meshes};
}

game_scene_with_other_data assets_manager::get_scene(const std::string& name) const
{
    // Load from file
    const auto directory_path{this->root_ + this->maps_path_ + name};
    if (!exists(fs::path{directory_path}))
    {
        LOG_ERROR << "Could not load scene: the directory " << directory_path << " does not exist";
        return {};
    }

    auto scene_file_path = directory_path + "/";
    scene_file_path += game_map_main_file_name;
    if (!exists(fs::path{scene_file_path}))
    {
        LOG_ERROR << "The map does not have main file: " << scene_file_path << " does not exist";
        return {};
    }

    auto info = load_json(scene_file_path);
    const std::string scene_name = info["name"];
    const glm::ivec3 size{info["size"]["x"], info["size"]["y"], info["size"]["z"]};

    // Load map
    const std::shared_ptr<game_map> map{new game_map{get_map(directory_path, size)}};

    // Load player data
    std::vector<std::shared_ptr<player>> players;
    auto players_json = info["players"];
    size_t player_id = 0;
    std::vector<std::shared_ptr<top_camera>> player_cameras;
    for (const auto& player_json : players_json)
    {
        std::vector<std::shared_ptr<game_unit>> units;
        size_t unit_id = 0;
        for (const auto& unit_json : player_json["units"])
        {
            auto position = unit_json["position"];
            units.emplace_back(new game_unit{
                unit_id, player_id, glm::ivec3{position["x"], position["y"], position["z"]}
            });
            unit_id += 1;
            map->set_blocked(units.back()->get_position(), true);
        }
        players.emplace_back(new player{player_id, units});
        
        player_cameras.emplace_back(new top_camera);
        auto camera_json = player_json["camera"];
        player_cameras[player_id]->set_target(glm::vec3{camera_json["lookAt"]["x"], camera_json["lookAt"]["y"], camera_json["lookAt"]["z"]});
        player_cameras[player_id]->set_current_layer(camera_json["lookAt"]["y"]);
        auto orient = camera_json["orientation"];
        if (orient == "top-left")
            player_cameras[player_id]->set_orientation(top_camera_orientation::top_left);
        else if (orient == "top-right")
            player_cameras[player_id]->set_orientation(top_camera_orientation::top_right);
        else if (orient == "bottom-right")
            player_cameras[player_id]->set_orientation(top_camera_orientation::bottom_right);
        else
            player_cameras[player_id]->set_orientation(top_camera_orientation::bottom_left);
        player_id += 1;
    }


    // Load lights
    std::vector<std::vector<point_light>> point_lights;
    point_lights.resize(map->get_size().y);
    for (const auto& light : info["lights"])
    {
        if (light["type"] != "point") continue;
        const auto y = static_cast<int>(light["position"][1]);
        point_lights[y].push_back({
            glm::vec3{light["position"][0], light["position"][1], light["position"][2]},
            glm::vec3{light["ambient"][0], light["ambient"][1], light["ambient"][2]},
            glm::vec3{light["diffuse"][0], light["diffuse"][1], light["diffuse"][2]},
            glm::vec3{light["specular"][0], light["specular"][1], light["specular"][2]},
            light["constant"],
            light["linear"],
            light["quadratic"]
        });
    }

    const auto world_ambient = glm::vec3(info["worldAmbient"][0], info["worldAmbient"][1], info["worldAmbient"][2]);

    return {
        game_scene{scene_name, map, players},
        point_lights,
        world_ambient,
        player_cameras
    };
}

game_map assets_manager::get_map(const std::string& directory_path, const glm::ivec3 size) const
{
    std::vector<game_map::tiles_t> layers{};
    for (auto y = 0; y < size.y; ++y)
    {
        auto layer_file_path = directory_path + "/";
        layer_file_path += std::to_string(y);
        layer_file_path += game_map_layer_extension;
        if (!exists(fs::path{layer_file_path}))
        {
            LOG_WARNING << "The map could not load layer: " << layer_file_path << " does not exist";
            break;
        }

        auto layer_data = load_text_file(layer_file_path);
        if (layer_data.empty())
        {
            break;
        }

        game_map::tiles_t layer{};
        layer.resize(size.x);

        std::stringstream ss{layer_data};
        std::string line;
        auto x = size.x - 1;;
        while (std::getline(ss, line))
        {
            if (line.empty())
                continue;
            if (x < 0)
            {
                LOG_WARNING << "Layer " << y << " too high, discarding the rest";
                break;
            }

            std::vector<tile_type> row;
            auto z = 0;
            for (const auto& c : line)
            {
                if (z >= size.z)
                {
                    LOG_WARNING << "Layer " << y << "too wide on line " << z << ", discarding the rest";
                    break;
                }
                row.push_back(static_cast<tile_type>(static_cast<int>(c - '0')));
                ++z;
            }
            if (z < size.z)
            {
                LOG_WARNING << "Layer " << y << " too thin on line " << z << ", appending empty columns";
                while (z < size.z)
                {
                    row.push_back(static_cast<tile_type>(0));
                    ++z;
                }
            }
            layer[x] = row;
            --x;
        }
        if (x >= 0)
        {
            LOG_WARNING << "Layer " << y << " too low, appending empty rows";
            while (x >= 0)
            {
                std::vector<tile_type> row;
                for (auto z = 0; z < size.z; ++z)
                {
                    row.push_back(static_cast<tile_type>(0));
                }
                layer.push_back(row);
                --x;
            }
        }
        layers.push_back(layer);
    }

    return game_map{layers};
}

std::string assets_manager::load_text_file(const std::string& path)
{
    std::ifstream text_file{path};
    if (!text_file.is_open() || !text_file.good())
    {
        LOG_ERROR << "Could not load text file \"" << path << "\": " << get_fstream_error();
        return "";
    }

    std::ostringstream ss;
    ss << text_file.rdbuf();
    return ss.str();
}

json assets_manager::load_json(const std::string& path)
{
    std::ifstream text_file{path};
    if (!text_file.is_open() || !text_file.good())
    {
        LOG_ERROR << "Could not load JSON file \"" << path << "\": " << get_fstream_error();
        return "";
    }

    json j;
    text_file >> j;
    return j;
}

std::vector<std::shared_ptr<mesh>> assets_manager::process_model_node(aiNode* const ai_node, const aiScene* const scene)
{
    std::vector<std::shared_ptr<mesh>> meshes;

    for (unsigned int i = 0; i < ai_node->mNumMeshes; ++i)
    {
        const auto node_mesh = scene->mMeshes[ai_node->mMeshes[i]];
        meshes.push_back(process_model_mesh(node_mesh, scene));
    }

    for (unsigned int i = 0; i < ai_node->mNumChildren; ++i)
    {
        auto children_meshes = process_model_node(ai_node->mChildren[i], scene);
        meshes.insert(meshes.end(), children_meshes.begin(), children_meshes.end());
    }

    return meshes;
}

std::shared_ptr<mesh> assets_manager::process_model_mesh(aiMesh* ai_mesh, const aiScene* const scene)
{
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    material mat{};

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i)
    {
        vertex v{};
        v.position = {ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z};
        v.normal = {ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z};
        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        const auto face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (ai_mesh->mMaterialIndex >= 0)
    {
        const auto ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
        aiColor3D ai_color;

        // ReSharper disable once CppExpressionWithoutSideEffects
        ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ai_color);
        mat.ambient = glm::vec3(ai_color.r, ai_color.g, ai_color.b);

        // ReSharper disable once CppExpressionWithoutSideEffects
        ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
        mat.diffuse = glm::vec3(ai_color.r, ai_color.g, ai_color.b);

        // ReSharper disable once CppExpressionWithoutSideEffects
        ai_material->Get(AI_MATKEY_COLOR_SPECULAR, ai_color);
        mat.specular = glm::vec3(ai_color.r, ai_color.g, ai_color.b);

        auto f{0.0f};
        // ReSharper disable once CppExpressionWithoutSideEffects
        ai_material->Get(AI_MATKEY_SHININESS, f);
        mat.shininess = f;
    }

    return std::make_shared<mesh>(vertices, indices, mat);
}

