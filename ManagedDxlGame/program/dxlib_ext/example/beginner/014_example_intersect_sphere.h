/*

//-----------------------------------------------------------------------------------------------------------
//
//
// 円同士の衝突判定と補正サンプル
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// 円の座標と半径
tnl::Vector3 posA = { 100, 400, 0 };
float radiusA = 100;

tnl::Vector3 posB = { 600, 400, 0 };
float radiusB = 150;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {


	// A, B それぞれの移動処理
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) posA.x -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) posA.x += 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) posA.y -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) posA.y += 6;

	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) posB.x -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) posB.x += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_UP))	posB.y -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) posB.y += 5;


	DrawStringEx(0, 0, -1, "WASD で A を移動");
	DrawStringEx(0, 30, -1, "カーソルキー で B を移動");

	// 衝突判定
	if (tnl::IsIntersectSphere(posA, radiusA, posB, radiusB) ) {

		// A -> B と B -> A の正規化ベクトル
		tnl::Vector3 ab = tnl::Vector3::Normalize(posB - posA);
		tnl::Vector3 ba = tnl::Vector3::Normalize(posA - posB);

		// A B の半径を考慮した中間地点( 補正の起点座標 )
		tnl::Vector3 center = ( ( posA + (ab * radiusA) ) + ( posB + (ba * radiusB) ) ) / 2.0f;

		posA = center + ba * radiusA;
		posB = center + ab * radiusB;
	}


	DrawCircle(posA.x, posA.y, radiusA, -1, false);
	DrawCircle(posB.x, posB.y, radiusB, -1, false);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}


*/