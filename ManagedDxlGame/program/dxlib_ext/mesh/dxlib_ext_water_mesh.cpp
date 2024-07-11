#include <VertexTypes.h>
#include "../dxlib_ext.h"
#include "dxlib_ext_water_mesh.h"

namespace dxe {

	using namespace DirectX;

	// シェーダ定数定義
	typedef struct _CBUFFER
	{
		DirectX::XMMATRIX	mat_view_;              // ビュー 行列
		DirectX::XMMATRIX	mat_proj_;              // 射影 行列
		DirectX::XMMATRIX	mat_world_;
		float max_height_;
		float progress_time_;
	} WATER_MESH_CBUFFER;

	//------------------------------------------------------------------------------------------------------------------------------
	WaterMesh::WaterMesh(
		float width
		, float depth
		, uint32_t div_w
		, uint32_t div_d
		, const std::string& perlin_noize_file_path
		, const Shared<dxe::Texture>& texture ) {

		height_max_ = 30.0f;
		time_scale_ = 3.0f;

		HRESULT hr = E_FAIL;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA InitData;

		ID3DBlob* p_err = nullptr;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		auto tpl = tnl::LoadGraphicColorBuffer(perlin_noize_file_path);
		uint8_t* col_buf = std::get<0>(tpl).get();
		uint32_t tsw = std::get<1>(tpl);
		uint32_t tsh = std::get<2>(tpl);

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_d * 2) - (div_d - 1));

		//-------------------------------------------------------------------------------------------------
		// 頂点バッファの作成
		ID3D11Buffer* p_vertex_buffer = nullptr;

		WATER_MESH_VERTEX* vertexs = new WATER_MESH_VERTEX[vtx_num];

		float sx = width * 0.5f;
		float sz = depth * 0.5f;

		float u_size = 1.0f;
		float v_size = 1.0f;

		for (int i = 0; i < (div_d + 1); ++i) {
			float v = i / ((float)div_d);
			float z = -sz + v * depth;

			for (int k = 0; k < (div_w + 1); ++k) {
				float u = k / ((float)div_w);
				tnl::Vector3 vv = { -sx + u * width, 0, z };
				vertexs[(i * (div_w + 1)) + k].position.x = vv.x;
				vertexs[(i * (div_w + 1)) + k].position.z = vv.z;

				int px = tsw / (div_w+1) * k;
				int py = tsh / (div_d+1) * i;
				uint8_t* ps = &col_buf[(py * tsw * 4) + (px * 4)];
				uint8_t r = ps[0];
				uint8_t g = ps[1];
				uint8_t b = ps[2];

				float topology = (((float)r + (float)g + (float)b) / (255.0f * 3.0f)) ;
				vertexs[(i * (div_w + 1)) + k].position.y = topology * 10 ;

				vertexs[(i * (div_w + 1)) + k].normal.x = 0;
				vertexs[(i * (div_w + 1)) + k].normal.y = 1;
				vertexs[(i * (div_w + 1)) + k].normal.z = 0;

				vertexs[(i * (div_w + 1)) + k].textureCoordinate.x = (u * u_size);
				vertexs[(i * (div_w + 1)) + k].textureCoordinate.y = 1.0f - (v * v_size);

				vertexs[(i * (div_w + 1)) + k].height_max = tnl::GetRandomDistributionFloat(0.1f, height_max_) ;
			}
		}

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = (UINT)(sizeof(WATER_MESH_VERTEX) * vtx_num );
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexs;
		hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_vertex_buffer);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Vertex Buffer \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		vertex_buffer_.Attach(p_vertex_buffer);
		delete[] vertexs;


		//-------------------------------------------------------------------------------------------------
		// インデックスバッファの作成
		index_num_ = div_w * div_d * 6;

		ID3D11Buffer* p_index_buffer = nullptr;
		uint32_t* indexs = new uint32_t[index_num_];

		// 頂点インデックスの計算
		for (int i = 0; i < div_d; ++i) {
			for (int k = 0; k < div_w; ++k) {
				int a = (i * div_w + k) * 6;

				// 左上の三角形
				indexs[a + 0] = (i * (div_w + 1)) + k;
				indexs[a + 1] = indexs[a + 0] + 1;
				indexs[a + 2] = indexs[a + 0] + (div_w + 1);
				// 右下の三角形
				indexs[a + 3] = indexs[a + 2];
				indexs[a + 4] = indexs[a + 1];
				indexs[a + 5] = indexs[a + 3] + 1;

			}
		}

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t) * index_num_;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = indexs;
		hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_index_buffer);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Index Buffer \n");
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
			{ "HEIGHT_MAX",     0, DXGI_FORMAT_R32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		//-------------------------------------------------------------------------------------------------
		// 頂点シェーダーの作成
		ID3DBlob* p_blob_vertex = nullptr;

		hr = D3DCompileFromFile(L"shader/water_mesh/water_mesh.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_vertex, &p_err);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Compile Vertex Shader \n");
			OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

		ID3D11VertexShader* vertex_shader = nullptr;
		hr = pd3dDevice->CreateVertexShader(p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(), NULL, &vertex_shader);
		vertex_shader_.Attach(vertex_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Vertex Shader \n");
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
			tnl::DebugTrace("Error : WaterMesh Create Input Layout \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

		//-------------------------------------------------------------------------------------------------
		// ピクセルシェーダの作成
		ID3DBlob* p_blob_pixel = nullptr;
		hr = D3DCompileFromFile(L"shader/water_mesh/water_mesh.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_pixel, &p_err);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Compile Pixel Shader \n");
			OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

		ID3D11PixelShader* ps = nullptr;
		hr = pd3dDevice->CreatePixelShader(p_blob_pixel->GetBufferPointer(), p_blob_pixel->GetBufferSize(), NULL, &ps);
		pixel_shader_.Attach(ps);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Pixel Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}


		//-------------------------------------------------------------------------------------------------
		// シェーダ定数バッファの作成
		ID3D11Buffer* constant_buffer = nullptr;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(WATER_MESH_CBUFFER);
		hr = pd3dDevice->CreateBuffer(&bd, nullptr, &constant_buffer);
		constant_buffer_.Attach(constant_buffer);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Constant Buffer \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}


		//-------------------------------------------------------------------------------------------------
		// テクスチャのロード
		ID3D11Resource* res = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
		hr = CreateWICTextureFromMemory(pd3dDevice, (uint8_t*)texture->getFileData(), (size_t)texture->getDataSize(), &res, &srv);
		texture_resouce_.Attach(res);
		shader_resouce_view_.Attach(srv);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Load Texture \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}


	}

	//------------------------------------------------------------------------------------------------------------------------------
	void WaterMesh::render(const Shared<dxe::Camera>& camera) {

		progress_time_ += dxe::GetDeltaTime() * time_scale_ ;

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		//-------------------------------------------------------------------------------------------
		// 
		// 描画
		// 
		{

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
			UINT stride = sizeof(WATER_MESH_VERTEX) ;
			UINT offset = 0;

			ID3D11Buffer* pBuf = vertex_buffer_.Get() ;
			pImmediateContext->IASetVertexBuffers(0, 1, &pBuf, &stride, &offset);

			//インデックスバッファのセット
			pImmediateContext->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

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
			ID3D11ShaderResourceView* sv = shader_resouce_view_.Get();
			pImmediateContext->PSSetShaderResources(0, 1, &sv);

			// サンプラステートを設定する
			ID3D11SamplerState* st = GetSamplerState(sampler_state_);
			pImmediateContext->PSSetSamplers(0, 1, &st);


			tnl::Matrix world = tnl::Matrix::AffineTransformation(pos_, scl_, rot_);

			XMMATRIX matWorld, matView, matProj;
			matView = XMLoadFloat4x4(&camera->view_);
			matProj = XMLoadFloat4x4(&camera->proj_);
			matWorld = XMLoadFloat4x4(&world);

			WATER_MESH_CBUFFER sb;
			sb.mat_view_ = XMMatrixTranspose(matView);
			sb.mat_proj_ = XMMatrixTranspose(matProj);
			sb.mat_world_ = XMMatrixTranspose(matWorld);
			sb.max_height_ = height_max_;
			sb.progress_time_ = progress_time_;

			ID3D11Buffer* cb = constant_buffer_.Get();
			pImmediateContext->UpdateSubresource(cb, 0, nullptr, &sb, 0, 0);
			pImmediateContext->VSSetConstantBuffers(0, 1, &cb);
			pImmediateContext->PSSetConstantBuffers(0, 1, &cb);

			// シェーダに存在する変数に値を渡す
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			if (SUCCEEDED(pImmediateContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource))) {
				memcpy(MappedResource.pData, &sb, sizeof(WATER_MESH_CBUFFER));
				pImmediateContext->Unmap(cb, 0);
			}


			//描画
			pImmediateContext->DrawIndexedInstanced(index_num_, 1, 0, 0, 0);


		}


	}

}