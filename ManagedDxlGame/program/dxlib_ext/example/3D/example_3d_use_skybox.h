#pragma once

/*

//-----------------------------------------------------------------------------------------------------------
//
// スカイボックス利用サンプル
//
// ※ フォグを利用する場合はスカイボックスを描画した後に設定しましょう
// ※ スカイボックスは大きく作るのでカメラの far 設定次第では描画されなくなるので注意
//
//-----------------------------------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> skybox = nullptr;
dxe::GuiMenuSelector* gui_menu_selector = nullptr;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));
	SetBackgroundColor(32, 32, 32);
	SetDefaultLightParameter("directional_light_parameter.bin");

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	skybox = dxe::Mesh::CreateCubeMV(30000, 20, 20);
	skybox->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_a.png"));
	skybox->loadMaterial("material.bin");

	gui_menu_selector = new dxe::GuiMenuSelector(
		[&](uint32_t select_index) {

			std::string path[5] = {
				"graphics/skybox/_skybox_a.png",
				"graphics/skybox/_skybox_b.png",
				"graphics/skybox/_skybox_c.png"
			};
			skybox->setTexture(dxe::Texture::CreateFromFile(path[select_index]));


		}, "skybox_a", "skybox_b", "skybox_c"
	);
	gui_menu_selector->setPosition({ 330, 0 });
	gui_menu_selector->setLocation(dxe::GuiMenuSelector::eLocation::DOWN);
}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	//-------------------------------------------------------------------------------
	//
	// update
	//
	//-------------------------------------------------------------------------------

	skybox->mulRotation( tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f)) );

	camera->update();
	gui_menu_selector->update();


	//-------------------------------------------------------------------------------
	//
	// draw
	//
	//-------------------------------------------------------------------------------

	skybox->render(camera);

	DrawGridGround(camera, 50, 20);
	DrawDefaultLightGuiController();

	skybox->drawGuiMaterialControlloer();

	gui_menu_selector->draw();

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/
