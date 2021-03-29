#include "Particle.h"
#include "LTexture.h"
#include "enum/State.h"
#include "Logger.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>

bool Particle::initialized = false;
std::array<LTexture, Particle::NUMBER_OF_TEXTURES> Particle::particle_textures{
  { LTexture("red"), LTexture("green"), LTexture("blue") }
};
LTexture Particle::shimmerTexture;

Particle::Particle(int x, int y)
{
  if (!initialized) {
    throw std::runtime_error("Particle::Particle: Static members not initialized.");
  }
  //Set offsets
  mPosX = x - 5 + (rand() % 25);
  mPosY = y - 5 + (rand() % 25);

  //Initialize animation
  mFrame = rand() % 5;

  //Set type
  mTexture = &particle_textures[rand() % NUMBER_OF_TEXTURES];
}

State Particle::init(SDL_Renderer *renderer)
{
  if (initialized) {
    Logger::warn("Particle::init: Particle already initialized.");
    return State::FAILURE;
  }
  State state = State::SUCCESS;

  std::for_each(particle_textures.begin(), particle_textures.end(), [&renderer](LTexture &texture) {
    texture.loadFromFile(renderer);
    texture.setAlpha(192);
  });

  //Load shimmer texture
  if (!shimmerTexture.loadFromFile("38_particle_engines/shimmer.bmp", renderer)) {
    Logger::error("Failed to load shimmer texture!\n");
    return State::FAILURE;
  }
  shimmerTexture.setAlpha(192);

  return State::SUCCESS;
}
