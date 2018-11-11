#include "assets_service_impl.h"

#include <array>
#include <fstream>
#include <utility>

#include <plog/Log.h>

using namespace tactics_game;

assets_service_impl::assets_service_impl()
    : root_{""}
{
}

assets_service_impl::assets_service_impl(std::string root)
    : root_{std::move(root)}
{
}

const std::string& assets_service_impl::get_root() const
{
    return this->root_;
}

void assets_service_impl::set_root(const std::string& root)
{
    this->root_ = root;
}

std::string assets_service_impl::get_shader_source(const std::string& name) const
{
    const auto path = this->root_ + this->shaders_path_ + name;
    std::ifstream shader_file{path};
    std::array<char, 512> error_str{};
    if (!shader_file.is_open() || !shader_file.good())
    {
        strerror_s(&error_str[0], error_str.size(), errno);
        LOG_ERROR << "Could not load shader \"" << path << "\": " << &error_str[0];
        return "";
    }

    std::ostringstream ss;
    ss << shader_file.rdbuf();
    return ss.str();
}
