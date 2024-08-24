#include "chrono"
#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Engine\SrssEngn_SoundHandler.hpp"
#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_InputHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_Level.hpp"
#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"
#include "SarissaEngine\Runtime\SrssEngn_Component.hpp"
#include "SarissaEngine\Runtime\SrssEngn_InputHandler.hpp"

#include "Player.h"
#include "MovingCircle.h"

#pragma comment (lib , "winmm.lib")

/* �yLayer:0�z */

/* �Q�[���G���W���y���C�u�����z ���C�� */

/* �����ł̓Q�[���{�̂̃R�[�f�B���O�����Ȃ��B�����܂ł��V�X�e���ʁy�G���W�������z�̃R�[�h�̂ݏ������� */

// �G���g���[�|�C���g���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace SarissaEngine::Runtime::System;

	// �t���[�����[�g�����̂��߂̕ϐ��Q
	// �P�t���[��������̎���[ms]
	float frameTime = 0;
	// �ߋ�����
	std::chrono::system_clock::time_point clockStarted;
	// ���ݎ���
	std::chrono::system_clock::time_point clockEnded;
	// �b
	float deltaTime = frameTime / 1000.0f;
	// �o�ߎ���
	float elapsedTime = 0;

	// Initialize:

	Level* level = new Level;

	// DXLib�̏�����
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// �E�B���h�E�̃Z�b�g�A�b�v
	SetScreenSize(1000, 800);
	auto screenSize = GetScreenSize();
	SetChangeWindowMode(true);
	ChangeWindowMode(GetChangeWindowMode());
	SetRefreshRate(60);
	frameTime = (1000.0f / (GetRefreshRate()) + 0.5f);
	SetQuitKey(KEY_INPUT_ESCAPE);

	// �e�X�g�p�̃V�[�����Z�b�g�A�b�v
	auto player = new Player;
	player->SetPlacedLevel(level);

	InputHandler* input = new InputHandler;
	player->AddComponent(input);

	level->AddObject(player);


	// MainLoop:

	// ���ʉ��ǂݍ���
	int se = LoadSoundToMemory(".\\Resources\\pigeon_se_.mp3");

	ClearDrawScreen();
	level->MainLoopEntry();

	while (1)
	{
		if (ProcessMessage() == -1)
		{
			break;
		}

		// �v���J�n
		clockStarted = std::chrono::system_clock::now();

		ClearDrawScreen();
		DrawFormatStringF(0, screenSize.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit : ElapsedTime = %f", elapsedTime);

		level->MainLoopUpdate(deltaTime);
		elapsedTime += deltaTime;

		if (CheckHitKey(GetQuitKey()) == 1)
		{
			level->MainLoopExit();

			delete level;

			DxLib_End();

			break;
		}

		ScreenFlip();

		clockEnded = std::chrono::system_clock::now();
		double deltaMilSec = (static_cast<double>
			(std::chrono::duration_cast<std::chrono::microseconds>
				(clockEnded - clockStarted).count()));
		deltaTime = deltaMilSec / 1000.0 / 1000.0;
		if (frameTime > deltaMilSec)
		{
			timeBeginPeriod(1);
			Sleep((DWORD)(frameTime - deltaMilSec));
			timeEndPeriod(1);
		}
	}

	return 0;

}

