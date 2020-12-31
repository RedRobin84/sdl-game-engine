#include "ProgramSwitcher.h"
#include "Logger.h"
#include "SDL_platform.h"

#include <iostream>

int main(int argc, char *args[])
{
    Logger::init(Logger::DEBUG_MODE::ON, Logger::BUFFERED_LOGGING::ON);
    try
    {
        ProgramSwitcher ps;
        while (!ps.getProgramStack().empty())
        {
            ps.getProgramStack().top().get()->run();
            ps.switchProgram();
        }
        
    }
    catch(const std::exception& e)
    {
        Logger::error(e.what());
        return EXIT_FAILURE;
    }
    
    Logger::finalCleanup();
    return EXIT_SUCCESS;
}
