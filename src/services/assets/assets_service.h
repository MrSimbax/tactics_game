#pragma once

#include <string>

namespace tactics_game
{
class assets_service
{
public:
    assets_service(const assets_service&) = delete;
    assets_service& operator=(const assets_service&) = delete;
    assets_service(const assets_service&&) = delete;
    assets_service& operator=(const assets_service&&) = delete;
    virtual ~assets_service() = default;

    virtual const std::string& get_root() const = 0;
    virtual void set_root(const std::string& root) = 0;

    virtual std::string get_shader_source(const std::string& name) const = 0;

protected:
    assets_service() = default;;
};
}
