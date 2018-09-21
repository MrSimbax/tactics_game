#include "application/application.h"

int main(int argc, char *argv[])
{
    TacticsGame::Application application{};
    return application.execute(argc, argv);
}
