#pragma once

/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ 入門 ] じゃんけんゲームサンプル
//
//
//---------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

//------------------------------------------------------------------------------------------------------------
//
// 全体用 定数・変数
//

// シーンIDを表す定数をenumで定義し、初期値としてタイトルシーンを設定しています
enum {
	SCENE_ID_TITLE,
	SCENE_ID_PLAY
};
int g_scene_id = SCENE_ID_TITLE;


//------------------------------------------------------------------------------------------------------------
//
// タイトルシーン用 定数・変数
//

// タイトルシーンで使用する文字列や座標を定義しています
const std::string TITLE_LOGO_STR = "じゃんけんゲーム";
const tnl::Vector3 TITLE_LOGO_POS = { 200, 100, 0 };

const std::string TITLE_MENU_STR = "start enter";
const tnl::Vector3 TITLE_MENU_POS = { 550, 400, 0 };

const float TITLE_MENU_FLASHINTIME = 0.5f; // タイトルシーンのメニューの点滅時間の定数
float g_title_menu_flashing_time_cont = 0; // タイトルシーンのメニューの点滅時間のカウント変数
bool g_is_title_menu_on_draw = true; // タイトルシーンのメニューの描画フラグ

//------------------------------------------------------------------------------------------------------------
//
// プレイシーン用 定数・変数
//

// 手の選択・結果表時のシーケンスを分ける定数
enum {
	SEQ_SELECT_HAND,
	SEQ_RESULT_DISPLAY
};

// じゃんけんの結果を定義する定数
enum {
	RESULT_TYPE_WIN,
	RESULT_TYPE_LOSE,
	RESULT_TYPE_DRAW,
	RESULT_TYPE_MAX
};

// 手の種類を定義する定数
enum {
	HAND_TYPE_STONE,
	HAND_TYPE_SCISSORS,
	HAND_TYPE_PAPER,
	HAND_TYPE_MAX
};

const std::string PLAY_SEQ_SEL_HAND_STR = "手を選択して Enter キーを押してください";
const tnl::Vector3 PLAY_SEQ_SEL_HAND_STR_POS = { 450, 100, 0 };

const tnl::Vector3 PLAY_SEQ_SEL_HAND_POS_TBL[HAND_TYPE_MAX] = {
	{300, 500, 0},
	{600, 500, 0},
	{900, 500, 0}
};

const tnl::Vector3 PLAY_RESULT_CPU_HAND_POS = { 600, 150, 0 };
const tnl::Vector3 PLAY_RESULT_PLAYER_HAND_POS = { 600, 450, 0 };

const std::string RETURN_TITLE_MASSAGE_STR = "エンターキーでタイトル画面へ戻ります";
const std::string RESULT_MESSAGE[RESULT_TYPE_MAX] = { "Your Win", "Your Lose", "  Draw  " };

int g_seq_scene_play = SEQ_SELECT_HAND;
int g_gpc_cursor_hdl = 0;
int g_gpc_hand_hdls[HAND_TYPE_MAX];

int g_sel_cpu_hand = HAND_TYPE_STONE;
int g_sel_player_hand = HAND_TYPE_STONE;
int g_reslut = RESULT_TYPE_DRAW;
tnl::Vector3 g_sel_cursor_pos = PLAY_SEQ_SEL_HAND_POS_TBL[g_sel_player_hand];

//------------------------------------------------------------------------------------------------------------
// タイトルシーンで毎フレーム実行されます
void sceneTitle(float delta_time) {

	// 点滅処理
	g_title_menu_flashing_time_cont += delta_time;
	if (g_title_menu_flashing_time_cont > TITLE_MENU_FLASHINTIME) {
		g_is_title_menu_on_draw = !g_is_title_menu_on_draw;
		g_title_menu_flashing_time_cont = 0;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		g_scene_id = SCENE_ID_PLAY;
	}

	SetFontSize(100);
	DrawStringEx(TITLE_LOGO_POS.x, TITLE_LOGO_POS.y, -1, TITLE_LOGO_STR.c_str());

	if (g_is_title_menu_on_draw) {
		SetFontSize(DEFAULT_FONT_SIZE);
		DrawStringEx(TITLE_MENU_POS.x, TITLE_MENU_POS.y, -1, TITLE_MENU_STR.c_str());
	}

}


//------------------------------------------------------------------------------------------------------------
// プレイシーンで毎フレーム実行されます
void scenePlay(float delta_time) {

	if (SEQ_SELECT_HAND == g_seq_scene_play) {

		// プレイヤーの手の選択処理
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
			g_sel_player_hand--;
			if (g_sel_player_hand < 0) g_sel_player_hand = HAND_TYPE_PAPER;
		}
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT)) {
			g_sel_player_hand++;
			if (g_sel_player_hand >= HAND_TYPE_MAX) g_sel_player_hand = HAND_TYPE_STONE;
		}
		g_sel_cursor_pos = PLAY_SEQ_SEL_HAND_POS_TBL[g_sel_player_hand];


		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

			// CPU が手を選択
			g_sel_cpu_hand = rand() % HAND_TYPE_MAX;

			// じゃんけんの勝敗判定
			g_reslut = RESULT_TYPE_DRAW;
			if (g_sel_player_hand == HAND_TYPE_STONE) {
				if (g_sel_cpu_hand == HAND_TYPE_SCISSORS)	g_reslut = RESULT_TYPE_WIN;
				if (g_sel_cpu_hand == HAND_TYPE_PAPER)		g_reslut = RESULT_TYPE_LOSE;
			}
			if (g_sel_player_hand == HAND_TYPE_SCISSORS) {
				if (g_sel_cpu_hand == HAND_TYPE_PAPER)		g_reslut = RESULT_TYPE_WIN;
				if (g_sel_cpu_hand == HAND_TYPE_STONE)		g_reslut = RESULT_TYPE_LOSE;
			}
			if (g_sel_player_hand == HAND_TYPE_PAPER) {
				if (g_sel_cpu_hand == HAND_TYPE_STONE)	g_reslut = RESULT_TYPE_WIN;
				if (g_sel_cpu_hand == HAND_TYPE_SCISSORS)	g_reslut = RESULT_TYPE_LOSE;
			}

			// 次のフレームからは結果表示シーケンス
			g_seq_scene_play = SEQ_RESULT_DISPLAY;

		}

		SetFontSize(DEFAULT_FONT_SIZE);
		DrawStringEx(PLAY_SEQ_SEL_HAND_STR_POS.x, PLAY_SEQ_SEL_HAND_STR_POS.y, -1, PLAY_SEQ_SEL_HAND_STR.c_str());

		DrawRotaGraphF(g_sel_cursor_pos.x + 25, g_sel_cursor_pos.y + 120, 0.25f, 0, g_gpc_cursor_hdl, true);

		for (int i = 0; i < HAND_TYPE_MAX; ++i) {
			DrawRotaGraphF(PLAY_SEQ_SEL_HAND_POS_TBL[i].x, PLAY_SEQ_SEL_HAND_POS_TBL[i].y, 0.5f, 0, g_gpc_hand_hdls[i], true);
		}

	}
	else if (SEQ_RESULT_DISPLAY == g_seq_scene_play) {

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			g_seq_scene_play = SEQ_SELECT_HAND;
			g_scene_id = SCENE_ID_TITLE;
		}

		SetFontSize(30);
		DrawStringEx(PLAY_RESULT_CPU_HAND_POS.x, PLAY_RESULT_CPU_HAND_POS.y - 100, -1, "CPU");
		DrawStringEx(PLAY_RESULT_PLAYER_HAND_POS.x - 20, PLAY_RESULT_PLAYER_HAND_POS.y + 100, -1, "PLAYER");

		DrawStringEx(PLAY_RESULT_CPU_HAND_POS.x - 50, PLAY_RESULT_CPU_HAND_POS.y + 140, -1, RESULT_MESSAGE[g_reslut].c_str());

		DrawRotaGraphF(PLAY_RESULT_CPU_HAND_POS.x, PLAY_RESULT_CPU_HAND_POS.y, 0.5f, 0, g_gpc_hand_hdls[g_sel_cpu_hand], true);
		DrawRotaGraphF(PLAY_RESULT_PLAYER_HAND_POS.x, PLAY_RESULT_PLAYER_HAND_POS.y, 0.5f, 0, g_gpc_hand_hdls[g_sel_player_hand], true);

		SetFontSize(DEFAULT_FONT_SIZE);
		DrawStringEx(PLAY_RESULT_PLAYER_HAND_POS.x - 130, PLAY_RESULT_PLAYER_HAND_POS.y + 220, -1, RETURN_TITLE_MASSAGE_STR.c_str());
	}


}

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	g_gpc_cursor_hdl = LoadGraph("graphics/cursor.png");
	g_gpc_hand_hdls[HAND_TYPE_STONE] = LoadGraph("graphics/stone.png");
	g_gpc_hand_hdls[HAND_TYPE_SCISSORS] = LoadGraph("graphics/scissors.png");
	g_gpc_hand_hdls[HAND_TYPE_PAPER] = LoadGraph("graphics/paper.png");

}

//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	switch (g_scene_id) {
	case SCENE_ID_TITLE:
		sceneTitle(delta_time);
		break;
	case SCENE_ID_PLAY:
		scenePlay(delta_time);
		break;
	}
}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
}


*/