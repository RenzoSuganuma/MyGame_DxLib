/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] 乱数取得サンプル
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// int 型乱数取得用 ( 0 ～ 65536 )
int i_random_value_A = 0;

// int 型乱数取得用 ( 0 ～ 100 )
int i_random_value_B = 0;

// int 型乱数取得用 ( -100 ～ 100 )
int i_random_value_C = 0;


// float 型乱数取得用( -10.0 ～ 10.0 )
float f_random_value = 0;


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {

	// 乱数シードの設定( ゲームを起動した瞬間の時間を種にする )
	srand(time(0));

	SetFontSize(30);

	// 65536 までの乱数取得
	i_random_value_A = rand() ;

	// 65536 までの乱数を 100 で割った余りにすることで 0 ～ 100 までの乱数にする
	i_random_value_B = rand() % 100 ;

	// -100 に 200 までの乱数を足す事で -100 ～ +100 までの乱数にする
	i_random_value_C = -100 + (rand() % 200);

	// 範囲指定で小数の乱数取得をする関数
	f_random_value = tnl::GetRandomDistributionFloat(-10.0f, 10.0f);
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// エンターキーで乱数を更新
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		i_random_value_A = rand();
		i_random_value_B = rand() % 100;
		i_random_value_C = -100 + (rand() % 200);
		f_random_value = tnl::GetRandomDistributionFloat(-10.0f, 10.0f);
	}


	DrawStringEx(0, 0, -1, "65536 までの乱数 [ %d ]", i_random_value_A);
	DrawStringEx(0, 30, -1, "0 ～ 100 までの乱数 [ %d ]", i_random_value_B);
	DrawStringEx(0, 60, -1, "-100 から +100 までの乱数 [ %d ]", i_random_value_C);
	DrawStringEx(0, 90, -1, "-10.0 から +10.0 までの乱数 [ %f ]", f_random_value);


	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}

*/