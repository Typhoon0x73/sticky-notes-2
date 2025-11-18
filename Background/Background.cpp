#include "Background.h"
#include "../ServiceLocator/SingletonLocator.h"

namespace stn
{

	Background::Background()
		: super{ Layer::Background }
	{
	}

	Background::~Background()
	{
	}

	void Background::draw() const
	{
		const auto* pCamera = STN_GET_LOCATOR(BasicCamera2D).get();
		if (pCamera == nullptr)
		{
			return;
		}
		int32 space = (pCamera->getScale() < 0.3 ? 100 : 50);
		const auto& region = pCamera->getRegion();
		const double offsetX = region.x - static_cast<int32>(region.x) % space;
		const double offsetY = region.y - static_cast<int32>(region.y) % space;
		for (double y = offsetY; y < region.bottomY(); y += space)
		{
			for (double x = offsetX; x < region.rightX(); x += space)
			{
				Circle{ x, y, 3 }.draw(Palette::Darkgray);
			}
		};
	}

}

