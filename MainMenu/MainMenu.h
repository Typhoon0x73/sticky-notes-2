#pragma once
#include <Siv3D.hpp>
#include "DrawObject/DrawObject.h"

namespace noco
{
	class Canvas;
	class Node;
}

namespace stn
{
	class MainMenu : public DrawObject
	{
		using super = DrawObject;
	public:

		MainMenu(Layer layer);
		virtual ~MainMenu();

		virtual void update();
		virtual void draw() const override;

	private:

		std::shared_ptr<noco::Canvas> m_pCanvas;
	};
}
