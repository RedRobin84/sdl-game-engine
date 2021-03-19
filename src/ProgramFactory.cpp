#include "ProgramFactory.h"
#include "Menu.h"
#include "Audio.h"

#include <memory>
#include <stdexcept>

std::unique_ptr<Program> ProgramFactory::create(ProgramTypeEnum anEnum)
{
  switch (anEnum) {
  case ProgramTypeEnum::MENU:
    return std::make_unique<Menu>();
  case ProgramTypeEnum::AUDIO:
    return std::make_unique<Audio>();

  default:
    throw std::invalid_argument("ProgramFactory::create: Unable to create new program instance from enum.");
  }
}