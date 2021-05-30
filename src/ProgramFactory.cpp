#include "ProgramFactory.h"
#include "Menu.h"
#include "Audio.h"
#include "ParticleEngines.h"

#include <memory>
#include <stdexcept>

std::shared_ptr<Program> ProgramFactory::create(ProgramTypeEnum anEnum, const std::shared_ptr<Registry> &registry)
{
  switch (anEnum) {
  case ProgramTypeEnum::MENU:
    return std::make_shared<Menu>(registry);
  case ProgramTypeEnum::AUDIO:
    return std::make_shared<Audio>(registry);
  case ProgramTypeEnum::PARTICLE_ENGINES:
    return std::make_shared<ParticleEngines>(registry);

  default:
    throw std::invalid_argument("ProgramFactory::create: Unable to create new program instance from enum.");
  }
}
