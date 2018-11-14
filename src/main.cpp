#ifdef VLD_FOUND
#include <vld.h>
#endif

#include "application/game_application.h"
#include <plog/Appenders/ColorConsoleAppender.h>
#include "plog/Appenders/DebugOutputAppender.h"
#include "plog/Log.h"

void init_logger()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
    static plog::DebugOutputAppender<plog::TxtFormatter> debug_output_appender;
    init(plog::verbose, &console_appender).addAppender(&debug_output_appender);
    LOG_INFO << "Logger is working.";
}

int main(const int argc, char* argv[])
{
    init_logger();
    tactics_game::game_application application{};
    return application.execute(argc, argv);
}
