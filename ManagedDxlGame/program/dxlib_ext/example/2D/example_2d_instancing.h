#pragma once
//----------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���X�^���V���O�ƃO���[�v���b�V���̃T���v��
//
// �� draw �R�[�������炷�ׁA�����`��̃��b�V�����ʂɂP�x�ɕ`�悵�܂�
//
// �� [ �C���X�^���V���O�̓��� ]
// �@�@�E���b�V���̂P�P�𓮓I�ɓ����������\
//�@�@ �E���b�V���͂��ꂼ��ʂ̃e�N�X�`�����w��\
//
//      ���@[ �d�v ]�@std::shared_ptr �Ŋm�ۂ��� dxe::InstMeshPool �͕K�� gameEnd() ��
// �@�@               �^�C�~���O�܂łɎQ�ƃJ�E���^�� 0 �ɂȂ�悤���Z�b�g���Ă������� ( gameEnd �֐����Q�� )
//
//      ���@[ �d�v ]�@���̋@�\�� DxLib �̋@�\�ł͂���܂���
// �@�@               dxe::Particle �� dxe::InstMeshPool �Ȃ� DirectX �𒼐ڐ��䂷��N���X�� render �֐���
//�@�@                dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() �ň͂������ŃR�[�����Ă�������
//
//
//
// �� �C���X�^���V���O �� �O���[�v���b�V�����ʂ̃}�e���A���͐ݒ�ł��܂��� ( �S�͉̂\ )
//
//----------------------------------------------------------------------------------------------------------------------------------------------------

/*

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"



// �J����
Shared<dxe::Camera> camera = nullptr;

// �������ɂȂ郁�b�V��
Shared<dxe::Mesh> origine_mesh = nullptr;

// �C���X�^���V���O���b�V���v�[��
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// ���b�V���v�[�����瓾����C���X�^���V���O���b�V��
std::list<Shared<dxe::InstMesh>> inst_meshs;



//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {

    srand(time(0));

    SetBackgroundColor(32, 32, 32);


    // 2D �p�J�����̍쐬
    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dxe::Camera::eDimension::Type2D);


    // �������ɂȂ郁�b�V�����쐬
    // �� MV�t�@�C���ȊO�ɂ��{�b�N�X��X�t�B�A���ł��\
    //    MV �t�@�C���̏ꍇ�A�����̃p�[�c�ō\������Ă��Ă��ŏ��̃T�u�Z�b�g�̂݃C���X�^���V���O�E�O���[�v���ł��܂�
    origine_mesh = dxe::Mesh::CreatePlaneMV({ 25, 25, 0 }, 1, 1);


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // �C���X�^���V���O�v�[���Ƃ��č쐬
    //
    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 50*50, &textures);
    mesh_pool->setMtrlEmissive({ 1, 1, 1 });

    int sx = -DXE_WINDOW_WIDTH / 2;
    int sy = -DXE_WINDOW_HEIGHT / 2;

    // �쐬�������b�V������ׂ� & ���X�g�ɕۑ�
    for(int i = 0 ; i < 50 ; ++i){
        sx = -DXE_WINDOW_WIDTH / 2;
        for (int k = 0; k < 50; ++k) {
            auto inst = mesh_pool->CreateInstMesh();
            inst->setPosition({ (float)sx, (float)sy, 0 });
            sx += 25;
            // �g�p����e�N�X�`���ԍ���ݒ�
            // MeshPool �� Create �֐��ɓn���ꂽ�e�N�X�`���z��ԍ��Ŏw�肵�܂�
            inst->setUseTextureIndex( rand() % 3 );

            inst_meshs.emplace_back(inst);
        }
        sy += 25;
    }

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {


    //----------------------------------------------------------------------------------------------------
    //
    // �J��������
    //
    tnl::Input::RunIndexKeyDown(
        [&](uint32_t index) {
            tnl::Vector3 v[4] = {
                camera->left(),
                camera->right(),
                tnl::Vector3::up,
                tnl::Vector3::down
            };
            camera->pos_ += v[index] * 3.0f;

        }, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);

    // �C���X�^���X���b�V���𑀍�( �Ⴆ�Ή�] )
    //for (auto mesh : inst_meshs) {
    //    mesh->setRotation(mesh->getRotation() * tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(1)));
    //}


    camera->update();


    // �C���X�^���V���O�v�[���̕`��( DirectXRenderBegin, DirectXRenderEnd �ň͂� )
    dxe::DirectXRenderBegin();

    mesh_pool->render(camera);

    dxe::DirectXRenderEnd();


    DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
    mesh_pool.reset();
}

*/


