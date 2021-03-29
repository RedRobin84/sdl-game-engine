#pragma once

#include <array>

enum class State;
class SDL_Renderer;
class LTexture;

class Particle
{
public:
  static constexpr int NUMBER_OF_TEXTURES = 3;

  //Initialize position and animation
  Particle(int x, int y);

  //Shows the particle
  void render();

  //Initialize static variables
  static State init(SDL_Renderer *renderer);

  //Checks if particle is dead
  bool isDead();

private:
  //Offsets
  int mPosX, mPosY;

  //Current frame of animation
  int mFrame;

  //Type of particle
  LTexture *mTexture;

static bool initialized;
  static std::array<LTexture, NUMBER_OF_TEXTURES> particle_textures;
  static LTexture shimmerTexture;
};