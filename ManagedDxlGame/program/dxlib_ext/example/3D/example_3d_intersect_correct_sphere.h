#pragma once

/*



//-----------------------------------------------------------------------------------------------------------
//
//
// 球同士の衝突判定と補正サンプル
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
Shared<dxe::Mesh> sphereA = nullptr;
Shared<dxe::Mesh> sphereB = nullptr;
const float RADIUS = 50;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	sphereA = dxe::Mesh::CreateSphereMV(RADIUS);
	sphereA->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sphereA->setPosition( { 100, 0, 0 } );

	sphereB = sphereA->createClone();
	sphereB->setPosition( {-100, 0, 0} );

}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------
	tnl::Input::RunIndexKeyDown(
		[&](uint32_t index) {
			tnl::Vector3 v[4] = {
				tnl::Vector3::left,
				tnl::Vector3::right,
				tnl::Vector3::forward,
				tnl::Vector3::back
			};
			sphereA->addPosition( v[index] * 3.0f );
		}, eKeys::KB_LEFT, eKeys::KB_RIGHT, eKeys::KB_UP, eKeys::KB_DOWN);

	// 衝突判定と補正処理
	tnl::Vector3 posA = sphereA->getPosition();
	tnl::Vector3 posB = sphereB->getPosition();

	if (tnl::IsIntersectSphere(posA, RADIUS, posB, RADIUS)) {
		tnl::Vector3 center = (posA + posB) / 2.0f;
		tnl::Vector3 toA = tnl::Vector3::Normalize(posA - posB);
		tnl::Vector3 toB = tnl::Vector3::Normalize(posB - posA);
		sphereA->setPosition( center + (toA * RADIUS) ) ;
		sphereB->setPosition( center + (toB * RADIUS) ) ;
	}

	camera->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	DrawGridGround(camera, 50, 20);

	sphereA->render(camera);
	sphereB->render(camera);

	DrawDefaultLightGuiController();

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}




*/