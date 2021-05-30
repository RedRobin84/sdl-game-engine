#pragma once

#include "enum/ProgramTypeEnum.h"
#include "Menu.h"

#include <memory>

class Program;

class ProgramFactory
{
public:
  std::shared_ptr<Program> create(ProgramTypeEnum anEnum, const std::shared_ptr<Registry> &Registry);
};
