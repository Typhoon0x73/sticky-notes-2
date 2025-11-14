#pragma once
#include <Siv3D.hpp>

namespace stn
{

	class DoubleClick
	{
	public:

		DoubleClick() = default;
		virtual ~DoubleClick() = default;

		void update();

		[[nodiscard]]
		bool doubleClicked() const noexcept;

	private:

		// 1 回目のクリックの長さ（ミリ秒）
		static constexpr int32 MaxClickTimeMilliSec = 500;

		// 1 回目のクリックと 2 回目のクリックの最大間隔（ミリ秒）
		static constexpr int32 DoubleClickThresholdMilliSec = 500;

		int32 m_step = 0;

		int64 m_previousTimeMilliSec = 0;
	};
}
