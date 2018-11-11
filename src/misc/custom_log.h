#pragma once

#include <array>
#include <vector>

#include <plog/Log.h>

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
}
