# include <Siv3D.hpp> // Siv3D v0.6.16
# include "Common.h"
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

# include "ServiceLocator/SingletonLocator.h"
# include "DrawObject/DrawObjectManager.h"
# include "DragEditRect/DragEditRect.h"

namespace stn
{
	namespace
	{
		constexpr const size_t EffectStringTabSize = 2;
	}
	class EditableEffectString
	{
	public:

		enum class UpdateResult
		{
			None,
			Edit,
		};

		EditableEffectString(double fontSize, const FontStyle& style, const RectF& rect, const String& str)
			: m_rect{ rect }
			, m_fontColor{ Palette::Black }
			, m_fontSize{ fontSize }
			, m_fontStyle{ style }
			, m_isStrikeThrough{ false }
			, m_string{ str }
			, m_index{ 0 }
			, m_glyphs{}
			, m_glyphIndices{}
		{
			m_font = Font(32, Typeface::Mplus_Regular, style);
			rebuildGlyphs();
		}

		virtual ~EditableEffectString() = default;

		auto update() -> EditableEffectString::UpdateResult
		{
			const auto& editingText = TextInput::GetEditingText();
			if (editingText.empty())
			{
				const size_t prevIndex = m_index;
				// 選択位置の操作
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
				if (prevIndex != m_index)
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

		void draw() const
		{
			m_font(m_string).draw(m_fontSize, m_rect, m_fontColor);

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

		void setRect(const RectF& rect) { m_rect = rect; rebuildGlyphs(); }
		//void setFontColor(const ColorF& color) {}
		//void setFontSize(double fontSize) {}
		//void setFontStyle(FontStyle fontStyle) {}
		//void setStrikeThrough(bool enable) {}
		//void setString(const String& str) {}

	private:

		Font m_font;
		RectF m_rect;
		ColorF m_fontColor;
		double m_fontSize;
		FontStyle m_fontStyle;
		bool m_isStrikeThrough;
		String m_string;
		size_t m_index;
		Array<Glyph> m_glyphs;
		Array<Vector2D<uint16>> m_glyphIndices;

		void rebuildGlyphs()
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
				auto nextLine = [&]() {
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

		size_t getIndex(const Vec2& pos) const
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
					continue;
				}
				if (tempX < pos.x && pos.x <= tempX + glyph.xAdvance)
				{
					return index;
				}
				tempX += glyph.xAdvance;
			}
			return m_string.length();
		}
	};

	class StickyNote
	{
	public:

		enum class UpdateResult
		{
			None,
			Move,
			Resize,
		};

		//StickyNote(const RectF& rect);
		//virtual ~StickyNote() = default;
		//
		//UpdateResult update();
		//void draw() const;
		//
		//const RectF& getRect() const;

	private:

		//bool m_isSelected;
		//ColorF m_baseColor;
		//DragEditRect m_editRect;
		//EditableEffectString m_effectString;
	};
}

//===================================================================
// エントリ
//===================================================================
void Main()
{

	Window::SetTitle(stn::APP_NAME);
	Window::SetStyle(WindowStyle::Sizable);

	// 描画管理の登録
	stn::DrawObjectManager drawManager;
	STN_GET_LOCATOR(stn::DrawObjectManager).set(&drawManager);

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	stn::DragEditRect rect{ 100, 100, 100, 100 };
	stn::EditableEffectString editEffectStr{ 16.0, FontStyle::Default, rect.stretched(-12), U"default" };

	while (System::Update())
	{
		if (rect.update() != stn::DragEditRect::UpdateResult::None)
		{
			editEffectStr.setRect(rect.stretched(-12));
		}
		editEffectStr.update();
		rect.draw().drawFrame(1.0, Palette::Green);
		editEffectStr.draw();
		drawManager.draw();
	}
}
