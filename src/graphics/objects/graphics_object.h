#pragma once

#include <vector>
#include <memory>

namespace TacticsGame::Graphics::Objects
{

class AbstractObject
{
public:
    virtual ~AbstractObject() {};
    virtual void render() const = 0;
};

}
