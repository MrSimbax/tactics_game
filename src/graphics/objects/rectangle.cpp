#include "rectangle.h"

#include "../../misc/custom_log.h"

#include <glad/glad.h>

using namespace TacticsGame::Graphics::Objects;

const Rectangle::IndicesContainer Rectangle::indices = {
    0, 1, 2,
    1, 2, 3
};

Rectangle::Rectangle(Rectangle::VerticesContainer&& vertices) :
    vertices{ vertices },
    vertexArrayObjectId{ 0 },
    vertexBufferObjectId{ 0 },
    elementBufferObjectId{ 0 }
{
    this->setUpBuffers();
}

void Rectangle::setUpBuffers()
{
    LOG_DEBUG << "" << this->vertices;

    if (!this->vertexArrayObjectId)
    {
        glGenVertexArrays(1, &this->vertexArrayObjectId);
    }

    if (!this->vertexBufferObjectId)
    {
        glGenBuffers(1, &this->vertexBufferObjectId);
    }

    if (!this->elementBufferObjectId)
    {
        glGenBuffers(1, &this->elementBufferObjectId);
    }

    glBindVertexArray(this->vertexArrayObjectId);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferObjectId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Rectangle::render() const
{
    glBindVertexArray(this->vertexArrayObjectId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Rectangle::~Rectangle()
{
    if (!this->vertexArrayObjectId)
    {
        glDeleteVertexArrays(1, &this->vertexArrayObjectId);
    }

    if (!this->vertexBufferObjectId)
    {
        glDeleteBuffers(1, &this->vertexBufferObjectId);
    }

    if (!this->elementBufferObjectId)
    {
        glDeleteBuffers(1, &this->elementBufferObjectId);
    }
}
