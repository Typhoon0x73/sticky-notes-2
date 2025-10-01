#include "stdafx.h"
#include "IdleState.h"
#include "DrawObject/LayerDefine.h"
#include "MainMenu/MainMenu.h"

namespace stn
{
	IdleState::IdleState()
		:	m_pMainMenu{ nullptr }
	{
	}

	IdleState::~IdleState()
	{
	}

	bool IdleState::onCreate()
	{
		m_pMainMenu = std::make_unique<MainMenu>(Layer::Front);
		return true;
	}

	void IdleState::onUpdate()
	{
		if (m_pMainMenu)
		{
			m_pMainMenu->update();
		}
	}

	void IdleState::onRelease()
	{
	}

	Optional<State> IdleState::getNextState() const
	{
		return none;
	}

}
