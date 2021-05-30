#include "ParticleEngines.h"
#include "Renderer.h"

ParticleEngines::ParticleEngines(const std::shared_ptr<Registry> &registry) : Program(ProgramTypeEnum::PARTICLE_ENGINES, registry), m_dot(registry.get()) {}

void ParticleEngines::handleEvents()
{
  if (event.key.keysym.sym == SDLK_ESCAPE) {
    Program::endProgram();
  }
  m_dot.handleEvent(event);
  m_renderer->setDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
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
