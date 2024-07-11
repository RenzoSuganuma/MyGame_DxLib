#pragma once
//----------------------------------------------------------------------------------------------------------------------------------------------------
//
// インスタンシングとグループメッシュのサンプル
//
// ※ draw コールを減らす為、同じ形状のメッシュを大量に１度に描画します
//
// ※ [ インスタンシングの特徴 ]
// 　　・メッシュの１つ１つを動的に動かす事が可能
//　　 ・メッシュはそれぞれ別のテクスチャを指定可能
//
//      ※　[ 重要 ]　std::shared_ptr で確保した dxe::InstMeshPool は必ず gameEnd() の
// 　　               タイミングまでに参照カウンタが 0 になるようリセットしてください ( gameEnd 関数を参照 )
//
//      ※　[ 重要 ]　この機能は DxLib の機能ではありません
// 　　               dxe::Particle や dxe::InstMeshPool など DirectX を直接制御するクラスの render 関数は
//　　                dxe::DirectXRenderBegin() dxe::DirectXRenderEnd() で囲った中でコールしてください
//
//
//
// ※ インスタンシング も グループメッシュも個別のマテリアルは設定できません ( 全体は可能 )
//
//----------------------------------------------------------------------------------------------------------------------------------------------------

/*

#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"



// カメラ
Shared<dxe::Camera> camera = nullptr;

// 複製元になるメッシュ
Shared<dxe::Mesh> origine_mesh = nullptr;

// インスタンシングメッシュプール
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// メッシュプールから得られるインスタンシングメッシュ
std::list<Shared<dxe::InstMesh>> inst_meshs;



//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {

    srand(time(0));

    SetBackgroundColor(32, 32, 32);


    // 2D 用カメラの作成
    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dxe::Camera::eDimension::Type2D);


    // 複製元になるメッシュを作成
    // ※ MVファイル以外にもボックスやスフィア等でも可能
    //    MV ファイルの場合、複数のパーツで構成されていても最初のサブセットのみインスタンシング・グループ化できます
    origine_mesh = dxe::Mesh::CreatePlaneMV({ 25, 25, 0 }, 1, 1);


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // インスタンシングプールとして作成
    //
    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 50*50, &textures);
    mesh_pool->setMtrlEmissive({ 1, 1, 1 });

    int sx = -DXE_WINDOW_WIDTH / 2;
    int sy = -DXE_WINDOW_HEIGHT / 2;

    // 作成したメッシュを並べる & リストに保存
    for(int i = 0 ; i < 50 ; ++i){
        sx = -DXE_WINDOW_WIDTH / 2;
        for (int k = 0; k < 50; ++k) {
            auto inst = mesh_pool->CreateInstMesh();
            inst->setPosition({ (float)sx, (float)sy, 0 });
            sx += 25;
            // 使用するテクスチャ番号を設定
            // MeshPool の Create 関数に渡されたテクスチャ配列番号で指定します
            inst->setUseTextureIndex( rand() % 3 );

            inst_meshs.emplace_back(inst);
        }
        sy += 25;
    }

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

    // インスタンスメッシュを操作( 例えば回転 )
    //for (auto mesh : inst_meshs) {
    //    mesh->setRotation(mesh->getRotation() * tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(1)));
    //}


    camera->update();


    // インスタンシングプールの描画( DirectXRenderBegin, DirectXRenderEnd で囲う )
    dxe::DirectXRenderBegin();

    mesh_pool->render(camera);

    dxe::DirectXRenderEnd();


    DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
    mesh_pool.reset();
}

*/


