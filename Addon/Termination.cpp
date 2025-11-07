#include <Siv3D.hpp>
#include "Termination.h"
#include "AppDefine.h"
#include "Config/Config.h"

namespace stn
{
	Termination::Termination()
	{
	}

	Termination::~Termination()
	{
		saveConfig();
	}

	void Termination::saveConfig()
	{
		Config config;
		const auto& json = config.toJson();
		if (json.isEmpty())
		{
			return;
		}

		if (!json.save(APP_SETTING_FILE_PATH))
		{
			Logger << U"設定ファイルの出力に失敗";
			STN_DEBUG_BREAK(false);
			return;
		}
		Logger << U"設定ファイルの出力に成功";
		Logger << APP_SETTING_FILE_PATH;
	}

}

