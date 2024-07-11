#include <chrono>
#pragma comment(lib,"winmm.lib")

#include "DxLib.h"
#include "../library/tnl_input.h"
#include "../game/gm_main.h"
#include "../dxlib_ext/dxlib_ext.h"


static std::chrono::system_clock::time_point clock_start, clock_end ; 
static std::chrono::system_clock::time_point fps_clock_start, fps_clock_end;


// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// ウィンドウモードで起動
	ChangeWindowMode( true ) ;

	// ウィンドウサイズ設定
	SetGraphMode(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 32 ) ;

	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		
	{
		// エラーが起きたら直ちに終了
		return -1 ;				
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// 計測開始時間
	clock_start = std::chrono::system_clock::now();

	// キー入力制御の初期化
	HWND hWnd = GetMainWindowHandle();
	HDC hdc = GetDC(hWnd);
	tnl::Input::Initialize(hInstance,hWnd, hdc, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	// ゲームスタート処理
	gameStart();

	// メッセージループ
	while( 1 )
	{
		// フレーム間の経過時間
		// マイクロ秒で計測して秒に変換
		clock_end = std::chrono::system_clock::now();
		double micro_seconds = static_cast<double>( std::chrono::duration_cast<std::chrono::microseconds>(clock_end - clock_start).count() );
		float delta_time = static_cast<float>( micro_seconds / 1000.0 / 1000.0 );
		clock_start = clock_end;

		if( ProcessMessage() == -1 ){
			break ;
		}

		fps_clock_start = std::chrono::system_clock::now();

		// 画面をクリア
		ClearDrawScreen() ;

		// キー状態の更新
		tnl::Input::Update();

		// ゲームメインルーチン
		gameMain(delta_time);

		// バックバッファをフリップ
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

		// フレームレートコントロール
		fps_clock_end = std::chrono::system_clock::now();
		double fps_mic = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(fps_clock_end - fps_clock_start).count());
		float fps_mils = static_cast<float>(fps_mic / 1000.0);
		float fps_lim = 1000.0f / DXE_FIX_FPS;

		if (fps_lim > fps_mils) {
			timeBeginPeriod(1);
			Sleep(DWORD(fps_lim - fps_mils));
			timeEndPeriod(1);
		}

	}

	// ゲーム側の終了処理
	gameEnd();

	ReleaseDC(hWnd, hdc);

	// ＤＸライブラリ使用の終了処理
	DxLib_End() ;			

	// ソフトの終了
	return 0 ;				 
}
