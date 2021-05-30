#include "Menu.h"
#include "Logger.h"
#include "Strings.h"
#include "Renderer.h"
#include "ProgramType.h"

#include <stdexcept>
#include <algorithm>

using Programs::Menu::SELECT_PROGRAM;

Menu::Menu(const std::shared_ptr<Registry> &registry) : Program(ProgramTypeEnum::MENU, registry)

{
  if (m_programsToDisplay.empty()) {
    throw std::runtime_error("Menu::Menu: Empty program container.");
  }
  m_nextProgram = m_programsToDisplay.front();
  if (loadMedia() == State::FAILURE) {
    throw std::runtime_error("Menu::Menu: Some exceptions occured during program initialization.");
  }
}

void Menu::handleEvents()
{
  //Special key input
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_UP:
      m_index->texture.loadFromRenderedText(m_index->name, d_textColor, d_font.get(), m_renderer.get());
      if (m_index == m_menuItems.begin()) {
        m_index = m_menuItems.end();
      }
      m_index--;

      m_index->texture.loadFromRenderedText(m_index->name, d_highlightColor, d_font.get(), m_renderer.get());
      m_nextProgram = m_index->programType;
      break;

    case SDLK_DOWN:
      m_index->texture.loadFromRenderedText(m_index->name, d_textColor, d_font.get(), m_renderer.get());
      if (m_index == std::prev(m_menuItems.end())) {
        m_index = std::prev(m_menuItems.begin());
      }
      m_index++;

      m_index->texture.loadFromRenderedText(m_index->name, d_highlightColor, d_font.get(), m_renderer.get());
      m_nextProgram = m_index->programType;
      break;

    case SDLK_RETURN:
      Program::addNewProgram(m_nextProgram);
      break;
    case SDLK_ESCAPE:
      Program::endProgram();
      break;
    }
  }
}

State Menu::loadMedia()
{
  m_menuItems.resize(m_programsToDisplay.size());

  //Render the prompt
  if (!m_promptTextTexture.loadFromRenderedText(SELECT_PROGRAM, d_textColor, d_font.get(), m_renderer.get())) {
    Logger::error("Menu::loadMedia: Failed to render prompt text!");
    return State::FAILURE;
  }

  int index = 0;
  State state = State::SUCCESS;
  std::for_each(m_programsToDisplay.begin(), m_programsToDisplay.end(), [this, index, state](const auto &programType) mutable {
    m_menuItems[index].programType = programType;
    m_menuItems[index].name = ProgramTypeEnumUtils::getProgramName(programType);
    if (!m_menuItems[index].texture.loadFromRenderedText(m_menuItems[index].name, d_textColor, d_font.get(), m_renderer.get())) {
      Logger::error("Menu::getProgramNames: Unable to load texture %s from program names.", m_menuItems[index].name.data());
      state = State::FAILURE;
    }
    index++;
  });
  m_index = m_menuItems.begin();
  return state;
}

void Menu::update() {}

void Menu::render()
{
  m_renderer->setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
  //Render text textures
  int counter = 0;
  m_promptTextTexture.render((Renderer::SCREEN_WIDTH - m_promptTextTexture.getWidth()) / 2, 0);
  std::for_each(begin(m_menuItems), end(m_menuItems), [this, counter](const auto &menuItem) mutable {
    menuItem.texture.render((Renderer::SCREEN_WIDTH - menuItem.texture.getWidth()) / 2, m_promptTextTexture.getHeight() + menuItem.texture.getHeight() * counter);
    counter++;
  });
}
