/*

//-----------------------------------------------------------------------------------------------------------
//
//
// �����Ɖ~�̏Փ˔���ƕ␳�T���v��
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// �~�̍��W�Ɣ��a
tnl::Vector3 circle_pos = { 100, 400, 0 };
float circle_radius = 50;

// �����̎n�_�ƏI�_
tnl::Vector3 line_s = { 500, 200, 0 };
tnl::Vector3 line_e = { 700, 500, 0 };


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}

//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// �~�̈ړ��O�̍��W��ۑ�
	tnl::Vector3 before_circle = circle_pos ;

	// �~�̈ړ�����
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) circle_pos.x -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) circle_pos.x += 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) circle_pos.y -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) circle_pos.y += 6;


	DrawStringEx(0, 0, -1, "WASD �� �~ ���ړ�");

	if (tnl::IsIntersectLineCircle(line_s, line_e, circle_pos, circle_radius)) {
		tnl::CorrectPositionLineCircle(line_s, line_e, before_circle, circle_radius, circle_pos);
	}

	DrawCircle(circle_pos.x, circle_pos.y, circle_radius, -1, false);

	DrawLine(line_s.x, line_s.y, line_e.x, line_e.y, -1);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}

*/