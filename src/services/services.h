#pragma once

#include "assets/assets_service.h"
#include "assets/null_assets_service.h"

namespace tactics_game
{
class services
{
public:
    static void initialize();

    static assets_service& get_assets_service();

    static void provide(std::unique_ptr<assets_service> service);

private:
    static std::unique_ptr<assets_service> assets_service_;
};
}
