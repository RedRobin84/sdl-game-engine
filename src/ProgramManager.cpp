#include "ProgramManager.h"
#include "enum/ProgramTypeEnum.h"
#include "Menu.h"
#include "Logger.h"

#include "SDL_ttf_platform.h"

ProgramManager::ProgramManager(ProgramTypeEnum startingProgramType) : m_factory(std::make_unique<ProgramFactory>())
{
  m_programStack.emplace(m_factory->create(startingProgramType));
}

ProgramManager::~ProgramManager()
{
  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

void ProgramManager::manageCurrentProgram()
{
  ProgramState programState = m_programStack.top()->run();

  switch (programState) {
  case ProgramState::REMOVE:
    m_programStack.pop();
    Logger::debug("ProgramManager::manageCurrentProgram: Removed program. Current stack: %u", m_programStack.size());
    break;
  case ProgramState::ADD:
    m_programStack.emplace(m_factory->create(m_programStack.top().get()->getNextProgram()));
    Logger::debug("ProgramManager::manageCurrentProgram: Added program. Current stack: %u", m_programStack.size());
    break;
  default:
    m_programStack = ProgramStack();
    Logger::debug("ProgramManager::manageCurrentProgram: TERMINUS or unknown type of ProgramState. Exiting...");
    break;
  }
}
