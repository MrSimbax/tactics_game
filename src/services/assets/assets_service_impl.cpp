#include "assets_service_impl.h"

#include <array>
#include <fstream>
#include <utility>

#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace tactics_game;

assets_service_impl::assets_service_impl()
    : root_{""}
{
}

assets_service_impl::assets_service_impl(std::string root)
    : root_{std::move(root)}
{
}

const std::string& assets_service_impl::get_root() const
{
    return this->root_;
}

void assets_service_impl::set_root(const std::string& root)
{
    this->root_ = root;
}

std::string assets_service_impl::get_shader_source(const std::string& name) const
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

model assets_service_impl::get_model(const std::string& name) const
{
    Assimp::Importer importer;
    const auto scene = importer.ReadFile(root_ + models_path_ + name, 0);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR << "Could not load model using Assimp: " << importer.GetErrorString();
        model empty{{}};
        return empty;
    }
    const auto meshes = process_model_node(scene->mRootNode, scene);
    return model{ meshes };
}

std::vector<mesh> assets_service_impl::process_model_node(aiNode* const ai_node, const aiScene* const scene)
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

mesh assets_service_impl::process_model_mesh(aiMesh* ai_mesh, const aiScene* const scene)
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
