#pragma once

#include "ProgramType.h"
#include "ProgramFactory.h"

#include "SDL_platform.h"

#include <memory>
#include <stack>

class Program;

class ProgramManager
{
public:
  using ProgramStack = std::stack<std::unique_ptr<Program>>;

  ProgramManager() = delete;
  ProgramManager(ProgramTypeEnum startingProgramType);
  ~ProgramManager();
  ProgramManager(const ProgramManager &) = delete;
  ProgramManager(ProgramManager &&) = delete;
  ProgramManager &operator=(ProgramManager) = delete;
  ProgramManager &operator=(ProgramManager &&) = delete;

  inline const ProgramStack &getProgramStack() const { return m_programStack; }
  void manageCurrentProgram();

private:
  ProgramStack m_programStack;
  std::unique_ptr<ProgramFactory> m_factory;
};
