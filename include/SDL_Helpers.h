#pragma once

#include "SDL_platform.h"
#include "SDL_ttf_platform.h"

struct TTF_Destroyers
{
  void operator()(TTF_Font *f) const { TTF_CloseFont(f); }
};
