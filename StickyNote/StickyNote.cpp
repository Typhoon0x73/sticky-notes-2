#include "StickyNote.h"

namespace
{
	constexpr double TextAreaMargin = 12.0;
}

namespace stn
{

	StickyNote::StickyNote(const RectF& rect, double fontSize, FontStyle style, const String& str)
		: m_isSelected{ false }
		, m_baseColor{ Palette::Lemonchiffon }
		, m_editRect{ rect.pos, rect.size }
		, m_effectString{ fontSize, style, getTextArea(), str }
		, m_doubleClick{}
	{
	}

	auto StickyNote::update() -> UpdateResult
	{
		if (!m_isSelected)
		{
			return UpdateResult::None;
		}

		// アクティブの切り替え
		m_doubleClick.update();
		if (getTextArea().mouseOver())
		{
			if (m_doubleClick.doubleClicked())
			{
				m_effectString.setInputActive(true);
			}
		}
		else if (MouseL.down())
		{
			m_effectString.setInputActive(false);
		}

		// 矩形操作
		if (auto result = m_editRect.update();
			result != DragEditRect::UpdateResult::None)
		{
			m_effectString.setRect(getTextArea());
			switch (result)
			{
			case DragEditRect::UpdateResult::Move: return UpdateResult::Move;
			case DragEditRect::UpdateResult::Resize: return UpdateResult::Resize;
			case DragEditRect::UpdateResult::Release: return UpdateResult::EditRectEnd;
			default: STN_DEBUG_BREAK(false); break;
			}
		}

		// テキスト操作
		if (auto result = m_effectString.update();
			result != EditableEffectString::UpdateResult::None)
		{
			switch (result)
			{
			case EditableEffectString::UpdateResult::Edit: return UpdateResult::EditText;
			default: STN_DEBUG_BREAK(false); break;
			}
		}
		return UpdateResult::None;
	}

	void StickyNote::draw() const
	{
		const auto& roundRect = m_editRect.rounded(4);
		roundRect.drawShadow(Vec2{ 2.0, 2.0 }, 12.0).draw(m_baseColor);
		m_effectString.draw();

		if (m_isSelected)
		{
			roundRect.drawFrame(1.0, Palette::Green);
		}
	}

	void StickyNote::setSelect(bool isSelect)
	{
		m_isSelected = isSelect;
		if (isSelect == false)
		{
			m_effectString.setInputActive(false);
		}
	}

	bool StickyNote::isSelect() const
	{
		return m_isSelected;
	}

	void StickyNote::setRect(const RectF& rect)
	{
		m_editRect.set(rect);
		m_effectString.setRect(getTextArea());
	}

	RectF StickyNote::getRect() const
	{
		return m_editRect.getEditRect();
	}

	void StickyNote::setFontColor(const ColorF& color)
	{
		m_effectString.setFontColor(color);
	}

	const ColorF& StickyNote::getFontColor() const
	{
		return m_effectString.getFontColor();
	}

	void StickyNote::setStrikeColor(const ColorF& color)
	{
		m_effectString.setStrikeColor(color);
	}

	const ColorF& StickyNote::getStrikeColor() const
	{
		return m_effectString.getStrikeColor();
	}

	void StickyNote::setFontSize(double fontSize)
	{
		m_effectString.setFontSize(fontSize);
	}

	double StickyNote::getFontSize() const
	{
		return m_effectString.getFontSize();
	}

	void StickyNote::setFontStyle(FontStyle fontStyle)
	{
		m_effectString.setFontStyle(fontStyle);
	}

	FontStyle StickyNote::getFontStyle() const
	{
		return m_effectString.getFontStyle();
	}

	void StickyNote::setStrikeThrough(bool enable)
	{
		m_effectString.setStrikeThrough(enable);
	}

	bool StickyNote::isStrikeThrough() const
	{
		return m_effectString.isStrikeThrough();
	}

	void StickyNote::setString(const String& str)
	{
		m_effectString.setString(str);
	}

	const String& StickyNote::getString() const
	{
		return m_effectString.getString();
	}

	RectF StickyNote::getTextArea() const
	{
		return m_editRect.stretched(-TextAreaMargin);
	}

}
