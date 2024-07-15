#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//��S��ʂɃZ�b�g
	SetGraphMode(640, 480, 32);//��ʃT�C�Y�w��

	if (DxLib_Init() == -1)	// DXLib�̏�����
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