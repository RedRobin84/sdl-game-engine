#include "ProgramType.h"
#include <stdexcept>

namespace {
constexpr std::string_view PARTICLE_ENGINES = "Particle engines";
constexpr std::string_view AUDIO = "Audio";
}// namespace

std::string_view ProgramType::getName(ProgramTypeEnum programTypeEnum)
{
  switch (programTypeEnum) {
  case ProgramTypeEnum::PARTICLE_ENGINES:
    return PARTICLE_ENGINES;
  case ProgramTypeEnum::AUDIO:
    return AUDIO;
  default:
    throw std::invalid_argument("ProgramType::getName: Unknown program type.");
  }
}
