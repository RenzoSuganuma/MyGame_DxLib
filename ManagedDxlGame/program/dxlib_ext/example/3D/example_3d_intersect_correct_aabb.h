#pragma once

/*


//-----------------------------------------------------------------------------------------------------------
//
//
// AABB�{�b�N�X���m�̏Փ˔���ƕ␳�T���v��
//
//
//-----------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> boxA = nullptr;
Shared<dxe::Mesh> boxB = nullptr;

float jump_vel = 0;

const float BOX_SIZE = 100;
tnl::Vector3 box_size_A = { BOX_SIZE, BOX_SIZE, BOX_SIZE };
tnl::Vector3 box_size_B = { BOX_SIZE, BOX_SIZE, BOX_SIZE };


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	boxA = dxe::Mesh::CreateCubeMV(BOX_SIZE);
	boxA->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	boxA->setPosition( { 100, 0, 0 } );

	boxB = boxA->createClone();
	boxB->setPosition( { -100, 0, 0 } );

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------

	//
	// XZ���ʏ�̈ړ�����
	//
	tnl::Vector3 before_posA = boxA->getPosition();
	tnl::Vector3 before_posB = boxB->getPosition();

	tnl::Input::RunIndexKeyDown(
		[&](uint32_t index) {
			tnl::Vector3 v[4] = {
				tnl::Vector3::left,
				tnl::Vector3::right,
				tnl::Vector3::forward,
				tnl::Vector3::back
			};
			boxA->addPosition( v[index] * 3.0f );
		}, eKeys::KB_LEFT, eKeys::KB_RIGHT, eKeys::KB_UP, eKeys::KB_DOWN);


	//
	// �W�����v�Ɨ�������
	//
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) {
		jump_vel = 15.0f;
	}

	boxA->addPosition({ 0, jump_vel, 0 });

	if (boxA->getPosition().y <= 0) {
		boxA->setPosition( { boxA->getPosition().x, 0, boxA->getPosition().z });
		jump_vel = 0;
	}
	else {
		jump_vel -= 0.98f;
	}

	//
	// �Փ˔���ƕ␳����
	//
	if (tnl::IsIntersectAABB(boxA->getPosition(), box_size_A, boxB->getPosition(), box_size_B)) {

		//----------------------------------------------------------------------------------------------
		// AABB ���m�̍��W�␳
		// arg1.2.		A, B �̈ړ��O�̍��W
		// arg3.4.		A, B �̃T�C�Y
		// arg5.6.		A, B �̈ړ���̍��W ( ���̊֐��ɂ���ĕ␳��̍��W�ɍX�V )
		// arg7.8.9.	��, �c, �� �����̕␳�^�C�v
		// .......		[ PWRFL_A : A �� ���� ] [ PWRFL_B : B �� ���� ] [ BOTH : A �� B �͉������� ]
		// arg10..		�␳��� A �� B �̊Ԃɐ݂����� ( �f�t�H���g 1.0f )
		// ret....		��` A �� B �ɑ΂��Ăǂ̕����֕␳���ꂽ��
		tnl::Vector3 posA = boxA->getPosition();
		tnl::Vector3 posB = boxB->getPosition();
		tnl::eCorrResAABB n = tnl::CorrectPositionAABB(
			before_posA
			, before_posB
			, box_size_A
			, box_size_B
			, posA
			, posB
			, tnl::eCorrTypeAABB::PWRFL_A
			, tnl::eCorrTypeAABB::PWRFL_B
			, tnl::eCorrTypeAABB::PWRFL_B, 0.1f);

		boxA->setPosition(posA);
		boxB->setPosition(posB);

		// ��ɕ␳���ꂽ��W�����v�̓��Z�b�g
		if (tnl::eCorrResAABB::UP == n) jump_vel = 0;
	}


	camera->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	DrawGridGround(camera, 50, 20);

	boxA->render(camera);
	boxB->render(camera);

	DrawDefaultLightGuiController();

	DrawStringEx(0, 0, -1, "�J�[�\�� �������� �ړ� Z �ŃW�����v");

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}
//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}

*/

