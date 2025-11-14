#pragma once
#include <Siv3D.hpp>
#include "DoubleClick/DoubleClick.h"
#include "DragEditRect/DragEditRect.h"
#include "EditableEffectString/EditableEffectString.h"

namespace stn
{

	class StickyNote
	{
	public:

		enum class UpdateResult
		{
			None,
			Move,
			Resize,
			EditRectEnd,
			EditText,
		};

		StickyNote(const RectF& rect, double fontSize, FontStyle style, const String& str);
		virtual ~StickyNote() = default;

		auto update() -> UpdateResult;
		void draw() const;


		void setSelect(bool isSelect);
		bool isSelect() const;

		void setRect(const RectF& rect);
		RectF getRect() const;

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

		bool m_isSelected;
		ColorF m_baseColor;
		DragEditRect m_editRect;
		EditableEffectString m_effectString;
		DoubleClick m_doubleClick;

		RectF getTextArea() const;
	};
}
