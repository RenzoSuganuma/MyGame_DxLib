#include <VertexTypes.h>
#include "../dxlib_ext.h"
#include "dxlib_ext_mesh.h"
#include "dxlib_ext_inst_mesh.h"
#include "../effects/shader/instatncing/shader_instancing_vs.h"
#include "../effects/shader/instatncing/shader_instancing_ps.h"


namespace dxe {

#define DXE_INSTANCING_SHADER_COMPLIE 0

    using namespace DirectX;

    InstMesh::~InstMesh() {
        Shared<InstMeshPool> mesh_pool = ref_instance_->ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        ref_instance_->setDrawEnable(false);
        ref_instance_->setPosition({0, 0, 0});
        ref_instance_->setScale({ 1, 1, 1 });
        ref_instance_->setRotation(tnl::Quaternion());
        ref_instance_->setUseTextureIndex(0);
        mesh_pool->free_instance_stack_.push( ref_instance_ );
    }

    const tnl::Vector3& InstMesh::getPosition() { return ref_instance_->getPosition(); }
    const tnl::Vector3& InstMesh::getScale() { return ref_instance_->getScale(); }
    const tnl::Quaternion& InstMesh::getRotation() { return ref_instance_->getRotation(); }
    uint32_t InstMesh::getUseTextureIndex() { return ref_instance_->getUseTextureIndex(); }
    bool InstMesh::getDrawEnable() { return ref_instance_->getDrawEnable(); }

    void InstMesh::setPosition(const tnl::Vector3& pos) { ref_instance_->setPosition(pos); }
    void InstMesh::setScale(const tnl::Vector3& scl) { ref_instance_->setScale(scl); }
    void InstMesh::setRotation(const tnl::Quaternion& rot) { ref_instance_->setRotation(rot); }
    void InstMesh::setUseTextureIndex(uint32_t index) { ref_instance_->setUseTextureIndex(index); }
    void InstMesh::setDrawEnable(bool enable) { ref_instance_->setDrawEnable(enable); }



    InstMeshPool::Instance::Instance(const Shared<InstMeshPool>& pool) : ref_mesh_pool_(pool) {}
    InstMeshPool::Instance::~Instance() {}

    void InstMeshPool::Instance::setPosition(const tnl::Vector3& pos) {
        pos_ = pos;
        Shared<InstMeshPool> mesh_pool = ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        mesh_pool->setNeedsUpdated(true);
    }
    void InstMeshPool::Instance::setScale(const tnl::Vector3& scl) {
        scl_ = scl;
        Shared<InstMeshPool> mesh_pool = ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        mesh_pool->setNeedsUpdated(true);
    }
    void InstMeshPool::Instance::setRotation(const tnl::Quaternion& rot) {
        rot_ = rot;
        Shared<InstMeshPool> mesh_pool = ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        mesh_pool->setNeedsUpdated(true);
    }
    void InstMeshPool::Instance::setUseTextureIndex(uint32_t index) {
        Shared<InstMeshPool> mesh_pool = ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        if (mesh_pool->ref_texture_index_num_ < index) return;
        use_tex_index_ = index;
        mesh_pool->setNeedsUpdated(true);
    }
    void InstMeshPool::Instance::setDrawEnable(bool enable) {
        draw_enable_ = enable;
        Shared<InstMeshPool> mesh_pool = ref_mesh_pool_.lock();
        if (!mesh_pool) return;
        mesh_pool->setNeedsUpdated(true);
    }


    // シェーダ定数定義
    typedef struct _CBUFFER2
    {
        DirectX::XMMATRIX	mat_view_;              // ビュー 行列
        DirectX::XMMATRIX	mat_proj_;              // 射影 行列
        DxLib::COLOR_F diffuse_ = { 0, 0, 0, 0 };
        DxLib::COLOR_F ambient_ = { 0, 0, 0, 0 };
        DxLib::COLOR_F emissive_ = { 0, 0, 0, 0 };
        DxLib::COLOR_F specular_ = { 0, 0, 0, 0 };

        DxLib::VECTOR   light_direction_ = { 0, 0, 0 };
        float           alpha_ = 0;
        DxLib::COLOR_F  light_diffuse_ = { 0, 0, 0, 0 };
        DxLib::COLOR_F  light_ambient_ = { 0, 0, 0, 0 };
        DxLib::COLOR_F  light_specular_ = { 0, 0, 0, 0 };
        int             light_enable_ = 0;
    } INST_MESH_POOL_CBUFFER2;


    typedef struct _INSTANCE_DATA
    {
        tnl::Matrix	mat_world_;
        uint32_t tex_index_;
    }INSTANCE_DATA ;


    //--------------------------------------------------------------------------------------------------------------------------------
    Shared<InstMeshPool> InstMeshPool::Create(
        const Shared<Mesh>& mesh
        , uint32_t instance_num
        , const std::vector<Shared<dxe::Texture>>* textures ) {

        bool success = true;
        if (textures) {
            uint32_t w = (*textures)[0]->getWidth();
            uint32_t h = (*textures)[0]->getHeight();
            for (size_t i = 0; i < textures->size(); ++i) {
                if ((*textures)[i]->getWidth() != w) { success = false; break; }
                if ((*textures)[i]->getHeight() != h) { success = false; break; }
            }
        }
        else {
            if(!mesh->getTexture()) success = false;
        }
        if (!success) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : InstMeshPool::Create Texture is abnormal \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return nullptr;
        }


        // MV を直接ロードしたデータなど
        Shared<Mesh> clone = mesh->createClone();
        if (clone->getVertexs().empty()) {

            int err = MV1SetupReferenceMesh(clone->getDxMvHdl(), -1, 0, 0, -1);
            auto info = MV1GetReferenceMesh(clone->getDxMvHdl(), -1, 0, 0, -1);
            MV1TerminateReferenceMesh(clone->getDxMvHdl(), -1, 0, 0, -1);

            clone->idxs_.resize(info.PolygonNum * 3) ;
            clone->vtxs_.resize(info.PolygonNum * 3) ;

            for (uint32_t i = 0; i < (uint32_t)info.PolygonNum; ++i) {
                clone->idxs_[i * 3 + 0] = i * 3 + 0;
                clone->idxs_[i * 3 + 1] = i * 3 + 1;
                clone->idxs_[i * 3 + 2] = i * 3 + 2;

                clone->vtxs_[i * 3 + 0].pos = info.Vertexs[info.Polygons[i].VIndex[0]].Position;
                clone->vtxs_[i * 3 + 1].pos = info.Vertexs[info.Polygons[i].VIndex[1]].Position;
                clone->vtxs_[i * 3 + 2].pos = info.Vertexs[info.Polygons[i].VIndex[2]].Position;

                clone->vtxs_[i * 3 + 0].norm = info.Vertexs[info.Polygons[i].VIndex[0]].Normal;
                clone->vtxs_[i * 3 + 1].norm = info.Vertexs[info.Polygons[i].VIndex[1]].Normal;
                clone->vtxs_[i * 3 + 2].norm = info.Vertexs[info.Polygons[i].VIndex[2]].Normal;

                clone->vtxs_[i * 3 + 0].dif = info.Vertexs[info.Polygons[i].VIndex[0]].DiffuseColor;
                clone->vtxs_[i * 3 + 1].dif = info.Vertexs[info.Polygons[i].VIndex[1]].DiffuseColor;
                clone->vtxs_[i * 3 + 2].dif = info.Vertexs[info.Polygons[i].VIndex[2]].DiffuseColor;

                clone->vtxs_[i * 3 + 0].spc = info.Vertexs[info.Polygons[i].VIndex[0]].SpecularColor;
                clone->vtxs_[i * 3 + 1].spc = info.Vertexs[info.Polygons[i].VIndex[1]].SpecularColor;
                clone->vtxs_[i * 3 + 2].spc = info.Vertexs[info.Polygons[i].VIndex[2]].SpecularColor;

                clone->vtxs_[i * 3 + 0].su = 0;
                clone->vtxs_[i * 3 + 1].su = 0;
                clone->vtxs_[i * 3 + 2].su = 0;

                clone->vtxs_[i * 3 + 0].sv = 0;
                clone->vtxs_[i * 3 + 1].sv = 0;
                clone->vtxs_[i * 3 + 2].sv = 0;

                clone->vtxs_[i * 3 + 0].u = info.Vertexs[info.Polygons[i].VIndex[0]].TexCoord[0].u;
                clone->vtxs_[i * 3 + 1].u = info.Vertexs[info.Polygons[i].VIndex[1]].TexCoord[0].u;
                clone->vtxs_[i * 3 + 2].u = info.Vertexs[info.Polygons[i].VIndex[2]].TexCoord[0].u;

                clone->vtxs_[i * 3 + 0].v = info.Vertexs[info.Polygons[i].VIndex[0]].TexCoord[0].v;
                clone->vtxs_[i * 3 + 1].v = info.Vertexs[info.Polygons[i].VIndex[1]].TexCoord[0].v;
                clone->vtxs_[i * 3 + 2].v = info.Vertexs[info.Polygons[i].VIndex[2]].TexCoord[0].v;
            }

        }



        Shared<InstMeshPool> pool = std::shared_ptr<InstMeshPool>( new InstMeshPool(clone, instance_num, textures) );
        pool->instances_.resize(instance_num);
        for (uint32_t i = 0; i < instance_num; ++i) {
            pool->instances_[i] = std::shared_ptr<InstMeshPool::Instance>(new InstMeshPool::Instance(pool));
            pool->free_instance_stack_.push(pool->instances_[i]);
        }
        return pool;
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    Shared<InstMesh> InstMeshPool::CreateInstMesh() {
        if (free_instance_stack_.empty()) return nullptr;
        Shared<InstMesh> inst_mesh = std::shared_ptr<InstMesh>(new InstMesh(free_instance_stack_.top()));
        inst_mesh->setDrawEnable(true);
        free_instance_stack_.pop();
        return inst_mesh;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    InstMeshPool::InstMeshPool(
        const Shared<Mesh>& mesh
        , uint32_t instance_num
        , const std::vector<Shared<dxe::Texture>>* textures ) {

        if (textures) ref_texture_index_num_ = (uint32_t)textures->size();

        origine_clone_ = mesh;
        instance_num_ = instance_num;

		HRESULT hr = E_FAIL;
        D3D11_BUFFER_DESC bd;
        D3D11_SUBRESOURCE_DATA InitData;

		ID3DBlob* p_err = nullptr;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();


        //-------------------------------------------------------------------------------------------------
        // 頂点バッファの作成
        ID3D11Buffer* p_vertex_buffer = nullptr;
        auto meshVert = mesh->getVertexs();
        DirectX::VertexPositionNormalTexture* vertexs = new DirectX::VertexPositionNormalTexture[meshVert.size()];
        for (size_t i = 0; i < meshVert.size(); ++i) {
            vertexs[i].position = { TNL_DEP_V3( meshVert[i].pos ) };
            vertexs[i].normal   = { TNL_DEP_V3( meshVert[i].norm ) };
            vertexs[i].textureCoordinate = { meshVert[i].u, meshVert[i].v };
        }

        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = (UINT)( sizeof(DirectX::VertexPositionNormalTexture) * meshVert.size() ) ;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0 ;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = vertexs;
        hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_vertex_buffer);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Vertex Buffer \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
        vertex_buffer_.Attach(p_vertex_buffer);
        delete[] vertexs ;


        //-------------------------------------------------------------------------------------------------
        // インスタンシングバッファの作成
        ID3D11Buffer* p_instance_buffer = nullptr;
        INSTANCE_DATA* instance = new INSTANCE_DATA[instance_num] ;

        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(INSTANCE_DATA) * instance_num ;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = instance;
        hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_instance_buffer);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Instance Buffer \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
        instance_buffer_.Attach(p_instance_buffer);
        delete[] instance;

        //-------------------------------------------------------------------------------------------------
        // インデックスバッファの作成
        ID3D11Buffer* p_index_buffer = nullptr;
        auto meshIndexs = mesh->getIndexs() ;
        index_num_ = (uint32_t)meshIndexs.size();
        uint16_t* indexs = new uint16_t[index_num_];
        for (size_t i = 0; i < index_num_; ++i) {
            indexs[i] = meshIndexs[i];
        }
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(uint16_t) * index_num_;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        InitData.pSysMem = indexs;
        hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_index_buffer);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Index Buffer \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
        index_buffer_.Attach(p_index_buffer);
        delete[] indexs;



        // 頂点のインプットレイアウト
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "SV_Position",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",         0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",       0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

            { "MATRIX",         0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "MATRIX",         1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "MATRIX",         2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "MATRIX",         3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEX_INDEX",      0, DXGI_FORMAT_R32_UINT,           1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };

        //-------------------------------------------------------------------------------------------------
        // 頂点シェーダーの作成
        ID3DBlob* p_blob_vertex = nullptr;


#if DXE_INSTANCING_SHADER_COMPLIE 

        hr = D3DCompileFromFile(L"shader/instancing/instancing.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_vertex, &p_err);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Compile Vertex Shader \n");
            OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        ID3D11VertexShader* vertex_shader = nullptr;
        hr = pd3dDevice->CreateVertexShader(p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(), NULL, &vertex_shader);
        vertex_shader_.Attach(vertex_shader);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Vertex Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //-------------------------------------------------------------------------------------------------
        ID3D11InputLayout* il = nullptr;
        hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(),
            &il);
        input_layout_.Attach(il);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Input Layout \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

#else

        ID3D11VertexShader* vertex_shader = nullptr;
        hr = pd3dDevice->CreateVertexShader(&g_instancing_VSMain, sizeof(g_instancing_VSMain), NULL, &vertex_shader);
        vertex_shader_.Attach(vertex_shader);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Vertex Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //-------------------------------------------------------------------------------------------------
        // 頂点のインプットレイアウト作成
        ID3D11InputLayout* il = nullptr;
        hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            &g_instancing_VSMain, sizeof(g_instancing_VSMain),
            &il);
        input_layout_.Attach(il);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Input Layout \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

#endif



#if DXE_INSTANCING_SHADER_COMPLIE 

        //-------------------------------------------------------------------------------------------------
        // ピクセルシェーダの作成
        ID3DBlob* p_blob_pixel = nullptr;
        hr = D3DCompileFromFile(L"shader/instancing/instancing.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_pixel, &p_err);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Compile Pixel Shader \n");
            OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        ID3D11PixelShader* ps = nullptr;
        hr = pd3dDevice->CreatePixelShader(p_blob_pixel->GetBufferPointer(), p_blob_pixel->GetBufferSize(), NULL, &ps);
        pixel_shader_.Attach(ps);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Pixel Shader \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }
#else

        //
        // コンパイル済みヘッダから作成
        //
        ID3D11PixelShader* ps = nullptr;
        hr = pd3dDevice->CreatePixelShader(
            &g_instancing_PSMain,
            sizeof(g_instancing_PSMain), NULL, &ps);
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
        ID3D11Buffer* constant_buffer = nullptr;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.ByteWidth = sizeof(INST_MESH_POOL_CBUFFER2);
        hr = pd3dDevice->CreateBuffer(&bd, nullptr, &constant_buffer);
        constant_buffer_.Attach(constant_buffer);
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Create Constant Buffer \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }


        //-------------------------------------------------------------------------------------------------
        // テクスチャのロード
        ID3D11Resource* res = nullptr;
        ID3D11ShaderResourceView* srv = nullptr;
        if (textures) {

            // テクスチャの作成
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = (*textures)[0]->getWidth();
            textureDesc.Height = (*textures)[0]->getHeight();

            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = (UINT)textures->size() ;
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32ビットRGBA形式
            textureDesc.SampleDesc.Count = 1;
            textureDesc.Usage = D3D11_USAGE_DEFAULT ;
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE ;

            textureDesc.SampleDesc.Quality = 0;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;


            D3D11_SUBRESOURCE_DATA* initData = new D3D11_SUBRESOURCE_DATA[textures->size()];


            std::vector<std::tuple<std::unique_ptr<uint8_t>, uint32_t, uint32_t, uint32_t>> textureData ;
            textureData.resize(textures->size());

            for (int i = 0; i < textures->size(); ++i) {
                textureData[i] = tnl::LoadGraphicColorBufferFromMemory((unsigned char*)(*textures)[i]->getFileData(), (*textures)[i]->getDataSize());
                initData[i].pSysMem = std::get<0>(textureData[i]).get();
                initData[i].SysMemPitch = (*textures)[i]->getWidth() * sizeof(uint32_t);
                initData[i].SysMemSlicePitch = 0;
            }
            ID3D11Texture2D* texture2DArray;
            pd3dDevice->CreateTexture2D(&textureDesc, initData, &texture2DArray);
            texture_resouces_.Attach(texture2DArray);

            // シェーダーリソースビューの作成
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = textureDesc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            srvDesc.Texture2DArray.MostDetailedMip = 0;
            srvDesc.Texture2DArray.MipLevels = 1;
            srvDesc.Texture2DArray.FirstArraySlice = 0;
            srvDesc.Texture2DArray.ArraySize = (UINT)textures->size();

            ID3D11ShaderResourceView* srv;
            pd3dDevice->CreateShaderResourceView(texture2DArray, &srvDesc, &srv);
            shader_resouce_views_.Attach(srv);

            delete[] initData;
        }
        else {
            //textures_.emplace_back(mesh->getTexture());
            auto textureData = tnl::LoadGraphicColorBufferFromMemory((unsigned char*)(mesh->getTexture()->getFileData()), mesh->getTexture()->getDataSize());
                
            // テクスチャの作成
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = mesh->getTexture()->getWidth() ;
            textureDesc.Height = mesh->getTexture()->getHeight() ;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1 ;
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32ビットRGBA形式
            textureDesc.SampleDesc.Count = 1;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = std::get<0>(textureData).get();
            initData.SysMemPitch = mesh->getTexture()->getWidth() * sizeof(uint32_t);

            ID3D11Texture2D* texture2D;
            pd3dDevice->CreateTexture2D(&textureDesc, &initData, &texture2D);
            texture_resouces_.Attach(texture2D);

            // シェーダーリソースビューの作成
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = textureDesc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            srvDesc.Texture2DArray.MostDetailedMip = 0;
            srvDesc.Texture2DArray.MipLevels = 1;
            srvDesc.Texture2DArray.FirstArraySlice = 0;
            srvDesc.Texture2DArray.ArraySize = 1;  // 配列サイズを設定

            ID3D11ShaderResourceView* srv;
            pd3dDevice->CreateShaderResourceView(texture2D, &srvDesc, &srv);
            shader_resouce_views_.Attach(srv);
        }
	}


    //----------------------------------------------------------------------------------------------------------------------------
    void InstMeshPool::mapInstances(const Shared<dxe::Camera>& camera) {

        // 更新の必要はない
        if (!is_needs_updated_) return ;

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
        ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

        //インスタンスバッファにマップ
        D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
        D3D11_MAPPED_SUBRESOURCE mappedBuffer;

        //行列のマップ
        hr = pImmediateContext->Map(instance_buffer_.Get(), 0, mapType, 0, &mappedBuffer) ;
        if (S_OK != hr) {
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            tnl::DebugTrace("Error : Instancing Map \n");
            tnl::DebugTrace("-----------------------------------------------------------------\n");
            return;
        }

        //行列の変更
        auto* instance = (INSTANCE_DATA*)mappedBuffer.pData;
        XMMATRIX matT, matS, matR, matW ;
        for (size_t i = 0; i < instance_num_; i++) {
            if (dxe::Camera::eDimension::Type3D == camera->dimension_) {
                matT = XMMatrixTranslation(TNL_DEP_V3(instances_[i]->pos_));
            }
            else {
                matT = XMMatrixTranslation(instances_[i]->pos_.x, -instances_[i]->pos_.y, 0);
            }
            matR = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&instances_[i]->rot_) ) ;
            if (instances_[i]->draw_enable_) {
                matS = XMMatrixScaling(TNL_DEP_V3(instances_[i]->scl_));
            }
            else {
                matS = XMMatrixScaling(0, 0, 0);
            }
            matW = matS * matR * matT ;
            DirectX::XMStoreFloat4x4(&instance[i].mat_world_, XMMatrixTranspose(matW));
            instance[i].tex_index_ = instances_[i]->use_tex_index_;
        }

        //アンマップ
        pImmediateContext->Unmap(instance_buffer_.Get(), 0);


        is_needs_updated_ = false;
    }


    //----------------------------------------------------------------------------------------------------------------------------
    void InstMeshPool::render(const Shared<dxe::Camera>& camera) {

        mapInstances(camera);

        HRESULT hr = E_FAIL;
        ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
        ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

        //-------------------------------------------------------------------------------------------
        // 
        // 描画
        // 
        {
            D3D11_PRIMITIVE_TOPOLOGY bk_toplogy;
            ID3D11VertexShader* bk_vs = nullptr;
            ID3D11PixelShader* bk_ps = nullptr;
            ID3D11GeometryShader* bk_gs = nullptr;
            ID3D11Buffer* bk_ps_cb = nullptr;
            UINT bk_vtx_stride[2] = { 0 };
            UINT bk_vtx_offset[2] = { 0 };
            ID3D11Buffer* bk_vtx_buf[2] = { nullptr };
            ID3D11Buffer* bk_idx_buf = nullptr ;
            DXGI_FORMAT bk_idx_fmt;
            ID3D11InputLayout* bk_input_layout = nullptr;

            pImmediateContext->IAGetPrimitiveTopology(&bk_toplogy);
            pImmediateContext->VSGetShader(&bk_vs, NULL, 0);
            pImmediateContext->PSGetShader(&bk_ps, NULL, 0);
            pImmediateContext->GSGetShader(&bk_gs, NULL, 0);
            pImmediateContext->PSGetConstantBuffers(0, 1, &bk_ps_cb);
            pImmediateContext->IAGetVertexBuffers(0, 2, bk_vtx_buf, bk_vtx_stride, bk_vtx_offset);
            pImmediateContext->IAGetIndexBuffer(&bk_idx_buf, &bk_idx_fmt, 0);
            pImmediateContext->IAGetInputLayout(&bk_input_layout);


            // プリミティブ タイプおよびデータの順序に関する情報を設定
            pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            if (dxe::Camera::eDimension::Type3D == camera->getDimension()) {
                if (render_param_.getDepthTestEnable() && render_param_.getWriteDepthBufferEnable()) {
                    pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_ON_T_ON), 0);
                } 
                else if (render_param_.getDepthTestEnable() && !render_param_.getWriteDepthBufferEnable()) {
                    pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_ON), 0);
                }
                else if (!render_param_.getDepthTestEnable() && render_param_.getWriteDepthBufferEnable()) {
                    pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_ON_T_OFF), 0);
                }
                else if (!render_param_.getDepthTestEnable() && !render_param_.getWriteDepthBufferEnable()) {
                    pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_OFF), 0);
                }
            }
            else {
                pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_OFF), 0);
            }


            // 深度テストを無効にする
            pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_ON_T_ON), 0);

            // ラスタライザステート設定
            ID3D11RasterizerState* rs = GetRasterizerState(rasterizer_state_);
            pImmediateContext->RSSetState(rs);

            // ブレンドステート設定
            pImmediateContext->OMSetBlendState(GetBlendState(blend_state_), 0, 0xffffffff);

            // ハルシェーダーを無効にする。
            pImmediateContext->HSSetShader(NULL, NULL, 0);

            // ドメインシェーダーを無効にする。
            pImmediateContext->DSSetShader(NULL, NULL, 0);

            // ジオメトリシェーダを無効にする
            pImmediateContext->GSSetShader(NULL, NULL, 0);

            // コンピュートシェーダーを無効にする。
            pImmediateContext->CSSetShader(NULL, NULL, 0);


            //ストライドとオフセット
            //形状の頂点バッファと行列バッファを設定
            UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(INSTANCE_DATA) };
            UINT offset[2] = { 0, 0 };

            ID3D11Buffer* pBuf[2] = { vertex_buffer_.Get(), instance_buffer_.Get() };
            pImmediateContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

            //インデックスバッファのセット
            pImmediateContext->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R16_UINT, 0);

#if 1
            // 頂点データレイアウト設定
            ID3D11InputLayout* il = input_layout_.Get();
            pImmediateContext->IASetInputLayout(il);

            // 頂点シェーダーをセット
            ID3D11VertexShader* vs = vertex_shader_.Get();
            pImmediateContext->VSSetShader(vs, NULL, 0);


            // ピクセルシェーダーをセット
            ID3D11PixelShader* ps = pixel_shader_.Get();
            pImmediateContext->PSSetShader(ps, NULL, 0);

            // テクスチャーを設定する
            ID3D11ShaderResourceView* sv = shader_resouce_views_.Get();
            pImmediateContext->PSSetShaderResources(0, 1, &sv);

            // サンプラステートを設定する
            ID3D11SamplerState* st = GetSamplerState(sampler_state_);
            pImmediateContext->PSSetSamplers(0, 1, &st);


            XMMATRIX matView, matProj;
            matView = XMLoadFloat4x4(&camera->view_);
            matProj = XMLoadFloat4x4(&camera->proj_);
  

            INST_MESH_POOL_CBUFFER2 sb;
            sb.mat_view_ = XMMatrixTranspose(matView);
            sb.mat_proj_ = XMMatrixTranspose(matProj);
            tnl::Vector3 diff = getMtrlDiffuse();
            tnl::Vector3 ambi = getMtrlAmbient();
            tnl::Vector3 emis = getMtrlEmissive();
            tnl::Vector3 spec = getMtrlSpecular();


            sb.diffuse_ = { TNL_DEP_V3(diff), 0 };
            sb.ambient_ = { TNL_DEP_V3(ambi), 0 };
            sb.emissive_ = { TNL_DEP_V3(emis), 0 };
            sb.specular_ = { TNL_DEP_V3(spec), getMtrlSpecPower() };

            sb.light_direction_ = GetLightDirection();
            sb.alpha_ = alpha_;
            sb.light_diffuse_ = GetLightDifColor();
            sb.light_ambient_ = GetLightAmbColor();
            sb.light_specular_ = GetLightSpcColor();
            sb.light_enable_ = (is_lighting_enable_ && camera->getDimension() == dxe::Camera::eDimension::Type3D) ? 1 : 0;

            ID3D11Buffer* cb = constant_buffer_.Get();
            pImmediateContext->UpdateSubresource(cb, 0, nullptr, &sb, 0, 0);
            pImmediateContext->VSSetConstantBuffers(0, 1, &cb);
            pImmediateContext->PSSetConstantBuffers(0, 1, &cb);

            // シェーダに存在する変数に値を渡す
            D3D11_MAPPED_SUBRESOURCE MappedResource;
            if (SUCCEEDED(pImmediateContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource))) {
                memcpy(MappedResource.pData, &sb, sizeof(INST_MESH_POOL_CBUFFER2));
                pImmediateContext->Unmap(cb, 0);
            }


            //描画
            pImmediateContext->DrawIndexedInstanced(index_num_, instance_num_, 0, 0, 0);
#endif

            pImmediateContext->IASetPrimitiveTopology(bk_toplogy);
            pImmediateContext->VSSetShader(bk_vs, NULL, 0);
            pImmediateContext->PSSetShader(bk_ps, NULL, 0);
            pImmediateContext->GSSetShader(bk_gs, NULL, 0);
            pImmediateContext->PSSetConstantBuffers(0, 1, &bk_ps_cb);
            pImmediateContext->IASetVertexBuffers(0, 2, bk_vtx_buf, bk_vtx_stride, bk_vtx_offset);
            pImmediateContext->IASetIndexBuffer(bk_idx_buf, bk_idx_fmt, 0);
            pImmediateContext->IASetInputLayout(bk_input_layout);

        }

    }


    void InstMeshPool::reserveShadow() {
        for (auto inst : instances_) {
            if (!inst->getDrawEnable()) continue;
            origine_clone_->pos_ = inst->getPosition();
            origine_clone_->rot_ = inst->getRotation();
            origine_clone_->scl_ = inst->getScale();
            origine_clone_->reserveShadow();
        }
    }



}