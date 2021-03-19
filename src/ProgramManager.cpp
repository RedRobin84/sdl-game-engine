#include "ProgramManager.h"
#include "enum/ProgramTypeEnum.h"
#include "Menu.h"
#include "Logger.h"

#include "SDL_ttf_platform.h"

ProgramManager::ProgramManager() : m_factory(std::make_unique<ProgramFactory>())
{
  m_programStack.emplace(m_factory->create(ProgramTypeEnum::MENU));
}

ProgramManager::~ProgramManager()
{
  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

void ProgramManager::manageCurrentProgram()
{
  ProgramTypeEnum programType = m_programStack.top()->run();
  if (ProgramTypeEnum::TERMINUS == programType) {
  m_programStack = ProgramStack();
  return;
  }
  if (m_programStack.top()->getNextProgram() == ProgramTypeEnum::NO_TYPE) {
    Logger::debug("ProgramManager::switchProgram: NO_TYPE program. Popping up.");
    m_programStack.pop();
    return;
  }
  m_programStack.emplace(m_factory->create(m_programStack.top().get()->getNextProgram()));
  Logger::debug("ProgramManager::switchProgram: Adding program. Current stack: %u", m_programStack.size());
}
