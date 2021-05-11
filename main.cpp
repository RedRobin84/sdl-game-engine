#include "ProgramManager.h"
#include "Logger.h"

#include "SDL_platform.h"

#include <iostream>

int main(int argc, char *argv[])
{
    Logger::init(Logger::WRITE_TO_FILE | Logger::BUFFERED_LOGGING | Logger::DEBUG_MODE);
    try
    {
        ProgramManager pm(ProgramTypeEnum::MENU);
        while (!pm.getProgramStack().empty())
        {
            pm.manageCurrentProgram();
        }
        
    }
    catch(const std::exception& e)
    {
        Logger::error("EXCEPTION: %s", e.what());
        return EXIT_FAILURE;
    }
    
    Logger::finalCleanup();
    return EXIT_SUCCESS;
}
