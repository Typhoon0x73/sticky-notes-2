#pragma once
#include <Siv3D.hpp>

namespace stn
{

	class EditableEffectString
	{
	public:

		enum class UpdateResult
		{
			None,
			Edit,
		};

		EditableEffectString(double fontSize, const FontStyle& style, const RectF& rect, const String& str);
		virtual ~EditableEffectString() = default;

		auto update() -> UpdateResult;
		void draw() const;

		void setInputActive(bool enable);
		bool isInputActive() const;

		void setRect(const RectF& rect);
		const RectF& getRect() const;

		void setFontColor(const ColorF& color);
		const ColorF& getFontColor() const;

		void setStrikeColor(const ColorF& color);
		const ColorF& getStrikeColor() const;

		void setFontSize(double fontSize);
		double getFontSize() const;

		void setFontStyle(FontStyle fontStyle);
		FontStyle getFontStyle() const;

		void setStrikeThrough(bool enable);
		bool isStrikeThrough() const;

		void setString(const String& str);
		const String& getString() const;

	private:

		bool m_isInputActive;
		Font m_font;
		RectF m_rect;
		ColorF m_fontColor;
		ColorF m_strikeColor;
		double m_fontSize;
		FontStyle m_fontStyle;
		bool m_isStrikeThrough;
		String m_string;
		size_t m_index;
		Array<Glyph> m_glyphs;
		Array<Vector2D<uint16>> m_glyphIndices;

		void rebuildGlyphs();
		size_t getIndex(const Vec2& pos) const;
		bool updateIndex();

		void drawStrike() const;
		void drawCursor() const;
	};
}
