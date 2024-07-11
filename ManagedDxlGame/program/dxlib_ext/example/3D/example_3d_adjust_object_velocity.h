#pragma once

/*

//---------------------------------------------------------------------------------------------------------------
//
//
// 移動ベクトルと質量と摩擦からオブジェクトに簡易的な遠心力と慣性の表現を行うサンプルコードです
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
	// 姿勢計算
	//

	if (center_of_gravity_.length() > FLT_EPSILON) {
		// 重心位置を利用して傾いてほしいアッパーベクトルを作成
		tnl::Vector3 upper = tnl::Vector3::Normalize({ center_of_gravity_.x, centroid_radius_, center_of_gravity_.z });
		// 傾きの角度を計算
		float angle = upper.angle({ 0, 1, 0 });
		// 傾きベクトルと真上ベクトルの外積から回転軸を計算し、傾き角度を調整して回転クォータニオンを作成
		rot_xz_ = tnl::Quaternion::RotationAxis(tnl::Vector3::Cross(upper, { 0, 1, 0 }), -(angle * 0.2f));
	}

	// 最終的な姿勢
	mesh_->setRotation(rot_y_ * rot_xz_);

}


void Car::draw(Shared<dxe::Camera> camera) {
	mesh_->render(camera);
}


Shared<Car> car = nullptr;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
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
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//--------------------------------------------------------------------------
	// ゲームロジック
	//

	car->update(delta_time);

	camera->target_ = car->mesh_->getPosition();

	camera->update();


	//--------------------------------------------------------------------------
	// 描画
	//

	DrawGridGround(camera, 50, 20);

	car->draw(camera);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}



*/
