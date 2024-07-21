#include "ConfigData.h"

using SarissaEngine::Runtime::System::ConfigData;

const std::pair< unsigned int, unsigned int >
const ConfigData::GetFHDScreenSize() const
{
	return std::make_pair(1920, 1080);
}

ConfigData::ConfigData() {}
ConfigData::~ConfigData() {}

ConfigData::ConfigData(unsigned int width, unsigned int height)
	: windowWidth_(width), windowHeigth_(height)
{}

void const ConfigData::SetScreenSize(unsigned int width, unsigned int height)
{
	windowWidth_ = width;
	windowHeigth_ = height;

	SetGraphMode(windowWidth_, windowHeigth_, 32);
}

std::pair< unsigned int, unsigned int >
const ConfigData::GetScreenSize() const
{
	return std::make_pair(windowWidth_, windowHeigth_);
}

void const ConfigData::SetChangeWindowMode(bool cond)
{
	changeWindowMode_ = cond;
	ChangeWindowMode(changeWindowMode_);
}

const bool const ConfigData::GetChangeWindowMode() const
{
	return changeWindowMode_;
}

const unsigned int ConfigData::GetRefreshRate() const
{
	return refreshRate_;
}

void const ConfigData::SetRefreshRate(unsigned int rate)
{
	refreshRate_ = rate;

	SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
}

const int const ConfigData::GetQuitKey() const
{
	return quitKey_;
}

void const ConfigData::SetQuitKey(int keyCode)
{
	quitKey_ = keyCode;
}

const DWORD const ConfigData::GetCurrentSystemTime() const
{
	return	timeGetTime();
}
