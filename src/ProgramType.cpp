#include "ProgramType.h"
#include "Strings.h"
#include "Menu.h"
#include "Logger.h"

const ProgramNameMap ProgramType::m_programNames =
    {
        {ProgramTypeEnum::MENU, ProgramNames::MENU},
        {ProgramTypeEnum::AUDIO, ProgramNames::AUDIO}
    };

ProgramType::ProgramType(ProgramTypeEnum anEnum) : m_programTypeEnum(anEnum)
{
}

ProgramTypeEnum ProgramType::next()
{
    switch (m_programTypeEnum)
    {
    case ProgramTypeEnum::MENU:
        return ProgramTypeEnum::AUDIO;

    case ProgramTypeEnum::AUDIO:
        return ProgramTypeEnum::MENU;

    default:
        Logger::error("ProgramType::next: Enum switch error. Setting default MENU.");
        return ProgramTypeEnum::MENU;
    }
}

ProgramTypeEnum ProgramType::previous()
{
    switch (m_programTypeEnum)
    {
    case ProgramTypeEnum::MENU:
        return ProgramTypeEnum::AUDIO;

    case ProgramTypeEnum::AUDIO:
        return ProgramTypeEnum::MENU;

    default:
        Logger::error("ProgramType::prevoius: Enum switch error. Setting default MENU");
        return ProgramTypeEnum::MENU;
    }
}

const std::string &ProgramType::getName()
{
    auto programName = m_programNames.find(m_programTypeEnum);
    if (programName == m_programNames.end())
    {
        throw std::invalid_argument("ERROR: ProgramType::getName: Map value not found by key");
    }

    return programName->second;
}

std::unique_ptr<Program> ProgramType::createInstance(ProgramTypeEnum anEnum)
{
    switch (anEnum)
    {
    case ProgramTypeEnum::MENU:
        return std::make_unique<Menu>();

    default:
        throw std::invalid_argument("ERROR: ProgramType::createInstance: Unable to create program instance by type.");
    }
}