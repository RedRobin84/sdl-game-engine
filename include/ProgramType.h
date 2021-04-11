#pragma once

#include "enum/ProgramTypeEnum.h"
#include "EnumUtils.h"

#include <string_view>

class ProgramType : public EnumUtils<ProgramTypeEnum>
{
public:
  static std::string_view getName(ProgramTypeEnum programTypeEnum);
};
