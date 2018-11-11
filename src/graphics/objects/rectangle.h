#pragma once

#include "graphics_object.h"

#include <array>

namespace tactics_game::Graphics::Objects
{
class Rectangle : public AbstractObject
{
public:
    typedef std::array<float, 3 * 4> VerticesContainer;
    typedef std::array<unsigned int, 3 * 2> IndicesContainer;

    /**
     * vertices:
     * 0-1
     * |/|
     * 2-3
     */
    explicit Rectangle(VerticesContainer&& vertices);
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);
    Rectangle(Rectangle&& other);
    Rectangle& operator=(Rectangle&& other);
    ~Rectangle() override;

    void render() const override;

private:
    void setUpBuffers();

private:
    static const IndicesContainer indices;
    VerticesContainer vertices;

    unsigned int vertexBufferObjectId;
    unsigned int vertexArrayObjectId;
    unsigned int elementBufferObjectId;
};
}
