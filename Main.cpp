# include <Siv3D.hpp> // Siv3D v0.6.16
# include "Common.h"
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

# include "FontKeyTable.h"
# include "ServiceLocator/SingletonLocator.h"
# include "DrawObject/DrawObjectManager.h"
# include "DragEditRect/DragEditRect.h"
# include "EditableEffectString/EditableEffectString.h"
# include "DoubleClick/DoubleClick.h"
# include "StickyNote/StickyNote.h"
# include "Camera/DragCamera2D.h"

namespace stn
{
	class Board
	{
	public:

		Board() = default;
		virtual ~Board() = default;

		void update()
		{

		}

		void draw() const
		{

		}

	private:

		DragCamera2D m_camera;
		Array<std::unique_ptr<StickyNote>> m_notes;
	};

}

//===================================================================
// エントリ
//===================================================================
void Main()
{

	Window::SetTitle(stn::APP_NAME);
	Window::SetStyle(WindowStyle::Sizable);
	Scene::SetBackground(Palette::Whitesmoke);

	// フォントの登録
	for (const auto& [style, key] : stn::FontKeyTable)
	{
		FontAsset::Register(key, FontMethod::MSDF, 32, Typeface::Mplus_Regular, style);
	}

	// 描画管理の登録 描画管理出番ないかも。
	stn::DrawObjectManager drawManager;
	STN_GET_LOCATOR(stn::DrawObjectManager).set(&drawManager);

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	stn::StickyNote note{ RectF{ 100, 100, 100, 100 }, 16, FontStyle::Bold, U"BOLD" };
	
	while (System::Update())
	{
		Cursor::RequestStyle(CursorStyle::Default);
		if (MouseL.down())
		{
			note.setSelect(note.getRect().mouseOver());
		}
		note.update();
		note.draw();

		drawManager.draw();
	}
}
