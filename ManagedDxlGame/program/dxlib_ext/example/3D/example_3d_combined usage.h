#pragma once

//-----------------------------------------------------------------------------------------------------------
//
//
// 特殊な使い方をするクラスを複合的に使用するサンプル
// シャドウマップ・スクリーンエフェクト・パーティクル・インスタンシング の複合使用
//
//
//-----------------------------------------------------------------------------------------------------------

/*

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> origin = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::InstMeshPool> mesh_pool = nullptr;
Shared<dxe::Particle> ptcl = nullptr;
Shared<dxe::ScreenEffect> screen_efct = nullptr;
Shared<dxe::ShadowMap> shadow = nullptr;

std::list<Shared<dxe::InstMesh>> inst_meshs;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	camera->pos_ = { 0, 200, -500 };

	origin = dxe::Mesh::CreateCubeMV(50, 20, 20);
	origin->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	origin->setPosition({ 100, 100, 0 });

	ground = dxe::Mesh::CreatePlaneMV({ 2000, 2000, 0 }, 20, 20);
	ground->setTexture(dxe::Texture::CreateFromFile("graphics/lawn.png"));
	ground->setRotation(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)));

	// インスタンシングプール
	std::vector<Shared<dxe::Texture>> textures;
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));
	mesh_pool = dxe::InstMeshPool::Create(origin, 3, &textures);

	// インスタンシングプールから個別のメッシュを取得
	for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
		auto inst = mesh_pool->CreateInstMesh();
		inst->setPosition({ float(i * 100), 100, float(i * 50) });

		// 使用するテクスチャ番号を設定
		// MeshPool の Create 関数に渡されたテクスチャ配列番号で指定します
		inst->setUseTextureIndex(i % textures.size());

		inst->setDrawEnable(true);
		inst_meshs.emplace_back(inst);
	}

	// パーティクル
	ptcl = std::make_shared<dxe::Particle>("particle/preset/default.bin");


	// シャドウマップ作成
	shadow = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);

	// ブルームエフェクト作成
	screen_efct = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);


}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	// インスタンスメッシュを操作( 例えば回転 )
	for (auto mesh : inst_meshs) {
		mesh->setRotation(mesh->getRotation() * tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1)));
	}


	camera->update();

	//-------------------------------------------------------------------------------------
	//
	// シャドウマップ利用の準備
	//
	// ※ 囲った処理がわかりやすいようにインデントしています
	//

	// シャドウマップへの描画開始
	shadow->reserveBegin();

		mesh_pool->reserveShadow();
		ground->reserveShadow();

	// 終了
	shadow->reserveEnd();


	//-------------------------------------------------------------------------------------
	//
	// シャドウマップ・スクリーンエフェクト・インスタンシング・パーティクル適用
	//
	// ※ 囲った処理がわかりやすいようにインデントしています
	//

	// シャドウマップとスクリーンエフェクト適用開始
	screen_efct->renderBegin();
	shadow->renderBegin();

	// 通常メッシュ描画
	ground->render(camera);


	// DirectX 専用描画開始
	dxe::DirectXRenderBegin();

		mesh_pool->render(camera);
		ptcl->render(camera);

	// DirectX 専用描画終了
	dxe::DirectXRenderEnd();


	// シャドウマップとスクリーンエフェクト適用終了
	shadow->renderEnd();
	screen_efct->renderEnd();


	// デバッグ情報描画
	DrawDefaultLightGuiController();
	screen_efct->drawGuiController({ 0, 0 });

	DrawFpsIndicator({ 0, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
	ptcl.reset();
	mesh_pool.reset();
}


*/
