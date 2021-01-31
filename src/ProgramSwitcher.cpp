#include "ProgramSwitcher.h"
#include "Menu.h"
#include "Logger.h"

#include "SDL_ttf_platform.h"

ProgramSwitcher::ProgramSwitcher() : m_factory(std::make_unique<ProgramFactory>())
{
  m_programStack.emplace(m_factory->create(ProgramTypeEnum::MENU));
}

ProgramSwitcher::~ProgramSwitcher()
{
  //Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

inline bool ProgramSwitcher::programIsRunning()
{
  return this->m_programStack.top().get()->isRunning();
}

void ProgramSwitcher::switchProgram()
{
  if (m_programStack.top()->getNextProgram() == ProgramTypeEnum::NO_TYPE) {
    Logger::debug("ProgramSwitcher::switchProgram: NO_TYPE program. Popping up.");
    m_programStack.pop();
    return;
  }

  m_programStack.emplace(m_factory->create(m_programStack.top().get()->getNextProgram()));
  Logger::debug("ProgramSwitcher::switchProgram: Adding program. Current stack: %uz", m_programStack.size());
}
