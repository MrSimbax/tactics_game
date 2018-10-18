#include "triangle.h"

#include "../../misc/custom_log.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace TacticsGame::Graphics::Objects;

Triangle::Triangle(Triangle::VerticesContainer&& vertices) :
    vertices{ vertices },
    vertexArrayObjectId{ 0 },
    vertexBufferObjectId{ 0 }
{
    this->setUpBuffers();
}

void Triangle::setUpBuffers()
{
    if (!this->vertexArrayObjectId)
    {
        glGenVertexArrays(1, &this->vertexArrayObjectId);
    }

    if (!this->vertexBufferObjectId)
    {
        glGenBuffers(1, &this->vertexBufferObjectId);
    }

    glBindVertexArray(this->vertexArrayObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::render() const
{
    glBindVertexArray(this->vertexArrayObjectId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Triangle::~Triangle()
{
    if (!this->vertexArrayObjectId)
    {
        glDeleteVertexArrays(1, &this->vertexArrayObjectId);
    }

    if (!this->vertexBufferObjectId)
    {
        glDeleteBuffers(1, &this->vertexBufferObjectId);
    }
}
