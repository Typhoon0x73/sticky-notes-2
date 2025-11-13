#pragma once
#include <Siv3D.hpp>

namespace stn
{
	class DragEditRect : public RectF
	{
	public:

		enum class UpdateResult
		{
			None,
			Move,
			Resize,
			Release,
		};

		using RectF::RectF;
		virtual ~DragEditRect() = default;

		
		UpdateResult update();

	private:

		int32 m_dragFrameDir{ 0 };

		bool isDragFrame() const;
		int32 mouseOverDir() const;
		CursorStyle getCursorStyle(int32 dir) const;

		bool dragResize();
		bool dragMove();
	};
}
