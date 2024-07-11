#pragma once

//-----------------------------------------------------------------------------------------------------------
//
//
// ����Ȏg����������N���X�𕡍��I�Ɏg�p����T���v��
// �V���h�E�}�b�v�E�X�N���[���G�t�F�N�g�E�p�[�e�B�N���E�C���X�^���V���O �̕����g�p
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
// �Q�[���N�����ɂP�x�������s����܂�
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

	// �C���X�^���V���O�v�[��
	std::vector<Shared<dxe::Texture>> textures;
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
	textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));
	mesh_pool = dxe::InstMeshPool::Create(origin, 3, &textures);

	// �C���X�^���V���O�v�[������ʂ̃��b�V�����擾
	for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
		auto inst = mesh_pool->CreateInstMesh();
		inst->setPosition({ float(i * 100), 100, float(i * 50) });

		// �g�p����e�N�X�`���ԍ���ݒ�
		// MeshPool �� Create �֐��ɓn���ꂽ�e�N�X�`���z��ԍ��Ŏw�肵�܂�
		inst->setUseTextureIndex(i % textures.size());

		inst->setDrawEnable(true);
		inst_meshs.emplace_back(inst);
	}

	// �p�[�e�B�N��
	ptcl = std::make_shared<dxe::Particle>("particle/preset/default.bin");


	// �V���h�E�}�b�v�쐬
	shadow = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);

	// �u���[���G�t�F�N�g�쐬
	screen_efct = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);


}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	// �C���X�^���X���b�V���𑀍�( �Ⴆ�Ή�] )
	for (auto mesh : inst_meshs) {
		mesh->setRotation(mesh->getRotation() * tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1)));
	}


	camera->update();

	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v���p�̏���
	//
	// �� �͂����������킩��₷���悤�ɃC���f���g���Ă��܂�
	//

	// �V���h�E�}�b�v�ւ̕`��J�n
	shadow->reserveBegin();

		mesh_pool->reserveShadow();
		ground->reserveShadow();

	// �I��
	shadow->reserveEnd();


	//-------------------------------------------------------------------------------------
	//
	// �V���h�E�}�b�v�E�X�N���[���G�t�F�N�g�E�C���X�^���V���O�E�p�[�e�B�N���K�p
	//
	// �� �͂����������킩��₷���悤�ɃC���f���g���Ă��܂�
	//

	// �V���h�E�}�b�v�ƃX�N���[���G�t�F�N�g�K�p�J�n
	screen_efct->renderBegin();
	shadow->renderBegin();

	// �ʏ탁�b�V���`��
	ground->render(camera);


	// DirectX ��p�`��J�n
	dxe::DirectXRenderBegin();

		mesh_pool->render(camera);
		ptcl->render(camera);

	// DirectX ��p�`��I��
	dxe::DirectXRenderEnd();


	// �V���h�E�}�b�v�ƃX�N���[���G�t�F�N�g�K�p�I��
	shadow->renderEnd();
	screen_efct->renderEnd();


	// �f�o�b�O���`��
	DrawDefaultLightGuiController();
	screen_efct->drawGuiController({ 0, 0 });

	DrawFpsIndicator({ 0, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
	ptcl.reset();
	mesh_pool.reset();
}


*/
