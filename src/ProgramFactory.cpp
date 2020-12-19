#include "ProgramFactory.h"
#include "Menu.h"

std::unique_ptr<Program> ProgramFactory::create(ProgramTypeEnum anEnum)
{
    switch (anEnum)
    {
    case ProgramTypeEnum::MENU:
        return std::make_unique<Menu>();
    
    default:
        throw std::invalid_argument("ProgramFactory::create: Unable to create new program instance from enum.");
    }
}