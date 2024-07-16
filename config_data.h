#pragma once

#include "iostream"

namespace ConfigData
{
	class ConfigDatas final
	{
	private:
		unsigned int windowWidth_ = 1920;
		unsigned int windowHeigth_ = 1080;
		unsigned int refreshRate_ = 60;
		bool changeWindowMode_ = true;

	public:
		const std::pair< unsigned int, unsigned int >
			const GetFHDScreenSize() const
		{
			return std::make_pair(1920, 1080);
		}

		ConfigDatas() {}
		~ConfigDatas() {}

		ConfigDatas(unsigned int width, unsigned int height)
			: windowWidth_(width), windowHeigth_(height)
		{}

		void const SetScreenSize(unsigned int width, unsigned int height)
		{
			windowWidth_ = width;
			windowHeigth_ = height;

			SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
		}

		std::pair< unsigned int, unsigned int >
			const GetScreenSize() const
		{
			return std::make_pair(windowWidth_, windowHeigth_);
		}

		void const SetChangeWindowMode(bool cond)
		{
			changeWindowMode_ = cond;
			ChangeWindowMode(changeWindowMode_);
		}

		const bool const GetChangeWindowMode() const
		{
			return changeWindowMode_;
		}

		const unsigned int GetRefreshRate() const
		{
			return refreshRate_;
		}

		void const SetRefreshRate(unsigned int rate)
		{
			refreshRate_ = rate;

			SetGraphMode(windowWidth_, windowHeigth_, 32, refreshRate_);
		}
	};
}
