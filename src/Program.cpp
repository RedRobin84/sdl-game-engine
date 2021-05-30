#include "Program.h"
#include "Logger.h"
#include "SDL_Helpers.h"
#include "Renderer.h"
#include <stdexcept>

// Static variables init
std::unique_ptr<TTF_Font, TTF_Destroyers> Program::d_font = std::unique_ptr<TTF_Font, TTF_Destroyers>();
SDL_Color Program::d_textColor;
SDL_Color Program::d_highlightColor;
SDL_Event Program::event;
bool Program::initialized = false;
ProgramState Program::programState = ProgramState::TERMINUS;

//Constants
namespace {
constexpr char LAZY_FONT_PATH[] = "../assets/fonts/lazy.ttf";
constexpr int MEDIUM_FONT_SIZE = 28;
}// namespace

Program::Program(ProgramTypeEnum anEnum, const std::shared_ptr<Registry> &registry) :  Registrable(registry), m_programType(anEnum)
{
  if (!initialized) {
    Logger::info("Program::ctr: Performing one-time initialization...");
    init();
    initialized = true;
  }
  m_renderer = getRegistry()->registerRenderer();
}

Program::~Program()
{
  getRegistry()->removeExpired();
}

void Program::init()
{
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    Logger::error("Program::init: SDL could not initialize! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Set texture filtering to linear
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0U) {
    Logger::warn("Program::init: Linear texture filtering not enabled!");
  }

  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if ((IMG_Init(imgFlags) & imgFlags) == 0) {
    Logger::error("Program::init: SDL_image could not initialize! SDL_image Error:");
    throw std::runtime_error(IMG_GetError());
  }

  //Initialize SDL_ttf
  if (TTF_Init() == -1) {
    Logger::error("Program::init: SDL_ttf could not initialize! SDL_ttf Error:");
    throw std::runtime_error(TTF_GetError());
  }

  d_font.reset(TTF_OpenFont(LAZY_FONT_PATH, MEDIUM_FONT_SIZE));
  if (d_font == nullptr) {
    Logger::error("Program::init: Failed to load lazy font! SDL_ttf Error: %s");
    throw std::runtime_error(TTF_GetError());
  }

  d_textColor = { 0, 0, 0, 0xFF };
  d_highlightColor = { 0xFF, 0, 0, 0xFF };
}

void Program::addNewProgram(ProgramTypeEnum nextType)
{
  programState = ProgramState::ADD;
  m_nextProgram = nextType;
}

void Program::endProgram()
{
  programState = ProgramState::REMOVE;
}

ProgramTypeEnum Program::run()
{
  programState = ProgramState::RUNNING;
  //While application is running
  while (programState == ProgramState::RUNNING) {

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      //User requests quit
      if (event.type == SDL_QUIT) {
        return ProgramTypeEnum::NO_TYPE;
      } else {
        handleEvents();
      }
    }
    update();
    m_renderer->renderClear();
    render();
    m_renderer->update();
  }
  return m_nextProgram;
}
