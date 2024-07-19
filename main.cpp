#include "DxLib.h"
#include "SarissaEngine\Runtime\Actor.h"
#include "SarissaEngine\Runtime\Component.h"
#include "SarissaEngine\Runtime\Main_Loop.h"
#include "SarissaEngine\Engine\Config_Data.h"

/* 【Layer:0】 */

/* ゲームエンジン【ライブラリ】 レイヤ */

/* ここではゲーム本体のコーディングをしない。あくまでもシステム面【エンジン内部】のコードのみ書くこと */

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigData* datas = new ConfigData;
	MainLoop* main_loop = new MainLoop;


// Initialize:


	// DXLibの初期化
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
		ClearDrawScreen();
		DrawFormatStringF(0, screen_size.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit");

		main_loop->MainLoopUpdate();

		if (CheckHitKey(datas->GetQuitKey()) == 1)
		{
			main_loop->MainLoopExit();

			DxLib_End();

			break;
		}
	}

	return 0;
}

