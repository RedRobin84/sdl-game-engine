#pragma once

#include "SDL_platform.h"
#include "Particle.h"

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
  Dot();

  //Deallocates particles
  ~Dot();

  //Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event &e);

  //Moves the dot
  void move();

  //Shows the dot on the screen
  void render();

  static State init(SDL_Renderer *renderer);

private:
  //The particles
  Particle *particles[TOTAL_PARTICLES];

  //Texture
  static LTexture m_texture;

  static bool initialized;

  //Shows the particles
  void renderParticles();

  //The X and Y offsets of the dot
  int mPosX, mPosY;

  //The velocity of the dot
  int mVelX, mVelY;
};