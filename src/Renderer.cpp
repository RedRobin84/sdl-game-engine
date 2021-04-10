#include "Renderer.h"

#include "Logger.h"

#include <string_view>

namespace {
constexpr Uint8 BRIGHTEST = 0xFF;
constexpr std::string_view PROGRAM_NAME = "MY_FIRST_ENGINE";
}// namespace

SDL_Window *Renderer::m_window;
SDL_Renderer *Renderer::m_renderer;

void Renderer::init()
{
  //Create window
  m_window = SDL_CreateWindow(PROGRAM_NAME.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (m_window == nullptr) {
    Logger::error("Window::ctr: Window could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (m_renderer == nullptr) {
    Logger::error("Renderer::ctr: Renderer could not be created! SDL Error:");
    throw std::runtime_error(SDL_GetError());
  }
}

void Renderer::setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 opacity)
{
  SDL_SetRenderDrawColor(m_renderer, red, green, blue, opacity);
}

void Renderer::renderClear()
{
  SDL_RenderClear(m_renderer);
}

void Renderer::update()
{
  SDL_RenderPresent(m_renderer);
}
