#pragma once

#include "list"
#include "string"

namespace SarissaEngine::Runtime::System
{

	extern enum class SoundPlayMode
	{
		Normal,
		BackGround,
		Loop
	};

	extern std::list< int > soundHandlers;

	extern const int LoadSoundMemory(const std::string filePath);

	extern void PlaySoundMemory(const int soundHandler, const SoundPlayMode soundPlayMode);

}

