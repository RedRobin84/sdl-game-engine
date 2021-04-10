#pragma once

#include "enum/State.h"

#include <memory>

class SDL_Renderer;
class LTexture;

class Particle
{
public:
  //Initialize position and animation
  Particle(int x, int y, const LTexture *texture, const LTexture *shimmerTexture);

  //Shows the particle
  void render();

  //Checks if particle is dead
  bool isDead();

private:
  //Offsets
  int m_PosX, m_PosY;

  //Current frame of animation
  int m_Frame;

  //Type of particle
  const LTexture *m_Texture;
  const LTexture *m_ShimmeringTexture;
};
