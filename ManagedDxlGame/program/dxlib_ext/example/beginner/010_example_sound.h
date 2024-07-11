/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] サウンド再生サンプル
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

// ロードしたサウンドデータを識別するハンドル変数
int bgm_handle = 0;
int se_handle = 0;


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);

	// サウンドファイルのロード
	// ! ロードは重い処理でメモリも消費します　必要な時に１度だけ実行しましょう ( 毎フレーム実行されないよう工夫しましょう )
	// 第１引数... ファイルパス
	// 第２引数... 同時再生を可能にする数 ( メモリを消費するので不必要に増やさない事 )
	bgm_handle = LoadSoundMem("sound/test_bgm.mp3", 1);
	se_handle = LoadSoundMem("sound/test_se.wav", 3);

	// 音量は100分の1デシベル単位 0 ～ 10000 で設定
	// ※ 実際には 5000db でもかなり小さくなります
	SetVolumeSoundMem(8500, bgm_handle);
	SetVolumeSoundMem(8500, se_handle);

	// BGM を再生
	// 再生命令も必要な個所で１度だけ実行します
	// 毎フレーム実行しないように注意
	// 第１引数... ロードしたサウンドハンドル
	// 第２引数... [ DX_PLAYTYPE_LOOP はループ再生 ]  [ DX_PLAYTYPE_BACK は単発再生 ]     [ ! DX_PLAYTYPE_NORMAL は再生中は操作を受け付けなくなるので基本的に使用しません !]
	// 第３引数... [ TRUE は初めから ] [ FALSE を設定すると停止中の BGM が途中から再生されます ]
	PlaySoundMem( bgm_handle, DX_PLAYTYPE_LOOP, TRUE );
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// BGM を途中再生
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Q)) {
		PlaySoundMem(bgm_handle, DX_PLAYTYPE_LOOP, FALSE);
	}
	// BGM を停止
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		StopSoundMem(bgm_handle);
	}
	// SE を再生
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_E)) {
		PlaySoundMem(se_handle, DX_PLAYTYPE_BACK);
	}

	DrawStringEx(0, 0, -1, "『Q』BGM 途中再生");
	DrawStringEx(0, 30, -1, "『W』BGM 停止");
	DrawStringEx(0, 60, -1, "『E』SE 再生");

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}


*/