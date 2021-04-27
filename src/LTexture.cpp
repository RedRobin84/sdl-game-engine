#include "Logger.h"

#include "LTexture.h"
#include "enum/State.h"
#include "Renderer.h"
#include <memory>

bool LTexture::loadFromFile(const std::string_view path)
{
  if (!m_name.empty()) {
  Logger::warn("LTexture::loadFromFile: Overwriting %s with %s", m_name.data(), path.data());
  }
  reset();
  auto loadedSurface = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)>(IMG_Load(path.data()), SDL_FreeSurface);
  if (loadedSurface == nullptr) {
    Logger::error("LTexture::loadFromFile: Unable to load image %s! SDL_Image Error: %s!", path.data(), IMG_GetError());
    return false;
  }
  SDL_SetColorKey(loadedSurface.get(), SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, FULL_INTENSITY, FULL_INTENSITY));
  m_Texture = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(SDL_CreateTextureFromSurface(Renderer::get(), loadedSurface.get()), SDL_DestroyTexture);
  if (m_Texture == nullptr) {
    Logger::error("LTexture::loadFromFile: Unable to create texture from %s! SDL_Error: %s\n", path.data(), SDL_GetError());
    return false;
  }
  m_Width = loadedSurface->w;
  m_Height = loadedSurface->h;
  m_name = path;

  return true;
}

bool LTexture::loadFromRenderedText(std::string_view textureText, SDL_Color textColor, TTF_Font *font)
{
  reset();
  auto textSurface = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)>(TTF_RenderText_Solid(font, textureText.data(), textColor), SDL_FreeSurface);
  if (textSurface == nullptr) {
    Logger::error("LTexture::loadFromRenderedText: Unable to create surface from text %s! SDL_Image Error: %s!", textureText.data(), IMG_GetError());
    return false;
  }
  m_Texture = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(SDL_CreateTextureFromSurface(Renderer::get(), textSurface.get()), SDL_DestroyTexture);
  if (m_Texture == nullptr) {
    Logger::error("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    return false;
  }
  m_Width = textSurface->w;
  m_Height = textSurface->h;
  m_name = textureText;

  return true;
}

void LTexture::reset()
{
  //Free texture if it exists
  if (m_Texture != nullptr) {
    m_Texture.release();
    m_Width = 0;
    m_Height = 0;
  }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  //Modulate texture rgb
  SDL_SetTextureColorMod(m_Texture.get(), red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  //Set blending function
  SDL_SetTextureBlendMode(m_Texture.get(), blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  //Modulate texture alpha
  SDL_SetTextureAlphaMod(m_Texture.get(), alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) const
{
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, m_Width, m_Height };

  //Set clip rendering dimensions
  if (clip != nullptr) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx(Renderer::get(), m_Texture.get(), clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() const
{
  return m_Width;
}

int LTexture::getHeight() const
{
  return m_Height;
}
