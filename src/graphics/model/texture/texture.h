#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

namespace tactics_game
{
class texture
{
public:
    typedef std::vector<glm::vec<4, uint8_t>> data_t;

    texture();
    texture(data_t data, unsigned width, unsigned height);

    texture(const texture& other) = delete;
    texture& operator=(const texture& other) = delete;

    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;

    ~texture();

    void bind() const;
    static void unbind();

private:
    void set_up_texture();

    unsigned id_{};

    data_t data_;
    unsigned width_;
    unsigned height_;
};

class texture_not_created_error : public std::runtime_error
{
public:
    explicit texture_not_created_error(const char* what)
        : runtime_error(what)
    {
    }
};
}
