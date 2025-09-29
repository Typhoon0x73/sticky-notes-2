#pragma once

#include <Siv3D.hpp>

namespace stn
{
	/// @brief アプリケーションの設定
	class Config
	{
	public:

		Config();
		virtual ~Config();

		void setData(const JSON& data);
		JSON toJson() const;

	private:
	};

}
