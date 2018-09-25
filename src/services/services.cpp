#include "services.h"

using namespace TacticsGame;

std::unique_ptr<Service::Resources::ResourcesManager> Services::resourcesManager = nullptr;
Service::Resources::NullResourcesManager Services::nullResourcesManager{};

void Services::initialize()
{
    // Here would be setting of every service to its null alernative but I dont know how to do that with unique_ptr
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
