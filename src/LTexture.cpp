#include "Logger.h"

#include "LTexture.h"
#include "Commons.h"

LTexture::~LTexture()
{
  free();
}

bool LTexture::loadFromFile(const std::string_view path, SDL_Renderer *renderer)
{
  free();

  SDL_Texture *newTexture = nullptr;

  SDL_Surface *loadedSurface = IMG_Load(path.data());
  if (loadedSurface == nullptr) {
    Logger::error("Unable to load image %s! SDL_Image Error: %s!", path.data(), IMG_GetError());
  } else {
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, FULL_INTENSITY, FULL_INTENSITY));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
      Logger::error("Unable to create texture from %s! SDL_Error: %s\n", path.data(), SDL_GetError());
    } else {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }

    SDL_FreeSurface(loadedSurface);
  }
  mTexture = newTexture;
  return mTexture != nullptr;
}

bool LTexture::loadFromRenderedText(SDL_Renderer *renderer, std::string_view textureText, SDL_Color textColor, TTF_Font *font)
{
  //Get rid of preexisting texture
  free();

  //Render text surface
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, textureText.data(), textColor);
  if (textSurface != nullptr) {
    //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (mTexture == nullptr) {
      Logger::error("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    } else {
      //Get image dimensions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }

    //Get rid of old surface
    SDL_FreeSurface(textSurface);
  } else {
    Logger::error("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
  }


  //Return success
  return mTexture != nullptr;
}

void LTexture::free()
{
  //Free texture if it exists
  if (mTexture != nullptr) {
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  //Modulate texture rgb
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  //Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  //Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  //Set clip rendering dimensions
  if (clip != nullptr) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() const
{
  return mWidth;
}

int LTexture::getHeight() const
{
  return mHeight;
}