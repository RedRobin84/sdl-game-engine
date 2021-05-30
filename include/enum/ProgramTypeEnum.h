#pragma once

enum class ProgramTypeEnum : unsigned int {
  NO_TYPE,
  MENU,
  PARTICLE_ENGINES,
  AUDIO,
  FIRST = NO_TYPE,
  LAST = AUDIO,
};

namespace ProgramTypeEnumUtils {

static const char *getProgramName(ProgramTypeEnum programTypeEnum)
{
  switch (programTypeEnum) {
  case ProgramTypeEnum::PARTICLE_ENGINES:
    return "Particle engines";
  case ProgramTypeEnum::AUDIO:
    return "Audio";
  default:
    return "ERROR_UNKNOWN";
  }
}

}// namespace ProgramTypeEnumUtils
