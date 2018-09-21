//#include "services.h"
//
//using namespace TacticsGame;
//
//Service::Logging::Logger* Services::logger = nullptr;
//Service::Logging::NullLogger Services::nullLogger{};
//
//void Services::initialize()
//{
//    Services::logger = &Services::nullLogger;
//}
//
//Service::Logging::Logger& Services::log()
//{
//    if (!Services::logger) Services::logger = &Services::nullLogger;
//    return *Services::logger;
//}
//
//void Services::provide(Service::Logging::Logger* logger)
//{
//    Services::logger = logger;
//}
