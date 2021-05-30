#pragma once

#include "SDL_platform.h"
#include "enum/State.h"

#include <memory>
#include <vector>

class Registry;
class Particle;
class LTexture;

class Dot
{
public:
  static constexpr int TOTAL_PARTICLES = 10;
  //The dimensions of the dot
  static constexpr int DOT_WIDTH = 20;
  static constexpr int DOT_HEIGHT = 20;

  //Maximum axis velocity of the dot
  static constexpr int DOT_VEL = 10;

  //Initializes the variables and allocates particles
  Dot(Registry *registry);

  //Deallocates particles
  ~Dot();

  //Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event &e);

  //Moves the dot
  void update();

  //Shows the dot on the screen
  void render();

private:
  //The particles
  Particle *m_particles[TOTAL_PARTICLES];

  std::vector<std::shared_ptr<LTexture>> m_particleTextures;
  std::shared_ptr<LTexture> m_ShimmeringTexture;

  //Texture
  std::shared_ptr<LTexture> m_texture;

  //Shows the particles
  void renderParticles();

  //The X and Y offsets of the dot
  int m_PosX, m_PosY;

  //The velocity of the dot
  int m_VelX, m_VelY;
};
