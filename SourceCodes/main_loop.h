#pragma once

#include "DxLib.h"

namespace MainLoop
{
	class Main_Loop final
	{
	public:
		Main_Loop();
		~Main_Loop();

		void MainLoopEntry();

		void MainLoopUpdate();

		void MainLoopExit();
	};
}
