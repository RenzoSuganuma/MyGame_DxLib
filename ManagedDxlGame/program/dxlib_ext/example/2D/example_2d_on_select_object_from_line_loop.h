#pragma once

/*

//-----------------------------------------------------------------------------------------------------------
//
//
// �}�E�X�ŕ`�������̓����ɑ��݂���I�u�W�F�N�g��I������T���v��
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
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetBackgroundColor(32, 32, 32);
	for (int i = 0; i < 100; ++i) {
		circles.emplace_back(new Circle({ (float)(rand() % DXE_WINDOW_WIDTH), (float)(rand() % DXE_WINDOW_HEIGHT), 0 }));
	}
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
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
		// �q���������̏W���ɑ΂��ė]�����������J�b�g����
		// arg1... �����\������_���W�Q
		// arg2... �[�_���q�����Ă��Ȃ��ꍇ�Ɍq���邩�ǂ���
		tnl::CutOverLineLoop(line_points, true);

		for (auto ci : circles) {

			//----------------------------------------------------------------------------------------------
			// �_���W���������L�΂������A���p�`���\������ӂɑ΂��Č��������񐔂��擾
			// arg1... ���p�`�̒��_
			// arg2... �_���W
			// ret.... ������
			uint32_t n = tnl::CountPolygonIntersections(line_points, { (int)ci->pos_.x, (int)ci->pos_.y });
			if (1 == n % 2) ci->is_select_ = true;
		}
	}


	// �N���b�N�����܂܃}�E�X���ړ������Ă���Ƃ���苗����������J�[�\�����W���L�^
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

	DrawStringEx(0, 0, 0xff00ff00, "�}�E�X�Ő���`���đI���������ʂ��͂��Ă�������");
	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {


}



*/