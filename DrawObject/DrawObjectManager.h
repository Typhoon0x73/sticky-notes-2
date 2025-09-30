#pragma once

// INCLUDE
#include "LayerDefine.h"
#include <Siv3D.hpp>

namespace stn
{

	class DrawObject;
	class DrawObjectManager
	{
	public:
		DrawObjectManager();
		~DrawObjectManager();

		void addObject(Layer layer, DrawObject* obj);
		void setRootScale(double x, double y);
		void setRootScale(double x, double y, const Array<Layer>& layer);

		void draw() const;

		void print() const;

	private:

		void drawObject(const DrawObject* obj) const;

		std::unique_ptr<DrawObject> m_pRootObjects[FromEnum(Layer::Count)];
	};
}
