#include "assets_manager.h"

#include <array>
#include <fstream>
#include <utility>

#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

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

game_map assets_manager::get_map(const std::string& name) const
{
    const auto directory_path{this->root_ + this->maps_path_ + name};
    if (!exists(std::filesystem::path{directory_path}))
    {
        LOG_ERROR << "Could not load map: the directory " << directory_path << " does not exist";
        return game_map{{}};
    }

    std::vector<game_map::fields_t> layers{};
    for (size_t y = 0; ; ++y)
    {
        auto layer_file_path = directory_path + "/";
        layer_file_path += std::to_string(y);
        layer_file_path += game_map_layer_extension;
        if (!exists(std::filesystem::path{layer_file_path}))
        {
            if (y == 0)
            {
                LOG_WARNING << "The map could not load any layers: " << layer_file_path << " does not exist";
            }
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
        while (std::getline(ss, line))
        {
            std::vector<field_type> column;
            if (line.empty())
                continue;
            for (const auto& c : line)
            {
                column.push_back(static_cast<field_type>(static_cast<int>(c - '0')));
            }
            layer.push_back(column);
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
