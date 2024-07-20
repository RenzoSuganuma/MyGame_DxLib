#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Runtime\Actor.h"
#include "SarissaEngine\Runtime\Component.h"
#include "SarissaEngine\Runtime\MainLoop.h"
#include "SarissaEngine\Engine\ConfigData.h"

#pragma comment (lib , "winmm.lib")


/* �yLayer:0�z */

/* �Q�[���G���W���y���C�u�����z ���C�� */

/* �����ł̓Q�[���{�̂̃R�[�f�B���O�����Ȃ��B�����܂ł��V�X�e���ʁy�G���W�������z�̃R�[�h�̂ݏ������� */

// �G���g���[�|�C���g���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// System:
	// �\���f�[�^
	ConfigData* datas = new ConfigData;
	// �P�t���[��������̎���
	DWORD frameTime = 1000 / datas->GetRefreshRate();
	// ���ݎ���
	DWORD currentTime = 0;
	// �ߋ�����
	DWORD prevTime = 0;
	// �V�X�e�����Ԃ̐��x��1ms�ɕύX
	timeBeginPeriod(1);
	// ���ݎ��Ԃ��ߋ����Ԃɐݒ�
	prevTime = timeGetTime();


	// Initialize:


	MainLoop* main_loop = new MainLoop;
	int frame_count = 0;


	// DXLib�̏�����
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	datas->SetScreenSize(1920, 1080);

	auto screen_size = datas->GetScreenSize();

	datas->SetChangeWindowMode(false);
	ChangeWindowMode(datas->GetChangeWindowMode());

	datas->SetQuitKey(KEY_INPUT_ESCAPE);

	auto a = new Actor;
	Component c;
	a->AddComponent(c);

	main_loop->AddObject(a);


	// MainLoop:


	ClearDrawScreen();
	main_loop->MainLoopEntry();

	while (ProcessMessage() == 0)
	{

		// System:
		currentTime = timeGetTime();	// ���ݎ��Ԃ̎擾

		frame_count++;

		if (currentTime - prevTime >= frameTime)	// �t���[�����Ԍo�߂�����
		{
			Sleep(1);

			currentTime = timeGetTime();
		}

		prevTime = currentTime;

		ClearDrawScreen();
		DrawFormatStringF(0, screen_size.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit  :  FrameCount %2d", frame_count);

		main_loop->MainLoopUpdate();

		if (frame_count >= 60)
		{
			frame_count = 0;
		}

		if (CheckHitKey(datas->GetQuitKey()) == 1)
		{
			main_loop->MainLoopExit();

			DxLib_End();

			timeEndPeriod(1);

			break;
		}

	}

	return 0;

}

