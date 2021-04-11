#include "Dot.h"
#include "Logger.h"
#include "LTexture.h"
#include "Registry.h"
#include "Particle.h"
#include "Renderer.h"

#include <string_view>
#include <array>
#include <algorithm>

namespace {
constexpr std::string_view DOT_PATH = "../assets/img/dot.bmp";
constexpr std::string_view SHIMMER_PATH = "../assets/img/shimmer.bmp";
constexpr std::array<std::string_view, 3> PARTICLE_TEXTURES = { { "../assets/img/red.bmp", "../assets/img/green.bmp", "../assets/img/blue.bmp" } };
}// namespace

Dot::Dot(Registry &registry)
{
  m_texture = registry.registerTexture(DOT_PATH);
  m_texture->setAlpha(192);
  m_ShimmeringTexture = registry.registerTexture(SHIMMER_PATH);
  m_ShimmeringTexture->setAlpha(192);

  std::for_each(PARTICLE_TEXTURES.begin(), PARTICLE_TEXTURES.end(), [&](std::string_view texturePath) {
    m_particleTextures.emplace_back(registry.registerTexture(texturePath));
  });
  std::for_each(m_particleTextures.begin(), m_particleTextures.end(), [](const auto &texture) {
    texture->setAlpha(192);
  });

  //Initialize the offsets
  m_PosX = 0;
  m_PosY = 0;

  //Initialize the velocity
  m_VelX = 0;
  m_VelY = 0;

  //Initialize particles
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    m_particles[i] = new Particle(m_PosX, m_PosY, m_particleTextures[1].get(), m_ShimmeringTexture.get());
  }
}

void Dot::handleEvent(SDL_Event &e)
{
  //If a key was pressed
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    //Adjust the velocity
    switch (e.key.keysym.sym) {
    case SDLK_UP:
      m_VelY -= DOT_VEL;
      break;
    case SDLK_DOWN:
      m_VelY += DOT_VEL;
      break;
    case SDLK_LEFT:
      m_VelX -= DOT_VEL;
      break;
    case SDLK_RIGHT:
      m_VelX += DOT_VEL;
      break;
    }
  }
  //If a key was released
  else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
    //Adjust the velocity
    switch (e.key.keysym.sym) {
    case SDLK_UP:
      m_VelY += DOT_VEL;
      break;
    case SDLK_DOWN:
      m_VelY -= DOT_VEL;
      break;
    case SDLK_LEFT:
      m_VelX += DOT_VEL;
      break;
    case SDLK_RIGHT:
      m_VelX -= DOT_VEL;
      break;
    }
  }
}

void Dot::update()
{

  //Move the dot left or right
  m_PosX += m_VelX;

  //If the dot went too far to the left or right
  if ((m_PosX < 0) || (m_PosX + DOT_WIDTH > Renderer::SCREEN_WIDTH)) {
    //Move back
    m_PosX -= m_VelX;
  }

  //Move the dot up or down
  m_PosY += m_VelY;

  //If the dot went too far up or down
  if ((m_PosY < 0) || (m_PosY + DOT_HEIGHT > Renderer::SCREEN_HEIGHT)) {
    //Move back
    m_PosY -= m_VelY;
  }
}


void Dot::render()
{
  //Show the dot
  m_texture->render(m_PosX, m_PosY);

  //Show particles on top of dot
  renderParticles();
}

void Dot::renderParticles()
{
  //Go through particles
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    //Delete and replace dead particles
    if (m_particles[i]->isDead()) {
      delete m_particles[i];
      m_particles[i] = new Particle(m_PosX, m_PosY, m_particleTextures[1].get(), m_ShimmeringTexture.get());
    }
  }

  //Show particles
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    m_particles[i]->render();
  }
}

Dot::~Dot()
{
  //Delete particles
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    delete m_particles[i];
  }
}
