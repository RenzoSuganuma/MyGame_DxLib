#include "DxLib.h"
#include "main_loop.h"
#include "config_data.h"
#include "enhance_cpp_programing.h"

/* �����ł̓Q�[���{�̂̃R�[�f�B���O�����Ȃ��B�����܂ł��V�X�e���ʁy�G���W�������z�̃R�[�h�̂ݏ������� */

// �G���g���[�|�C���g���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigData::ConfigDatas* datas = new ConfigData::ConfigDatas;
	MainLoop::Main_Loop* main_loop = new MainLoop::Main_Loop;

	datas->SetScreenSize(1920, 1080);

	auto screen_size = datas->GetScreenSize();

	datas->SetChangeWindowMode(false);
	ChangeWindowMode(datas->GetChangeWindowMode());

	if (DxLib_Init() IS(-1))	// DXLib�̏�����
	{
		return -1;
	}
	else
	{
		main_loop->MainLoopEntry();
	}

	while (ProcessMessage() IS 0)
	{
		ClearDrawScreen();

		DrawFormatStringF(0, screen_size.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit");

		datas->SetQuitKey(KEY_INPUT_ESCAPE);

		if (CheckHitKey(datas->GetQuitKey()) IS 1)
		{
			main_loop->MainLoopExit();

			DxLib_End();
		}
		else
		{
			main_loop->MainLoopUpdate();
		}
	}

	DxLib_End();

	return 0;
}
