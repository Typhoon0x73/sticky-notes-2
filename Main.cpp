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
# include "Command/CommandManager.h"
# include "Background/Background.h"

namespace stn
{
	class Board
	{
	public:

		Board()
			: m_isActive{ false }
			, m_camera{ Scene::CenterF() }
			, m_notes{}
			, m_commandManager{}
		{
		}
		virtual ~Board() = default;

		void update()
		{
		}

		void draw() const
		{
		}

		void activate()
		{
			m_isActive = true;
			STN_GET_LOCATOR(BasicCamera2D).set(&m_camera);
		}

		void hide()
		{
			m_isActive = false;
		}

		bool isActive() const { return m_isActive; }

	private:

		bool m_isActive;
		DragCamera2D m_camera;
		Array<std::unique_ptr<StickyNote>> m_notes;
		CommandManager m_commandManager;
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

	// 背景
	stn::Background background;

	// アドオンの登録
	Addon::Register<stn::Termination>(U"Termination");

	stn::Board board;
	board.activate();
	
	while (System::Update())
	{
		Cursor::RequestStyle(CursorStyle::Default);

		if (board.isActive())
		{
			board.update();
			board.draw();
		}

		drawManager.draw();
	}
}
