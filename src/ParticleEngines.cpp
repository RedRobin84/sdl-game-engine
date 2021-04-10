#include "ParticleEngines.h"
#include "Renderer.h"

ParticleEngines::ParticleEngines() : Program(ProgramTypeEnum::PARTICLE_ENGINES), m_dot(Program::registry) {}

void ParticleEngines::handleEvents()
{
  if (event.key.keysym.sym == SDLK_ESCAPE) {
    Program::endProgram();
  }
  m_dot.handleEvent(event);
  Renderer::setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
}

[[nodiscard]] State ParticleEngines::loadMedia() { return State::SUCCESS; }


void ParticleEngines::update()
{
  m_dot.update();
}

void ParticleEngines::render()
{
  m_dot.render();
}
