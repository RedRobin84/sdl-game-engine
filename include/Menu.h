#pragma once

#include "Program.h"
#include "LTexture.h"

#include <vector>

class Menu : public Program
{

public:
  Menu();

private:
  struct MenuItem
  {
    LTexture texture;
    std::string_view name;
    ProgramTypeEnum programType;
  };

  std::vector<MenuItem> m_menuItems;
  std::vector<MenuItem>::iterator m_index;
  const std::vector<ProgramTypeEnum> m_programsToDisplay{ ProgramTypeEnum::AUDIO, ProgramTypeEnum::PARTICLE_ENGINES };
  LTexture m_promptTextTexture;

  void handleEvents() override;
  void update() override;
  [[nodiscard]] State loadMedia() override;
  void render() override;
};
