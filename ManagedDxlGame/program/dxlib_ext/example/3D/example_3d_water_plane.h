#pragma once

/*

//----------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���ʃG�t�F�N�g ���p�T���v��
//
// ���@���̃T���v���͐��ʃG�f�B�^�Ƃ��Ă��@�\���܂�
//
// ���@���̃T���v���ł͐��ʂɑ΂��đ��̃I�u�W�F�N�g�𔽉f�������T���v���ł��B
// �@�@�f�荞�݂Ƃ��āA�ʏ�I�u�W�F�N�g�E�p�[�e�B�N���E�C���X�^���V���O�v�[�� �𔽉f���܂�
//
//----------------------------------------------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


//-------------------------------------------------------------------------------------------------------
//
// ���W�Ǝp���Ő��䂷��J����
//
class TransformCamera : public dxe::Camera {
public:

    TransformCamera() {};
    TransformCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h) {}

    void update();


    inline tnl::Vector3 up() {
        up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, rot_);
        return up_;
    }
    inline tnl::Vector3 down() { return -up(); }

    inline tnl::Vector3 forward() override {
        target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
        return tnl::Vector3::Normalize(target_ - pos_);
    }
    inline tnl::Vector3 back() override { return -forward(); }
    inline tnl::Vector3 left() override { return tnl::Vector3::Cross(forward(), up()); }
    inline tnl::Vector3 right() override { return tnl::Vector3::Cross(up(), forward()); }

    tnl::Quaternion rot_;

};

void TransformCamera::update() {

    //
    // �p���p�����[�^����^�[�Q�b�g���W�ƃA�b�p�[�x�N�g�����v�Z
    //

    target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
    up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, rot_);
    dxe::Camera::update();
}



Shared<TransformCamera> camera = nullptr;

// ���ʃI�u�W�F�N�g
Shared<dxe::WaterPlane> water = nullptr;

Shared<dxe::Mesh> skybox = nullptr;
Shared<dxe::Mesh> canoe = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::Mesh> bridge = nullptr;
Shared<dxe::Mesh> test_object = nullptr;
std::shared_ptr<dxe::Particle> ptcl = nullptr;


// �������ɂȂ郁�b�V��
Shared<dxe::Mesh> origine_mesh = nullptr;

// �C���X�^���V���O���b�V���v�[��
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// ���b�V���v�[�����瓾����C���X�^���V���O���b�V��
std::list<Shared<dxe::InstMesh>> inst_meshs;


bool is_draw_controller = true;

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {

    srand(time(0));
    ChangeLightTypeDir(VGet(0.0f, -1.0f, 1.0f));
    SetBackgroundColor(32, 32, 32);

    camera = std::make_shared<TransformCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
    camera->pos_ = { 0, 100, -700 };

    // ( �e�X�g�p ) �J�k�[�쐬
    canoe = dxe::Mesh::CreateFromFileMV("mesh/mv/canoe/Canoe.mv1");
    canoe->setScaling( { 10, 10, 10 } );
    canoe->setPosition( { 100, 3, -375 } );

    // ( �e�X�g�p ) �V���쐬
    bridge = dxe::Mesh::CreateFromFileMV("mesh/mv/bridge/WoodenBridge_01.mv1");
    bridge->setScaling( { 0.25f, 0.25f, 0.25f } );
    bridge->setRotation( tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90)) );
    bridge->setPosition( { 100, -10, -450 } );

    // ( �e�X�g�p ) �X�J�C�{�b�N�X�쐬
    skybox = dxe::Mesh::CreateCubeMV(30000, 20, 20);
    skybox->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_b.png"));
    skybox->loadMaterial("material.bin");

    // ( �e�X�g�p ) ���l�쐬
    ground = dxe::Mesh::CreatePlaneMV({ 1000, 300, 0 });
    ground->setTexture(dxe::Texture::CreateFromFile("graphics/sandy_beach.png"));
    ground->mulRotation( tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(95)) );
    ground->setPosition( { 0, 0, -500 } ) ;

    // ( �e�X�g�p ) ���߂��I�u�W�F�N�g���쐬
    test_object = dxe::Mesh::CreateConeMV(50, 100, 20, 20);
    test_object->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
    test_object->setPosition( { 300, -100, 100 } );


    // ���ʃI�u�W�F�N�g���f�[�^����쐬
    water = std::make_shared<dxe::WaterPlane>("water_plane.bin");

    // �J�k�[�͖A�Ƃ��蔲����K�p
    water->addLandingMesh(canoe, dxe::WaterPlane::fLandingMeshUse::BUBLLE | dxe::WaterPlane::fLandingMeshUse::CUT_OUT);

    // �V���͖A��K�p
    water->addLandingMesh(bridge, dxe::WaterPlane::fLandingMeshUse::BUBLLE);

    // ���l�͖A�ƃt�F�[�h��K�p
    water->addLandingMesh(ground, dxe::WaterPlane::fLandingMeshUse::BUBLLE | dxe::WaterPlane::fLandingMeshUse::FADE_ALPHA, 1.0f);

    // ( �e�X�g�p ) �p�[�e�B�N���쐬
    ptcl = std::make_shared<dxe::Particle>("particle/preset/fire.bin");
    ptcl->setPosition({ -50, 5, -450 });



    // �������ɂȂ郁�b�V�����쐬
    // �� MV�t�@�C���ȊO�ɂ��{�b�N�X��X�t�B�A���ł��\
    //    MV �t�@�C���̏ꍇ�A�����̃p�[�c�ō\������Ă��Ă��ŏ��̃T�u�Z�b�g�̂݃C���X�^���V���O�E�O���[�v���ł��܂�
    origine_mesh = dxe::Mesh::CreateFromFileMV("mesh/mv/test.mv");
    origine_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));


    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 3, &textures);

    // �C���X�^���V���O�v�[������ʂ̃��b�V�����擾
    for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
        auto inst = mesh_pool->CreateInstMesh();
        inst->setPosition({ float(i * 50), 20, float(i * 50) });
        inst->setRotation(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)));

        // �g�p����e�N�X�`���ԍ���ݒ�
        // MeshPool �� Create �֐��ɓn���ꂽ�e�N�X�`���z��ԍ��Ŏw�肵�܂�
        inst->setUseTextureIndex(i % textures.size());

        inst_meshs.emplace_back(inst);
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

    if (tnl::Input::IsMouseDown(tnl::Input::eMouse::RIGHT)) {
        tnl::Vector3 mvel = tnl::Input::GetMouseVelocity();
        camera->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(mvel.x * 0.2f));
        camera->rot_ *= tnl::Quaternion::RotationAxis(camera->right(), tnl::ToRadian(mvel.y * 0.2f));
    }
    camera->pos_ += camera->forward().xz() * tnl::Input::GetMouseWheel() * 0.3f;


    if (tnl::Input::IsKeyDown(eKeys::KB_C)) {
        canoe->mulRotation( tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(1)) );
    }
    if (tnl::Input::IsKeyDown(eKeys::KB_V)) {
        canoe->mulRotation( tnl::Quaternion::RotationAxis({ 0,0,-1 }, tnl::ToRadian(1)) );
    }
    if (tnl::Input::IsKeyDown(eKeys::KB_B)) {
        canoe->mulRotation( tnl::Quaternion::RotationAxis({ 0,1,0 }, tnl::ToRadian(1)) ) ;
    }
    skybox->mulRotation( tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0.01f)) );

    // GUI �`�� ON / OFF
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F3)) is_draw_controller = !is_draw_controller;


    //
    // �L���[�u�}�b�v�ւ̏������ݏ���
    //
    tnl::Vector3 cube_map_cam_pos = camera->pos_ * tnl::Vector3(1, -1, 1);
    water->setCubeMapCameraPosition(cube_map_cam_pos);
    water->writeToCubeMap([&](Shared<dxe::Camera>& cube_camera) {
        skybox->render(cube_camera);
        canoe->render(cube_camera);
        bridge->render(cube_camera);

        // �C���X�^���V���O�v�[���̕`��
        // �� �����x�̂���I�u�W�F�N�g( �p�[�e�B�N���� )�����O�ɕ`��
        mesh_pool->render(cube_camera);

        // �L���[�u�}�b�v�ւ̏������݂ł͎��Ԃ�i�߂Ȃ��悤�ɂ��� ( ������ false )
        // �L���[�u�}�b�v�ւ̏������݂ł� DirectXRenderBegin DirectXRenderEnd �͕K�v�Ȃ�
        ptcl->render(cube_camera, false);

        });


    camera->update();



    //----------------------------------------------------------------------------------------------------
    //
    //
    // �ȉ� �`�揈��
    //
    //

    // �X�J�C�{�b�N�X�̓t�H�O�Ȃ�
    SetFogEnable(false);
    skybox->render(camera);


    // �t�H�O�K�p
    SetFogEnable(true);
    SetFogColor(32, 32, 32);
    SetFogStartEnd(500.0f, 1500.0f);


    ground->render(camera);
    bridge->render(camera);
    canoe->render(camera);
    test_object->render(camera);


    //
    // ���ʂ̕`�� + ���p�e�X�g�ׂ̈̃p�[�e�B�N�� & �C���X�^���V���O�`��
    //
    dxe::DirectXRenderBegin();

    mesh_pool->render(camera);
    water->render(camera);
    ptcl->render(camera);

    dxe::DirectXRenderEnd();


    if (is_draw_controller) water->drawGuiController({ 0, 0 });

    DrawFpsIndicator({ DXE_WINDOW_WIDTH - 310, 0, 0 }, delta_time);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(DXE_WINDOW_WIDTH - 270, 15, DXE_WINDOW_WIDTH, 130, 0, true);
    DrawString(DXE_WINDOW_WIDTH - 260, 20, "�E�N���b�N : ����", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 40, "�z�C�[�� : �O��", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 60, "A D : ���E�ړ�", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 80, "W S : �㉺�ړ�", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 100, "F3 : GUI �`�� ON/OFF", -1);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


    //
    // �f�o�b�O�p�@�����_�[�e�N�X�`���̕`��
    //
    //auto ltx = water->getLandingCutOutTexture();
    //auto srv = ltx->getSrvTexture();
    //srv->getRect().setScaleFromSize(200);
    //srv->getRect().setPosition(dxe::WINDOW_RECT.getOtherPosition(tnl::Rect::eLocation::IN_RIGHT_BOTTOM, srv->getRect()));
    //srv->drawShaderResouceView(srv->getRect().getPosition(), srv->getRect().getScale());

    //auto btx = water->getLandingProjectionTexture();
    //auto blur = btx->getBlurTexture();
    //blur->getRect().setScaleFromSize(200);
    //blur->getRect().setPosition(srv->getRect().getOtherPosition(tnl::Rect::eLocation::OUT_LEFT, blur->getRect()));
    //blur->drawShaderResouceView(blur->getRect().getPosition(), blur->getRect().getScale());

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
    water.reset();
    ptcl.reset();
    mesh_pool.reset();
}


*/