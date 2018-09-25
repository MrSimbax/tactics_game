#pragma once

#include <vector>
#include <memory>

namespace TacticsGame::Graphics
{

class Object
{
public:
    explicit Object(std::vector<float> vertices);

    const std::vector<float>& getVertices() const;
    void setVertices(std::vector<float>&& vertices);

    void render();

private:
    std::vector<float> vertices;

    unsigned int vertexBufferObjectId;
    unsigned int vertexArrayObjectId;
};

}
