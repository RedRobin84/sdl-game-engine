#include "Menu.h"
#include "Logger.h"
#include "Strings.h"

#include <stdexcept>
#include <algorithm>

Menu::Menu() : Program(ProgramTypeEnum::MENU),
               TOTAL_DATA(ProgramType::getNameMap().size()),
               m_currentData(0),
               m_currentType(ProgramTypeEnum::MENU)

{
  init();
  loadMedia();
}

void Menu::init()
{
  if (initialized) {
    Logger::warn("Menu::init: Main initialization already done. Stopping main initialization.");
    return;
  }

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    Logger::error("Menu::init: SDL could not initialize! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Set texture filtering to linear
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0U) {
    Logger::warn("Menu::init: Linear texture filtering not enabled!");
  }

  //Create window
  m_window.reset(SDL_CreateWindow(Programs::Menu::TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
  if (m_window == nullptr) {
    Logger::error("Menu::init: Window could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Create vsynced renderer for window
  m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (m_renderer == nullptr) {
    Logger::error("Menu::init: Renderer could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
  //Initialize renderer color
  SDL_SetRenderDrawColor(m_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if ((IMG_Init(imgFlags) & imgFlags) == 0) {
    Logger::error("Menu::init: SDL_image could not initialize! SDL_image Error:");
    throw std::runtime_error(IMG_GetError());
  }

  //Initialize SDL_ttf
  if (TTF_Init() == -1) {
    Logger::error("Menu::init: SDL_ttf could not initialize! SDL_ttf Error:");
    throw std::runtime_error(TTF_GetError());
  }


  initialized = true;
}

void Menu::handleEvents()
{
  //Special key input
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_UP:
      m_programNames[m_currentData].loadFromRenderedText(m_renderer.get(), m_currentType.getName(), m_textColor, m_font.get());
      m_currentType.set(m_currentType.previous());
      --m_currentData;
      if (m_currentData < 0) {
        m_currentData = TOTAL_DATA - 1;
      }
      m_programNames[m_currentData].loadFromRenderedText(m_renderer.get(), m_currentType.getName(), m_highlightColor, m_font.get());
      break;

    case SDLK_DOWN:
      m_programNames[m_currentData].loadFromRenderedText(m_renderer.get(), m_currentType.getName(), m_textColor, m_font.get());
      m_currentType.set(m_currentType.next());
      ++m_currentData;
      if (m_currentData >= TOTAL_DATA) {
        m_currentData = 0;
      }
      m_programNames[m_currentData].loadFromRenderedText(m_renderer.get(), m_currentType.getName(), m_highlightColor, m_font.get());
      break;

    case SDLK_RETURN:
      if (m_currentType.get() == m_programType.get()) {
        break;
      }
      Program::exit(m_currentType.get());
      break;
    case SDLK_ESCAPE:
      Program::exit(ProgramTypeEnum::NO_TYPE);
      break;
    }
  }
}

void Menu::loadMedia()
{

  m_programNames.resize(ProgramType::getNameMap().size());
  m_textColor = { 0, 0, 0, 0xFF };
  m_highlightColor = { 0xFF, 0, 0, 0xFF };

  m_font.reset(TTF_OpenFont(LAZY_FONT_PATH.data(), MEDIUM_FONT_SIZE));
  if (m_font == nullptr) {
    Logger::error("Menu::loadMedia: Failed to load lazy font! SDL_ttf Error: %s");
    throw std::runtime_error(TTF_GetError());
  }
  //Render the prompt
  if (!m_promptTextTexture.loadFromRenderedText(m_renderer.get(), Programs::Menu::SELECT_PROGRAM, m_textColor, m_font.get())) {
    throw std::runtime_error("Menu::loadMedia: Failed to render prompt text!");
  }

  if (m_programNames.size() != ProgramType::getNameMap().size()) {
    throw std::runtime_error("Menu::loadMedia: Size of both containers does not match.");
  }

  int index = 0;
  std::for_each(ProgramType::getNameMap().begin(), ProgramType::getNameMap().end(), [this, index](const std::pair<ProgramTypeEnum, const std::string &> &mapRecord) mutable {
    if (!m_programNames[index].loadFromRenderedText(m_renderer.get(), mapRecord.second, m_textColor, m_font.get())) {
      throw std::runtime_error("Menu::getProgramNames: Unable to load texture from program names.");
    }
    index++;
  });
}

void Menu::renderMain()
{
  //Render text textures
  int counter = 0;
  m_promptTextTexture.render(m_renderer.get(), (SCREEN_WIDTH - m_promptTextTexture.getWidth()) / 2, 0);
  std::for_each(begin(m_programNames), end(m_programNames), [this, counter](LTexture &texture) mutable {
    texture.render(m_renderer.get(), (SCREEN_WIDTH - texture.getWidth()) / 2, m_promptTextTexture.getHeight() + texture.getHeight() * counter);
    counter++;
  });
}
