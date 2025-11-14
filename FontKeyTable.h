#pragma once
#include <Siv3D.hpp>

namespace stn
{
	constexpr std::pair<FontStyle, const char32_t*> FontKeyTable[] =
	{
		{ FontStyle::Default, U"Default" },
		{ FontStyle::Bold, U"Bold" },
		{ FontStyle::Italic, U"Italic" },
		{ FontStyle::BoldItalic, U"BoldItalic" },
	};
}
