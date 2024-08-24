#pragma once

#include "list"
#include "string"

namespace SarissaEngine::Runtime::System
{

	// �T�E���h�Đ����[�h
	extern enum class SoundPlayMode
	{
		Normal,
		BackGround,
		Loop
	};

	extern std::list< int > soundHandlers;

	extern const int LoadSoundToMemory(const std::string filePath);

	extern void PlaySoundFromMemory(const int soundHandler, const SoundPlayMode soundPlayMode);

}

