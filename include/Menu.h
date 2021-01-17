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
    constexpr static int MEDIUM_FONT_SIZE = 28;
    constexpr static char LAZY_FONT_PATH[] = "../assets/fonts/lazy.ttf";
    
    int m_currentData;
    std::unique_ptr<TTF_Font, TTF_Destroyers> m_font;
    std::vector<LTexture> m_programNames;
    ProgramType m_currentType;
    LTexture m_promptTextTexture;
    SDL_Color m_textColor;
    SDL_Color m_highlightColor;

    void loadMedia() override;
    void renderMain() override;
};