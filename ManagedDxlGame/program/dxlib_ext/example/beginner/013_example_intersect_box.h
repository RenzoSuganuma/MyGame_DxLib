/*

//-----------------------------------------------------------------------------------------------------------
//
//
// 矩形同士の衝突判定と補正サンプル
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

// ボックスの座標とサイズ
tnl::Vector3 posA = { 100, 400, 0 };
tnl::Vector2i sizeA = { 50, 50 };

tnl::Vector3 posB = { 600, 400, 0 };
tnl::Vector2i sizeB = { 200, 20 };

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// 移動前の座標を覚えておく
	tnl::Vector3 before_A = posA;
	tnl::Vector3 before_B = posB;

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
	if (tnl::IsIntersectRect(posA, sizeA.x, sizeA.y, posB, sizeB.x, sizeB.y)) {

		// 矩形同士の座標補正
		// arg1.2. 矩形 A, B の移動前の座標
		// arg3.4. 矩形 A, B のサイズ
		// arg5.6. 矩形 A, B の移動後の座標 ( この関数によって補正後の座標に更新 )
		// arg7.8. 横, 縦 方向の補正タイプ
		// ....... [ PWRFL_A : A が 強い ] [ PWRFL_B : B が 強い ] [ BOTH : A と B は押し合う ]
		// arg9... 補正後に A と B の間に設ける空間 ( デフォルト 1.0f )
		// ret.... 矩形 A が B に対してどの方向へ補正されたか
		tnl::eCorrResRect n = tnl::CorrectPositionRect(
			before_A
			, before_B
			, sizeA
			, sizeB
			, posA
			, posB
			, tnl::eCorrTypeRect::PWRFL_A
			, tnl::eCorrTypeRect::PWRFL_A, 1.0f);

		std::string s[4] = { "A を左に補正", "A を右に補正", "A を上に補正", "A を下に補正" };
		DrawStringEx(0, 60, -1, "%s", s[(int)n].c_str());
	}


	DrawBoxEx(posA, sizeA.x, sizeA.y, false, -1);
	DrawBoxEx(posB, sizeB.x, sizeB.y, false, -1);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/