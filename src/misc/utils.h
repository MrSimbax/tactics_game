#pragma once

#include <vector>
#include <numeric>
#include <functional>
#include <random>

namespace tactics_game::utils
{
/*template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& v)
{
    std::size_t total_size = std::reduce(v.begin(), v.end(), static_cast<size_t>(0),
                                         [](size_t acc, const std::vector<T>& v)
                                         {
                                             return acc + v.size();
                                         });
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

template <typename T, typename Q, typename F>
std::vector<Q> flatten(std::vector<T>& v, F vector_getter)
{
    std::size_t total_size = std::reduce(v.begin(), v.end(), static_cast<size_t>(0),
                                         [&](size_t acc, T& second)
                                         {
                                             return acc + vector_getter(second).size();
                                         });
    std::vector<Q> result;
    result.reserve(total_size);
    for (T& t : v)
    {
        std::vector<Q> q = vector_getter(t);
        result.insert(result.end(), q.begin(), q.end());
    }
    return result;
}*/

inline float random_float()
{
    static std::random_device rd;
    static std::mt19937 e{rd()};
    static std::uniform_real_distribution<float> dist{0.0f, 1.0f};
    return dist(e);
}
}
