#pragma once

#include "enum/ProgramTypeEnum.h"
#include "Menu.h"

#include <memory>

class Program;

class ProgramFactory
{
public:
  std::unique_ptr<Program> create(ProgramTypeEnum anEnum);
};
