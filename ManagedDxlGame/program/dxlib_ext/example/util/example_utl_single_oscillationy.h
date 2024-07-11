#pragma once

/*

//---------------------------------------------------------------------------------------------------------------
//
//
// 減衰振動サンプル
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

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
}


float progress_time1 = 0;
float progress_time2 = 0;
float progress_time3 = 0;
float progress_time4 = 0;

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-----------------------------------------------------------------------------------------
	//
	// 画面左上サンプル
	//
	{
		progress_time1 += delta_time;
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_1)) progress_time1 = 0;

		float osc = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0.5f, 20, progress_time1);

		float lenght = 100;
		float start_x = 300;

		DrawLine(200, 200, 400, 200, -1);
		DrawLine(200, 195, 200, 205, -1);
		DrawLine(300, 195, 300, 205, -1);
		DrawLine(400, 195, 400, 205, -1);
		DrawCircle(start_x + lenght * osc, 200, 10, -1, false);

		DrawStringEx(0, 0, -1, "振幅は 0 スタートで 1.0f ～ -1.0 まで振れ 0 に収束");
		DrawStringEx(0, 20, -1, "1 キーで時間リセット");
		DrawStringEx(0, 40, -1, "%f", osc);
	}

	//-----------------------------------------------------------------------------------------
	//
	// 画面右上サンプル
	//
	{
		if (tnl::Input::IsKeyDown(eKeys::KB_2)) progress_time2 = 0;

		float osc = tnl::SingleOscillationy(tnl::eOscStart::STOK, 0.5f, 20, progress_time2);

		float lenght = 100;
		float start_x = 200;
		float scr_ofs_x = DXE_WINDOW_WIDTH * 0.5f;

		DrawLine(scr_ofs_x + 200, 200, scr_ofs_x + 400, 200, -1);
		DrawLine(scr_ofs_x + 200, 195, scr_ofs_x + 200, 205, -1);
		DrawLine(scr_ofs_x + 300, 195, scr_ofs_x + 300, 205, -1);
		DrawLine(scr_ofs_x + 400, 195, scr_ofs_x + 400, 205, -1);
		DrawCircle(scr_ofs_x + start_x + lenght * osc, 200, 10, -1, false);

		DrawStringEx((int)scr_ofs_x, 0, -1, "振幅は 0 スタートで 0.0f ～ 2.0 まで振れ 1 に収束");
		DrawStringEx((int)scr_ofs_x, 20, -1, "2 キーで時間リセット");
		DrawStringEx((int)scr_ofs_x, 40, -1, "%f", osc);

		progress_time2 += delta_time;
	}


	//-----------------------------------------------------------------------------------------
	//
	// 画面左下サンプル
	//
	{
		progress_time3 += delta_time;
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_3)) progress_time3 = 0;

		float osc = tnl::SingleOscillationy(tnl::eOscStart::CENTER, 0.1f, 5, progress_time3);

		float lenght = 100;
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis({0, 0, 1}, osc);
		tnl::Vector3 anc = { 300, 500, 0 };
		tnl::Vector3 pos = anc + tnl::Vector3::TransformCoord({0, lenght, 0}, rot);

		DrawLine(anc.x, anc.y, pos.x, pos.y, -1);
		DrawCircle(pos.x, pos.y, 10, -1, false);

		int scr_ofs_y = DXE_WINDOW_HEIGHT * 0.5f;
		DrawStringEx(0, scr_ofs_y + 0, -1, "振幅は 0 スタートで 1.0f ～ -1.0 まで振れ 0 に収束");
		DrawStringEx(0, scr_ofs_y + 20, -1, "3 キーで時間リセット");
		DrawStringEx(0, scr_ofs_y + 40, -1, "%f", osc);
	}

	//-----------------------------------------------------------------------------------------
	//
	// 画面右下サンプル
	//
	{
		float osc = 0;
		int scr_ofs_x = DXE_WINDOW_WIDTH * 0.5f;
		tnl::Vector3 anc = { (float)scr_ofs_x + 300, 500, 0 };

		static int step = -1;
		static tnl::Vector3 release_pos = anc ;
		static tnl::Vector3 draw_pos = release_pos;
		static tnl::Vector3 to_anc;


		int color = -1;

		tnl::Vector3 ms = tnl::Input::GetMousePosition();
		if ((ms - draw_pos).length() < 10) {
			color = 0xffff0000;
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) step = 0;
		}

		if (0 == step) {
			release_pos = ms;
			progress_time4 = 0 ;
			draw_pos = release_pos;
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
				step = 1;
				to_anc = anc - release_pos;
			}
		}
		else if (1 == step) {
			progress_time4 += delta_time;

			osc = tnl::SingleOscillationy(tnl::eOscStart::STOK, 3, 40, progress_time4);

			draw_pos = release_pos + tnl::Vector3::Normalize(to_anc) * to_anc.length() * osc;
		}


		DrawCircle(anc.x, anc.y, 5, 0x88888888, false);
		DrawLine(anc.x, anc.y, draw_pos.x, draw_pos.y, -1);

		DrawCircle(draw_pos.x, draw_pos.y, 10, color, false);

		int scr_ofs_y = DXE_WINDOW_HEIGHT * 0.5f;
		DrawStringEx(scr_ofs_x, scr_ofs_y + 0, -1, "振幅は 0 スタートで 0.0f ～ 2.0 まで振れ 1 に収束");
		DrawStringEx(scr_ofs_x, scr_ofs_y + 20, -1, "マウスで玉を引っ張りましょう");
		DrawStringEx(scr_ofs_x, scr_ofs_y + 40, -1, "%f", osc);
	}



	DrawLine(DXE_WINDOW_WIDTH >> 1, 0, DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT, 0x88888888);
	DrawLine(0, DXE_WINDOW_HEIGHT >> 1, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT >> 1, 0x88888888);
	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}



*/