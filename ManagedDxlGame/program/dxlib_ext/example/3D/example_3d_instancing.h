#pragma once

/*


//----------------------------------------------------------------------------------------------------------------------------------------------------
//
//
// �C���X�^���V���O�ƃO���[�v���b�V���̃T���v��
//
// �� draw �R�[�������炷�ׁA�����`��̃��b�V�����ʂɂP�x�ɕ`�悵�܂�
//
// �� [ �C���X�^���V���O�̓��� ]
// �@�@�E���b�V���̂P�P�𓮓I�ɓ����������\
//�@�@ �E���b�V���͂��ꂼ��ʂ̃e�N�X�`�����w��\
// �@�@�E�Օ����Ƃ��ăV���h�E�}�b�v �� reserve�ŗ��p�ł��܂����A���̃��b�V�����̂ɉe�𗎂Ƃ����Ƃ͂ł��܂��� ( DxLib �Ƃ̌��ˍ��� )
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
// �� [ �O���[�v���b�V���̓��� ]
// �@�@�E�������Ɏp����z�u�����߁A�ȍ~�͌ʂɓ��������͂ł��܂���
//�@�@ �E�O���[�v�ɂȂ��Ă��郁�b�V���ł͓����e�N�X�`�������L���܂�
// �@�@�E�ʏ�̃��b�V���Ɠ��l�̕`����@ ( render ���邾�� )�ŏ����V���h�E�}�b�v�ł��g�p�ł��܂� ( �e�𗎂Ƃ����Ƃ��\ )
//
//
//
// �� �C���X�^���V���O �� �O���[�v���b�V�����ʂ̃}�e���A���͐ݒ�ł��܂��� ( �S�͉̂\ )
//
//----------------------------------------------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"



Shared<dxe::Camera> camera = nullptr;


// �������ɂȂ郁�b�V��
Shared<dxe::Mesh> origine_mesh = nullptr;

// �O���[�v���b�V��
Shared<dxe::Mesh> group_mesh = nullptr;

// �C���X�^���V���O���b�V���v�[��
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// ���b�V���v�[�����瓾����C���X�^���V���O���b�V��
std::list<Shared<dxe::InstMesh>> inst_meshs ;



//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
    srand(time(0));

    SetFogEnable(true);
    SetFogColor(32, 32, 32);
    SetFogStartEnd(500.0f, 1500.0f);
    ChangeLightTypeDir(VGet(0.0f, -1.0f, 1.0f));
    SetBackgroundColor(32, 32, 32);


    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
    camera->pos_ = { 0, 100, -250 };


    // �������ɂȂ郁�b�V�����쐬
    // �� MV�t�@�C���ȊO�ɂ��{�b�N�X��X�t�B�A���ł��\
    //    MV �t�@�C���̏ꍇ�A�����̃p�[�c�ō\������Ă��Ă��ŏ��̃T�u�Z�b�g�̂݃C���X�^���V���O�E�O���[�v���ł��܂�
    origine_mesh = dxe::Mesh::CreateFromFileMV("mesh/mv/test.mv");
    origine_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // �O���[�v���b�V���Ƃ��č쐬����ꍇ
    //
    std::vector<tnl::Matrix> mats;
    mats.emplace_back(tnl::Matrix::Translation(-100, 0, 100));
    mats.emplace_back(tnl::Matrix::Translation(-200, 0, 200));
    mats.emplace_back(tnl::Matrix::Translation(-300, 0, 300));
    mats.emplace_back(tnl::Matrix::AffineTransformation({-400, 50, 400}, {2, 2, 2}, tnl::Quaternion::RotationAxis({1, 0, 0}, tnl::ToRadian(90))));

    group_mesh = dxe::Mesh::CreateStaticMeshGroupMV(origine_mesh, mats);



    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // �C���X�^���V���O�v�[���Ƃ��č쐬����ꍇ
    //
    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 3, &textures);

    // �C���X�^���V���O�v�[������ʂ̃��b�V�����擾
    for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
        auto inst = mesh_pool->CreateInstMesh();
        inst->setPosition({ float(i * 50), 100, float(i * 50) });
        inst->setRotation(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)));

        // �g�p����e�N�X�`���ԍ���ݒ�
        // MeshPool �� Create �֐��ɓn���ꂽ�e�N�X�`���z��ԍ��Ŏw�肵�܂�
        inst->setUseTextureIndex( i % textures.size() );

        inst_meshs.emplace_back(inst);
    }

}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {


    // �C���X�^���X���b�V���𑀍�( �Ⴆ�Ή�] )
    for (auto mesh : inst_meshs) {
        mesh->setRotation( mesh->getRotation()* tnl::Quaternion::RotationAxis({0, 1, 0}, tnl::ToRadian(1)) );
    }


    camera->update();
    DrawGridGround(camera, 50, 20);


    // �O���[�v���b�V���̕`��( �ʏ�̃��b�V���Ɠ��l�̕`����@ )
    group_mesh->render(camera);



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
