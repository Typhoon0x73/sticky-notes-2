#include "stdafx.h"
#include "MainMenu.h"

namespace
{
	constexpr const char32_t* MAIN_MENU_LAYOUT_FILE = U"config/main_menu_layout.noco";

	// NCH
	constexpr const char32_t* MAIN_MENU_FILE_NAME = U"MainMenuFile";
	constexpr const char32_t* MAIN_MENU_EDIT_NAME = U"MainMenuEdit";

	enum class MainMenuLabel
	{
		File,
		Edit,
	};
}

namespace stn
{
	MainMenu::MainMenu(Layer layer)
		: super{ layer }
		, m_pCanvas{ nullptr }
	{
		m_pCanvas = noco::Canvas::LoadFromFile(MAIN_MENU_LAYOUT_FILE);
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::update()
	{
		if (m_pCanvas)
		{
			m_pCanvas->update();
		}
	}

	void MainMenu::draw() const
	{
		if (m_pCanvas)
		{
			m_pCanvas->draw();
		}
	}

}
