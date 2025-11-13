#include "DragEditRect.h"

namespace
{
	enum DragFrameDir : int32
	{
		DragFrameDir_None = 0,
		DragFrameDir_Left = 1 << 0,
		DragFrameDir_Top = 1 << 1,
		DragFrameDir_Right = 1 << 2,
		DragFrameDir_Bottom = 1 << 3,
		DragFrameDir_TopLeft = DragFrameDir_Top | DragFrameDir_Left,
		DragFrameDir_TopRight = DragFrameDir_Top | DragFrameDir_Right,
		DragFrameDir_BottomLeft = DragFrameDir_Bottom | DragFrameDir_Left,
		DragFrameDir_BottomRight = DragFrameDir_Bottom | DragFrameDir_Right,
	};
}

namespace stn
{
	DragEditRect::UpdateResult DragEditRect::update()
	{
		const int32 dir = mouseOverDir();
		const bool isDraggingFrame = isDragFrame();
		const auto& cursorStyle = getCursorStyle(isDraggingFrame ? m_dragFrameDir : dir);
		Cursor::RequestStyle(cursorStyle);

		if (MouseL.down())
		{
			m_dragFrameDir = dir;
		}
		if (MouseL.up())
		{
			m_dragFrameDir = DragFrameDir_None;
			return UpdateResult::Release;
		}
		if (isDraggingFrame)
		{
			if (dragResize())
			{
				return UpdateResult::Resize;
			}
		}
		else if (mouseOver() && MouseL.pressed())
		{
			if (dragMove())
			{
				return UpdateResult::Move;
			}
		}
		return UpdateResult::None;
	}

	bool DragEditRect::isDragFrame() const
	{
		return m_dragFrameDir != DragFrameDir_None;
	}

	int32 DragEditRect::mouseOverDir() const
	{
		const double cornerHitRadius = 1.75;
		const double lineHitThickness = 2.0;
		const Circle cornerTopLeft{ tl(), cornerHitRadius };
		const Circle cornerTopRight{ tr(), cornerHitRadius };
		const Circle cornerBottomLeft{ bl(), cornerHitRadius };
		const Circle cornerBottomRight{ br(), cornerHitRadius };
		const auto& left = RectF::left().withThickness(lineHitThickness);
		const auto& right = RectF::right().withThickness(lineHitThickness);
		const auto& top = RectF::top().withThickness(lineHitThickness);
		const auto& bottom = RectF::bottom().withThickness(lineHitThickness);
		if (cornerTopLeft.mouseOver()) { return DragFrameDir_TopLeft; }
		else if (cornerTopRight.mouseOver()) { return DragFrameDir_TopRight; }
		else if (cornerBottomLeft.mouseOver()) { return DragFrameDir_BottomLeft; }
		else if (cornerBottomRight.mouseOver()) { return DragFrameDir_BottomRight; }
		else if (left.mouseOver()) { return DragFrameDir_Left; }
		else if (right.mouseOver()) { return DragFrameDir_Right; }
		else if (top.mouseOver()) { return DragFrameDir_Top; }
		else if (bottom.mouseOver()) { return DragFrameDir_Bottom; }
		return DragFrameDir_None;
	}

	CursorStyle DragEditRect::getCursorStyle(int32 dir) const
	{
		if (dir == DragFrameDir_TopLeft
			|| dir == DragFrameDir_BottomRight)
		{
			return CursorStyle::ResizeNWSE;
		}
		else if (dir == DragFrameDir_TopRight
			|| dir == DragFrameDir_BottomLeft)
		{
			return CursorStyle::ResizeNESW;
		}
		else if (dir == DragFrameDir_Left
			|| dir == DragFrameDir_Right)
		{
			return CursorStyle::ResizeLeftRight;
		}
		else if (dir == DragFrameDir_Top
			|| dir == DragFrameDir_Bottom)
		{
			return CursorStyle::ResizeUpDown;
		}
		return CursorStyle::Default;
	}

	bool DragEditRect::dragResize()
	{
		bool isResize = false;
		const auto& move = Cursor::DeltaF();
		if (m_dragFrameDir & DragFrameDir_Left)
		{
			x += move.x;
			w -= move.x;
			isResize = true;
		}
		if (m_dragFrameDir & DragFrameDir_Right)
		{
			w += move.x;
			isResize = true;
		}
		if (m_dragFrameDir & DragFrameDir_Top)
		{
			y += move.y;
			h -= move.y;
			isResize = true;
		}
		if (m_dragFrameDir & DragFrameDir_Bottom)
		{
			h += move.y;
			isResize = true;
		}
		return isResize;
	}

	bool DragEditRect::dragMove()
	{
		const auto& move = Cursor::DeltaF();
		moveBy(move);
		return (Vec2::Zero() != move);
	}

}
