#include <stdio.h>
#include "application/application.h"

int main(int argc, char *argv[])
{
    //scanf("", NULL);

    TacticsGame::Application application{};

    return application.execute(argc, argv);
}
