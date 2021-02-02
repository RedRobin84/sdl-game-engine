#pragma once

#include "ProgramType.h"
#include "SDL_Helpers.h"

#include "SDL_image_platform.h"

class Program
{
public:
  Program(const Program &program) = delete;
  Program &operator=(const Program &program) = delete;
  Program &operator=(Program&&) = delete;
  Program(Program&&) = delete;
  virtual ~Program() = default;

  [[nodiscard]] inline bool isRunning() const { return !quit; }
  inline constexpr ProgramTypeEnum getProgramType() { return this->m_programType.get(); }
  inline constexpr ProgramTypeEnum getNextProgram() { return this->m_nextProgram; }
  virtual void init() = 0;
  void run();

protected:
  const ProgramType m_programType;
  ProgramTypeEnum m_nextProgram = ProgramTypeEnum::NO_TYPE;

  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  explicit Program(ProgramTypeEnum anEnum);
  virtual void loadMedia() = 0;
  virtual void renderInit();
  virtual void renderPresent();
  virtual void renderMain() = 0;
  virtual void handleEvents() = 0;
  virtual void exit(ProgramTypeEnum next);

  static std::unique_ptr<SDL_Renderer, SDL_Destroyers> m_renderer;
  static std::unique_ptr<SDL_Window, SDL_Destroyers> m_window;
  static SDL_Event event;
  bool initialized;
  bool quit;
};
