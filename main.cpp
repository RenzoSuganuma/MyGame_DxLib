#include "chrono"
#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Runtime\MainLoop.h"
#include "SarissaEngine\Engine\ConfigData.h"
#include "SarissaEngine\Runtime\SarissaEngine_RuntimeClasses.h"

#pragma comment (lib , "winmm.lib")


/* �yLayer:0�z */

/* �Q�[���G���W���y���C�u�����z ���C�� */

/* �����ł̓Q�[���{�̂̃R�[�f�B���O�����Ȃ��B�����܂ł��V�X�e���ʁy�G���W�������z�̃R�[�h�̂ݏ������� */

// �G���g���[�|�C���g���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	using SarissaEngine::Runtime::System::ConfigData;

	// System:
	// �\���f�[�^
	ConfigData* datas = new ConfigData;
	// �P�t���[��������̎���[ms]
	float frameTime = (1000.0f / (datas->GetRefreshRate() + 0.5f));
	// �ߋ�����
	std::chrono::system_clock::time_point clockStarted;
	// ���ݎ���
	std::chrono::system_clock::time_point clockEnded;
	// �b
	float deltaTime = frameTime / 1000.0f;
	// �o�ߎ���
	float elapsedTime = 0;


	// Initialize:

	using SarissaEngine::Runtime::System::MainLoop;
	MainLoop* main_loop = new MainLoop;

	int frame_count = 0;

	// DXLib�̏�����
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// �E�B���h�E�̃Z�b�g�A�b�v
	datas->SetScreenSize(1000, 600);
	auto screen_size = datas->GetScreenSize();
	datas->SetChangeWindowMode(true);
	ChangeWindowMode(datas->GetChangeWindowMode());
	datas->SetQuitKey(KEY_INPUT_ESCAPE);

	// �e�X�g�p�̃V�[�����Z�b�g�A�b�v
	auto a = new SarissaEngine::Runtime::Framework::Actor;
	SarissaEngine::Runtime::Framework::Component c;
	a->AddComponent(c);
	main_loop->AddObject(a);


	// MainLoop:

	int sh = LoadSoundMem(".\\Resources\\se_sound.mp3");

	ClearDrawScreen();
	main_loop->MainLoopEntry();

	while (1)
	{
		if (ProcessMessage() == -1)
		{
			break;
		}

		// �v���J�n
		clockStarted = std::chrono::system_clock::now();

		ClearDrawScreen();
		DrawFormatStringF(0, screen_size.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit  :  FrameCount %2d : Elapsed = %f", frame_count, elapsedTime);

		main_loop->MainLoopUpdate(deltaTime);

		frame_count++;
		elapsedTime += deltaTime;

		if (frame_count >= datas->GetRefreshRate())
		{
			frame_count = 0;
		}

		if (CheckHitKey(datas->GetQuitKey()) == 1)
		{
			main_loop->MainLoopExit();

			DxLib_End();

			break;
		}

		ScreenFlip();

		clockEnded = std::chrono::system_clock::now();
		double mic_sec = static_cast<double>
			(std::chrono::duration_cast<std::chrono::microseconds>
				(clockEnded - clockStarted).count());
		float mil_sec = static_cast<float>(mic_sec / 1000.0);
		deltaTime = mil_sec / 100.0f;
		if (frameTime > mil_sec)
		{
			timeBeginPeriod(1);
			Sleep(DWORD(frameTime - mil_sec));
			timeEndPeriod(1);
		}
	}

	return 0;

}

