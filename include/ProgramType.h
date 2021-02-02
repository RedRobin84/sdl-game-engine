#pragma once

#include "ProgramTypeEnum.h"

#include <string_view>
#include <map>
#include <memory>

class Program;

using ProgramNameMap = std::map<ProgramTypeEnum, std::string_view>;

class ProgramType
{

public:
  explicit ProgramType(ProgramTypeEnum anEnum);
  ProgramTypeEnum next() const;
  ProgramTypeEnum previous() const;
  const std::string_view getName();
  inline static const ProgramNameMap &getNameMap() { return m_programNames; }
  inline ProgramTypeEnum get() const { return this->m_programTypeEnum; }
  inline void set(ProgramTypeEnum anEnum) { this->m_programTypeEnum = anEnum; }

private:
  ProgramTypeEnum m_programTypeEnum;
  static const ProgramNameMap m_programNames;
};