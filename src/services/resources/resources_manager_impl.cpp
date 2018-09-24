#include "resources_manager_impl.h"

#include "../utils.h"

#include <array>
#include <fstream>
#include <memory>

#include <plog/Log.h>

using namespace TacticsGame::Service::Resources;

ResourcesManagerImpl::ResourcesManagerImpl() :
    root{""}
{
}

ResourcesManagerImpl::ResourcesManagerImpl(const std::string& root) :
    root{root}
{
}

const std::string& ResourcesManagerImpl::getRoot() const
{
    return this->root;
}

void ResourcesManagerImpl::setRoot(const std::string& root)
{
    this->root = root;
}

std::string ResourcesManagerImpl::getShaderSource(const std::string& name) const
{
    std::string path = this->root + this->shadersPath + name;
    std::ifstream shaderFile{ path };
    std::array<char, 512> errorStr;
    if (!shaderFile.is_open() || !shaderFile.good())
    {
        strerror_s(&errorStr[0], errorStr.size(), errno);
        LOG_ERROR << "Could not load shader \"" << path << "\": " << &errorStr[0];
        return "";
    }

    std::ostringstream ss;
    ss << shaderFile.rdbuf();
    return ss.str();
}
