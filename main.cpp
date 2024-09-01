#include "chrono"
#include "DxLib.h"
#include "Windows.h"
#pragma comment (lib , "winmm.lib")

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
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

	ClearDrawScreen();
	
	while (1)
	{
		if (ProcessMessage() == -1)
		{
			break;
		}

		// 計測開始
		clockStarted = std::chrono::system_clock::now();

		ClearDrawScreen();
		
		elapsedTime += deltaTime;

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
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

