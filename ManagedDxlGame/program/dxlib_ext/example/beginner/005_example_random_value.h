/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ ���� ] �����擾�T���v��
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// int �^�����擾�p ( 0 �` 65536 )
int i_random_value_A = 0;

// int �^�����擾�p ( 0 �` 100 )
int i_random_value_B = 0;

// int �^�����擾�p ( -100 �` 100 )
int i_random_value_C = 0;


// float �^�����擾�p( -10.0 �` 10.0 )
float f_random_value = 0;


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {

	// �����V�[�h�̐ݒ�( �Q�[�����N�������u�Ԃ̎��Ԃ���ɂ��� )
	srand(time(0));

	SetFontSize(30);

	// 65536 �܂ł̗����擾
	i_random_value_A = rand() ;

	// 65536 �܂ł̗����� 100 �Ŋ������]��ɂ��邱�Ƃ� 0 �` 100 �܂ł̗����ɂ���
	i_random_value_B = rand() % 100 ;

	// -100 �� 200 �܂ł̗����𑫂����� -100 �` +100 �܂ł̗����ɂ���
	i_random_value_C = -100 + (rand() % 200);

	// �͈͎w��ŏ����̗����擾������֐�
	f_random_value = tnl::GetRandomDistributionFloat(-10.0f, 10.0f);
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// �G���^�[�L�[�ŗ������X�V
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		i_random_value_A = rand();
		i_random_value_B = rand() % 100;
		i_random_value_C = -100 + (rand() % 200);
		f_random_value = tnl::GetRandomDistributionFloat(-10.0f, 10.0f);
	}


	DrawStringEx(0, 0, -1, "65536 �܂ł̗��� [ %d ]", i_random_value_A);
	DrawStringEx(0, 30, -1, "0 �` 100 �܂ł̗��� [ %d ]", i_random_value_B);
	DrawStringEx(0, 60, -1, "-100 ���� +100 �܂ł̗��� [ %d ]", i_random_value_C);
	DrawStringEx(0, 90, -1, "-10.0 ���� +10.0 �܂ł̗��� [ %f ]", f_random_value);


	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
}

*/