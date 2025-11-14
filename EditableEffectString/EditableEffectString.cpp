#include "EditableEffectString.h"
#include "FontKeyTable.h"

namespace
{
	constexpr const size_t EffectStringTabSize = 2;
}

namespace stn
{

	EditableEffectString::EditableEffectString(double fontSize, const FontStyle& style, const RectF& rect, const String& str)
		: m_isInputActive{ false }
		, m_font{}
		, m_rect{ rect }
		, m_fontColor{ Palette::Black }
		, m_strikeColor{ Palette::Black }
		, m_fontSize{ fontSize }
		, m_fontStyle{ style }
		, m_isStrikeThrough{ false }
		, m_string{ str }
		, m_index{ 0 }
		, m_glyphs{}
		, m_glyphIndices{}
	{
		setFontStyle(style);
	}

	auto EditableEffectString::update() -> UpdateResult
	{
		if (!m_isInputActive)
		{
			return UpdateResult::None;
		}
		if (m_rect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::IBeam);
		}
		const auto& editingText = TextInput::GetEditingText();
		if (editingText.empty())
		{
			if (updateIndex())
			{
				return UpdateResult::None;
			}
		}
		const String prev = m_string;
		m_index = TextInput::UpdateText(m_string, m_index, TextInputMode::Default);
		if (prev != m_string)
		{
			rebuildGlyphs();
			return UpdateResult::Edit;
		}
		return UpdateResult::None;
	}

	void EditableEffectString::draw() const
	{
		m_font(m_string).draw(m_fontSize, m_rect, m_fontColor);

		if (m_isStrikeThrough)
		{
			drawStrike();
		}

		if (m_isInputActive)
		{
			drawCursor();
		}
	}

	bool EditableEffectString::isInputActive() const
	{
		return m_isInputActive;
	}

	void EditableEffectString::setInputActive(bool enable)
	{
		m_isInputActive = enable;
	}

	void EditableEffectString::setRect(const RectF& rect)
	{
		m_rect = rect;
		rebuildGlyphs();
	}

	const RectF& EditableEffectString::getRect() const
	{
		return m_rect;
	}

	void EditableEffectString::setFontColor(const ColorF& color)
	{
		m_fontColor = color;
	}

	const ColorF& EditableEffectString::getFontColor() const
	{
		return m_fontColor;
	}

	void EditableEffectString::setStrikeColor(const ColorF& color)
	{
		m_strikeColor = color;
	}

	const ColorF& EditableEffectString::getStrikeColor() const
	{
		return m_strikeColor;
	}

	void EditableEffectString::setFontSize(double fontSize)
	{
		m_fontSize = fontSize;
		rebuildGlyphs();
	}

	double EditableEffectString::getFontSize() const
	{
		return 0.0;
	}

	void EditableEffectString::setFontStyle(FontStyle fontStyle)
	{
		auto begin = std::begin(FontKeyTable);
		auto end = std::end(FontKeyTable);
		auto it = std::find_if(
			begin, end,
			[fontStyle](const std::pair<FontStyle, const char32_t*>& v) { return v.first == fontStyle; }
		);
		STN_DEBUG_BREAK(it != end);
		const char32_t* fontKey = (it == end ? begin->second : it->second);
		m_font = FontAsset(fontKey);
		rebuildGlyphs();
	}

	FontStyle EditableEffectString::getFontStyle() const
	{
		return m_fontStyle;
	}

	void EditableEffectString::setStrikeThrough(bool enable)
	{
		m_isStrikeThrough = enable;
	}

	bool EditableEffectString::isStrikeThrough() const
	{
		return m_isStrikeThrough;
	}

	void EditableEffectString::setString(const String& str)
	{
		m_string = str;
		rebuildGlyphs();
	}

	const String& EditableEffectString::getString() const
	{
		return m_string;
	}

	void EditableEffectString::rebuildGlyphs()
	{
		const double spaceWidth = m_font.spaceWidth();

		const double scale = m_fontSize / static_cast<double>(m_font.fontSize());
		m_glyphs = m_font.getGlyphs(m_string);

		m_glyphIndices.clear();
		double x = 0.0;
		uint16 indexX = 0, indexY = 0;
		for (auto& glyph : m_glyphs)
		{
			glyph.xAdvance *= scale;
			glyph.yAdvance *= scale;
			auto nextLine = [&]()
				{
					indexX = 0;
					indexY++;
					x = 0.0;
				};
			if (m_rect.w < x + glyph.xAdvance)
			{
				nextLine();
			}
			if (glyph.codePoint == U'\t')
			{
				glyph.xAdvance = (spaceWidth * EffectStringTabSize * scale);
			}
			else if (IsControl(glyph.codePoint))
			{
				glyph.xAdvance = 0;
				if (glyph.codePoint == U'\n')
				{
					m_glyphIndices.emplace_back(indexX, indexY);
					nextLine();
				}
				continue;
			}
			m_glyphIndices.emplace_back(indexX, indexY);
			x += glyph.xAdvance;
			indexX++;
		}
		m_glyphIndices.emplace_back(indexX, indexY);
	}

	size_t EditableEffectString::getIndex(const Vec2& pos) const
	{
		double tempX = m_rect.x, tempY = m_rect.y;
		double fontHeight = m_font.height(m_fontSize);
		for (const auto& [index, glyph] : IndexedRef(m_glyphs))
		{
			if (glyph.codePoint == U'\n'
				|| m_rect.rightX() < tempX + glyph.xAdvance)
			{
				tempY += fontHeight;
				tempX = m_rect.x;
			}
			if (tempY + fontHeight < pos.y)
			{
				tempX += glyph.xAdvance;
				continue;
			}
			if (tempX < pos.x && pos.x <= tempX + glyph.xAdvance)
			{
				if (pos.x <= tempX + glyph.xAdvance * 0.5)
				{
					return index;
				}
				return index + 1;
			}
			tempX += glyph.xAdvance;
		}
		return m_string.length();
	}

	bool EditableEffectString::updateIndex()
	{
		const size_t prevIndex = m_index;
		if (m_rect.leftClicked())
		{
			const auto& pos = Cursor::PosF();
			m_index = getIndex(pos);
		}
		if (KeyUp.down())
		{
			const auto& glyphIndex = m_glyphIndices[m_index];
			if (0 < glyphIndex.y)
			{
				for (const auto& [index, it] : IndexedRef(m_glyphIndices))
				{
					if ((glyphIndex.y - 1 == it.y)
						&& (glyphIndex.x == it.x))
					{
						m_index = index;
						break;
					}
					if (glyphIndex.y == it.y)
					{
						m_index = index - 1;
						break;
					}
				}
			}
			else
			{
				m_index = 0;
			}
		}
		if (KeyDown.down())
		{
			size_t tail = m_glyphIndices.size() - 1;
			const auto& glyphIndex = m_glyphIndices[m_index];
			const auto& glyphTailIndex = m_glyphIndices[tail];
			if (glyphIndex.y < glyphTailIndex.y)
			{
				if ((glyphIndex.y + 1 == glyphTailIndex.y)
					&& (glyphTailIndex.x < glyphIndex.x))
				{
					m_index = m_string.length();
				}
				else
				{
					for (const auto& [index, it] : IndexedRef(m_glyphIndices))
					{
						if ((glyphIndex.y + 1 == it.y)
							&& (glyphIndex.x == it.x))
						{
							m_index = index;
							break;
						}
					}
				}
			}
			else
			{
				m_index = m_string.length();
			}
		}
		if (KeyLeft.down())
		{
			if (0 < m_index)
			{
				m_index--;
			}
		}
		if (KeyRight.down())
		{
			if (m_index < m_string.length())
			{
				m_index++;
			}
		}
		return prevIndex != m_index;
	}

	void EditableEffectString::drawStrike() const
	{
		const auto& fontHeight = m_font.height(m_fontSize);
		double strikeY = m_rect.y + fontHeight * 0.5;
		double strikeX = m_rect.x;
		for (const auto& [index, it] : IndexedRef(m_glyphIndices))
		{
			if (m_glyphs.size() <= index)
			{
				break;
			}
			if (it.x == 0)
			{
				strikeX = m_rect.x;
			}
			double lineY = strikeY + it.y * fontHeight;
			Line{ strikeX, lineY, strikeX + m_glyphs[index].xAdvance, lineY }.draw(m_strikeColor);
			strikeX += m_glyphs[index].xAdvance;
		}
	}

	void EditableEffectString::drawCursor() const
	{
		const size_t indexY = m_glyphIndices[m_index].y;
		double cursorX = 0.0;
		for (size_t i = 0; i < m_index; i++)
		{
			if (indexY != m_glyphIndices[i].y)
			{
				continue;
			}
			cursorX += m_glyphs[i].xAdvance;
		}
		double cursorY = indexY * m_font.height(m_fontSize);
		double cursorH = m_font.height(m_fontSize);
		Line{ cursorX, cursorY, cursorX, cursorY + cursorH }.moveBy(m_rect.pos).draw(Palette::Black);
	}

}
