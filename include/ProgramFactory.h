#pragma once

#include "ProgramTypeEnum.h"
#include "Menu.h"

#include <memory>

class Program;

class ProgramFactory
{
    public:
    static std::unique_ptr<Program> create(ProgramTypeEnum anEnum);
};