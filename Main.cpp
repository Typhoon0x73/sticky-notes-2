# include <Siv3D.hpp> // Siv3D v0.6.16
# include "Common.h"
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

# include "ServiceLocator/SingletonLocator.h"
# include "DrawObject/DrawObjectManager.h"

void Main()
{

	Window::SetTitle(stn::APP_NAME);
	Window::SetStyle(WindowStyle::Sizable);

	// 描画管理の登録
	stn::DrawObjectManager drawManager;
	STN_GET_LOCATOR(stn::DrawObjectManager).set(&drawManager);

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	// アプリのステートを登録

	while (System::Update())
	{
		drawManager.draw();
	}
}
