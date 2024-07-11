/*

//-----------------------------------------------------------------------------------------------------------
//
//
// ��`�Ɖ~�̏Փ˔���ƕ␳�T���v��
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// �~�̍��W�Ɣ��a
tnl::Vector3 posA = { 100, 400, 0 };
float radiusA = 50;

// ��`�̍��W�ƃT�C�Y
tnl::Vector3 posB = { 600, 400, 0 };
tnl::Vector2i sizeB = { 200, 300 };


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
	tnl::Vector3 before_circle = posA ;

	// ���ꂼ��̈ړ�����
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) posA.x -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) posA.x += 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) posA.y -= 6;
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) posA.y += 6;

	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) posB.x -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) posB.x += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_UP))	posB.y -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) posB.y += 5;


	DrawStringEx(0, 0, -1, "WASD �� �~ ���ړ�");
	DrawStringEx(0, 30, -1, "�J�[�\���L�[ �� ��` ���ړ�");

	if (tnl::IsIntersectRectCircle(posB, sizeB, posA, radiusA)) {
		tnl::CorrectPositionRectCircle(posB, sizeB, before_circle, radiusA, posA );
	}

	DrawCircle(posA.x, posA.y, radiusA, -1, false);

	DrawBoxEx(posB, sizeB.x, sizeB.y, false, -1);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}


*/