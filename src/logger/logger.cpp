#include "logger.h"

using namespace TacticsGame::Logger;

namespace TacticsGame
{
namespace Logger
{

Logger logger{};

const LoggerStream error{ "[ERROR] ", LoggerLevel::ERROR };
const LoggerStream warning{ "[WARN] ", LoggerLevel::WARNING };
const LoggerStream info{ "[INFO] ", LoggerLevel::INFO };
const LoggerStream debug{ "[DEBUG] ", LoggerLevel::DEBUG };

const LoggerStream& operator<<(const LoggerStream& stream, std::ostream& (*ostream_manipulator)(std::ostream&))
{
    return operator<< <std::ostream& (*)(std::ostream&)>(stream, ostream_manipulator);
}

} // Logger
} // TacticsGame

Logger::Logger() : streams{}, maxLevel{ LoggerLevel::ERROR }
{
}

void Logger::setLoggerLevel(LoggerLevel level)
{
    this->maxLevel = level;
}

LoggerLevel Logger::getCurrentLoggerLevel() const
{
    return this->maxLevel;
}

void Logger::addStream(std::ostream& stream)
{
    this->streams.push_front(&stream);
}
