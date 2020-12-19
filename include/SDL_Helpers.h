#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

struct SDL_Destroyers
{
    void operator()(SDL_Window* w) const { SDL_DestroyWindow(w); }
    void operator()(SDL_Renderer* r) const { SDL_DestroyRenderer(r); }
};

struct TTF_Destroyers
{
    void operator()(TTF_Font * f) const { TTF_CloseFont(f); }
};