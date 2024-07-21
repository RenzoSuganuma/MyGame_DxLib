#include "chrono"
#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Runtime\MainLoop.h"
#include "SarissaEngine\Engine\ConfigData.h"
#include "SarissaEngine\Runtime\SarissaEngine_RuntimeClasses.h"

#pragma comment (lib , "winmm.lib")


/* 【Layer:0】 */

/* ゲームエンジン【ライブラリ】 レイヤ */

/* ここではゲーム本体のコーディングをしない。あくまでもシステム面【エンジン内部】のコードのみ書くこと */

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	using SarissaEngine::Runtime::System::ConfigData;

	// System:
	// 構成データ
	ConfigData* datas = new ConfigData;

	// フレームレート制限のための変数群
	// １フレーム当たりの時間[ms]
	float frameTime = (1000.0f / (datas->GetRefreshRate()));
	// 過去時間
	std::chrono::system_clock::time_point clockStarted;
	// 現在時間
	std::chrono::system_clock::time_point clockEnded;
	// δ秒
	float deltaTime = frameTime / 1000.0f;


	// Initialize:

	using SarissaEngine::Runtime::System::MainLoop;
	MainLoop* mainLoop = new MainLoop;

	// DXLibの初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// ウィンドウのセットアップ
	datas->SetScreenSize(1000, 600);
	auto screenSize = datas->GetScreenSize();
	datas->SetChangeWindowMode(true);
	ChangeWindowMode(datas->GetChangeWindowMode());
	datas->SetRefreshRate(144);
	datas->SetQuitKey(KEY_INPUT_ESCAPE);

	// テスト用のシーン内セットアップ
	auto a = new SarissaEngine::Runtime::Framework::Actor;
	SarissaEngine::Runtime::Framework::Component c;
	a->AddComponent(c);
	mainLoop->AddObject(a);


	// MainLoop:

	int sh = LoadSoundMem(".\\Resources\\se_sound.mp3");

	ClearDrawScreen();
	mainLoop->MainLoopEntry();

	while (1)
	{
		if (ProcessMessage() == -1)
		{
			break;
		}

		// 計測開始
		clockStarted = std::chrono::system_clock::now();

		ClearDrawScreen();
		DrawFormatStringF(0, screenSize.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit");

		mainLoop->MainLoopUpdate(deltaTime);

		if (CheckHitKey(datas->GetQuitKey()) == 1)
		{
			mainLoop->MainLoopExit();

			DxLib_End();

			break;
		}

		ScreenFlip();

		clockEnded = std::chrono::system_clock::now();
		double delta_mil_sec = (static_cast<double>
			(std::chrono::duration_cast<std::chrono::microseconds>
				(clockEnded - clockStarted).count())) / 1000.0;
		deltaTime = delta_mil_sec / 100.0f;
		if (frameTime > delta_mil_sec)
		{
			timeBeginPeriod(1);
			Sleep((DWORD)(frameTime - delta_mil_sec));
			timeEndPeriod(1);
		}
	}

	return 0;

}

