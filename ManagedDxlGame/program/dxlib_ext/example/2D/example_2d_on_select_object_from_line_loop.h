#pragma once

/*

//-----------------------------------------------------------------------------------------------------------
//
//
// マウスで描いた線の内側に存在するオブジェクトを選択するサンプル
//
//
//-----------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


class Circle {
public:
	Circle() {}
	Circle(const tnl::Vector3& pos) : pos_(pos) {}
	bool is_select_ = false;
	tnl::Vector3 pos_;
	void draw() {
		int color = (is_select_) ? 0xffff0000 : -1;
		DrawCircle((int)pos_.x, (int)pos_.y, 10, color, true);
	}
};


bool on_clicked = false;
std::vector<tnl::Vector2i> line_points;
std::vector<Circle*> circles;


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));
	SetBackgroundColor(32, 32, 32);
	for (int i = 0; i < 100; ++i) {
		circles.emplace_back(new Circle({ (float)(rand() % DXE_WINDOW_WIDTH), (float)(rand() % DXE_WINDOW_HEIGHT), 0 }));
	}
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
		on_clicked = true;
		line_points.clear();
		for (auto ci : circles) ci->is_select_ = false;
	}

	if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
		on_clicked = false;

		//----------------------------------------------------------------------------------------------
		// 繋がった線の集合に対して余った部分をカットする
		// arg1... 線を構成する点座標群
		// arg2... 端点が繋がっていない場合に繋げるかどうか
		tnl::CutOverLineLoop(line_points, true);

		for (auto ci : circles) {

			//----------------------------------------------------------------------------------------------
			// 点座標から光線を伸ばした時、多角形を構成する辺に対して交差した回数を取得
			// arg1... 多角形の頂点
			// arg2... 点座標
			// ret.... 交差回数
			uint32_t n = tnl::CountPolygonIntersections(line_points, { (int)ci->pos_.x, (int)ci->pos_.y });
			if (1 == n % 2) ci->is_select_ = true;
		}
	}


	// クリックしたままマウスを移動させているとき一定距離動いたらカーソル座標を記録
	if (on_clicked) {
		tnl::Vector3 ms = tnl::Input::GetMousePosition();
		if (line_points.empty()) line_points.emplace_back(tnl::Vector2i((int)ms.x, (int)ms.y));
		else {
			float dist = (ms - tnl::Vector3(line_points[line_points.size() - 1])).length();
			if (dist > 3.0f) {
				line_points.emplace_back(tnl::Vector2i((int)ms.x, (int)ms.y));
			}
		}
	}


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------
	for (int i = 1; i < line_points.size(); ++i) {
		DrawLine(line_points[i - 1].x, line_points[i - 1].y, line_points[i].x, line_points[i].y, -1);
	}

	for (auto ci : circles) {
		ci->draw();
	}

	DrawStringEx(0, 0, 0xff00ff00, "マウスで線を描いて選択したい玉を囲ってください");
	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {


}



*/