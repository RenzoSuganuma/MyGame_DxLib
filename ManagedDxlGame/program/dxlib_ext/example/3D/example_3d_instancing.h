#pragma once

/*


//----------------------------------------------------------------------------------------------------------------------------------------------------
//
//
// インスタンシングとグループメッシュのサンプル
//
// ※ draw コールを減らす為、同じ形状のメッシュを大量に１度に描画します
//
// ※ [ インスタンシングの特徴 ]
// 　　・メッシュの１つ１つを動的に動かす事が可能
//　　 ・メッシュはそれぞれ別のテクスチャを指定可能
// 　　・遮蔽物としてシャドウマップ の reserveで利用できますが、このメッシュ自体に影を落とすことはできません ( DxLib との兼ね合い )
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
// ※ [ グループメッシュの特徴 ]
// 　　・生成時に姿勢や配置を決め、以降は個別に動かす事はできません
//　　 ・グループになっているメッシュでは同じテクスチャを共有します
// 　　・通常のメッシュと同様の描画方法 ( render するだけ )で尚且つシャドウマップでも使用できます ( 影を落とすことも可能 )
//
//
//
// ※ インスタンシング も グループメッシュも個別のマテリアルは設定できません ( 全体は可能 )
//
//----------------------------------------------------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"



Shared<dxe::Camera> camera = nullptr;


// 複製元になるメッシュ
Shared<dxe::Mesh> origine_mesh = nullptr;

// グループメッシュ
Shared<dxe::Mesh> group_mesh = nullptr;

// インスタンシングメッシュプール
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// メッシュプールから得られるインスタンシングメッシュ
std::list<Shared<dxe::InstMesh>> inst_meshs ;



//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
    srand(time(0));

    SetFogEnable(true);
    SetFogColor(32, 32, 32);
    SetFogStartEnd(500.0f, 1500.0f);
    ChangeLightTypeDir(VGet(0.0f, -1.0f, 1.0f));
    SetBackgroundColor(32, 32, 32);


    camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
    camera->pos_ = { 0, 100, -250 };


    // 複製元になるメッシュを作成
    // ※ MVファイル以外にもボックスやスフィア等でも可能
    //    MV ファイルの場合、複数のパーツで構成されていても最初のサブセットのみインスタンシング・グループ化できます
    origine_mesh = dxe::Mesh::CreateFromFileMV("mesh/mv/test.mv");
    origine_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));


    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // グループメッシュとして作成する場合
    //
    std::vector<tnl::Matrix> mats;
    mats.emplace_back(tnl::Matrix::Translation(-100, 0, 100));
    mats.emplace_back(tnl::Matrix::Translation(-200, 0, 200));
    mats.emplace_back(tnl::Matrix::Translation(-300, 0, 300));
    mats.emplace_back(tnl::Matrix::AffineTransformation({-400, 50, 400}, {2, 2, 2}, tnl::Quaternion::RotationAxis({1, 0, 0}, tnl::ToRadian(90))));

    group_mesh = dxe::Mesh::CreateStaticMeshGroupMV(origine_mesh, mats);



    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // インスタンシングプールとして作成する場合
    //
    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 3, &textures);

    // インスタンシングプールから個別のメッシュを取得
    for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
        auto inst = mesh_pool->CreateInstMesh();
        inst->setPosition({ float(i * 50), 100, float(i * 50) });
        inst->setRotation(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)));

        // 使用するテクスチャ番号を設定
        // MeshPool の Create 関数に渡されたテクスチャ配列番号で指定します
        inst->setUseTextureIndex( i % textures.size() );

        inst_meshs.emplace_back(inst);
    }

}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {


    // インスタンスメッシュを操作( 例えば回転 )
    for (auto mesh : inst_meshs) {
        mesh->setRotation( mesh->getRotation()* tnl::Quaternion::RotationAxis({0, 1, 0}, tnl::ToRadian(1)) );
    }


    camera->update();
    DrawGridGround(camera, 50, 20);


    // グループメッシュの描画( 通常のメッシュと同様の描画方法 )
    group_mesh->render(camera);



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
