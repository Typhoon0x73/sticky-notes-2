#include "stdafx.h"
#include "InitState.h"
#include "AppDefine.h"
#include "Config/Config.h"

namespace stn
{
	InitState::InitState()
	{
	}

	InitState::~InitState()
	{
	}

	bool InitState::onCreate()
	{
		// 最初にアプリの設定ファイルを読み込む。
		{
			Config config;
			if (FileSystem::Exists(APP_SETTING_FILE_PATH))
			{
				const JSON& json = JSON::Load(APP_SETTING_FILE_PATH);
				config.setData(json);
			}
		}
		return true;
	}

	void InitState::onUpdate()
	{
	}

	void InitState::onRelease()
	{
	}

	Optional<State> InitState::getNextState() const
	{
		return State::Idle;
	}


}
