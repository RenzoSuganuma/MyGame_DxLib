#pragma once
/*

//---------------------------------------------------------------------------------------------------------------
//
//
// 数値の補間サンプル
//
//
//---------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetBackgroundColor(32, 32, 32);
}


// 経過時間
float ct = 0;

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	float start_x = 200;
	float goal_x = DXE_WINDOW_WIDTH - 200;

	float time_limit = 3.0f;
	float x0 = tnl::UniformLerp(start_x, goal_x, time_limit, ct);

	float x1 = tnl::SmoothLerp(start_x, goal_x, time_limit, ct, 0);
	float x2 = tnl::SmoothLerp(start_x, goal_x, time_limit, ct, 1);
	float x3 = tnl::SmoothLerp(start_x, goal_x, time_limit, ct, 2);

	float x4 = tnl::AccelLerp(start_x, goal_x, time_limit, ct, 0);
	float x5 = tnl::AccelLerp(start_x, goal_x, time_limit, ct, 1);
	float x6 = tnl::AccelLerp(start_x, goal_x, time_limit, ct, 2);

	float x7 = tnl::DecelLerp(start_x, goal_x, time_limit, ct);

	DrawCircle(x0, 150, 10, -1, false);
	DrawCircle(x1, 200, 10, -1, false);
	DrawCircle(x2, 250, 10, -1, false);
	DrawCircle(x3, 300, 10, -1, false);
	DrawCircle(x4, 350, 10, -1, false);
	DrawCircle(x5, 400, 10, -1, false);
	DrawCircle(x6, 450, 10, -1, false);
	DrawCircle(x7, 500, 10, -1, false);

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) ct = 0;

	ct += delta_time;
	ct = (ct > time_limit) ? time_limit : ct;

	DrawStringEx((int)goal_x, 0, -1, "経過時間 : %.2f", ct);
	DrawStringEx((int)goal_x, 20, -1, "Enterキーでリセット");

	DrawStringEx(0, 150, -1, "UniformLerp");
	DrawStringEx(0, 200, -1, "SmoothLerp - 0");
	DrawStringEx(0, 250, -1, "SmoothLerp - 1");
	DrawStringEx(0, 300, -1, "SmoothLerp - 2");
	DrawStringEx(0, 350, -1, "AccelLerp - 0");
	DrawStringEx(0, 400, -1, "AccelLerp - 1");
	DrawStringEx(0, 450, -1, "AccelLerp - 2");
	DrawStringEx(0, 500, -1, "DecelLerp");

	DrawLine(start_x, 0, start_x, DXE_WINDOW_HEIGHT, 0xffff00ff);
	DrawLine(goal_x, 0, goal_x, DXE_WINDOW_HEIGHT, 0xffff00ff);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/