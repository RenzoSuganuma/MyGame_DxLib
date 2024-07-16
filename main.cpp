#include "DxLib.h"
#include "config_data.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigData::ConfigDatas* datas = new ConfigData::ConfigDatas;

	datas->SetScreenSize( 600, 500 );

	auto screen_size = datas->GetScreenSize();

	ChangeWindowMode( true );

	SetGraphMode( screen_size.first, screen_size.second, 32 );

	if (DxLib_Init() == -1)	// DXLib‚Ì‰Šú‰»
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