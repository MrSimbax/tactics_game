#include "utils.h"

namespace tactics_game::utils
{

float random_float()
{
    static std::random_device rd;
    static std::mt19937 e{ rd() };
    static std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };
    return dist(e);
}

int distance2(const glm::ivec3 from, const glm::ivec3 to)
{
    const auto dx = from.x - to.x;
    const auto dy = from.y - to.y;
    const auto dz = from.z - to.z;
    return (dx * dx + dy * dy + dz * dz);
}

std::string get_base_path()
{
    static std::string ret;
    if (ret.empty())
    {
        char* base_path = SDL_GetBasePath();
        ret = base_path;
        SDL_free(base_path);
    }
    return ret;
}

}
