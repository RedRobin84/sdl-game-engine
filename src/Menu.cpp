#include "Menu.h"
#include "Logger.h"
#include "Strings.h"

#include <stdexcept>
#include <algorithm>

using Programs::Menu::SELECT_PROGRAM;

Menu::Menu() : Program(ProgramTypeEnum::MENU),
               TOTAL_DATA(ProgramType::getNameMap().size()),
               m_currentData(0),
               m_currentType(ProgramTypeEnum::MENU)

{
  if (loadMedia() == State::FAILURE) {
    throw std::runtime_error("Audio::init: Some exceptions occured during program initialization.");
  }
}

void Menu::handleEvents()
{
  //Special key input
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_UP:
      m_programNames[m_currentData].loadFromRenderedText(d_renderer.get(), m_currentType.getName(), d_textColor, d_font.get());
      m_currentType.set(m_currentType.previous());
      --m_currentData;
      if (m_currentData < 0) {
        m_currentData = TOTAL_DATA - 1;
      }
      m_programNames[m_currentData].loadFromRenderedText(d_renderer.get(), m_currentType.getName(), d_highlightColor, d_font.get());
      break;

    case SDLK_DOWN:
      m_programNames[m_currentData].loadFromRenderedText(d_renderer.get(), m_currentType.getName(), d_textColor, d_font.get());
      m_currentType.set(m_currentType.next());
      ++m_currentData;
      if (m_currentData >= TOTAL_DATA) {
        m_currentData = 0;
      }
      m_programNames[m_currentData].loadFromRenderedText(d_renderer.get(), m_currentType.getName(), d_highlightColor, d_font.get());
      break;

    case SDLK_RETURN:
      if (m_currentType.get() == m_programType.get()) {
        break;
      }
      Program::stop(m_currentType.get());
      break;
    case SDLK_ESCAPE:
      Program::stop(ProgramTypeEnum::NO_TYPE);
      break;
    }
  }
}

State Menu::loadMedia()
{
  m_programNames.resize(ProgramType::getNameMap().size());

  //Render the prompt
  if (!m_promptTextTexture.loadFromRenderedText(d_renderer.get(), SELECT_PROGRAM, d_textColor, d_font.get())) {
    Logger::error("Menu::loadMedia: Failed to render prompt text!");
    return State::FAILURE;
  }

  if (m_programNames.size() != ProgramType::getNameMap().size()) {
    Logger::error("Menu::loadMedia: Size of both containers does not match.");
    return State::FAILURE;
  }

  int index = 0;
    State state = State::SUCCESS;
  std::for_each(ProgramType::getNameMap().begin(), ProgramType::getNameMap().end(), [this, index, state](const auto &mapRecord) mutable {
    if (!m_programNames[index].loadFromRenderedText(d_renderer.get(), mapRecord.second, d_textColor, d_font.get())) {
      Logger::error("Menu::getProgramNames: Unable to load texture %s from program names.", mapRecord.second);
      state = State::FAILURE;
    }
    index++;
  });
  return state;
}

void Menu::renderMain()
{
  //Render text textures
  int counter = 0;
  m_promptTextTexture.render(d_renderer.get(), (SCREEN_WIDTH - m_promptTextTexture.getWidth()) / 2, 0);
  std::for_each(begin(m_programNames), end(m_programNames), [this, counter](LTexture &texture) mutable {
    texture.render(d_renderer.get(), (SCREEN_WIDTH - texture.getWidth()) / 2, m_promptTextTexture.getHeight() + texture.getHeight() * counter);
    counter++;
  });
}
