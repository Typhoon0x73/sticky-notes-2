#include "DoubleClick.h"

namespace
{
	enum DoubleClickStep
	{
		None,
		SingleClicked,
		DoubleClickWait,
		DoubleClicked,
	};
}

namespace stn
{

	void DoubleClick::update()
	{
		if (m_step == DoubleClickStep::DoubleClicked)
		{
			m_step = DoubleClickStep::None;
		}

		if (MouseL.down())
		{
			if (m_step == DoubleClickStep::None)
			{
				m_step = DoubleClickStep::SingleClicked;
			}
			else if (m_step == DoubleClickStep::DoubleClickWait)
			{
				if (const uint64 d = (Time::GetMillisec() - m_previousTimeMilliSec);
					d < DoubleClickThresholdMilliSec)
				{
					m_step = DoubleClickStep::DoubleClicked;
				}
				else
				{
					m_step = DoubleClickStep::SingleClicked;
				}
			}
		}

		if (m_step == DoubleClickStep::None)
		{
			return;
		}

		if (not Cursor::Delta().isZero())
		{
			m_step = DoubleClickStep::None;
		}

		if ((m_step == DoubleClickStep::SingleClicked) && MouseL.up())
		{
			if (MouseL.pressedDuration() < Milliseconds{ MaxClickTimeMilliSec })
			{
				m_step = DoubleClickStep::DoubleClickWait;
				m_previousTimeMilliSec = Time::GetMillisec();
			}
			else
			{
				m_step = DoubleClickStep::None;
			}
		}
	}

	bool DoubleClick::doubleClicked() const noexcept
	{
		return (m_step == DoubleClickStep::DoubleClicked);
	}

}
