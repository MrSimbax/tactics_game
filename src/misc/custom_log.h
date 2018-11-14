#pragma once

#include <array>
#include <vector>

#include <plog/Log.h>
#include <glm/glm.hpp>

namespace plog
{
template <typename T>
Record& operator<<(Record& record, const std::vector<T>& container)
{
    record << "\n{";
    const auto separator = ", ";
    const auto* sep = "";
    for (const auto& value : container)
    {
        record << sep << value;
        sep = separator;
    }
    record << "}";
    return record;
}

template <typename T, size_t Size>
Record& operator<<(Record& record, const std::array<T, Size>& container)
{
    record << "\n{";
    const auto separator = ", ";
    const auto* sep = "";
    for (const auto& value : container)
    {
        record << sep << value;
        sep = separator;
    }
    record << "}";
    return record;
}

inline Record& operator<<(Record& record, const glm::mat4 matrix)
{
    for (unsigned i = 0; i < 4; ++i)
    {
        record << "\n[";
        for (unsigned j = 0; j < 4; ++j)
        {
            record << matrix[i][j];
            if (j < 3)
                record << ", ";
        }
        record << "]";
    }
    return record;
}

inline Record& operator<<(Record& record, const glm::vec3 vec)
{
    return record << "\n[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
}

inline Record& operator<<(Record& record, const glm::ivec2 vec)
{
    return record << "\n[" << vec.x << ", " << vec.y << "]";
}

inline Record& operator<<(Record& record, const glm::vec2 vec)
{
    return record << "\n[" << vec.x << ", " << vec.y << "]";
}
}
