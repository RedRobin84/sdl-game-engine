#pragma once

#include "System.h"

#include <memory>

#include <SDL_platform.h>

class Renderer : public System
{
public:
  Renderer();
  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  void setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 opacity);
  void renderClear();
  void update();

private:
  SDL_Renderer *m_renderer;
  SDL_Window *m_window;
};
