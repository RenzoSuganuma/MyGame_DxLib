/*


//---------------------------------------------------------------------------------------------------------------
//
//
// [ ���� ] �}�E�X���͂����m�����{�I�T���v��
//
//
//---------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

// ���N���b�N���������ςȂ��ŃJ�E���g�A�b�v��������ϐ�
int on_keep_count_up_value = 0;

// �E�N���b�N�������ꂽ�u�Ԃ����J�E���g�A�b�v����ϐ�
int on_trigger_count_up_value = 0;

// �}�E�X���W�擾�p�ϐ�
tnl::Vector3 mouse_position;

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}



//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// �������ςȂ������m
	if (tnl::Input::IsMouseDown(eMouse::LEFT)) {
		on_keep_count_up_value++;
	}

	// �������u�Ԃ����m
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT)) {
		on_trigger_count_up_value++;
	}

	// �}�E�X���W���擾
	mouse_position = tnl::Input::GetMousePosition() ;


	DrawStringEx(0, 0, -1, "���N���b�N�������ςȂ�(Z) %d", on_keep_count_up_value);

	DrawStringEx(0, 30, -1, "�E�N���b�N�������u��(X) %d", on_trigger_count_up_value);

	DrawStringEx(0, 60, -1, "�}�E�X���W [ x = %.2f ] [ y = %.2f ]", mouse_position.x, mouse_position.y);



	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
}


*/