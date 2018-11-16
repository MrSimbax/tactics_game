#include "assets_manager.h"

#include <array>
#include <fstream>
#include <filesystem>
#include <utility>

#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace tactics_game;

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
    std::array<char, 512> error_str{};
    if (!shader_file.is_open() || !shader_file.good())
    {
        strerror_s(&error_str[0], error_str.size(), errno);
        LOG_ERROR << "Could not load shader \"" << path << "\": " << &error_str[0];
        return "";
    }

    std::ostringstream ss;
    ss << shader_file.rdbuf();
    return ss.str();
}

model assets_manager::get_model(const std::string& name) const
{
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

game_scene assets_manager::get_scene(const std::string& name) const
{
    const auto directory_path{this->root_ + this->maps_path_ + name};
    if (!exists(std::filesystem::path{directory_path}))
    {
        LOG_ERROR << "Could not load scene: the directory " << directory_path << " does not exist";
        return game_scene{};
    }

    auto scene_file_path = directory_path + "/";
    scene_file_path += game_map_main_file_name;
    if (!exists(std::filesystem::path{scene_file_path}))
    {
        LOG_ERROR << "The map does not have main file: " << scene_file_path << " does not exist";
        return game_scene{};
    }

    auto info = load_json(scene_file_path);
    const std::string scene_name = info["name"];
    const glm::ivec3 size{info["size"]["x"], info["size"]["y"], info["size"]["z"]};

    const std::shared_ptr<game_map> map{new game_map{get_map(directory_path, size)}};

    std::vector<std::shared_ptr<player>> players;
    auto players_json = info["players"];
    size_t player_id = 0;
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
        }
        players.emplace_back(new player{player_id, units});
        player_id += 1;
    }

    return game_scene{scene_name, map, players};
}

game_map assets_manager::get_map(const std::string& directory_path, const glm::ivec3 size) const
{
    std::vector<game_map::fields_t> layers{};
    for (auto y = 0; y < size.y; ++y)
    {
        auto layer_file_path = directory_path + "/";
        layer_file_path += std::to_string(y);
        layer_file_path += game_map_layer_extension;
        if (!exists(std::filesystem::path{layer_file_path}))
        {
            LOG_WARNING << "The map could not load layer: " << layer_file_path << " does not exist";
            break;
        }

        auto layer_data = load_text_file(layer_file_path);
        if (layer_data.empty())
        {
            break;
        }

        game_map::fields_t layer{};
        std::stringstream ss{layer_data};
        std::string line;
        auto x = 0;
        while (std::getline(ss, line))
        {
            if (line.empty())
                continue;
            if (x >= size.x)
            {
                LOG_WARNING << "Layer " << y << " too high, discarding the rest";
                break;
            }

            std::vector<field_type> row;
            auto z = 0;
            for (const auto& c : line)
            {
                if (z >= size.z)
                {
                    LOG_WARNING << "Layer " << y << "too wide on line " << z << ", discarding the rest";
                    break;
                }
                row.push_back(static_cast<field_type>(static_cast<int>(c - '0')));
                ++z;
            }
            if (z < size.z)
            {
                LOG_WARNING << "Layer " << y << " too thin on line " << z << ", appending empty columns";
                while (z < size.z)
                {
                    row.push_back(static_cast<field_type>(0));
                    ++z;
                }
            }
            layer.push_back(row);
            ++x;
        }
        if (x < size.x)
        {
            LOG_WARNING << "Layer " << y << " too low, appending empty rows";
            while (x < size.z)
            {
                std::vector<field_type> row;
                for (auto z = 0; z < size.z; ++z)
                {
                    row.push_back(static_cast<field_type>(0));
                }
                layer.push_back(row);
                ++x;
            }
        }
        layers.push_back(layer);
    }

    return game_map{layers};
}

std::string assets_manager::load_text_file(const std::string& path)
{
    std::ifstream text_file{path};
    std::array<char, 512> error_str{};
    if (!text_file.is_open() || !text_file.good())
    {
        strerror_s(&error_str[0], error_str.size(), errno);
        LOG_ERROR << "Could not load text file \"" << path << "\": " << &error_str[0];
        return "";
    }

    std::ostringstream ss;
    ss << text_file.rdbuf();
    return ss.str();
}

json assets_manager::load_json(const std::string& path)
{
    std::ifstream text_file{path};
    std::array<char, 512> error_str{};
    if (!text_file.is_open() || !text_file.good())
    {
        strerror_s(&error_str[0], error_str.size(), errno);
        LOG_ERROR << "Could not load JSON file \"" << path << "\": " << &error_str[0];
        return "";
    }

    json j;
    text_file >> j;
    return j;
}

std::vector<mesh> assets_manager::process_model_node(aiNode* const ai_node, const aiScene* const scene)
{
    std::vector<mesh> meshes;

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

mesh assets_manager::process_model_mesh(aiMesh* ai_mesh, const aiScene* const scene)
{
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

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

    return mesh{vertices, indices};
}
