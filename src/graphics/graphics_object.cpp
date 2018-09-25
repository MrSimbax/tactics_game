#include "graphics_object.h"
#include "../misc/custom_log.h"

#include <glad/glad.h>
#include <plog/Log.h>

using namespace TacticsGame::Graphics;

Object::Object(std::vector<float> vertices) :
    vertexArrayObjectId{ 0 },
    vertexBufferObjectId{ 0 }
{
    this->setVertices(std::move(vertices));
}

const std::vector<float>& Object::getVertices() const
{
    return this->vertices;
}

void Object::setVertices(std::vector<float>&& vertices)
{
    this->vertices = vertices;

    if (this->vertices.empty())
    {
        if (!vertexArrayObjectId)
        {
            glDeleteVertexArrays(1, &this->vertexArrayObjectId);
        }

        if (!vertexBufferObjectId)
        {
            glDeleteBuffers(1, &this->vertexBufferObjectId);
        }

        return;
    }

    //LOG_DEBUG << this->vertices;

    if (!vertexArrayObjectId)
    {
        glGenVertexArrays(1, &this->vertexArrayObjectId);
    }

    if (!vertexBufferObjectId)
    {
        glGenBuffers(1, &this->vertexBufferObjectId);
    }

    glBindVertexArray(this->vertexArrayObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::render()
{
    if (this->vertices.empty()) return;

    glBindVertexArray(this->vertexArrayObjectId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
