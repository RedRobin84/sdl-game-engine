#pragma once

#include "ProgramType.h"
#include "SDL_Helpers.h"
#include "Commons.h"

#include "SDL_image_platform.h"

class Program
{
public:
  Program(const Program &program) = delete;
  Program &operator=(const Program &program) = delete;
  Program &operator=(Program &&) = delete;
  Program(Program &&) = delete;
  virtual ~Program() = default;

  [[nodiscard]] inline bool isRunning() const { return !quit; }
  ProgramTypeEnum getProgramType() { return this->m_programType.get(); }
  ProgramTypeEnum getNextProgram() { return this->m_nextProgram; }
  void run();

protected:
  const ProgramType m_programType;
  ProgramTypeEnum m_nextProgram = ProgramTypeEnum::NO_TYPE;

  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  explicit Program(ProgramTypeEnum anEnum);
  [[nodiscard]] virtual State loadMedia() = 0;
  virtual void renderInit();
  virtual void renderPresent();
  virtual void renderMain() = 0;
  virtual void handleEvents() = 0;
  virtual void exit(ProgramTypeEnum next);

  static std::unique_ptr<SDL_Renderer, SDL_Destroyers> d_renderer;
  static std::unique_ptr<SDL_Window, SDL_Destroyers> d_window;
  static std::unique_ptr<TTF_Font, TTF_Destroyers> d_font;

  static SDL_Color d_textColor;
  static SDL_Color d_highlightColor;

  static SDL_Event event;
  bool quit;

private:
  static void init();
  static bool initialized;
};
