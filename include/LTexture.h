#pragma once

#include <string>
#include "SDL_platform.h"
#include "SDL_image_platform.h"
#include "SDL_ttf_platform.h"

class LTexture
{
public:
  LTexture();
  ~LTexture();
  bool loadFromFile(const std::string &path, SDL_Renderer *renderer);
  bool loadFromRenderedText(SDL_Renderer *renderer, const std::string &textureText, SDL_Color textColor, TTF_Font *font);
  void free();
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

  int getWidth();
  int getHeight();

private:
  SDL_Texture *mTexture;
  int mWidth;
  int mHeight;
};