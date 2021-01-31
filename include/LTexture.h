#pragma once

#include <string>
#include "SDL_platform.h"
#include "SDL_image_platform.h"
#include "SDL_stdinc.h"
#include "SDL_ttf_platform.h"

class LTexture
{
public:
  LTexture();
  LTexture(LTexture &) = delete;
  LTexture(LTexture &&) = default;
  LTexture &operator=(LTexture &) = delete;
  LTexture &operator=(LTexture &&) = delete;
  ~LTexture();

  constexpr static Uint8 FULL_INTENSITY = 0xFF;
  constexpr static Uint8 ZERO_INTENSITY = 0x00;

  bool loadFromFile(const std::string &path, SDL_Renderer *renderer);
  bool loadFromRenderedText(SDL_Renderer *renderer, const std::string &textureText, SDL_Color textColor, TTF_Font *font);
  void free();
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0, SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

private:
  SDL_Texture *mTexture;
  int mWidth;
  int mHeight;
};