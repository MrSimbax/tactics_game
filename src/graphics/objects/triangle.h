#pragma once

#include "graphics_object.h"

#include <array>

namespace TacticsGame::Graphics::Objects
{

class Triangle : public AbstractObject
{
public:
    typedef std::array<float, 3 * 3> VerticesContainer;

    explicit Triangle(VerticesContainer&& vertices);
    ~Triangle() override;

    void render() const override;

private:
    void setUpBuffers();

private:
    VerticesContainer vertices;

    unsigned int vertexBufferObjectId;
    unsigned int vertexArrayObjectId;
};

}
