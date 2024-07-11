#pragma once

//-----------------------------------------------------------------------------------------------------------
//
//
// �p�[�e�B�N�����p�T���v��
//
// ���@���̃T���v���̓p�[�e�B�N���G�f�B�^�Ƃ��Ă��@�\���܂�
//
// ���@[ �d�v ]�@std::shared_ptr �Ŋm�ۂ��� dxe::Particle �͕K�� gameEnd() ��
// �@�@�^�C�~���O�܂łɎQ�ƃJ�E���^�� 0 �ɂȂ�悤���Z�b�g���Ă������� ( gameEnd �֐����Q�� )
//
// ���@[ �d�v ]�@���̋@�\�� DxLib �̋@�\�ł͂���܂���
// �@�@dxe::Particle �� dxe::InstMeshPool �Ȃ� DirectX �𒼐ڐ��䂷��N���X�� render �֐���
//�@�@ dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() �ň͂������ŃR�[�����Ă�������  
//-----------------------------------------------------------------------------------------------------------

/*


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// �J����
Shared<dxe::Camera> camera = nullptr;

// �p�[�e�B�N��
std::shared_ptr<dxe::Particle> ptcl = nullptr;

bool is_draw_controller = true;

//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {

    srand(time(0));
    SetBackgroundColor(32, 32, 32);

    // 2D �p�J�����̍쐬
    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dxe::Camera::eDimension::Type2D);

    ptcl = std::make_shared<dxe::Particle>("particle/preset/default.bin");

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


    // GUI �`�� ON / OFF
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F3)) is_draw_controller = !is_draw_controller;

    // �J�����A�b�v�f�[�g
    camera->update();


    //----------------------------------------------------------------------------------------------------
    //
    // �`�揈��
    //

    //---------------------------------------------------------------------------
    //
    // �p�[�e�B�N���̕`��J�n
    //
    dxe::DirectXRenderBegin();

    // �p�[�e�B�N�� A ��`��
    ptcl->render(camera);

    // �p�[�e�B�N�� B ��`��
    // �p�[�e�B�N�� C ��...

    //---------------------------------------------------------------------------
    //
    // �p�[�e�B�N���̕`��I��
    //
    dxe::DirectXRenderEnd();




    if (is_draw_controller) ptcl->drawGuiController({ 0, 0 });

    DrawFpsIndicator({ DXE_WINDOW_WIDTH - 260, 0, 0 }, delta_time);

    DrawString(DXE_WINDOW_WIDTH - 260, 20, "A D : �J�������E�ړ�", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 40, "W S : �J�����㉺�ړ�", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 60, "F3 : GUI �`�� ON/OFF", -1);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {
    ptcl.reset();
}


*/