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
	// １フレーム当たりの時間[ms]
	double frameTime = (1000.0 / (datas->GetRefreshRate() + 0.5));
	// 過去時間
	std::chrono::system_clock::time_point clockStarted;
	// 現在時間
	std::chrono::system_clock::time_point clockEnded;
	// δ秒
	float deltaTime = frameTime / 1000.0f;
	// 経過時間
	float elapsedTime = 0;


	// Initialize:

	using SarissaEngine::Runtime::System::MainLoop;
	MainLoop* main_loop = new MainLoop;

	int frame_count = 0;

	// DXLibの初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// ウィンドウのセットアップ
	datas->SetScreenSize(1000, 600);
	auto screen_size = datas->GetScreenSize();
	datas->SetChangeWindowMode(true);
	ChangeWindowMode(datas->GetChangeWindowMode());
	datas->SetQuitKey(KEY_INPUT_ESCAPE);

	// テスト用のシーン内セットアップ
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

		// 計測開始
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
		double mil_sec = static_cast<double>
			(std::chrono::duration_cast<std::chrono::milliseconds>
				(clockEnded - clockStarted).count());
		deltaTime = mil_sec / 1000.0f;
		if (frameTime > mil_sec)
		{
			timeBeginPeriod(1);
			Sleep(DWORD(frameTime - mil_sec));
			timeEndPeriod(1);
		}
	}

	return 0;

}

