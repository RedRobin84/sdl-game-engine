#pragma once

#include "ProgramType.h"
#include "SDL_Helpers.h"
#include "Registrable.h"
#include "enum/ProgramTypeEnum.h"
#include "enum/ProgramState.h"
#include "enum/State.h"

#include "SDL_image_platform.h"

#include <memory>

class Program : public Registrable
{
public:
  Program(const Program &program) = delete;
  Program &operator=(const Program &program) = delete;
  Program &operator=(Program &&) = delete;
  Program(Program &&) = delete;
  virtual ~Program();

  ProgramTypeEnum getNextProgram() { return this->m_nextProgram; }
  ProgramTypeEnum run();

protected:
  const ProgramTypeEnum m_programType;
  ProgramTypeEnum m_nextProgram = ProgramTypeEnum::NO_TYPE;

  explicit Program(ProgramTypeEnum anEnum, const std::shared_ptr<Registry> &registry);
  [[nodiscard]] virtual State loadMedia() = 0;
  virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  void addNewProgram(ProgramTypeEnum nextProgram);
  void endProgram();

  std::shared_ptr<SDL_Renderer> m_renderer;
  static std::unique_ptr<TTF_Font, TTF_Destroyers> d_font;

  static SDL_Color d_textColor;
  static SDL_Color d_highlightColor;

  static SDL_Event event;

private:
  static void init();
  static bool initialized;
  static ProgramState programState;
};
