#pragma once

#include "iostream"

namespace ConfigData
{
	class ConfigDatas final
	{
	private:
		unsigned int WindowWidth_ = 1920;
		unsigned int WindowHeigth_ = 1080;
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
			: WindowWidth_(width), WindowHeigth_(height)
		{}

		void const SetScreenSize(unsigned int width, unsigned int height)
		{
			WindowWidth_ = width;
			WindowHeigth_ = height;
		}

		std::pair< unsigned int, unsigned int >
			const GetScreenSize() const
		{
			return std::make_pair(WindowWidth_, WindowHeigth_);
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
	};
}
