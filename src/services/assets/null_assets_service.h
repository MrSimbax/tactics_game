#pragma once

#include "assets_service.h"

#include <string>

namespace tactics_game
{
class null_assets_service : public assets_service
{
public:
    null_assets_service()
        : empty_{""}
    {
    }

    const std::string& get_root() const override
    {
        return this->empty_;
    }

    void set_root(const std::string& root) override
    {
    }

    std::string get_shader_source(const std::string& name) const override
    {
        return this->empty_;
    }

    model get_model(const std::string& name) const override
    {
        model ret{{}};
        return ret;
    }

private:
    std::string empty_;
};
}
