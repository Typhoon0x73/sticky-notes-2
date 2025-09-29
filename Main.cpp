# include <Siv3D.hpp> // Siv3D v0.6.16
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

void Main()
{
	Window::SetTitle(stn::APP_NAME);
	Window::SetStyle(WindowStyle::Sizable);

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	// 最初にアプリの設定ファイルを読み込む。
	{
		stn::Config config;
		if (FileSystem::Exists(stn::APP_SETTING_FILE_PATH))
		{
			const JSON& json = JSON::Load(stn::APP_SETTING_FILE_PATH);
			config.setData(json);
		}
	}

	while (System::Update())
	{
	}
}
