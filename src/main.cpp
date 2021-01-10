#include "ProgramSwitcher.h"
#include "Logger.h"

#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *args[])
{
    Logger::init(Logger::WRITE_TO_FILE | Logger::BUFFERED_LOGGING | Logger::DEBUG_MODE);
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
