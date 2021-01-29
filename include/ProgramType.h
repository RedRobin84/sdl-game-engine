#pragma once

#include "ProgramTypeEnum.h"

#include <string>
#include <map>
#include <memory>

class Program;

using ProgramNameMap = std::map<ProgramTypeEnum, const std::string &>;

class ProgramType
{

public:
  explicit ProgramType(ProgramTypeEnum anEnum);
  ProgramTypeEnum next();
  ProgramTypeEnum previous();
  const std::string &getName();
  inline static const ProgramNameMap &getNameMap() { return m_programNames; }
  inline constexpr ProgramTypeEnum get() { return this->m_programTypeEnum; }
  inline constexpr void set(ProgramTypeEnum anEnum) { this->m_programTypeEnum = anEnum; }

private:
  ProgramTypeEnum m_programTypeEnum;
  static const ProgramNameMap m_programNames;

  std::unique_ptr<Program> createInstance(ProgramTypeEnum anEnum);
};