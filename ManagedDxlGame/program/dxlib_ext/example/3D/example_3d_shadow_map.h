#pragma once
/*

//---------------------------------------------------------------------------------------------------------------
//
//
// �V���h�E�}�b�v�T���v��
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
Shared<dxe::Mesh> object = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::ShadowMap> shadow = nullptr;

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

	object = dxe::Mesh::CreateSphereMV(100, 20, 20);
	object->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	object->setPosition( { 0, 100, 0 } );

	ground = dxe::Mesh::CreatePlaneMV({ 2000, 2000, 0 }, 20, 20);
	ground->setTexture(dxe::Texture::CreateFromFile("graphics/lawn.png"));
	ground->setRotation( tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)) );

	// �V���h�E�}�b�v�쐬
	shadow = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	tnl::Vector3 pos = object->getPosition();

	if (tnl::Input::IsKeyDown(eKeys::KB_A)) pos.x -= 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) pos.x += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) pos.z += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) pos.z -= 5;

	if (tnl::Input::IsKeyDown(eKeys::KB_UP))	pos.y += 5;
	if (tnl::Input::IsKeyDown(eKeys::KB_DOWN))	pos.y -= 5;

	object->setPosition( pos );

	camera->update();



	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v���p�̏���
	//

	// �V���h�E�}�b�v�ւ̕`��J�n
	shadow->reserveBegin();

	ground->reserveShadow();
	object->reserveShadow();

	// �I��
	shadow->reserveEnd();



	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v��K�p�����`��
	//

	// �K�p�J�n
	shadow->renderBegin();

	object->render(camera);
	ground->render(camera);

	// �I��
	shadow->renderEnd();


	ground->drawGuiMaterialControlloer();

	DrawDefaultLightGuiController();

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

}


*/