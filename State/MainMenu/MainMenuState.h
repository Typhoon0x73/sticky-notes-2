#pragma once
#include "State/StateMachine.h"
#include "State/StateDefine.h"

namespace stn
{

	class MainMenuState : public IState<State>
	{
	public:

		MainMenuState();
		virtual ~MainMenuState();

		virtual bool onCreate() override;
		virtual void onUpdate() override;
		virtual void onRelease() override;
		virtual Optional<State> getNextState() const override;
	};

}
