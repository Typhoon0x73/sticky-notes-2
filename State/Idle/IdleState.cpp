#include "stdafx.h"
#include "IdleState.h"

namespace stn
{
	IdleState::IdleState()
	{
	}

	IdleState::~IdleState()
	{
	}

	bool IdleState::onCreate()
	{
		return true;
	}

	void IdleState::onUpdate()
	{
	}

	void IdleState::onRelease()
	{
	}

	Optional<State> IdleState::getNextState() const
	{
		return none;
	}

}
