#include "DxLib.h"
#include "config_data.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)	// DXLibÇÃèâä˙âª
	{
		return -1;
	}

	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			DxLib_End();
		}
	}

	return 0;
}