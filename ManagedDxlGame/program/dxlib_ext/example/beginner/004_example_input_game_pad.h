/*


//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] ゲームパッド入力を検知する基本的サンプル
//
// ※ 使用可能なゲームパッドは１つだけです
// ※ ゲームを起動する前にゲームパッドが PC 接続されている必要があります
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


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}



//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// □ (A) 押しっぱなしを検知
	if (tnl::Input::IsPadDown(ePad::KEY_0) ) {
		on_keep_count_up_value++;
	}

	// × (B) 押した瞬間を検知
	if (tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {
		on_trigger_count_up_value++;
	}

	// スティック軸を取得
	tnl::Vector3 l_stick_axis_value = tnl::Input::GetLeftStick();
	tnl::Vector3 r_stick_axis_value = tnl::Input::GetRightStick();

	DrawStringEx(0, 0, -1, "□ (A) 押しっぱなし %d", on_keep_count_up_value);
	DrawStringEx(0, 30, -1, "× (B) 押した瞬間 %d", on_trigger_count_up_value);
	DrawStringEx(0, 60, -1, "左スティック軸 [ axis x = %.2f ] [ axis y = %.2f ]", l_stick_axis_value.x, l_stick_axis_value.y);
	DrawStringEx(0, 90, -1, "右スティック軸 [ axis x = %.2f ] [ axis y = %.2f ]", r_stick_axis_value.x, r_stick_axis_value.y);



	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}

*/