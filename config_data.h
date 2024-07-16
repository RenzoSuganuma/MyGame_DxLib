#pragma once

#include "iostream"

namespace ConfigData
{
	class ConfigDatas
	{
	private:
		unsigned int WindowWidth = 1920;
		unsigned int WindowHeigth = 1080;

	public:
		ConfigDatas() {}
		~ConfigDatas() {}

		ConfigDatas(unsigned int width, unsigned int height)
			: WindowWidth(width), WindowHeigth(height)
		{}

		void SetScreenSize(unsigned int width, unsigned int height)
		{
			WindowWidth = width;
			WindowHeigth = height;
		}

		std::pair< unsigned int, unsigned int >
			GetScreenSize()
		{
			return std::make_pair( WindowWidth , WindowHeigth );
		}
	};
}
