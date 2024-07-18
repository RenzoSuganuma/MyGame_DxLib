#pragma once

#include "DxLib.h"
#include "iostream"

namespace ConfigData
{
	// システム周りのデータ管理クラス
	class ConfigDatas final
	{
	private:
		unsigned int windowWidth_ = 1920;
		unsigned int windowHeigth_ = 1080;
		unsigned int refreshRate_ = 60;
		bool changeWindowMode_ = true;
		int quitKey_ = KEY_INPUT_ESCAPE;

	public:
		const std::pair< unsigned int, unsigned int >
			const GetFHDScreenSize() const;

		ConfigDatas();
		~ConfigDatas();

		ConfigDatas(unsigned int width, unsigned int height);

		void const SetScreenSize(unsigned int width, unsigned int height);

		std::pair< unsigned int, unsigned int >
			const GetScreenSize() const;

		void const SetChangeWindowMode(bool cond);

		const bool const GetChangeWindowMode() const;

		const unsigned int GetRefreshRate() const;

		void const SetRefreshRate(unsigned int rate);

		const int const GetQuitKey() const;

		void const SetQuitKey(const int keyCode);
	};
}
