#ifdef VLD_FOUND
#include <vld.h>
#endif

#include "application/game_application.h"

int main(const int argc, char* argv[])
{
    tactics_game::game_application application{};
    return application.execute(argc, argv);
}
