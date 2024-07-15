#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(640, 480, 32);//画面サイズ指定

	if (DxLib_Init() == -1)	// DXLibの初期化
	{
		return -1;
	}

	ClearDrawScreen();

	SetDrawScreen(DX_SCREEN_BACK);

	LoadGraphScreen(0, 0, "DxLogo.jpg", TRUE);

	ScreenFlip();

	WaitKey();

	DxLib_End();

	return 0;
}