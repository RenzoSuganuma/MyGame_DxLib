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
	float frameTime = 0;
	// 過去時間
	std::chrono::system_clock::time_point clockStarted;
	// 現在時間
	std::chrono::system_clock::time_point clockEnded;
	// δ秒
	float deltaTime = frameTime / 1000.0f;
	// 経過時間
	float elapsedTime = 0;

	// Initialize:

	using SarissaEngine::Runtime::System::Level;
	Level* mainLoop = new Level;

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
	datas->SetRefreshRate(60);
	frameTime = (1000.0f / (datas->GetRefreshRate()) + 0.5f);
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
		DrawFormatStringF(0, screenSize.second - 20, GetColor(255, 255, 255), "Hit Escape Key To Exit : %f", elapsedTime);

		mainLoop->MainLoopUpdate(deltaTime);
		elapsedTime += deltaTime;

		if (CheckHitKey(datas->GetQuitKey()) == 1)
		{
			mainLoop->MainLoopExit();

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

