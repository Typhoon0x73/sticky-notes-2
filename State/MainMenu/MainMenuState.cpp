#include "stdafx.h"
#include "MainMenuState.h"

namespace stn
{
	MainMenuState::MainMenuState()
	{
	}

	MainMenuState::~MainMenuState()
	{
	}

	bool MainMenuState::onCreate()
	{
		return true;
	}

	void MainMenuState::onUpdate()
	{
	}

	void MainMenuState::onRelease()
	{
	}

	Optional<State> MainMenuState::getNextState() const
	{
		return none;
	}

}
