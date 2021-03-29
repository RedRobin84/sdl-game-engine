#include "ProgramType.h"
#include "enum/ProgramTypeEnum.h"
#include "Strings.h"
#include "Menu.h"
#include "Logger.h"
#include <stdexcept>

const ProgramNameMap ProgramType::m_programNames = {
  { ProgramTypeEnum::MENU, ProgramNames::MENU },
  { ProgramTypeEnum::AUDIO, ProgramNames::AUDIO }
};

ProgramType::ProgramType(ProgramTypeEnum anEnum) : m_programTypeEnum(anEnum)
{
}

ProgramTypeEnum ProgramType::next() const
{
  switch (m_programTypeEnum) {
  case ProgramTypeEnum::MENU:
    return ProgramTypeEnum::AUDIO;

  case ProgramTypeEnum::AUDIO:
    return ProgramTypeEnum::MENU;

  default:
    Logger::error("ProgramType::next: Enum switch error. Setting default MENU.");
    return ProgramTypeEnum::MENU;
  }
}

ProgramTypeEnum ProgramType::previous() const
{
  switch (m_programTypeEnum) {
  case ProgramTypeEnum::MENU:
    return ProgramTypeEnum::AUDIO;

  case ProgramTypeEnum::AUDIO:
    return ProgramTypeEnum::MENU;

  default:
    Logger::error("ProgramType::prevoius: Enum switch error. Setting default MENU");
    return ProgramTypeEnum::MENU;
  }
}

const std::string_view ProgramType::getName()
{
  auto programName = m_programNames.find(m_programTypeEnum);
  if (programName == m_programNames.end()) {
    throw std::invalid_argument("ERROR: ProgramType::getName: Map value not found by key");
  }

  return programName->second;
}