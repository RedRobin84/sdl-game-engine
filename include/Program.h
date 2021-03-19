#pragma once

#include "ProgramType.h"
#include "SDL_Helpers.h"
#include "enum/ProgramTypeEnum.h"
#include "enum/State.h"

#include "SDL_image_platform.h"

class Program
{
public:
  Program(const Program &program) = delete;
  Program &operator=(const Program &program) = delete;
  Program &operator=(Program &&) = delete;
  Program(Program &&) = delete;
  virtual ~Program() = default;

  ProgramTypeEnum getProgramType() { return this->m_programType.get(); }
  ProgramTypeEnum getNextProgram() { return this->m_nextProgram; }
  ProgramTypeEnum run();

protected:
  const ProgramType m_programType;
  static ProgramTypeEnum m_nextProgram;

  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  explicit Program(ProgramTypeEnum anEnum);
  [[nodiscard]] virtual State loadMedia() = 0;
  virtual void renderInit();
  virtual void renderPresent();
  virtual void renderMain() = 0;
  virtual void handleEvents() = 0;
  void stop(ProgramTypeEnum nextProgram);

  static std::unique_ptr<SDL_Renderer, SDL_Destroyers> d_renderer;
  static std::unique_ptr<SDL_Window, SDL_Destroyers> d_window;
  static std::unique_ptr<TTF_Font, TTF_Destroyers> d_font;

  static SDL_Color d_textColor;
  static SDL_Color d_highlightColor;

  static SDL_Event event;
  static bool isRunning;

private:
  static void init();
  static bool initialized;
};
