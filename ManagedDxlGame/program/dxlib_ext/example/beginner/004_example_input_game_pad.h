/*


//---------------------------------------------------------------------------------------------------------------
//
//
// [ ���� ] �Q�[���p�b�h���͂����m�����{�I�T���v��
//
// �� �g�p�\�ȃQ�[���p�b�h�͂P�����ł�
// �� �Q�[�����N������O�ɃQ�[���p�b�h�� PC �ڑ�����Ă���K�v������܂�
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


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetFontSize(30);
}



//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// �� (A) �������ςȂ������m
	if (tnl::Input::IsPadDown(ePad::KEY_0) ) {
		on_keep_count_up_value++;
	}

	// �~ (B) �������u�Ԃ����m
	if (tnl::Input::IsPadDownTrigger(ePad::KEY_1)) {
		on_trigger_count_up_value++;
	}

	// �X�e�B�b�N�����擾
	tnl::Vector3 l_stick_axis_value = tnl::Input::GetLeftStick();
	tnl::Vector3 r_stick_axis_value = tnl::Input::GetRightStick();

	DrawStringEx(0, 0, -1, "�� (A) �������ςȂ� %d", on_keep_count_up_value);
	DrawStringEx(0, 30, -1, "�~ (B) �������u�� %d", on_trigger_count_up_value);
	DrawStringEx(0, 60, -1, "���X�e�B�b�N�� [ axis x = %.2f ] [ axis y = %.2f ]", l_stick_axis_value.x, l_stick_axis_value.y);
	DrawStringEx(0, 90, -1, "�E�X�e�B�b�N�� [ axis x = %.2f ] [ axis y = %.2f ]", r_stick_axis_value.x, r_stick_axis_value.y);



	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
}

*/