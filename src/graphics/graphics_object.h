#pragma once

#include <vector>
#include <memory>

namespace TacticsGame::Graphics
{

class Object
{
public:
    explicit Object(std::unique_ptr<std::vector<float>> vertices);

    const std::vector<float>& getVertices() const;
    void setVertices(std::unique_ptr<std::vector<float>> vertices);

private:
    std::unique_ptr<std::vector<float>> vertices;
};

}