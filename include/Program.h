#pragma once

#include "ProgramType.h"
#include "SDL_Helpers.h"

#include "SDL_image_platform.h"

class Program
{
public:
    Program(const Program &program) = delete;
    Program &operator=(const Program &program) = delete;
    virtual ~Program() = default;

    inline bool isRunning() { return !quit; }
    inline ProgramTypeEnum getProgramType() { return this->m_programType.get(); }
    virtual void init() = 0;
    void run();

protected:
    ProgramType m_programType;

    constexpr static int SCREEN_WIDTH = 640;
    constexpr static int SCREEN_HEIGHT = 480;

    Program(ProgramTypeEnum anEnum);
    virtual void loadMedia() = 0;
    virtual void renderInit();
    virtual void renderPresent();
    virtual void renderMain() = 0;
    virtual void handleEvents() = 0;
    virtual void exit(ProgramTypeEnum next);

    static std::unique_ptr<SDL_Renderer, SDL_Destroyers> m_renderer;
    static std::unique_ptr<SDL_Window, SDL_Destroyers> m_window;
    static SDL_Event event;
    bool initialized;
    bool quit;
};
