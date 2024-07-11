#pragma once

/*

//------------------------------------------------------------------------------------------------
//
// Height Map ����n�`���쐬����T���v��
//
//------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> ground = nullptr;


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	ground = dxe::Mesh::CreateFromHeightMapMV("graphics/height_map.png", 10000, 10000, 100, 500, 500);
	ground->setTexture(dxe::Texture::CreateFromFile("graphics/height_map.png"));

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	ground->mulRotation( tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f)) );

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------
	camera->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	ground->render(camera);
	ground->drawGuiMaterialControlloer();

	DrawGridGround(camera, 50, 20);

	DrawDefaultLightGuiController();

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}


*/