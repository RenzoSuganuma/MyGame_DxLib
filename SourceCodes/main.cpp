#include "DxLib.h"
#include "main_loop.h"
#include "config_data.h"
#include "enhance_cpp_programing.h"

/* ここではゲーム本体のコーディングをしない。あくまでもシステム面【エンジン内部】のコードのみ書くこと */

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigData::ConfigDatas* datas = new ConfigData::ConfigDatas;
	MainLoop::Main_Loop* main_loop = new MainLoop::Main_Loop;

	datas->SetScreenSize(1920, 1080);

	auto screen_size = datas->GetScreenSize();

	datas->SetChangeWindowMode(false);
	ChangeWindowMode(datas->GetChangeWindowMode());

	if (DxLib_Init() IS(-1))	// DXLibの初期化
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
