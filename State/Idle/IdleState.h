#pragma once
#include "State/StateMachine.h"
#include "State/StateDefine.h"

namespace stn
{
	class MainMenu;
	class IdleState : public IState<State>
	{
	public:

		IdleState();
		virtual ~IdleState();

		virtual bool onCreate() override;
		virtual void onUpdate() override;
		virtual void onRelease() override;
		virtual Optional<State> getNextState() const override;

	private:

		std::unique_ptr<MainMenu> m_pMainMenu;
	};

}
