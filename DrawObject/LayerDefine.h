#pragma once

// INCLUDE
#include <cstddef>

namespace stn
{
	enum class Layer : std::size_t
	{
		Background = 0,
		Main,
		UI,
		Fade,
		Front,

		Count, //!< 最大数
		Default = Main,
	};
}
