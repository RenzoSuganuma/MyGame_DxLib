#pragma once

#include "DxLib.h"
#include "iostream"
#include "Windows.h"

#pragma comment (lib , "winmm.lib")

namespace SarissaEngine::Runtime::System
{

	// システム周りのデータ管理クラス
	class ConfigData final
	{
	private:
		unsigned int windowWidth_ = 1920;
		unsigned int windowHeigth_ = 1080;
		unsigned int refreshRate_ = 60;
		bool changeWindowMode_ = true;
		int quitKey_ = KEY_INPUT_ESCAPE;
		DWORD currentTime = 0;

	public:
		ConfigData();
		ConfigData(unsigned int width, unsigned int height);
		~ConfigData();

		void
			const
			SetScreenSize(unsigned int width, unsigned int height);

		std::pair< unsigned int, unsigned int >
			const
			GetScreenSize() const;

		void
			const
			SetChangeWindowMode(bool cond);

		const bool
			const
			GetChangeWindowMode() const;

		const unsigned int
			const
			GetRefreshRate() const;

		void
			const
			SetRefreshRate(unsigned int rate);

		const int
			const
			GetQuitKey() const;

		void
			const
			SetQuitKey(const int keyCode);

		const std::pair< unsigned int, unsigned int >
			const
			GetFHDScreenSize() const;

		const DWORD
			const
			GetCurrentSystemTime() const;
	};

}

