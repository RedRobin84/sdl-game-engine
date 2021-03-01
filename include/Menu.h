#pragma once

#include "Program.h"
#include "LTexture.h"

#include <vector>

class Menu : public Program
{

public:
  Menu();
  void handleEvents() override;

private:
  const int TOTAL_DATA;

  int m_currentData;
  std::vector<LTexture> m_programNames;
  ProgramType m_currentType;
  LTexture m_promptTextTexture;

  [[nodiscard]] State loadMedia() override;
  void renderMain() override;
};