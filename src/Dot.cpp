#include "Dot.h"
#include "Logger.h"
#include "LTexture.h"

LTexture Dot::m_texture;
bool Dot::initialized = false;

State Dot::init(SDL_Renderer *renderer)
{
    if (initialized) {
        Logger::warn("Dot::init: Dot class already initialized.");
        return State::FAILURE;
    }
  //Load dot texture
    if( !m_texture.loadFromFile("../assets/img/dot.bmp", renderer))
    {
        Logger::error("Dot::init: Failed to load dot texture!");
        return State::FAILURE;
    }
    return State::SUCCESS;
}

Dot::Dot()
{
  if (!initialized) {
    throw std::runtime_error("Particle::Particle: Static members not initialized.");
  }
}

namespace {

}
