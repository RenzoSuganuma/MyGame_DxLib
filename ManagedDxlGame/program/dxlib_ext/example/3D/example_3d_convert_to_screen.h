#pragma once

/*

//-----------------------------------------------------------------------------------------------------------
//
//
// 3次元座標をスクリーン座標へ変換するサンプル ( キャラクタのHPゲージなどの UI に利用しましょう )
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
Shared<dxe::Mesh> box = nullptr;

// オブジェクト周回移動用
float time_count = 0;
Shared<tnl::PointsLerp> lerp_points = nullptr;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	box = dxe::Mesh::CreateCubeMV(100);
	box->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	box->setPosition( { -100, 0, 0 } );


	std::vector<tnl::Vector3> points;
	points.emplace_back(tnl::Vector3(-300, 0, 300));
	points.emplace_back(tnl::Vector3(300, 0, 300));
	points.emplace_back(tnl::Vector3(300, 0, -300));
	points.emplace_back(tnl::Vector3(-300, 0, -300));
	points.emplace_back(tnl::Vector3(-300, 0, 300));
	lerp_points = std::make_shared<tnl::PointsLerp>(points);
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------
	time_count += delta_time * 0.1f;
	box->setPosition( lerp_points->getPosition(time_count) );

	tnl::Vector3 screen_pos = tnl::Vector3::ConvertToScreen(
		{ box->getPosition().x, 100, box->getPosition().z }
		, DXE_WINDOW_WIDTH
		, DXE_WINDOW_HEIGHT
		, camera->view_
		, camera->proj_);

	camera->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	DrawGridGround(camera, 50, 20);

	box->render(camera);

	DrawStringEx(screen_pos.x, screen_pos.y, -1, "test string");

	DrawDefaultLightGuiController();

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}


*/
