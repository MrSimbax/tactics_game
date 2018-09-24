#pragma once

#include "resources_manager.h"

#include <string>

namespace TacticsGame::Service::Resources
{

class ResourcesManagerImpl : public ResourcesManager
{
public:
    ResourcesManagerImpl();
    ResourcesManagerImpl(const std::string& root);

    const std::string& getRoot() const override;
    void setRoot(const std::string& root) override;

    std::string getShaderSource(const std::string& name) const override;

private:
    std::string root;
    const std::string shadersPath{"shaders/"};
};

}
