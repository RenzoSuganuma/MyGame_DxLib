#pragma once

/*

//---------------------------------------------------------------------------------------------------------------
//
//
// �ړ��x�N�g���Ǝ��ʂƖ��C����I�u�W�F�N�g�ɊȈՓI�ȉ��S�͂Ɗ����̕\�����s���T���v���R�[�h�ł�
//
//
//---------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


Shared<dxe::Camera> camera = nullptr;

class Car {
public:
	Car() {
		mesh_ = dxe::Mesh::CreateCubeMV(100);
		mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
		mesh_->setScaling({ 0.7f, 0.5f, 1.0f });
	}

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	float centroid_radius_ = 20;
	float mass_ = 98;
	float friction_ = 0.55f;
	tnl::Vector3 move_vel_;
	tnl::Vector3 past_move_vel_;
	tnl::Vector3 center_of_gravity_;
	tnl::Quaternion rot_y_;
	tnl::Quaternion rot_xz_;
	Shared<dxe::Mesh> mesh_ = nullptr;

};


void Car::update(float delta_time) {

	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-2));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_y_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(2));
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		move_vel_ += tnl::Vector3::TransformCoord({ 0, 0, 1.0f }, rot_y_);
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		move_vel_ -= tnl::Vector3::TransformCoord({ 0, 0, 0.3f }, rot_y_);
	}


	tnl::EasyAdjustObjectVelocity(centroid_radius_, mass_, friction_, past_move_vel_, move_vel_, center_of_gravity_);

	mesh_->addPosition(move_vel_);

	//--------------------------------------------------------------------------------------------------------------
	// �p���v�Z
	//

	if (center_of_gravity_.length() > FLT_EPSILON) {
		// �d�S�ʒu�𗘗p���ČX���Ăق����A�b�p�[�x�N�g�����쐬
		tnl::Vector3 upper = tnl::Vector3::Normalize({ center_of_gravity_.x, centroid_radius_, center_of_gravity_.z });
		// �X���̊p�x���v�Z
		float angle = upper.angle({ 0, 1, 0 });
		// �X���x�N�g���Ɛ^��x�N�g���̊O�ς����]�����v�Z���A�X���p�x�𒲐����ĉ�]�N�H�[�^�j�I�����쐬
		rot_xz_ = tnl::Quaternion::RotationAxis(tnl::Vector3::Cross(upper, { 0, 1, 0 }), -(angle * 0.2f));
	}

	// �ŏI�I�Ȏp��
	mesh_->setRotation(rot_y_ * rot_xz_);

}


void Car::draw(Shared<dxe::Camera> camera) {
	mesh_->render(camera);
}


Shared<Car> car = nullptr;

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	SetFogEnable(true);
	SetFogColor(32, 32, 32);
	SetFogStartEnd(500.0f, 1500.0f);
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	car = std::make_shared<Car>();

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	//--------------------------------------------------------------------------
	// �Q�[�����W�b�N
	//

	car->update(delta_time);

	camera->target_ = car->mesh_->getPosition();

	camera->update();


	//--------------------------------------------------------------------------
	// �`��
	//

	DrawGridGround(camera, 50, 20);

	car->draw(camera);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}



*/
