#pragma once

#include <memory>

#include <SDL_platform.h>

class Renderer
{
public:
  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  static SDL_Renderer *get()
  {
    if (!m_renderer) {
      init();
    }
    return m_renderer;
  }
  static void setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 opacity);
  static void renderClear();
  static void update();

private:
  Renderer();
  static void init();
  static SDL_Renderer *m_renderer;
  static SDL_Window *m_window;
};
