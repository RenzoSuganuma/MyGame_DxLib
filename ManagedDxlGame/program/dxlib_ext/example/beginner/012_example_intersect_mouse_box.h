/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ ���� ] �Փ˔���T���v�� �}�E�X�J�[�\���Ƌ�`
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// �{�b�N�X�̃T�C�Y
int size_w = 200;
int size_h = 200;

// �{�b�N�X�̒��S���W
tnl::Vector3 box_pos = { 600, 300, 0 };


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	tnl::Vector3 mouse_pos = tnl::Input::GetMousePosition();

	DrawStringEx(0, 0, -1, "�}�E�X�J�[�\���Ƌ�`�̏Փ˔���");


	if (tnl::IsIntersectPointRect(mouse_pos.x, mouse_pos.y, box_pos.x, box_pos.y, size_w, size_h) ) {
		DrawStringEx( 0, 30, -1, "hit");
	}

	DrawBoxEx(box_pos, size_w, size_h, false);


	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
}


*/