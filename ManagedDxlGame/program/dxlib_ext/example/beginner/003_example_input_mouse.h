/*


//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] マウス入力を検知する基本的サンプル
//
//
//---------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

// 左クリックを押しっぱなしでカウントアップし続ける変数
int on_keep_count_up_value = 0;

// 右クリックが押された瞬間だけカウントアップする変数
int on_trigger_count_up_value = 0;

// マウス座標取得用変数
tnl::Vector3 mouse_position;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}



//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// 押しっぱなしを検知
	if (tnl::Input::IsMouseDown(eMouse::LEFT)) {
		on_keep_count_up_value++;
	}

	// 押した瞬間を検知
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT)) {
		on_trigger_count_up_value++;
	}

	// マウス座標を取得
	mouse_position = tnl::Input::GetMousePosition() ;


	DrawStringEx(0, 0, -1, "左クリック押しっぱなし(Z) %d", on_keep_count_up_value);

	DrawStringEx(0, 30, -1, "右クリック押した瞬間(X) %d", on_trigger_count_up_value);

	DrawStringEx(0, 60, -1, "マウス座標 [ x = %.2f ] [ y = %.2f ]", mouse_position.x, mouse_position.y);



	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}


*/