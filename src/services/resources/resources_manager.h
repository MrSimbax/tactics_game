#pragma once

#include <string>

namespace TacticsGame::Service::Resources
{

class ResourcesManager
{
public:
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    virtual ~ResourcesManager() {};

    virtual const std::string& getRoot() const = 0;
    virtual void setRoot(const std::string& root) = 0;

    // the return type saves one line of code when passing the string to glShaderSource, since &str.c_str() doesn't work
    virtual std::string getShaderSource(const std::string& name) const = 0;

protected:
    ResourcesManager() {};
};

}
