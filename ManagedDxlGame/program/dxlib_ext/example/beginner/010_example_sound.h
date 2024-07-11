/*

//---------------------------------------------------------------------------------------------------------------
//
//
// [ ���� ] �T�E���h�Đ��T���v��
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

// ���[�h�����T�E���h�f�[�^�����ʂ���n���h���ϐ�
int bgm_handle = 0;
int se_handle = 0;


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetFontSize(30);

	// �T�E���h�t�@�C���̃��[�h
	// ! ���[�h�͏d�������Ń�����������܂��@�K�v�Ȏ��ɂP�x�������s���܂��傤 ( ���t���[�����s����Ȃ��悤�H�v���܂��傤 )
	// ��P����... �t�@�C���p�X
	// ��Q����... �����Đ����\�ɂ��鐔 ( �������������̂ŕs�K�v�ɑ��₳�Ȃ��� )
	bgm_handle = LoadSoundMem("sound/test_bgm.mp3", 1);
	se_handle = LoadSoundMem("sound/test_se.wav", 3);

	// ���ʂ�100����1�f�V�x���P�� 0 �` 10000 �Őݒ�
	// �� ���ۂɂ� 5000db �ł����Ȃ菬�����Ȃ�܂�
	SetVolumeSoundMem(8500, bgm_handle);
	SetVolumeSoundMem(8500, se_handle);

	// BGM ���Đ�
	// �Đ����߂��K�v�Ȍ��łP�x�������s���܂�
	// ���t���[�����s���Ȃ��悤�ɒ���
	// ��P����... ���[�h�����T�E���h�n���h��
	// ��Q����... [ DX_PLAYTYPE_LOOP �̓��[�v�Đ� ]  [ DX_PLAYTYPE_BACK �͒P���Đ� ]     [ ! DX_PLAYTYPE_NORMAL �͍Đ����͑�����󂯕t���Ȃ��Ȃ�̂Ŋ�{�I�Ɏg�p���܂��� !]
	// ��R����... [ TRUE �͏��߂��� ] [ FALSE ��ݒ肷��ƒ�~���� BGM ���r������Đ�����܂� ]
	PlaySoundMem( bgm_handle, DX_PLAYTYPE_LOOP, TRUE );
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// BGM ��r���Đ�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Q)) {
		PlaySoundMem(bgm_handle, DX_PLAYTYPE_LOOP, FALSE);
	}
	// BGM ���~
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		StopSoundMem(bgm_handle);
	}
	// SE ���Đ�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_E)) {
		PlaySoundMem(se_handle, DX_PLAYTYPE_BACK);
	}

	DrawStringEx(0, 0, -1, "�wQ�xBGM �r���Đ�");
	DrawStringEx(0, 30, -1, "�wW�xBGM ��~");
	DrawStringEx(0, 60, -1, "�wE�xSE �Đ�");

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
}


//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
}


*/