#include "DxLib.h"
#include "SrssEngn_SoundHandler.hpp"

namespace SarissaEngine::Runtime::System
{

	std::list< int > soundHandlers;

	const int LoadSoundToMemory(const std::string filePath)
	{
		int sh = LoadSoundMem(filePath.c_str());
		soundHandlers.emplace_back(sh);
		return sh;
	}

	void PlaySoundFromMemory(const int soundHandler, const SoundPlayMode soundPlayMode)
	{
		int mode;
		switch (soundPlayMode)
		{
		case SoundPlayMode::Normal:
			mode = DX_PLAYTYPE_NORMAL;
			break;
		case SoundPlayMode::BackGround:
			mode = DX_PLAYTYPE_BACK;
			break;
		case SoundPlayMode::Loop:
			mode = DX_PLAYTYPE_LOOP;
			break;
		default:
			break;
		}
		PlaySoundMem(soundHandler, mode);
	}

}
