#include "../dxlib_ext.h"
#include "dxlib_ext_particle.h"
#include "shader/particle/shader_particle_vs.h"
#include "shader/particle/shader_particle_ps.h"
#include "shader/particle/shader_particle_gs0.h"
#include "shader/particle/shader_particle_gs1.h"

namespace dxe {

#define DXE_PTCL_SHADER_COMPLIE 0

    using namespace DirectX;

    // シェーダ定数定義
    typedef struct _CBUFFER
    {
        DirectX::XMMATRIX	mat_view_;              // ビュー 行列
        DirectX::XMMATRIX	mat_proj_;              // 射影 行列
        tnl::Vector3        emitter_position_;      // パーティクルが生み出される座標
        float               size_x_;                // 粒子のサイズx
        tnl::Vector3        emissive_;              // エミッシブ
        float               time_limit_;            // 時間の限界値( この値を超えると初期値に戻す )
        tnl::Vector3        direction_;             // パーティクルがリセットされた時に再設定される放出向き
        float               delta_time_;            // デルタタイム
        tnl::Vector3        gravity_dir_;           // 重力方向
        float               disp_size_factor_;      // サイズ変位係数
        tnl::Vector3        camera_position_;       // カメラ座標
        float               start_velocity_;        // パーティクルがリセットされた時に再設定される初速
        tnl::Vector3        conv_position_;         // 収束座標
        float               gravity_factor_;        // 重力係数
        tnl::Vector3        origin_axis_;           // 生成の起点軸
        float               random_seed_;           // 乱数シード
        float               eject_random_angle_range_; // 放出向きに対するランダムな振れ幅
        float               waver_factor_;          // 揺らぎ係数
        float               lumi_wave_factor_;      // 輝度の揺らぎ係数
        float               rotate_factor_;         // 回転係数
        float               distortion_range_;      // 歪みの最大範囲
        float               distortion_velocity_;   // 歪みの早さ
        float               scale_;                 // スケール
        float               alpha_;                 // 透明度
        float               disp_alpha_factor_;     // 透明度の変位係数
        float               on_emitte_size_;        // 放出管理用サイズ
        float               moving_decay_factor_;   // 移動量の減衰係数
        float               near_alpha_distance_;   // 近すぎる場合に透明度を下げる境界距離
        float               origin_range_;          // 粒子を生み出す時の起点からの最大距離
        float               size_y_;                // 粒子のサイズy
        int                 origin_mode_;           // 生成の起点モード
        int                 play_type_;             // 再生タイプ( ループ or 単発 )
        int                 adulation_emitter_;     // 生み出した後の粒子がエミッターを追従するか
        int                 distortion_mode_;       // 歪みのタイプ
        int                 move_surface_;          // 移動平面
        int                 posture_type_;          // 姿勢タイプ
        int                 disp_size_mode_;        // サイズ変位モード
        int                 disp_alpha_mode_;       // 透明度変位モード
        int                 eject_type_;            // 拡散・収束モード
    } PTCL_CBUFFER;


    //--------------------------------------------------------------------------------------------------------------------------------
    Particle::Particle
        ( const Shared<dxe::Texture>& texture
        , ePlayType play_type
        , int particle_num
        , float time_limit ){

        texture_ = texture;
        play_type_ = play_type;
        particle_num_ = particle_num;
        time_limit_ = time_limit;
        random_value_ = tnl::GetRandomDistributionFloat(0.0f, 1.0f);

        refreshProcess() ;
        reloadTexture();
        createShader();
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    Particle::Particle(const std::string& resouce_file_path) {
        loadStatus(resouce_file_path);
        refreshProcess();
        reloadTexture();
        createShader();
    }

    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::createShader() {

        HRESULT hr = E_FAIL;
        ID3DBlob* p_err = nullptr;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
        ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

        //-------------------------------------------------------------------------------------------------
        // 頂点シェーダーの作成
        ID3DBlob* p_blob_vertex = nullptr;
        D3D11_INPUT_ELEMENT_DESC layout[] = {
           {"SPOWN_POSITION",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},   // 
           {"BEFORE_POSITION",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"UPDATE_POSITION",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"DIRECTION",            0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"MOVE_VELOCITY",        0, DXGI_FORMAT_R32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"PROGRESS_TIME",        0, DXGI_FORMAT_R32_FLOAT,       0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"MOVING_PROGRESS_TIME", 0, DXGI_FORMAT_R32_FLOAT,       0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"ALPHA",                0, DXGI_FORMAT_R32_FLOAT,       0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"SIZE_X",               0, DXGI_FORMAT_R32_FLOAT,       0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"SIZE_Y",               0, DXGI_FORMAT_R32_FLOAT,       0, 68, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"RAND_VALUE",           0, DXGI_FORMAT_R32_FLOAT,       0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"PROCESS_COUNTER",      0, DXGI_FORMAT_R32_SINT,        0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
           {"VERTEX_INDEX",         0, DXGI_FORMAT_R32_SINT,        0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0},  // 
        };


//
// リソースからコンパイル
//
#if DXE_PTCL_SHADER_COMPLIE

        hr = D3DCompileFromFile(L"shader/particle/particle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VS0_Main", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_vertex, &p_err);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Compile Vertex Shader \n");
            OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        ID3D11VertexShader* vertex_shader = nullptr;
        hr = pd3dDevice->CreateVertexShader(p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(), NULL, &vertex_shader);
        vertex_shader_.Attach(vertex_shader);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Vertex Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //-------------------------------------------------------------------------------------------------
        // 頂点のインプットレイアウト作成
        ID3D11InputLayout* il = nullptr;
        hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(),
            &il);
        input_layout_.Attach(il);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Input Layout \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
#else
//
// コンパイル済みヘッダから作成
//

        ID3D11VertexShader* vertex_shader = nullptr;
        hr = pd3dDevice->CreateVertexShader(&g_particle_VS0_Main, sizeof(g_particle_VS0_Main), NULL, &vertex_shader);
        vertex_shader_.Attach(vertex_shader);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Vertex Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //-------------------------------------------------------------------------------------------------
        // 頂点のインプットレイアウト作成
        ID3D11InputLayout* il = nullptr;
        hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            &g_particle_VS0_Main, sizeof(g_particle_VS0_Main),
            &il);
        input_layout_.Attach(il);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Input Layout \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

#endif


        //-------------------------------------------------------------------------------------------------
        // ストリームアウトプットとしてジオメトリシェーダーを作成

        // ターゲット入出力バッファーの定義
        D3D11_SO_DECLARATION_ENTRY decl[] = {
           { 0, "SPOWN_POSITION",   0, 0, 3, 0 },
           { 0, "BEFORE_POSITION",  0, 0, 3, 0 },
           { 0, "UPDATE_POSITION",  0, 0, 3, 0 },
           { 0, "DIRECTION",        0, 0, 3, 0 },
           { 0, "MOVE_VELOCITY",            0, 0, 1, 0 },
           { 0, "PROGRESS_TIME",            0, 0, 1, 0 },
           { 0, "MOVING_PROGRESS_TIME",     0, 0, 1, 0 },
           { 0, "ALPHA",                    0, 0, 1, 0 },
           { 0, "SIZE_X",                   0, 0, 1, 0 },
           { 0, "SIZE_Y",                   0, 0, 1, 0 },
           { 0, "RAND_VALUE",               0, 0, 1, 0 },
           { 0, "PROCESS_COUNTER",          0, 0, 1, 0 },
           { 0, "VERTEX_INDEX",             0, 0, 1, 0 },
        };
        UINT stride[1] = { sizeof(PTCL_VERTEX) };


#if DXE_PTCL_SHADER_COMPLIE

        UINT Flag1 = 0;
#if defined(DEBUG) || defined(_DEBUG)
        Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
        Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

        ID3DBlob* p_blob_geometry = nullptr;
        hr = D3DCompileFromFile(L"shader/particle/particle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "GS0_Main", "gs_4_0", Flag1, 0, &p_blob_geometry, &p_err);
        //D3DReadFileToBlob
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Compile Geometry Shader \n");
            OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


        ID3D11GeometryShader* gs = nullptr;
        hr = pd3dDevice->CreateGeometryShaderWithStreamOutput(
            p_blob_geometry->GetBufferPointer(),
            p_blob_geometry->GetBufferSize(),
            decl,
            _countof(decl),
            stride,
            _countof(stride),
            D3D11_SO_NO_RASTERIZED_STREAM,
            NULL,
            &gs);
        geometry_shaders_[0].Attach(gs);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Geometry Shader With Stream Output \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


        //-------------------------------------------------------------------------------------------------
        // 描画用のジオメトリシェーダーを作成する
        ID3DBlob* p_blob_geometry2 = nullptr;
#if defined(DEBUG) || defined(_DEBUG)
        Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
        Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif
        hr = D3DCompileFromFile(L"shader/particle/particle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "GS1_Main", "gs_4_0", Flag1, 0, &p_blob_geometry2, &p_err);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Compile Geometry Shader \n");
            OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


        hr = pd3dDevice->CreateGeometryShader(p_blob_geometry2->GetBufferPointer(), p_blob_geometry2->GetBufferSize(), NULL, &geometry_shaders_[1]);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Geometry Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

#else

//
// コンパイル済みヘッダから作成
//
        ID3D11GeometryShader* gs = nullptr;
        hr = pd3dDevice->CreateGeometryShaderWithStreamOutput(
            &g_particle_GS0_Main,
            sizeof(g_particle_GS0_Main),
            decl,
            _countof(decl),
            stride,
            _countof(stride),
            D3D11_SO_NO_RASTERIZED_STREAM,
            NULL,
            &gs);
        geometry_shaders_[0].Attach(gs);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Geometry Shader With Stream Output \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


        //-------------------------------------------------------------------------------------------------
        // 描画用のジオメトリシェーダーを作成する
        hr = pd3dDevice->CreateGeometryShader(
            &g_particle_GS1_Main,
            sizeof(g_particle_GS1_Main),
            NULL, &geometry_shaders_[1]);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Geometry Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


#endif


#if DXE_PTCL_SHADER_COMPLIE

        //-------------------------------------------------------------------------------------------------
        // ピクセルシェーダの作成
        ID3DBlob* p_blob_pixel = nullptr;
        hr = D3DCompileFromFile(L"shader/particle/particle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PS1_Main", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_pixel, &p_err);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Compile Pixel Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        ID3D11PixelShader* ps = nullptr;
        hr = pd3dDevice->CreatePixelShader(p_blob_pixel->GetBufferPointer(), p_blob_pixel->GetBufferSize(), NULL, &ps);
        pixel_shader_.Attach(ps);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Pixel Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
#else

        //
        // コンパイル済みヘッダから作成
        //
        ID3D11PixelShader* ps = nullptr;
        hr = pd3dDevice->CreatePixelShader(
            &g_particle_PS1_Main, 
            sizeof(g_particle_PS1_Main), NULL, &ps);
        pixel_shader_.Attach(ps);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Create Pixel Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


#endif


        //-------------------------------------------------------------------------------------------------
        // シェーダ定数バッファの作成
        {
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bd.ByteWidth = sizeof(PTCL_CBUFFER);
            ID3D11Buffer* cb = nullptr;
            hr = pd3dDevice->CreateBuffer(&bd, nullptr, &cb);
            constant_buffer_.Attach(cb);
            if (S_OK != hr) {
                tnl::DebugTrace("-----------------------------------------------------------------\n");
                tnl::DebugTrace("Error : Particle Create Constant Buffer \n");
                tnl::DebugTrace("-----------------------------------------------------------------\n");
                return;
            }
        }


    }


    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::refreshProcess() {

        is_init_draw_ = true ;

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
        ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

        // パーティクルの初期値を設定
        vertexs_ = std::shared_ptr<PTCL_VERTEX[]>(new PTCL_VERTEX[particle_num_]);
        for (int i = 0; i < particle_num_; i++)
        {
            vertexs_[i].spown_position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
            vertexs_[i].update_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
            vertexs_[i].direction_ = { 0, 0, 0 };
            vertexs_[i].move_velocity_ = 0;
            if (play_type_ == ePlayType::LOOP) {
                vertexs_[i].progress_time_ = (float)i / (float)particle_num_ * time_limit_;
                vertexs_[i].moving_progress_time_ = vertexs_[i].progress_time_;
            }
            else {
                vertexs_[i].progress_time_ = 0;
                vertexs_[i].moving_progress_time_ = 0;
            }
            vertexs_[i].alpha_ = 0;
            vertexs_[i].size_x_ = 0;
            vertexs_[i].size_y_ = 0;
            vertexs_[i].process_counter_ = 0;
            vertexs_[i].vertex_index_ = i;
            vertexs_[i].rand_value_ = tnl::GetRandomDistributionFloat(0, 1);
        };

        createTargetIOBuffer();

    }

    //--------------------------------------------------------------------------------------------------------------------------------
    void Particle::createTargetIOBuffer() {

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();

        //-------------------------------------------------------------------------------------------------
        // ターゲット入出力バッファーを作成
        // ジオメトリシェーダを２回通して
        // 頂点の移動処理 -> ポイントから４頂点に増幅してスプライトにするために使用
        D3D11_BUFFER_DESC BufferDesc;
        ::ZeroMemory(&BufferDesc, sizeof(BufferDesc));
        BufferDesc.ByteWidth = sizeof(PTCL_VERTEX) * particle_num_;
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
        BufferDesc.CPUAccessFlags = 0;
        BufferDesc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA mappedResource;
        mappedResource.pSysMem = vertexs_.get();
        mappedResource.SysMemPitch = 0;
        mappedResource.SysMemSlicePitch = 0;
        for (int i = 0; i < 2; i++)
        {
            ID3D11Buffer* buffer = nullptr;
            hr = pd3dDevice->CreateBuffer(&BufferDesc, &mappedResource, &buffer);
            target_io_buffer_[i].Attach(buffer);
            if (S_OK != hr) {
                tnl::DebugTrace("-----------------------------------------------------------------\n");
                tnl::DebugTrace("Error : Particle Target IO Buffer Create \n");
                tnl::DebugTrace("-----------------------------------------------------------------\n");
                return;
            }
        }
    }



    //--------------------------------------------------------------------------------------------------------------------------------
    void Particle::reloadTexture() {

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();

        //-------------------------------------------------------------------------------------------------
        // テクスチャのリロード
        ID3D11Resource* res = nullptr;
        ID3D11ShaderResourceView* srv = nullptr;
        //wchar_t buff[256] = { 0 };
        //tnl::ToWChara(buff, texture_file_path);
        //hr = CreateWICTextureFromFile(pd3dDevice, buff, &res, &srv);
        hr = CreateWICTextureFromMemory(pd3dDevice, (uint8_t*)texture_->getFileData(), (size_t)texture_->getDataSize(), & res, &srv);
        texture_resouce_.Attach(res);
        shader_resouce_view_.Attach(srv);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Particle Load Texture \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //ID3D11Texture2D* texture = nullptr ;
        //HRESULT result = res->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
        //if (SUCCEEDED(result))
        //{
        //    D3D11_TEXTURE2D_DESC desc;
        //    texture->GetDesc(&desc);

        //    UINT width = desc.Width;
        //    UINT height = desc.Height;
        //    texture->Release();
        //}
        //else {
        //    tnl::DebugTrace("-----------------------------------------------------------------\n");
        //    tnl::DebugTrace("Error : Particle Get Texture Size \n");
        //    tnl::DebugTrace("-----------------------------------------------------------------\n");
        //}

    }


    //--------------------------------------------------------------------------------------------------------------------------------
    //ID3D11VertexShader* Particle::default_vs = nullptr;
    //ID3D11PixelShader* Particle::default_ps = nullptr;
    //ID3D11HullShader* Particle::default_hs = nullptr;
    //ID3D11GeometryShader* Particle::default_gs = nullptr;
    //ID3D11DomainShader* Particle::default_ds = nullptr;
    //UINT32 Particle::default_sample_mask = 0;
    //ID3D11BlendState* Particle::default_blend_state = nullptr;
    //ID3D11RasterizerState* Particle::default_rasterize_state = nullptr;
    //ID3D11SamplerState* Particle::default_sampler_state = nullptr;
    //ID3D11ShaderResourceView* Particle::default_pixel_shader_resouce_view = nullptr ;
    //void Particle::renderBegin() {

    //    ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

    //    // DX ライブラリのシェーダバックアップ
    //    pImmediateContext->VSGetShader(&default_vs, NULL, 0);
    //    pImmediateContext->PSGetShader(&default_ps, NULL, 0);
    //    pImmediateContext->HSGetShader(&default_hs, NULL, 0);
    //    pImmediateContext->GSGetShader(&default_gs, NULL, 0);
    //    pImmediateContext->DSGetShader(&default_ds, NULL, 0);

    //    // ブレンドステートバックアップ
    //    pImmediateContext->OMGetBlendState(&default_blend_state, 0, &default_sample_mask);

    //    // ラスタライザステートバックアップ
    //    pImmediateContext->RSGetState(&default_rasterize_state);

    //    // サンプラステートバックアップ
    //    pImmediateContext->PSGetSamplers(0, 1, &default_sampler_state);

    //    // ピクセルシェーダリソースビュー
    //    pImmediateContext->PSGetShaderResources(0, 1, &default_pixel_shader_resouce_view);

    //}

    ////--------------------------------------------------------------------------------------------------------------------------------
    //void Particle::renderEnd() {

    //    ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

    //    // DX ライブラリへシェーダバックアップを復帰
    //    pImmediateContext->VSSetShader(default_vs, NULL, 0);
    //    pImmediateContext->PSSetShader(default_ps, NULL, 0);
    //    pImmediateContext->HSSetShader(default_hs, NULL, 0);
    //    pImmediateContext->GSSetShader(default_gs, NULL, 0);
    //    pImmediateContext->DSSetShader(default_ds, NULL, 0);

    //    pImmediateContext->OMSetBlendState(default_blend_state, 0, default_sample_mask);
    //    pImmediateContext->RSSetState(default_rasterize_state);
    //    pImmediateContext->PSSetSamplers(0, 1, &default_sampler_state);
    //    pImmediateContext->PSSetShaderResources(0, 1, &default_pixel_shader_resouce_view);

    //    DxLib::RefreshDxLibDirect3DSetting();
    //    //SetDefaultFontState(_T("NULL"), font_size, 9, DX_FONTTYPE_NORMAL, hdl);
    //}


    //--------------------------------------------------------------------------------------------------------------------------------
	void Particle::render( const Shared<Camera>& camera, const bool is_advance_time) {

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
        ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

        //-------------------------------------------------------------------------------------------
        // 
        // 定数バッファを設定
        //
        {
            PTCL_CBUFFER* cbuffer = nullptr ;
            XMMATRIX matView, matProj, matWVP, matWorld;
            D3D11_MAPPED_SUBRESOURCE mappedResource;

            matProj = XMLoadFloat4x4(&camera->proj_);
            matView = XMLoadFloat4x4(&camera->view_);

            matWorld = XMMatrixTranslation(TNL_DEP_V3(pos_));

            // 行列の合成
            matWVP = matWorld * matView * matProj;

            ID3D11Buffer* cb = constant_buffer_.Get();
            hr = pImmediateContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            cbuffer = (PTCL_CBUFFER*)(mappedResource.pData);

            // シェーダー内では列優先にしているので転置行列を作成する。
            cbuffer->mat_view_ = XMMatrixTranspose(matView);
            cbuffer->mat_proj_ = XMMatrixTranspose(matProj);
            if (dxe::Camera::eDimension::Type3D == camera->getDimension()) {
                cbuffer->emitter_position_ = pos_;
            }
            else {
                cbuffer->emitter_position_ = { pos_.x, -pos_.y, 0 };
            }
            cbuffer->emissive_ = emissive_;
            cbuffer->size_x_ = size_x_;
            cbuffer->size_y_ = size_y_;
            cbuffer->time_limit_ = time_limit_;
            if (is_advance_time) {
                cbuffer->delta_time_ = dxe::GetDeltaTime() * time_scale_;
            }
            else {
                cbuffer->delta_time_ = 0;
            }
            cbuffer->disp_size_factor_ = disp_size_factor_;
            cbuffer->direction_ = diff_direction_;
            cbuffer->gravity_dir_ = gravity_direction_;
            cbuffer->origin_axis_ = origin_axis_;
            cbuffer->eject_random_angle_range_ = diff_random_angle_range_;
            cbuffer->start_velocity_ = start_velocity_;
            cbuffer->gravity_factor_ = gravity_factor_;
            cbuffer->random_seed_ = random_value_;
            cbuffer->waver_factor_ = waver_factor_;
            cbuffer->lumi_wave_factor_ = lumi_wave_factor_;
            cbuffer->rotate_factor_ = rotate_factor_;
            cbuffer->camera_position_ = camera->pos_;
            cbuffer->distortion_range_ = distortion_range_;
            cbuffer->distortion_velocity_ = distortion_velocity_;
            cbuffer->scale_ = scale_;
            cbuffer->alpha_ = alpha_;
            cbuffer->disp_alpha_factor_ = disp_alpha_factor_;
            cbuffer->on_emitte_size_ = on_emitte_size_;
            cbuffer->moving_decay_factor_ = moving_decay_factor_;
            cbuffer->near_alpha_distance_ = near_alpha_distance_;
            cbuffer->origin_range_ = origin_range_;
            cbuffer->conv_position_ = conv_position_;
            cbuffer->play_type_ = static_cast<int>( play_type_ );
            cbuffer->adulation_emitter_ = (is_adulation_emitter) ? 1 : 0;
            cbuffer->distortion_mode_ = static_cast<int>(distortion_mode_);
            cbuffer->origin_mode_ = static_cast<int>(origin_mode_);
            cbuffer->move_surface_ = static_cast<int>(move_surface_);
            cbuffer->posture_type_ = static_cast<int>(posture_type_);
            cbuffer->disp_size_mode_ = static_cast<int>(disp_size_mode_);
            cbuffer->disp_alpha_mode_ = static_cast<int>(disp_alpha_mode_);
            cbuffer->eject_type_ = static_cast<int>(eject_type_);

            if (ePlayType::REPLAY == play_type_) {
                play_type_ = ePlayType::ONCE;
            }

            pImmediateContext->Unmap(cb, 0);
        }

        D3D11_PRIMITIVE_TOPOLOGY bk_toplogy;
        ID3D11VertexShader* bk_vs = nullptr;
        ID3D11PixelShader* bk_ps = nullptr;
        ID3D11GeometryShader* bk_gs = nullptr;
        pImmediateContext->IAGetPrimitiveTopology(&bk_toplogy);
        pImmediateContext->VSGetShader(&bk_vs, NULL, 0);
        pImmediateContext->PSGetShader(&bk_ps, NULL, 0);
        pImmediateContext->GSGetShader(&bk_gs, NULL, 0);


        //-------------------------------------------------------------------------------------------
        // 
        // パーティクルの更新
        //
        {
            // ターゲット出力バッファーを入れ替える
            target_io_buffer_[0].Swap(target_io_buffer_[1]);

            // 入力アセンブラー ステージに頂点バッファーの配列をバインドする。
            UINT stride = sizeof(PTCL_VERTEX);
            UINT offset = 0;

            ID3D11Buffer* target_buffer0 = target_io_buffer_[0].Get();
            pImmediateContext->IASetVertexBuffers(0, 1, &target_buffer0, &stride, &offset);

            // 出力バッファーの配列のオフセット値
            // パイプラインのストリーム出力ステージで使用されるターゲット出力バッファーを設定します。
            ID3D11Buffer* target_buffer1 = target_io_buffer_[1].Get();
            pImmediateContext->SOSetTargets(1, &target_buffer1, &offset);

            // プリミティブ タイプおよびデータの順序に関する情報を設定
            pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

            // 頂点データレイアウト設定
            ID3D11InputLayout* il = input_layout_.Get();
            pImmediateContext->IASetInputLayout(il);

            // 頂点シェーダーをセット
            ID3D11VertexShader* vs = vertex_shader_.Get();
            pImmediateContext->VSSetShader(vs, NULL, 0);

            // ハルシェーダーを無効にする。
            pImmediateContext->HSSetShader(NULL, NULL, 0);

            // ドメインシェーダーを無効にする。
            pImmediateContext->DSSetShader(NULL, NULL, 0);

            // ジオメトリシェーダーセット
            ID3D11GeometryShader* gs = geometry_shaders_[0].Get();
            pImmediateContext->GSSetShader(gs, NULL, 0);

            // ジオメトリシェーダーに定数バッファを設定する。
            ID3D11Buffer* cb = constant_buffer_.Get();
            pImmediateContext->GSSetConstantBuffers(0, 1, &cb);

            // ピクセルシェーダーを無効にする。
            pImmediateContext->PSSetShader(NULL, NULL, 0);

            // コンピュートシェーダーを無効にする。
            pImmediateContext->CSSetShader(NULL, NULL, 0);

            // 最初は頂点数がわかるので ID3D11DeviceContext::Draw() で描画する
            if (is_init_draw_)
            {
                pImmediateContext->Draw(particle_num_, 0);
                is_init_draw_ = false;
            }
            // ２回目移行は処理する頂点数が不明なので ID3D11DeviceContext::DrawAuto() で描画する
            else
            {
                pImmediateContext->DrawAuto();
            }

            ID3D11Buffer* pNullBuffer[] = { NULL };
            // パイプラインのストリーム出力ステージで使用されるターゲット出力バッファーを無効にする。
            pImmediateContext->SOSetTargets(1, pNullBuffer, NULL);

        }

        //-------------------------------------------------------------------------------------------
        // 
        // パーティクル描画
        // 
        {

            // プリミティブ タイプおよびデータの順序に関する情報を設定
            pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

            // 書き込み OFF テスト ON
            if (dxe::Camera::eDimension::Type3D == camera->getDimension()) {
                pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_ON), 0);
            }
            else {
                pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_OFF), 0);
            }

            // ラスタライザステート設定
            ID3D11RasterizerState* rs = GetRasterizerState(rasterizer_state_);
            pImmediateContext->RSSetState(rs);

            // ブレンドステート設定
            //ID3D11BlendState* bs = blend_state_.Get();
            pImmediateContext->OMSetBlendState(GetBlendState(blend_mode_), 0, 0xffffffff);

            // 入力アセンブラー ステージに頂点バッファーの配列をバインドする。
            UINT stride = sizeof(PTCL_VERTEX);
            UINT offset = 0;
            ID3D11Buffer* target_buffer1 = target_io_buffer_[1].Get();
            pImmediateContext->IASetVertexBuffers(0, 1, &target_buffer1, &stride, &offset);

            // 頂点データレイアウト設定
            ID3D11InputLayout* il = input_layout_.Get();
            pImmediateContext->IASetInputLayout(il);

            // 頂点シェーダーをセット
            ID3D11VertexShader* vs = vertex_shader_.Get();
            pImmediateContext->VSSetShader(vs, NULL, 0);

            // ハルシェーダーを無効にする。
            pImmediateContext->HSSetShader(NULL, NULL, 0);

            // ドメインシェーダーを無効にする。
            pImmediateContext->DSSetShader(NULL, NULL, 0);

            // ジオメトリシェーダーをセット
            ID3D11GeometryShader* gs = geometry_shaders_[1].Get();
            pImmediateContext->GSSetShader(gs, NULL, 0);

            // ジオメトリシェーダーに定数バッファを設定する。
            ID3D11Buffer* cb = constant_buffer_.Get();
            pImmediateContext->GSSetConstantBuffers(0, 1, &cb);

            // ピクセルシェーダーをセット
            ID3D11PixelShader* ps = pixel_shader_.Get();
            pImmediateContext->PSSetShader(ps, NULL, 0);

            // テクスチャーを設定する
            ID3D11ShaderResourceView* srv = shader_resouce_view_.Get();
            pImmediateContext->PSSetShaderResources(0, 1, &srv);

            // サンプラステートを設定する
            ID3D11SamplerState* st = GetSamplerState(sampler_state_);
            pImmediateContext->PSSetSamplers(0, 1, &st);

            // コンピュートシェーダーを無効にする。
            pImmediateContext->CSSetShader(NULL, NULL, 0);

            // パーティクルの描画
            pImmediateContext->DrawAuto();
        }

        pImmediateContext->IASetPrimitiveTopology(bk_toplogy);
        pImmediateContext->VSSetShader(bk_vs, NULL, 0);
        pImmediateContext->PSSetShader(bk_ps, NULL, 0);
        pImmediateContext->GSSetShader(bk_gs, NULL, 0);

	}

    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::saveStatus(const std::string& file_path) {

        FILE* fp = nullptr;
        fopen_s(&fp, file_path.c_str(), "wb");
        if (fp) {
            fwrite(&pos_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&emissive_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&diff_direction_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&gravity_direction_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&conv_position_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&origin_axis_, sizeof(tnl::Vector3), 1, fp);
            fwrite(&scale_, sizeof(float), 1, fp);
            fwrite(&size_x_, sizeof(float), 1, fp);
            fwrite(&size_y_, sizeof(float), 1, fp);
            fwrite(&disp_size_factor_, sizeof(float), 1, fp);
            fwrite(&alpha_, sizeof(float), 1, fp);
            fwrite(&disp_alpha_factor_, sizeof(float), 1, fp);
            fwrite(&time_limit_, sizeof(float), 1, fp);
            fwrite(&time_scale_, sizeof(float), 1, fp);
            fwrite(&start_velocity_, sizeof(float), 1, fp);
            fwrite(&moving_decay_factor_, sizeof(float), 1, fp);
            fwrite(&diff_random_angle_range_, sizeof(float), 1, fp);
            fwrite(&gravity_factor_, sizeof(float), 1, fp);
            fwrite(&waver_factor_, sizeof(float), 1, fp);
            fwrite(&lumi_wave_factor_, sizeof(float), 1, fp);
            fwrite(&rotate_factor_, sizeof(float), 1, fp);
            fwrite(&distortion_range_, sizeof(float), 1, fp);
            fwrite(&distortion_velocity_, sizeof(float), 1, fp);
            fwrite(&on_emitte_size_, sizeof(float), 1, fp);
            fwrite(&random_value_, sizeof(float), 1, fp);
            fwrite(&near_alpha_distance_, sizeof(float), 1, fp);
            fwrite(&particle_num_, sizeof(int), 1, fp);
            fwrite(&origin_range_, sizeof(int), 1, fp);

            int adulation_emitter = (is_adulation_emitter) ? 1 : 0;
            fwrite(&adulation_emitter, sizeof(int), 1, fp);

            int play_type = static_cast<int>(play_type_);
            fwrite(&play_type, sizeof(int), 1, fp);

            int blend_mode = static_cast<int>(blend_mode_);
            fwrite(&blend_mode, sizeof(int), 1, fp);

            int origin_mode = static_cast<int>(origin_mode_);
            fwrite(&origin_mode, sizeof(int), 1, fp);

            int eject_type = static_cast<int>(eject_type_);
            fwrite(&eject_type, sizeof(int), 1, fp);

            int move_surface = static_cast<int>(move_surface_);
            fwrite(&move_surface, sizeof(int), 1, fp);

            int posture_type = static_cast<int>(posture_type_);
            fwrite(&posture_type, sizeof(int), 1, fp);

            int disp_size_mode = static_cast<int>(disp_size_mode_);
            fwrite(&disp_size_mode, sizeof(int), 1, fp);

            int disp_alpha_mode = static_cast<int>(disp_alpha_mode_);
            fwrite(&disp_alpha_mode, sizeof(int), 1, fp);

            int distortion_mode = static_cast<int>(distortion_mode_);
            fwrite(&distortion_mode, sizeof(int), 1, fp);

            char filename[256] = { 0 };
            sprintf_s(filename, "%s", texture_->getFileName().c_str());
            fwrite(filename, sizeof(filename), 1, fp);

            int data_size = texture_->getDataSize();
            fwrite(&data_size, sizeof(int), 1, fp);

            fwrite(texture_->getFileData(), data_size, 1, fp);

            fclose(fp);
        }
    }

    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::loadStatus(const std::string& file_path) {
        FILE* fp = nullptr;
        fopen_s(&fp, file_path.c_str(), "rb");
        if (fp) {
            fread(&pos_, sizeof(tnl::Vector3), 1, fp);
            fread(&emissive_, sizeof(tnl::Vector3), 1, fp);
            fread(&diff_direction_, sizeof(tnl::Vector3), 1, fp);
            fread(&gravity_direction_, sizeof(tnl::Vector3), 1, fp);
            fread(&conv_position_, sizeof(tnl::Vector3), 1, fp);
            fread(&origin_axis_, sizeof(tnl::Vector3), 1, fp);
            fread(&scale_, sizeof(float), 1, fp);
            fread(&size_x_, sizeof(float), 1, fp);
            fread(&size_y_, sizeof(float), 1, fp);
            fread(&disp_size_factor_, sizeof(float), 1, fp);
            fread(&alpha_, sizeof(float), 1, fp);
            fread(&disp_alpha_factor_, sizeof(float), 1, fp);
            fread(&time_limit_, sizeof(float), 1, fp);
            fread(&time_scale_, sizeof(float), 1, fp);
            fread(&start_velocity_, sizeof(float), 1, fp);
            fread(&moving_decay_factor_, sizeof(float), 1, fp);
            fread(&diff_random_angle_range_, sizeof(float), 1, fp);
            fread(&gravity_factor_, sizeof(float), 1, fp);
            fread(&waver_factor_, sizeof(float), 1, fp);
            fread(&lumi_wave_factor_, sizeof(float), 1, fp);
            fread(&rotate_factor_, sizeof(float), 1, fp);
            fread(&distortion_range_, sizeof(float), 1, fp);
            fread(&distortion_velocity_, sizeof(float), 1, fp);
            fread(&on_emitte_size_, sizeof(float), 1, fp);
            fread(&random_value_, sizeof(float), 1, fp);
            fread(&near_alpha_distance_, sizeof(float), 1, fp);
            fread(&particle_num_, sizeof(int), 1, fp);
            fread(&origin_range_, sizeof(int), 1, fp);

            int adulation_emitter = 0;
            fread(&adulation_emitter, sizeof(int), 1, fp);
            is_adulation_emitter = (1 == adulation_emitter) ? true : false;

            int play_type = 0;
            fread(&play_type, sizeof(int), 1, fp);
            play_type_ = static_cast<ePlayType>(play_type);

            int blend_mode = 0;
            fread(&blend_mode, sizeof(int), 1, fp);
            blend_mode_ = static_cast<eBlendState>(blend_mode);

            int origin_mode = static_cast<int>(origin_mode_);
            fread(&origin_mode, sizeof(int), 1, fp);
            origin_mode_ = static_cast<eOriginMode>(origin_mode);

            int eject_type = static_cast<int>(eject_type_);
            fread(&eject_type, sizeof(int), 1, fp);
            eject_type_ = static_cast<eEjectType>(eject_type);

            int move_surface = static_cast<int>(move_surface_);
            fread(&move_surface, sizeof(int), 1, fp);
            move_surface_ = static_cast<eMoveSurface>(move_surface);

            int posture_type = static_cast<int>(posture_type_);
            fread(&posture_type, sizeof(int), 1, fp);
            posture_type_ = static_cast<ePostureType>(posture_type);

            int disp_size_mode = static_cast<int>(disp_size_mode_);
            fread(&disp_size_mode, sizeof(int), 1, fp);
            disp_size_mode_ = static_cast<eDispSizeMode>(disp_size_mode);

            int disp_alpha_mode = static_cast<int>(disp_alpha_mode_);
            fread(&disp_alpha_mode, sizeof(int), 1, fp);
            disp_alpha_mode_ = static_cast<eDispAlphaMode>(disp_alpha_mode);

            int distortion_mode = static_cast<int>(distortion_mode_);
            fread(&distortion_mode, sizeof(int), 1, fp);
            distortion_mode_ = static_cast<eDistortionMode>(distortion_mode);

            char file_name[256] = { 0 };
            fread(file_name, sizeof(file_name), 1, fp);

            int data_size = 0;
            fread(&data_size, sizeof(int), 1, fp);

            char* texture_data = new char[data_size];
            fread(texture_data, data_size, 1, fp);
            texture_ = dxe::Texture::CreateFromMemory(texture_data, data_size, file_name);
            delete[] texture_data;

            fclose(fp);

        }

    }

    //---------------------------------------------------------------------------------------------------------------------------------------------
    Shared<Particle> Particle::createClone() {
        Shared<Particle> clone = std::make_shared<Particle>();

        clone->pos_ = pos_;
        clone->emissive_ = emissive_;
        clone->diff_direction_ = diff_direction_;
        clone->gravity_direction_ = gravity_direction_ ;
        clone->conv_position_ = conv_position_ ;
        clone->origin_axis_ = origin_axis_ ;
         
        clone->size_x_ = size_x_;
        clone->size_y_ = size_y_;
        clone->scale_ = scale_;
        clone->alpha_ = alpha_;
        clone->disp_alpha_factor_ = disp_alpha_factor_;
        clone->time_limit_ = time_limit_;
        clone->time_scale_ = time_scale_;
        clone->disp_size_factor_ = disp_size_factor_;
        clone->start_velocity_ = start_velocity_;
        clone->moving_decay_factor_ = moving_decay_factor_;
        clone->diff_random_angle_range_ = diff_random_angle_range_;
        clone->gravity_factor_ = gravity_factor_;
        clone->waver_factor_ = waver_factor_;
        clone->lumi_wave_factor_ = lumi_wave_factor_;
        clone->rotate_factor_ = rotate_factor_;
        clone->distortion_range_ = distortion_range_;
        clone->distortion_velocity_ = distortion_velocity_;
        clone->on_emitte_size_ = on_emitte_size_;
        clone->random_value_ = random_value_;
        clone->near_alpha_distance_ = near_alpha_distance_;
        clone->origin_range_ = origin_range_;
        clone->particle_num_ = particle_num_;
        clone->is_adulation_emitter = is_adulation_emitter;

        clone->play_type_ = play_type_;
        clone->blend_mode_ = blend_mode_;
        clone->origin_mode_ = origin_mode_;
        clone->eject_type_ = eject_type_ ;
        clone->move_surface_ = move_surface_;
        clone->posture_type_ = posture_type_;
        clone->disp_size_mode_ = disp_size_mode_;
        clone->disp_alpha_mode_ = disp_alpha_mode_;
        clone->distortion_mode_ = distortion_mode_;

        clone->vertexs_ = vertexs_;
        clone->createTargetIOBuffer();
        clone->vertex_shader_ = vertex_shader_;
        clone->pixel_shader_ = pixel_shader_;
        clone->geometry_shaders_[0] = geometry_shaders_[0];
        clone->geometry_shaders_[1] = geometry_shaders_[1];
        clone->input_layout_ = input_layout_;
        clone->constant_buffer_ = constant_buffer_; // ★

        clone->texture_ = texture_;
        clone->shader_resouce_view_ = shader_resouce_view_;
        clone->texture_resouce_ = texture_resouce_;
        clone->rasterizer_state_ = rasterizer_state_;
        clone->sampler_state_ = sampler_state_;

        return clone ;
    }



}
