#include "SrssEngn_WindowHandler.hpp"
#include "Windows.h"
#include "DxLib.h"

namespace  SarissaEngine::Runtime::System
{

	unsigned int windowWidth_ = 1920;
	unsigned int windowHeigth_ = 1080;
	unsigned int refreshRate_ = 60;
	bool changeWindowMode_ = true;
	int quitKey_ = KEY_INPUT_ESCAPE;

	void const SetScreenSize(unsigned int width, unsigned int height)
	{
		windowWidth_ = width;
		windowHeigth_ = height;

		SetGraphMode(windowWidth_, windowHeigth_, 32);
	}

	std::pair< unsigned int, unsigned int >
		const GetScreenSize()
	{
		return std::make_pair(windowWidth_, windowHeigth_);
	}

	void const SetChangeWindowMode(bool cond)
	{
		changeWindowMode_ = cond;
		ChangeWindowMode(changeWindowMode_);
	}

	const bool const GetChangeWindowMode()
	{
		return changeWindowMode_;
	}

	void const SetRefreshRate(unsigned int rate)
	{
		refreshRate_ = rate;

		SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
	}

	const int const GetQuitKey()
	{
		return quitKey_;
	}

	void const SetQuitKey(int keyCode)
	{
		quitKey_ = keyCode;
	}

}

