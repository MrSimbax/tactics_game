#include "graphics_object.h"

using namespace TacticsGame::Graphics;

Object::Object(std::unique_ptr<std::vector<float>> vertices) :
    vertices{std::move(vertices)}
{
}

const std::vector<float>& Object::getVertices() const
{
    return *this->vertices.get();
}

void Object::setVertices(std::unique_ptr<std::vector<float>> vertices)
{
    this->vertices = std::move(vertices);
}
