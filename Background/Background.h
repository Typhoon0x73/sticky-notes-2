#pragma once
#include "../DrawObject/DrawObject.h"

namespace stn
{
	class Background : public DrawObject
	{
		using super = DrawObject;
	public:

		Background();
		virtual ~Background();

		virtual void draw() const override;
	};
}
