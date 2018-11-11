#include "rectangle.h"

#include "../../misc/custom_log.h"

#include <glad/glad.h>

using namespace tactics_game::Graphics::Objects;

const Rectangle::IndicesContainer Rectangle::indices = {
    0, 1, 2,
    1, 2, 3
};

Rectangle::Rectangle(VerticesContainer&& vertices)
    :
    vertices{vertices},
    vertexBufferObjectId{0},
    vertexArrayObjectId{0},
    elementBufferObjectId{0}
{
    this->setUpBuffers();
}

Rectangle::Rectangle(const Rectangle& other)
    :
    vertices{other.vertices},
    vertexBufferObjectId{0},
    vertexArrayObjectId{0},
    elementBufferObjectId{0}
{
    this->setUpBuffers();
}

tactics_game::Graphics::Objects::Rectangle& Rectangle::operator=(const Rectangle& other)
{
    this->vertices = other.vertices;
    this->vertexArrayObjectId = 0;
    this->vertexBufferObjectId = 0;
    this->elementBufferObjectId = 0;
    this->setUpBuffers();
    return *this;
}

Rectangle::Rectangle(Rectangle&& other)
    :
    vertices{std::move(other.vertices)},
    vertexBufferObjectId{other.vertexBufferObjectId},
    vertexArrayObjectId{other.vertexArrayObjectId},
    elementBufferObjectId{other.elementBufferObjectId}
{
    other.vertexArrayObjectId = 0;
    other.vertexBufferObjectId = 0;
    other.elementBufferObjectId = 0;
}

tactics_game::Graphics::Objects::Rectangle& Rectangle::operator=(Rectangle&& other)
{
    this->vertices = std::move(other.vertices);
    this->vertexArrayObjectId = other.vertexArrayObjectId;
    this->vertexBufferObjectId = other.vertexBufferObjectId;
    this->elementBufferObjectId = other.elementBufferObjectId;

    other.vertexArrayObjectId = 0;
    other.vertexBufferObjectId = 0;
    other.elementBufferObjectId = 0;
    return *this;
}

void Rectangle::setUpBuffers()
{
    if (!this->vertexArrayObjectId)
    {
        glGenVertexArrays(1, &this->vertexArrayObjectId);
        if (!this->vertexArrayObjectId)
        {
            LOG_WARNING << "Could not create vertex array object";
            return;
            }
        }

        if (!this->vertexBufferObjectId)
        {
            glGenBuffers(1, &this->vertexBufferObjectId);
            if (!this->vertexBufferObjectId)
            {
                LOG_WARNING << "Could not create vertex buffer object";
                this->vertexArrayObjectId = 0;
                return;
                }
            }

            if (!this->elementBufferObjectId)
            {
                glGenBuffers(1, &this->elementBufferObjectId);
                if (!this->elementBufferObjectId)
                {
                    LOG_WARNING << "Could not create element buffer object";
                    this->vertexArrayObjectId = 0;
                    this->vertexBufferObjectId = 0;
                    return;
                    }
                }

                glBindVertexArray(this->vertexArrayObjectId);

                glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObjectId);
                glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0],
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferObjectId);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0],
                             GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, static_cast<void*>(nullptr));
                glEnableVertexAttribArray(0);

                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

void Rectangle::render() const
{
    glBindVertexArray(this->vertexArrayObjectId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
