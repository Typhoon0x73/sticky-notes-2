#include <Siv3D.hpp>
#include "Config.h"

namespace stn
{
	namespace ConfigDataKey
	{
		constexpr const char32_t* WINDOW_POS = U"window_pos";
		constexpr const char32_t* WINDOW_SIZE = U"window_size";
	}

	Config::Config()
	{
	}

	Config::~Config()
	{
	}

	void Config::setData(const JSON& data)
	{
		if (auto size = data[ConfigDataKey::WINDOW_SIZE].getOpt<Size>(); size.has_value())
		{
			Window::Resize(size.value());
		}
		if (auto pos = data[ConfigDataKey::WINDOW_POS].getOpt<Point>(); pos.has_value())
		{
			Window::SetPos(pos.value());
		}
	}

	JSON Config::toJson() const
	{
		JSON data;
		data[ConfigDataKey::WINDOW_SIZE] = Scene::Size();
		data[ConfigDataKey::WINDOW_POS] = Window::GetPos();
		return data;
	}

}

