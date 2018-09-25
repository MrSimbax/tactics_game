#pragma once

#include <vector>

#include <plog/Log.h>

namespace plog
{

template<typename T>
Record& operator<<(Record& record, const std::vector<T>& vector)
{
    record << "\n{";
    for (const auto& value : vector)
    {
        record << "    " << value << ",\n";
    }
    record << "}\n";
    return record;
}

}
