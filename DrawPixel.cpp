#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);

	SetGraphMode(600, 500, 32);

	if (DxLib_Init() == -1)	// DXLibÇÃèâä˙âª
	{
		return -1;
	}

	while (ProcessMessage() == 0)
	{
		auto color = GetColor(255, 255, 255);

		if (CheckHitKey(KEY_INPUT_SPACE) == 1)
		{
			color = GetColor(255, 255, 255);
		}
		else
		{
			color = GetColor(255, 0, 0);
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			DxLib_End();
		}

		DrawCircle(200, 300, 100, color);
	}
	return 0;
}