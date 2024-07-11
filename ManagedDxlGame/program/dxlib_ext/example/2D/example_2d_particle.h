#pragma once

//-----------------------------------------------------------------------------------------------------------
//
//
// パーティクル利用サンプル
//
// ※　このサンプルはパーティクルエディタとしても機能します
//
// ※　[ 重要 ]　std::shared_ptr で確保した dxe::Particle は必ず gameEnd() の
// 　　タイミングまでに参照カウンタが 0 になるようリセットしてください ( gameEnd 関数を参照 )
//
// ※　[ 重要 ]　この機能は DxLib の機能ではありません
// 　　dxe::Particle や dxe::InstMeshPool など DirectX を直接制御するクラスの render 関数は
//　　 dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() で囲った中でコールしてください  
//-----------------------------------------------------------------------------------------------------------

/*


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"


// カメラ
Shared<dxe::Camera> camera = nullptr;

// パーティクル
std::shared_ptr<dxe::Particle> ptcl = nullptr;

bool is_draw_controller = true;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {

    srand(time(0));
    SetBackgroundColor(32, 32, 32);

    // 2D 用カメラの作成
    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dxe::Camera::eDimension::Type2D);

    ptcl = std::make_shared<dxe::Particle>("particle/preset/default.bin");

}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

    //----------------------------------------------------------------------------------------------------
    //
    // カメラ制御
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


    // GUI 描画 ON / OFF
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F3)) is_draw_controller = !is_draw_controller;

    // カメラアップデート
    camera->update();


    //----------------------------------------------------------------------------------------------------
    //
    // 描画処理
    //

    //---------------------------------------------------------------------------
    //
    // パーティクルの描画開始
    //
    dxe::DirectXRenderBegin();

    // パーティクル A を描画
    ptcl->render(camera);

    // パーティクル B を描画
    // パーティクル C を...

    //---------------------------------------------------------------------------
    //
    // パーティクルの描画終了
    //
    dxe::DirectXRenderEnd();




    if (is_draw_controller) ptcl->drawGuiController({ 0, 0 });

    DrawFpsIndicator({ DXE_WINDOW_WIDTH - 260, 0, 0 }, delta_time);

    DrawString(DXE_WINDOW_WIDTH - 260, 20, "A D : カメラ左右移動", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 40, "W S : カメラ上下移動", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 60, "F3 : GUI 描画 ON/OFF", -1);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
    ptcl.reset();
}


*/