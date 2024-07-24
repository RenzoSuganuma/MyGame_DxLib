#include "chrono"
#include "DxLib.h"
#include "Windows.h"
#include "SarissaEngine\Globals.h"
#include "SarissaEngine\Runtime\Level.h"
#include "SarissaEngine\Runtime\SarissaEngine_RuntimeClasses.h"

#include "Player.h"
#include "InputHandler.h"

#pragma comment (lib , "winmm.lib")

/* 【Layer:0】 */

/* ゲームエンジン【ライブラリ】 レイヤ */

/* ここではゲーム本体のコーディングをしない。あくまでもシステム面【エンジン内部】のコードのみ書くこと */

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// System:

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
	Level* level = new Level;

	// DXLibの初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// ウィンドウのセットアップ
	SetScreenSize(1000, 600);
	auto screenSize = GetScreenSize();
	SetChangeWindowMode(true);
	ChangeWindowMode(GetChangeWindowMode());
	SetRefreshRate(60);
	/* GetRefreshRate関数がほかの名前空間の関数と名前だけだと被るので完全修飾 */
	frameTime = (1000.0f / (GetRefreshRate()) + 0.5f);
	SetQuitKey(KEY_INPUT_ESCAPE);

	// テスト用のシーン内セットアップ
	auto player = new Player;
	InputHandler* input = new InputHandler;
	player->AddComponent(input);
	level->AddObject(player);


	// MainLoop:

	// 効果音読み込み
	pigeon_se = LoadSoundMem(".\\Resources\\pigeon_se_.mp3");

	ClearDrawScreen();
	level->MainLoopEntry();

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

		level->MainLoopUpdate(deltaTime);
		elapsedTime += deltaTime;

		if (CheckHitKey(GetQuitKey()) == 1)
		{
			level->MainLoopExit();

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

