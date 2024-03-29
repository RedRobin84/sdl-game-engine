#include "ProgramFactory.h"
#include "Menu.h"
#include "Audio.h"
#include "ParticleEngines.h"

#include <memory>
#include <stdexcept>

std::unique_ptr<Program> ProgramFactory::create(ProgramTypeEnum anEnum)
{
  switch (anEnum) {
  case ProgramTypeEnum::MENU:
    return std::make_unique<Menu>();
  case ProgramTypeEnum::AUDIO:
    return std::make_unique<Audio>();
  case ProgramTypeEnum::PARTICLE_ENGINES:
    return std::make_unique<ParticleEngines>();

  default:
    throw std::invalid_argument("ProgramFactory::create: Unable to create new program instance from enum.");
  }
}
