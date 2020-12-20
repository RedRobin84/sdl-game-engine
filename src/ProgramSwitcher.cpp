#include "ProgramSwitcher.h"
#include "Menu.h"

#include "SDL_ttf_platform.h"

ProgramSwitcher::ProgramSwitcher() :   m_factory(std::make_unique<ProgramFactory>())
{
	m_programStack.emplace(m_factory.get()->create(ProgramTypeEnum::MENU));
}

ProgramSwitcher::~ProgramSwitcher()
{
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

inline bool ProgramSwitcher::programIsRunning()
{
	return this->m_programStack.top().get()->isRunning();
}

void ProgramSwitcher::switchProgram()
{
	if (m_programStack.top().get()->getProgramType() == ProgramTypeEnum::NO_TYPE)
	{
		m_programStack.pop();
		return;
	}
	
	m_programStack.emplace(m_factory.get()->create(m_programStack.top().get()->getProgramType()));
}
