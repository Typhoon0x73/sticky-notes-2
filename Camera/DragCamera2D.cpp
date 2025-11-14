#include "DragCamera2D.h"

namespace stn
{

	constexpr DragCamera2D::DragCamera2D(const Vec2& center, double scale) noexcept
		: super{ center, scale }
	{
	}

	DragCamera2D::~DragCamera2D()
	{
	}

	void DragCamera2D::update(bool isMoveMouseL, double deltaTime)
	{
		updateWheel();
		updateMouse(isMoveMouseL);

		{
			const double oldScale = m_scale;
			m_scale = Math::SmoothDamp(m_scale, m_targetScale, m_scaleChangeVelocity, 0.2, unspecified, deltaTime);

			if (deltaTime && (m_scale != m_targetScale) && (m_scale == oldScale))
			{
				m_scale = m_targetScale;
			}
		}
	}

	void DragCamera2D::jumpTo(Vec2 center, double scale) noexcept
	{
		m_center = center;
		m_targetScale = m_scale = scale;
		m_scaleChangeVelocity = 0.0;
	}

	void DragCamera2D::updateWheel()
	{
		const double wheel = Mouse::Wheel();

		if (wheel == 0.0)
		{
			return;
		}

		if (wheel < 0.0)
		{
			m_targetScale *= 1.125;
		}
		else
		{
			m_targetScale /= 1.125;
		}

		m_targetScale = Clamp(m_targetScale, (1.0 / 5.0), 5.0);
	}

	void DragCamera2D::updateMouse(bool isMoveMouseL)
	{
		const auto t1 = Transformer2D{ Mat3x2::Identity(), TransformCursor::Yes, Transformer2D::Target::SetLocal };
		const auto t2 = Transformer2D{ Mat3x2::Identity(), TransformCursor::Yes, Transformer2D::Target::SetCamera };

		if ((isMoveMouseL && MouseL.pressed()) || MouseR.pressed())
		{
			Cursor::RequestStyle(U"grab");
			m_center -= Cursor::DeltaF() * (1.0 / m_scale);
		}
	}

}
