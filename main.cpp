#include "Logger.h"
#include "Program.h"
#include "Registry.h"

#include "SDL_platform.h"

#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
  Logger::init(Logger::WRITE_TO_FILE | Logger::BUFFERED_LOGGING | Logger::DEBUG_MODE);
  auto registry{ std::make_shared<Registry>() };

  try {
    auto currentProgram{ registry->registerProgram(ProgramTypeEnum::MENU, registry) };
    while (currentProgram) {
      ProgramTypeEnum nextProgram = currentProgram->run();
      if (ProgramTypeEnum::NO_TYPE == nextProgram) {
        break;
      }
      currentProgram = registry->registerProgram(nextProgram, registry);
    }

  } catch (const std::exception &e) {
    Logger::error("EXCEPTION: %s", e.what());
    return EXIT_FAILURE;
  }
  IMG_Quit();
  SDL_Quit();
  Logger::finalCleanup();
  return EXIT_SUCCESS;
}
