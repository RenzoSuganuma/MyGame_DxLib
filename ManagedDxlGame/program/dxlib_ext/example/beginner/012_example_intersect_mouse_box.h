/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] 衝突判定サンプル マウスカーソルと矩形
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// ボックスのサイズ
int size_w = 200;
int size_h = 200;

// ボックスの中心座標
tnl::Vector3 box_pos = { 600, 300, 0 };


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	tnl::Vector3 mouse_pos = tnl::Input::GetMousePosition();

	DrawStringEx(0, 0, -1, "マウスカーソルと矩形の衝突判定");


	if (tnl::IsIntersectPointRect(mouse_pos.x, mouse_pos.y, box_pos.x, box_pos.y, size_w, size_h) ) {
		DrawStringEx( 0, 30, -1, "hit");
	}

	DrawBoxEx(box_pos, size_w, size_h, false);


	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}


*/