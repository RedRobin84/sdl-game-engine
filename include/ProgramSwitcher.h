#pragma once

#include "ProgramType.h"
#include "ProgramFactory.h"

#include "SDL_platform.h"

#include <memory>
#include <stack>

class Program;

class ProgramSwitcher
{
public:
  using ProgramStack = std::stack<std::unique_ptr<Program>>;

  ProgramSwitcher();
  ~ProgramSwitcher();
  ProgramSwitcher(const ProgramSwitcher &) = delete;
  ProgramSwitcher(ProgramSwitcher &&) = delete;
  ProgramSwitcher &operator=(ProgramSwitcher) = delete;
  ProgramSwitcher &operator=(ProgramSwitcher &&) = delete;

  bool programIsRunning();
  inline const ProgramStack &getProgramStack() const { return m_programStack; }
  void switchProgram();

private:
  ProgramStack m_programStack;
  std::unique_ptr<ProgramFactory> m_factory;
};