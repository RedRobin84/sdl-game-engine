#include "Program.h"
#include "Logger.h"

// Static variables init
std::unique_ptr<SDL_Renderer, SDL_Destroyers> Program::m_renderer = std::unique_ptr<SDL_Renderer, SDL_Destroyers>();
std::unique_ptr<SDL_Window, SDL_Destroyers> Program::m_window = std::unique_ptr<SDL_Window, SDL_Destroyers>();
SDL_Event Program::event;

Program::Program(ProgramTypeEnum anEnum) : m_programType(anEnum), initialized(false), quit(false) {}

void Program::exit(ProgramTypeEnum next = ProgramTypeEnum::NO_TYPE)
{
  quit = true;
  this->m_programType.set(next);
}

void Program::renderInit()
{
  //Clear screen
  SDL_SetRenderDrawColor(Program::m_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(m_renderer.get());
}

void Program::renderPresent()
{
  //Update screen
  SDL_RenderPresent(m_renderer.get());
}

void Program::run()
{
  //While application is running
  while (!quit) {

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      //User requests quit
      if (event.type == SDL_QUIT) {
        quit = true;
      } else {
        handleEvents();
      }
    }
    renderInit();
    renderMain();
    renderPresent();
  }
}