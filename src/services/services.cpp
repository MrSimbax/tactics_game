#include "services.h"

using namespace TacticsGame;

std::unique_ptr<Service::Resources::ResourcesManager> Services::resourcesManager = nullptr;
Service::Resources::NullResourcesManager Services::nullResourcesManager{};

void Services::initialize()
{
}

Service::Resources::ResourcesManager& Services::getResourcesManager()
{
    if (!Services::resourcesManager) return Services::nullResourcesManager;
    return *Services::resourcesManager;
}

void Services::provide(std::unique_ptr<Service::Resources::ResourcesManager> resourcesManager)
{
    Services::resourcesManager = std::move(resourcesManager);
}
