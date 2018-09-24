#pragma once

#include "resources_manager.h"

#include <string>
#include <memory>

namespace TacticsGame::Service::Resources
{

class NullResourcesManager : public ResourcesManager
{
public:
    NullResourcesManager() : empty{ "" } {}

    const std::string& getRoot() const override { return this->empty; }
    void setRoot(const std::string& root) override {}

    std::string getShaderSource(const std::string& name) const override { return ""; }

private:
    const std::string empty;
};

}
