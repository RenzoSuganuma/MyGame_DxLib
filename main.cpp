#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Runtime\Actor.h"
#include "SarissaEngine\Runtime\Component.h"
#include "SarissaEngine\Runtime\MainLoop.h"
#include "SarissaEngine\Engine\ConfigData.h"

#pragma comment (lib , "winmm.lib")


/* 【Layer:0】 */

/* ゲームエンジン【ライブラリ】 レイヤ */

/* ここではゲーム本体のコーディングをしない。あくまでもシステム面【エンジン内部】のコードのみ書くこと */

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// System:
	// 構成データ
	ConfigData* datas = new ConfigData;
	// １フレーム当たりの時間
	DWORD frameTime = 1000 / datas->GetRefreshRate();
	// 現在時間
	DWORD currentTime = 0;
	// 過去時間
	DWORD prevTime = 0;
	// システム時間の制度を1msに変更
	timeBeginPeriod(1);
	// 現在時間を過去時間に設定
	prevTime = timeGetTime();


	// Initialize:


	MainLoop* main_loop = new MainLoop;
	int frame_count = 0;


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

		// System:
		currentTime = timeGetTime();	// 現在時間の取得

		frame_count++;

		if (currentTime - prevTime >= frameTime)	// フレーム時間経過したら
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

