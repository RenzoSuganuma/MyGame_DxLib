#pragma once

/*

//----------------------------------------------------------------------------------------------------------------------------------------------------
//
// 水面エフェクト 利用サンプル
//
// ※　このサンプルは水面エディタとしても機能します
//
// ※　このサンプルでは水面に対して他のオブジェクトを反映させたサンプルです。
// 　　映り込みとして、通常オブジェクト・パーティクル・インスタンシングプール を反映します
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
// 座標と姿勢で制御するカメラ
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
    // 姿勢パラメータからターゲット座標とアッパーベクトルを計算
    //

    target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
    up_ = tnl::Vector3::TransformCoord({ 0, 1, 0 }, rot_);
    dxe::Camera::update();
}



Shared<TransformCamera> camera = nullptr;

// 水面オブジェクト
Shared<dxe::WaterPlane> water = nullptr;

Shared<dxe::Mesh> skybox = nullptr;
Shared<dxe::Mesh> canoe = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::Mesh> bridge = nullptr;
Shared<dxe::Mesh> test_object = nullptr;
std::shared_ptr<dxe::Particle> ptcl = nullptr;


// 複製元になるメッシュ
Shared<dxe::Mesh> origine_mesh = nullptr;

// インスタンシングメッシュプール
Shared<dxe::InstMeshPool> mesh_pool = nullptr;

// メッシュプールから得られるインスタンシングメッシュ
std::list<Shared<dxe::InstMesh>> inst_meshs;


bool is_draw_controller = true;

//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {

    srand(time(0));
    ChangeLightTypeDir(VGet(0.0f, -1.0f, 1.0f));
    SetBackgroundColor(32, 32, 32);

    camera = std::make_shared<TransformCamera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
    camera->pos_ = { 0, 100, -700 };

    // ( テスト用 ) カヌー作成
    canoe = dxe::Mesh::CreateFromFileMV("mesh/mv/canoe/Canoe.mv1");
    canoe->setScaling( { 10, 10, 10 } );
    canoe->setPosition( { 100, 3, -375 } );

    // ( テスト用 ) 桟橋作成
    bridge = dxe::Mesh::CreateFromFileMV("mesh/mv/bridge/WoodenBridge_01.mv1");
    bridge->setScaling( { 0.25f, 0.25f, 0.25f } );
    bridge->setRotation( tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90)) );
    bridge->setPosition( { 100, -10, -450 } );

    // ( テスト用 ) スカイボックス作成
    skybox = dxe::Mesh::CreateCubeMV(30000, 20, 20);
    skybox->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_b.png"));
    skybox->loadMaterial("material.bin");

    // ( テスト用 ) 砂浜作成
    ground = dxe::Mesh::CreatePlaneMV({ 1000, 300, 0 });
    ground->setTexture(dxe::Texture::CreateFromFile("graphics/sandy_beach.png"));
    ground->mulRotation( tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(95)) );
    ground->setPosition( { 0, 0, -500 } ) ;

    // ( テスト用 ) 沈めたオブジェクトを作成
    test_object = dxe::Mesh::CreateConeMV(50, 100, 20, 20);
    test_object->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
    test_object->setPosition( { 300, -100, 100 } );


    // 水面オブジェクトをデータから作成
    water = std::make_shared<dxe::WaterPlane>("water_plane.bin");

    // カヌーは泡とくり抜きを適用
    water->addLandingMesh(canoe, dxe::WaterPlane::fLandingMeshUse::BUBLLE | dxe::WaterPlane::fLandingMeshUse::CUT_OUT);

    // 桟橋は泡を適用
    water->addLandingMesh(bridge, dxe::WaterPlane::fLandingMeshUse::BUBLLE);

    // 砂浜は泡とフェードを適用
    water->addLandingMesh(ground, dxe::WaterPlane::fLandingMeshUse::BUBLLE | dxe::WaterPlane::fLandingMeshUse::FADE_ALPHA, 1.0f);

    // ( テスト用 ) パーティクル作成
    ptcl = std::make_shared<dxe::Particle>("particle/preset/fire.bin");
    ptcl->setPosition({ -50, 5, -450 });



    // 複製元になるメッシュを作成
    // ※ MVファイル以外にもボックスやスフィア等でも可能
    //    MV ファイルの場合、複数のパーツで構成されていても最初のサブセットのみインスタンシング・グループ化できます
    origine_mesh = dxe::Mesh::CreateFromFileMV("mesh/mv/test.mv");
    origine_mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));


    std::vector<Shared<dxe::Texture>> textures;
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/blue1.bmp"));
    textures.emplace_back(dxe::Texture::CreateFromFile("graphics/green1.bmp"));

    mesh_pool = dxe::InstMeshPool::Create(origine_mesh, 3, &textures);

    // インスタンシングプールから個別のメッシュを取得
    for (int i = 0; i < mesh_pool->getMaxInstanceNum(); ++i) {
        auto inst = mesh_pool->CreateInstMesh();
        inst->setPosition({ float(i * 50), 20, float(i * 50) });
        inst->setRotation(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)));

        // 使用するテクスチャ番号を設定
        // MeshPool の Create 関数に渡されたテクスチャ配列番号で指定します
        inst->setUseTextureIndex(i % textures.size());

        inst_meshs.emplace_back(inst);
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

    // GUI 描画 ON / OFF
    if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F3)) is_draw_controller = !is_draw_controller;


    //
    // キューブマップへの書き込み処理
    //
    tnl::Vector3 cube_map_cam_pos = camera->pos_ * tnl::Vector3(1, -1, 1);
    water->setCubeMapCameraPosition(cube_map_cam_pos);
    water->writeToCubeMap([&](Shared<dxe::Camera>& cube_camera) {
        skybox->render(cube_camera);
        canoe->render(cube_camera);
        bridge->render(cube_camera);

        // インスタンシングプールの描画
        // ※ 透明度のあるオブジェクト( パーティクル等 )よりも前に描画
        mesh_pool->render(cube_camera);

        // キューブマップへの書き込みでは時間を進めないようにする ( 第二引数 false )
        // キューブマップへの書き込みでは DirectXRenderBegin DirectXRenderEnd は必要なし
        ptcl->render(cube_camera, false);

        });


    camera->update();



    //----------------------------------------------------------------------------------------------------
    //
    //
    // 以下 描画処理
    //
    //

    // スカイボックスはフォグなし
    SetFogEnable(false);
    skybox->render(camera);


    // フォグ適用
    SetFogEnable(true);
    SetFogColor(32, 32, 32);
    SetFogStartEnd(500.0f, 1500.0f);


    ground->render(camera);
    bridge->render(camera);
    canoe->render(camera);
    test_object->render(camera);


    //
    // 水面の描画 + 併用テストの為のパーティクル & インスタンシング描画
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
    DrawString(DXE_WINDOW_WIDTH - 260, 20, "右クリック : 向き", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 40, "ホイール : 前後", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 60, "A D : 左右移動", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 80, "W S : 上下移動", -1);
    DrawString(DXE_WINDOW_WIDTH - 260, 100, "F3 : GUI 描画 ON/OFF", -1);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


    //
    // デバッグ用　レンダーテクスチャの描画
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
// ゲーム終了時に１度だけ実行されます
void gameEnd() {
    water.reset();
    ptcl.reset();
    mesh_pool.reset();
}


*/