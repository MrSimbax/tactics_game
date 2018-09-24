#pragma once

#include "resources/resources_manager.h"
#include "resources/resources_manager_null.h"

namespace TacticsGame
{

class Services
{
public:
    static void initialize();

    static Service::Resources::ResourcesManager& getResourcesManager();

    static void provide(std::unique_ptr<Service::Resources::ResourcesManager> manager);

private:
    static std::unique_ptr<Service::Resources::ResourcesManager> resourcesManager;
    static Service::Resources::NullResourcesManager nullResourcesManager;
};

}
