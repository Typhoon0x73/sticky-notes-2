#pragma once
#include "State/StateMachine.h"
#include "State/StateDefine.h"

namespace stn
{

	class InitState : public IState<State>
	{
	public:

		InitState();
		virtual ~InitState();

		virtual bool onCreate() override;
		virtual void onUpdate() override;
		virtual void onRelease() override;
		virtual Optional<State> getNextState() const override;
	};

}
