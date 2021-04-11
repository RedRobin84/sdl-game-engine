#include "Particle.h"
#include "LTexture.h"
#include "Logger.h"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

Particle::Particle(int x, int y, const LTexture *texture, const LTexture *shimmerTexture)
{
  //Set offsets
  m_PosX = x - 5 + (rand() % 25);
  m_PosY = y - 5 + (rand() % 25);

  //Initialize animation
  m_Frame = rand() % 5;

  //Set type
  m_Texture = texture;
  m_ShimmeringTexture = shimmerTexture;
}

void Particle::render()
{
  //Show image
  m_Texture->render(m_PosX, m_PosY);

  //Show shimmer
  if (m_Frame % 2 == 0) {
    m_ShimmeringTexture->render(m_PosX, m_PosY);
  }

  //Animate
  m_Frame++;
}

bool Particle::isDead()
{
  return m_Frame > 10;
}
