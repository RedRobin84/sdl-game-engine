#include "Program.h"
#include "Logger.h"
#include "SDL_Helpers.h"
#include "enum/ProgramTypeEnum.h"
#include "Registry.h"
#include <stdexcept>

// Static variables init
std::unique_ptr<SDL_Renderer, SDL_Destroyers> Program::d_renderer = std::unique_ptr<SDL_Renderer, SDL_Destroyers>();
std::unique_ptr<SDL_Window, SDL_Destroyers> Program::d_window = std::unique_ptr<SDL_Window, SDL_Destroyers>();
std::unique_ptr<TTF_Font, TTF_Destroyers> Program::d_font = std::unique_ptr<TTF_Font, TTF_Destroyers>();
SDL_Color Program::d_textColor;
SDL_Color Program::d_highlightColor;
SDL_Event Program::event;
bool Program::initialized = false;
bool Program::isRunning = false;
ProgramTypeEnum Program::m_nextProgram = ProgramTypeEnum::NO_TYPE;
Registry Program::registry;

//Constants
namespace {
constexpr char PROGRAM_NAME[] = "LazyFoo CMake Tools";
constexpr char LAZY_FONT_PATH[] = "../assets/fonts/lazy.ttf";

constexpr int MEDIUM_FONT_SIZE = 28;
}// namespace

Program::Program(ProgramTypeEnum anEnum) : m_programType(anEnum)
{
  if (!initialized) {
    Logger::info("Program::ctr: Performing one-time initialization...");
    init();
    initialized = true;
  }
}

Program::~Program() 
{
  registry.removeOrphans();
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

  //Create window
  d_window.reset(SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
  if (d_window == nullptr) {
    Logger::error("Program::init: Window could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Create vsynced renderer for window
  d_renderer.reset(SDL_CreateRenderer(d_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (d_renderer == nullptr) {
    Logger::error("Program::init: Renderer could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Initialize renderer color
  SDL_SetRenderDrawColor(d_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

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

void Program::stop(ProgramTypeEnum nextType = ProgramTypeEnum::NO_TYPE)
{
  isRunning = false;
  m_nextProgram = nextType;
}

void Program::renderInit()
{
  //Clear screen
  SDL_SetRenderDrawColor(Program::d_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(d_renderer.get());
}

void Program::renderPresent()
{
  //Update screen
  SDL_RenderPresent(d_renderer.get());
}

ProgramTypeEnum Program::run()
{
  isRunning = true;
  //While application is running
  while (isRunning) {

    //Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      //User requests quit
      if (event.type == SDL_QUIT) {
        return ProgramTypeEnum::TERMINUS;
      } else {
        handleEvents();
      }
    }
    renderInit();
    renderMain();
    renderPresent();
  }
  return m_nextProgram;
}