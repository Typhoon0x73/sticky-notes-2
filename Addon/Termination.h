#pragma once

#include <Siv3D.hpp>

namespace stn
{
	class Termination : public IAddon
	{
	public:

		Termination();
		virtual ~Termination();

	private:

		void saveConfig();

	};

}
