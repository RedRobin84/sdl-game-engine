#pragma once

#include "SDL_platform.h"
#include "SDL_image_platform.h"
#include "SDL_ttf_platform.h"
#include "Asset.h"

#include <string_view>
#include <string>
#include <memory>

class LTexture : public Asset
{
public:
  LTexture() = default;
  LTexture(std::string_view path) : Asset(path) {}
  LTexture(LTexture &) = delete;
  LTexture(LTexture &&) = default;
  LTexture &operator=(LTexture &) = delete;
  LTexture &operator=(LTexture &&) = delete;
  ~LTexture() = default;

  static constexpr Uint8 FULL_INTENSITY = 0xFF;
  static constexpr Uint8 ZERO_INTENSITY = 0x00;

  [[nodiscard]] bool loadFromFile(std::string_view path);
  [[nodiscard]] bool loadFromRenderedText(std::string_view textureText, SDL_Color textColor, TTF_Font *font);
  void reset();
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  inline void setName(const std::string &name) { this->m_name = name; }
  [[nodiscard]] inline std::string_view getName() const { return this->m_name; }
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void render(int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0, SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

private:
  std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> m_Texture{nullptr, SDL_DestroyTexture};
  std::string m_name{nullptr};
  int m_Width = 0;
  int m_Height = 0;
};
