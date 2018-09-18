#include <iostream>
#include <memory>
#include <sstream>

#include "application/application.h"
#include "logger/logger.h"

int main(int argc, char *argv[])
{
    TacticsGame::Application application{};

    TacticsGame::Logger::logger.addStream(std::cout);

    TacticsGame::Logger::error << "Test" << std::endl;

    return application.execute(argc, argv);
}
