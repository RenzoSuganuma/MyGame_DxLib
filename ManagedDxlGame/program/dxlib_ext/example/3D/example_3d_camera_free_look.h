#pragma once
/*

//---------------------------------------------------------------------------------------------------------------
//
//
// フリールックカメラサンプル
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
// 注視点までの「 距離 」「 Y軸角度 」「 X軸角度 」で制御するカメラ
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
	// フリールックの制御パラメータからカメラ座標を計算
	//

	// Y軸回転した姿勢を作成
	tnl::Quaternion  q = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, axis_y_angle_);

	// 原点を基準として、単位ベクトルをY軸回転だけをした結果を xz 平面に計算
	tnl::Vector3 xz = tnl::Vector3::TransformCoord({ 0, 0, 1 }, q);

	// xz と真上のベクトルで外積を計算し、姿勢 q のローカルなX軸を計算
	tnl::Vector3 local_axis_x = tnl::Vector3::Cross({ 0, 1, 0 }, xz);

	// Y軸姿勢に対し、さらにローカルX軸で回転をかける
	// 原点からカメラ座標方向へ向く姿勢ができる
	q *= tnl::Quaternion::RotationAxis(local_axis_x, axis_x_angle_);

	// 姿勢方向への距離座標をターゲットの座標に足すことでカメラ座標となる
	pos_ = target_ + tnl::Vector3::TransformCoord({ 0, 0, -to_target_distance_ }, q);

	// ベースカメラのアップデート
	// ビュー行列・プロジェクション行列の更新
	dxe::Camera::update();
}




Shared<FreeLookCamera> camera = nullptr;
Shared<dxe::Mesh> mesh = nullptr;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
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
// 毎フレーム実行されます
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
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/