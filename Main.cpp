# include <Siv3D.hpp> // Siv3D v0.6.16
# include "Common.h"
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

# include "State/StateDefine.h"
# include "State/StateMachine.h"
# include "State/Init/InitState.h"
# include "State/Idle/IdleState.h"
# include "State/MainMenu/MainMenuState.h"

# include "ServiceLocator/SingletonLocator.h"
# include "DrawObject/DrawObjectManager.h"

void Main()
{
	noco::Init();

	Window::SetTitle(stn::APP_NAME);
	Window::SetStyle(WindowStyle::Sizable);

	// 描画管理の登録
	stn::DrawObjectManager drawManager;
	STN_GET_LOCATOR(stn::DrawObjectManager).set(&drawManager);

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	// アプリのステートを登録
	stn::StateMachine<stn::State> stateMachine;
	stateMachine
		.add<stn::InitState>(stn::State::Init)
		.add<stn::IdleState>(stn::State::Idle)
		.add<stn::MainMenuState>(stn::State::MainMenu)
		.init(stn::State::Init);

	while (System::Update())
	{
		bool isSucceeded = stateMachine.update();
		STN_DEBUG_BREAK(isSucceeded);

		drawManager.draw();
	}
}
