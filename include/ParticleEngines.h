#pragma once

#include "Program.h"
#include "Dot.h"

class ParticleEngines : public Program
{
public:
  ParticleEngines();

private:
  Dot m_dot;

  void handleEvents() override;
  void update() override;
  [[nodiscard]] State loadMedia() override;
  void render() override;
};
