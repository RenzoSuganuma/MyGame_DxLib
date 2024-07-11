#pragma once
/*

//---------------------------------------------------------------------------------------------------------------
//
//
// �t���[���b�N�J�����T���v��
//
//
//---------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


//-------------------------------------------------------------------------------------------------------
//
// �����_�܂ł́u ���� �v�u Y���p�x �v�u X���p�x �v�Ő��䂷��J����
//
class FreeLookCamera : public dxe::Camera {
public:
	FreeLookCamera() {};
	FreeLookCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h) {}

	void update();

	float to_target_distance_ = 500;
	float axis_x_angle_ = tnl::ToRadian(45);
	float axis_y_angle_ = 0;
};


void FreeLookCamera::update() {

	//
	// �t���[���b�N�̐���p�����[�^����J�������W���v�Z
	//

	// Y����]�����p�����쐬
	tnl::Quaternion  q = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, axis_y_angle_);

	// ���_����Ƃ��āA�P�ʃx�N�g����Y����]�������������ʂ� xz ���ʂɌv�Z
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0, 0, 1 }, q);

	// xz �Ɛ^��̃x�N�g���ŊO�ς��v�Z���A�p�� q �̃��[�J����X�����v�Z
	tnl::Vector3 local_axis_x = tnl::Vector3::Cross({ 0, 1, 0 }, xz);

	// Y���p���ɑ΂��A����Ƀ��[�J��X���ŉ�]��������
	// ���_����J�������W�����֌����p�����ł���
	q *= tnl::Quaternion::RotationAxis(local_axis_x, axis_x_angle_);

	// �p�������ւ̋������W���^�[�Q�b�g�̍��W�ɑ������ƂŃJ�������W�ƂȂ�
	pos_ = target_ + tnl::Vector3::TransformCoord({ 0, 0, -to_target_distance_ }, q);

	// �x�[�X�J�����̃A�b�v�f�[�g
	// �r���[�s��E�v���W�F�N�V�����s��̍X�V
	dxe::Camera::update();
}




Shared<FreeLookCamera> camera = nullptr;
Shared<dxe::Mesh> mesh = nullptr;

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	SetFogEnable(true);
	SetFogColor(32, 32, 32);
	SetFogStartEnd(500.0f, 1500.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);

	camera = std::make_shared<FreeLookCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	mesh = dxe::Mesh::CreateSphereMV(100, 20, 20);
	mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	tnl::Input::RunIndexKeyDown(
		[&](uint32_t index) {
			tnl::Vector3 v[4] = {
				camera->left(),
				camera->right(),
				camera->forward().xz(),
				camera->back().xz()
			};
			mesh->addPosition(v[index] * 3.0f);
			camera->target_ = mesh->getPosition() ;

		}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);


	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT))	camera->axis_y_angle_ += tnl::ToRadian(1);
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) camera->axis_y_angle_ -= tnl::ToRadian(1);
	if (tnl::Input::IsKeyDown(eKeys::KB_UP))	camera->axis_x_angle_ += tnl::ToRadian(1);
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN))	camera->axis_x_angle_ -= tnl::ToRadian(1);

	if (tnl::Input::IsKeyDown(eKeys::KB_Z))	camera->to_target_distance_ += 1;
	if (tnl::Input::IsKeyDown(eKeys::KB_X))	camera->to_target_distance_ -= 1;


	camera->update();

	DrawGridGround(camera, 50, 20);

	mesh->render(camera);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}

*/