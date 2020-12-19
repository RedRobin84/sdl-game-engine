#pragma once

#include "Program.h"
#include "LTexture.h"

#include <vector>

class Menu : public Program
{

public:
    Menu();
    void init() override;
    void handleEvents() override;

private:
    const int TOTAL_DATA;
    
    int m_currentData;
    std::unique_ptr<TTF_Font, TTF_Destroyers> m_font;
    std::vector<LTexture> m_programNames;
    ProgramType m_currentType;
    LTexture m_promptTextTexture;
    SDL_Color m_textColor;
    SDL_Color m_highlightColor;
	bool renderText;

    void loadMedia() override;
    void renderMain() override;
};