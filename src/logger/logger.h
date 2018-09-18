#pragma once

#include <ostream>
#include <forward_list>
#include <memory>

namespace TacticsGame
{
namespace Logger
{

enum class LoggerLevel
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

struct LoggerStream
{
    std::string prefix;
    const LoggerLevel level;
};

class Logger
{
public:
    Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void setLoggerLevel(LoggerLevel level);
    LoggerLevel getCurrentLoggerLevel() const;

    void addStream(std::ostream& stream);

private:
    template<typename T>
    friend const LoggerStream& operator<<(const LoggerStream& stream, const T& object);

private:
    std::forward_list<std::ostream*> streams;
    LoggerLevel maxLevel;
};

extern Logger logger;

extern const LoggerStream error;
extern const LoggerStream warning;
extern const LoggerStream info;
extern const LoggerStream debug;

template<typename T>
const LoggerStream& operator<<(const LoggerStream& stream, const T& object)
{
    for (const auto &ostream : logger.streams)
    {
        *ostream << object;
    }

    return stream;
}

const LoggerStream& operator<<(const LoggerStream& stream, std::ostream& (*ostream_manipulator)(std::ostream&));

}
}
