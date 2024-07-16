#include "DxLib.h"
#include "main_loop.h"
#include "config_data.h"
#include "enhance_cpp_programing.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigData::ConfigDatas* datas = new ConfigData::ConfigDatas;
	MainLoop::Main_Loop* main_loop = new MainLoop::Main_Loop;

	datas->SetScreenSize(600, 500);

	auto screen_size = datas->GetScreenSize();

	ChangeWindowMode(true);

	SetGraphMode(screen_size.first, screen_size.second, 32);

	if (DxLib_Init() Is -1)	// DXLib‚Ì‰Šú‰»
	{
		return -1;
	}
	else
	{
		main_loop->MainLoopEntry();
	}

	while (ProcessMessage() Is 0)
	{
		ClearDrawScreen();

		DrawString(0, screen_size.second - 20, "Hit ESC To Exit", GetColor(255, 255, 255));

		if (CheckHitKey(KEY_INPUT_ESCAPE) Is 1)
		{
			main_loop->MainLoopExit();

			DxLib_End();
		}
		else
		{
			main_loop->MainLoopUpdate();
		}
	}

	return 0;
}
