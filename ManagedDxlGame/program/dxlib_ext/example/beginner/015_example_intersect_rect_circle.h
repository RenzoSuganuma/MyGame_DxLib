/*

//-----------------------------------------------------------------------------------------------------------
//
//
// 矩形と円の衝突判定と補正サンプル
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// 円の座標と半径
tnl::Vector3 posA = { 100, 400, 0 };
float radiusA = 50;

// 矩形の座標とサイズ
tnl::Vector3 posB = { 600, 400, 0 };
tnl::Vector2i sizeB = { 200, 300 };


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// 円の移動前の座標を保存
	tnl::Vector3 before_circle = posA ;

	// それぞれの移動処理
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) posA.x -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) posA.x += 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) posA.y -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) posA.y += 6;

	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) posB.x -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) posB.x += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_UP))	posB.y -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) posB.y += 5;


	DrawStringEx(0, 0, -1, "WASD で 円 を移動");
	DrawStringEx(0, 30, -1, "カーソルキー で 矩形 を移動");

	if (tnl::IsIntersectRectCircle(posB, sizeB, posA, radiusA)) {
		tnl::CorrectPositionRectCircle(posB, sizeB, before_circle, radiusA, posA );
	}

	DrawCircle(posA.x, posA.y, radiusA, -1, false);

	DrawBoxEx(posB, sizeB.x, sizeB.y, false, -1);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}


*/