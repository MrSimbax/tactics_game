#include "services.h"

using namespace tactics_game;

std::unique_ptr<assets_service> services::assets_service_ = nullptr;

void services::initialize()
{
    assets_service_.reset(new null_assets_service{});
}

assets_service& services::get_assets_service()
{
    return *assets_service_;
}

void services::provide(std::unique_ptr<assets_service> service)
{
    assets_service_ = std::move(service);
}
