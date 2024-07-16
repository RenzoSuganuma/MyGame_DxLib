#include "config_data.h"

using ConfigData::ConfigDatas;

const std::pair< unsigned int, unsigned int >
const ConfigDatas::GetFHDScreenSize() const
{
	return std::make_pair(1920, 1080);
}

ConfigDatas::ConfigDatas() {}
ConfigDatas::~ConfigDatas() {}

ConfigDatas::ConfigDatas(unsigned int width, unsigned int height)
	: windowWidth_(width), windowHeigth_(height)
{}

void const ConfigDatas::SetScreenSize(unsigned int width, unsigned int height)
{
	windowWidth_ = width;
	windowHeigth_ = height;

	SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
}

std::pair< unsigned int, unsigned int >
const ConfigDatas::GetScreenSize() const
{
	return std::make_pair(windowWidth_, windowHeigth_);
}

void const ConfigDatas::SetChangeWindowMode(bool cond)
{
	changeWindowMode_ = cond;
	ChangeWindowMode(changeWindowMode_);
}

const bool const ConfigDatas::GetChangeWindowMode() const
{
	return changeWindowMode_;
}

const unsigned int ConfigDatas::GetRefreshRate() const
{
	return refreshRate_;
}

void const ConfigDatas::SetRefreshRate(unsigned int rate)
{
	refreshRate_ = rate;

	SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
}

const int const ConfigDatas::GetQuitKey() const
{
	return quitKey_;
}

void const ConfigDatas::SetQuitKey(int keyCode)
{
	quitKey_ = keyCode;
}
