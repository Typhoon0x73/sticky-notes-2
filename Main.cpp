# include <Siv3D.hpp> // Siv3D v0.6.16
# include "Common.h"
# include "AppDefine.h"
# include "Config/Config.h"
# include "Addon/Termination.h"

# include "ServiceLocator/SingletonLocator.h"
# include "DrawObject/DrawObjectManager.h"


//===================================================================
// レイアウト定義
//===================================================================
namespace
{
	constexpr int32 SideBarPadding = 16;
	constexpr int32 SideBarIconSize = 32;
	constexpr int32 SideBarPosX = 0;
	constexpr int32 SideBarPosY = 0;
	constexpr int32 SideBarWidthAccordionOff = SideBarIconSize + SideBarPadding * 2;
	constexpr int32 SideBarWidthAccordionOn = 250;
	using SideBarAccordionOpen = YesNo<struct SideBarAccordionOpen_tag>;
	static const Rect GetSideBarRect(SideBarAccordionOpen open)
	{
		const int32 width = open ? SideBarWidthAccordionOn : SideBarWidthAccordionOff;
		return Rect(SideBarPosX, SideBarPosY, width, Scene::Height());
	}

	constexpr const char32_t* SideBarFileTextureKey = U"ファイル";
	constexpr const char32_t* SideBarFileText = U"ファイル";

	constexpr const char32_t* SideBarFontKey = U"side_bar_font";
	constexpr int32 SideBarFontSize = 18;

}

//===================================================================
// 実装
//===================================================================

#include "DrawObject/DrawObject.h"
class IconLabel
{
public:

	explicit IconLabel(const Texture& icon, int32 iconSize, const String& label, const String& fontKey);
	virtual ~IconLabel();

	void draw(bool isDrawLabel, double x, double y, const ColorF& fontColor) const;

	const RectF region() const;

private:

	Texture m_icon;
	int32 m_iconSize;
	DrawableText m_text;
	int32 m_padding;
	double m_offsetY;
};

IconLabel::IconLabel(const Texture& icon, int32 iconSize, const String& label, const String& fontKey)
	: m_icon{ icon }
	, m_iconSize{ iconSize }
	, m_text{ FontAsset(fontKey)(label) }
	, m_padding{ 16 }
	, m_offsetY{ 0.0 }
{
	m_offsetY = (m_iconSize - m_text.region().h) * 0.5;
}

IconLabel::~IconLabel()
{
}

void IconLabel::draw(bool isDrawLabel, double x, double y, const ColorF& fontColor) const
{
	m_icon.draw(x + m_padding, y + m_padding);
	if (isDrawLabel)
	{
		m_text.draw(x + m_padding * 2 + m_iconSize, y + m_padding + m_offsetY, fontColor);
	}
}

const RectF IconLabel::region() const
{
	const auto& size = m_text.region().size;
	const double width = size.x + m_padding * 3.0 + m_iconSize;
	if (m_offsetY < 0)
	{
		return RectF{ 0, m_offsetY, width, size.y };
	}
	return RectF{ 0, 0, width, m_padding * 2.0 + m_iconSize };
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

	TextureAsset::Register(SideBarFileTextureKey, 0xF024B_icon, SideBarIconSize);
	FontAsset::Register(SideBarFontKey, FontMethod::MSDF, SideBarFontSize);
	IconLabel fileIconLabel(TextureAsset(SideBarFileTextureKey), SideBarIconSize, SideBarFileText, SideBarFontKey);

	bool isSideBarOpen = false;
	while (System::Update())
	{
		const auto& sideBarRect = GetSideBarRect(SideBarAccordionOpen(isSideBarOpen));
		isSideBarOpen = sideBarRect.mouseOver();

		sideBarRect.draw(Palette::Darkblue);
		fileIconLabel.draw(isSideBarOpen, 0, 0, Palette::White);
		drawManager.draw();
	}
}

// ==================================================================
// 分割
// ==================================================================
