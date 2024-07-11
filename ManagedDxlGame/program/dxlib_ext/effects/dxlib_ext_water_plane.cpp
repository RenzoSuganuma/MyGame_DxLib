#include <VertexTypes.h>
#include "../dxlib_ext.h"
#include "dxlib_ext_water_plane.h"
#include <DDSTextureLoader.h>
#include <DirectXTex.h>
#include "shader/water_plane/shader_water_plane_vs.h"
#include "shader/water_plane/shader_water_plane_ps.h"
#include "shader/water_plane/shader_water_plane_hs.h"
#include "shader/water_plane/shader_water_plane_ds.h"

namespace dxe {

#define DXE_WATER_PLANE_SHADER_COMPLIE 1

	using namespace DirectX;

	// 頂点定義
	struct WATER_PLANE_VERTEX
	{
		DirectX::XMFLOAT3 pos;    // 頂点座標
		DirectX::XMFLOAT2 texel;  // テクセル
	};


	// シェーダ定数定義
	typedef struct _CBUFFER
	{
		DirectX::XMMATRIX	mat_view_;              // ビュー 行列
		DirectX::XMMATRIX	mat_proj_;              // 射影 行列
		DirectX::XMMATRIX	mat_world_;
		DirectX::XMFLOAT4	sky_color_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	base_color_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	shallow_color_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	specular_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	wave_directionA_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	wave_directionB_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	wave_directionC_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4	wave_directionD_ = { 0, 0, 0, 0 };

		DirectX::XMFLOAT3	light_direction_ = { 0, 0, 0 };
		float				max_height_;
		DirectX::XMFLOAT3	cam_pos_;
		float				fresnel_base_alpha_;
		DirectX::XMFLOAT3	fog_color_;
		float				base_col_str_;
		float				shallow_col_str_;
		float				darkness_contrast_;
		float				specular_power_;
		float				shininess_;
		float				fresnel_factor_;
		float				noize_strength_;
		float				base_amp_factor_;
		float				cube_amp_factor_;
		float				frequency_factor_;
		float				steepness_factor_;
		float				progress_time_;
		float				size_width_;
		float				size_depth_;
		float				pattern_col_factor_;
		float				decoration_col_factor_;
		float				landing_projection_col_factor_;
		float				landing_projection_col_pow_;
		float				fog_start_;
		float				fog_end_;
		int					use_pattern_tex ;
		int					use_decoration_tex ;
		int					use_landing_projection_tex_;
		int					use_landing_cut_out_tex_;
		int					use_cube_map_tex ;
		int					light_enable_;
		int					wave_direciton_;
		int					gerstner_sampling_;
		int				    far_fade_;
	} WATER_PLANE_CBUFFER1;



	typedef struct _CBUFFER1
	{
		float g_fTessFactor;       // ポリゴンのエッジのテッセレーション係数
		float g_fInsideTessFactor; // ポリゴン内部のテッセレーション係数
		float g_Dummy2;
		float g_Dummy3;
	}WATER_MESH_CBUFFER2;


	//-------------------------------------------------------------------------------------------------------------------------------
	WaterPlane::~WaterPlane() {}

	//-------------------------------------------------------------------------------------------------------------------------------
	//
	//
	//
	WaterPlane::WaterPlane(
		eSize width
		, eSize depth
		, uint32_t div
		, uint32_t patch ){

		e_size_width_ = width;
		e_size_depth_ = depth;
		size_width_ = (float)SIZE_TABLE[static_cast<uint32_t>(width)];
		size_depth_ = (float)SIZE_TABLE[static_cast<uint32_t>(depth)];
		div_num_ = std::clamp<uint32_t>(div, 1, DIV_MAX);
		patch_num_ = (float)std::clamp<uint32_t>(patch, 1, PATCH_MAX);

		initialize();
		createVertexBuffer();
		setWaveDirection(e_wave_direction_);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	//
	//
	//
	WaterPlane::WaterPlane(const std::string& file_path) {
		initialize();
		loadStatus(file_path);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::initialize() {

		HRESULT hr = E_FAIL;
		D3D11_BUFFER_DESC bd;

		ID3DBlob* p_err = nullptr;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		//------------------------------------------------------------------------------------------------------------------
		// 頂点シェーダーの作成
		ID3DBlob* p_blob_vertex = nullptr;
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

#if DXE_WATER_PLANE_SHADER_COMPLIE

		hr = D3DCompileFromFile(L"shader/water_plane/water_plane.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VS_Main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_vertex, &p_err);
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

#else

		ID3D11VertexShader* vertex_shader = nullptr;
		hr = pd3dDevice->CreateVertexShader(&g_WaterPlane_VS_Main, sizeof(g_WaterPlane_VS_Main), NULL, &vertex_shader);
		vertex_shader_.Attach(vertex_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterPlane Create Vertex Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

		//-------------------------------------------------------------------------------------------------
		// 頂点のインプットレイアウト作成
		ID3D11InputLayout* il = nullptr;
		hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
			&g_WaterPlane_VS_Main, sizeof(g_WaterPlane_VS_Main),
			&il);
		input_layout_.Attach(il);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterPlane Create Input Layout \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

#endif



#if DXE_WATER_PLANE_SHADER_COMPLIE

		//------------------------------------------------------------------------------------------------------------------
		// ハルシェーダーの作成
		ID3DBlob* p_blob_hull = nullptr;
		hr = D3DCompileFromFile(L"shader/water_plane/water_plane.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"HS_Main", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_hull, &p_err);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Compile Hull Shader \n");
			OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		ID3D11HullShader* hull_shader = nullptr;
		hr = pd3dDevice->CreateHullShader(p_blob_hull->GetBufferPointer(), p_blob_hull->GetBufferSize(), NULL, &hull_shader);
		hull_shader_.Attach(hull_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Hull Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

#else

		ID3D11HullShader* hull_shader = nullptr;
		hr = pd3dDevice->CreateHullShader(&g_WaterPlane_HS_Main, sizeof(g_WaterPlane_HS_Main), NULL, &hull_shader);
		hull_shader_.Attach(hull_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterPlane Create Hull Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

#endif


#if DXE_WATER_PLANE_SHADER_COMPLIE

		//------------------------------------------------------------------------------------------------------------------
		// ドメインシェーダーの作成
		ID3DBlob* p_blob_domain = nullptr;
		hr = D3DCompileFromFile(L"shader/water_plane/water_plane.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"DS_Main", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_domain, &p_err);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Compile Domain Shader \n");
			OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		ID3D11DomainShader* domain_shader = nullptr;
		hr = pd3dDevice->CreateDomainShader(p_blob_domain->GetBufferPointer(), p_blob_domain->GetBufferSize(), NULL, &domain_shader);
		domain_shader_.Attach(domain_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Domain Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

#else

		ID3D11DomainShader* domain_shader = nullptr;
		hr = pd3dDevice->CreateDomainShader(&g_WaterPlane_DS_Main, sizeof(g_WaterPlane_DS_Main), NULL, &domain_shader);
		domain_shader_.Attach(domain_shader);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterPlane Create Domain Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}


#endif


#if DXE_WATER_PLANE_SHADER_COMPLIE

		//-------------------------------------------------------------------------------------------------
		// ピクセルシェーダの作成
		ID3DBlob* p_blob_pixel = nullptr;
		hr = D3DCompileFromFile(L"shader/water_plane/water_plane.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PS_Main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_pixel, &p_err);
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
#else

		ID3D11PixelShader* ps = nullptr;
		hr = pd3dDevice->CreatePixelShader(&g_WaterPlane_PS_Main, sizeof(g_WaterPlane_PS_Main), NULL, &ps);
		pixel_shader_.Attach(ps);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterPlane Create Pixel Shader \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

#endif


		//-------------------------------------------------------------------------------------------------
		// シェーダ定数バッファの作成
		ID3D11Buffer* constant_buffer1 = nullptr;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(WATER_PLANE_CBUFFER1);
		hr = pd3dDevice->CreateBuffer(&bd, nullptr, &constant_buffer1);
		constant_buffer1_.Attach(constant_buffer1);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Constant Buffer 1 \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		ID3D11Buffer* constant_buffer2 = nullptr;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(WATER_MESH_CBUFFER2);
		hr = pd3dDevice->CreateBuffer(&bd, nullptr, &constant_buffer2);
		constant_buffer2_.Attach(constant_buffer2);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Constant Buffer 2 \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}

		//-------------------------------------------------------------------------------------------------
		// 設置投影テクスチャの作成
		tex_landing_projection_ = std::make_shared<AreaProjectionTexture>(
			(uint32_t)size_width_, (uint32_t)size_depth_,
			AreaProjectionTexture::eDpiDivide::DIV2,
			AreaProjectionTexture::eDpiDivide::DIV4, true);
		tex_landing_projection_->setProjectionMode(AreaProjectionTexture::eProjectionMode::VOLUME_Y);

		//-------------------------------------------------------------------------------------------------
		// 設置くり抜きテクスチャの作成
		tex_landing_cut_out_ = std::make_shared<AreaProjectionTexture>(
			(uint32_t)size_width_, (uint32_t)size_depth_,
			AreaProjectionTexture::eDpiDivide::EQUAL,
			AreaProjectionTexture::eDpiDivide::EQUAL, false);
		tex_landing_cut_out_->setProjectionMode(AreaProjectionTexture::eProjectionMode::NEGATIVE_OVER_Y);
		tex_landing_cut_out_->setBorderPosition(3.0f);

		//-------------------------------------------------------------------------------------------------
		// DDSテクスチャのロード
		tex_cube_map_ = dxe::Texture::CreateCubeMap(dxe::Texture::eCubeMapSize::S256);

	}


	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::loadPatternTexture(const std::string& pattern_texture_path) {
		tex_pattern_ = dxe::Texture::CreateFromFile(pattern_texture_path);
		tex_pattern_->createShaderResourceView();
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::loadDecorationTexture(const std::string& decoration_texture_path) {
		tex_decoration_ = dxe::Texture::CreateFromFile(decoration_texture_path);
		tex_decoration_->createShaderResourceView();
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::setCubeMapCameraPosition(const tnl::Vector3& camera_position) {
		tex_cube_map_->setCubeMapCameraPosition(camera_position);
	}

	void WaterPlane::writeToCubeMap(const std::function< void(std::shared_ptr<dxe::Camera>&) >& render) {
		tex_cube_map_->writeToCubeMap(render);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::setWaveDecentralization(const float& decent) {
		wave_decentralization_ = decent;
		float r = wave_decentralization_;
		wave_directionA_.x = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionA_.y = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionA_.z = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionA_.w = tnl::GetRandomDistributionFloat(-r, r);

		wave_directionB_.x = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionB_.y = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionB_.z = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionB_.w = tnl::GetRandomDistributionFloat(-r, r);

		wave_directionC_.x = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionC_.y = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionC_.z = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionC_.w = tnl::GetRandomDistributionFloat(-r, r);

		wave_directionD_.x = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionD_.y = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionD_.z = tnl::GetRandomDistributionFloat(-r, r);
		wave_directionD_.w = tnl::GetRandomDistributionFloat(-r, r);

		if (e_wave_direction_ == eWaveDirection::RANDOM) {
			wave_directionA_ = { 0.35f, 0.31f, 0.08f, 0.60f };
			wave_directionB_ = { -0.95f, -0.74f, 0.7f, -0.5f };
			wave_directionC_ = { 0.7f, 0.6f, 0.10f, 0.38f };
			wave_directionD_ = { 0.43f, 0.07f, 0.42f, 0.61f };
		}
		else if (e_wave_direction_ == eWaveDirection::LEFT) {
			wave_directionA_.x = 1.0f;
			wave_directionA_.z = 1.0f;
			wave_directionB_.x = 1.0f;
			wave_directionB_.z = 1.0f;
			wave_directionC_.x = 1.0f;
			wave_directionC_.z = 1.0f;
			wave_directionD_.x = 1.0f;
			wave_directionD_.z = 1.0f;
		}
		else if (e_wave_direction_ == eWaveDirection::RIGHT) {
			wave_directionA_.x = -1.0f;
			wave_directionA_.z = -1.0f;
			wave_directionB_.x = -1.0f;
			wave_directionB_.z = -1.0f;
			wave_directionC_.x = -1.0f;
			wave_directionC_.z = -1.0f;
			wave_directionD_.x = -1.0f;
			wave_directionD_.z = -1.0f;
		}
		else if (e_wave_direction_ == eWaveDirection::FORWARD) {
			wave_directionA_.y = -1.0f;
			wave_directionA_.w = -1.0f;
			wave_directionB_.y = -1.0f;
			wave_directionB_.w = -1.0f;
			wave_directionC_.y = -1.0f;
			wave_directionC_.w = -1.0f;
			wave_directionD_.y = -1.0f;
			wave_directionD_.w = -1.0f;
		}
		else if (e_wave_direction_ == eWaveDirection::BACK) {
			wave_directionA_.y = 1.0f;
			wave_directionA_.w = 1.0f;
			wave_directionB_.y = 1.0f;
			wave_directionB_.w = 1.0f;
			wave_directionC_.y = 1.0f;
			wave_directionC_.w = 1.0f;
			wave_directionD_.y = 1.0f;
			wave_directionD_.w = 1.0f;
		}
		else if (e_wave_direction_ == eWaveDirection::LEFT_FORWARD) {
			float n = 0.5f;
			wave_directionA_.x += n;
			wave_directionA_.z += n;
			wave_directionA_.y -= n;
			wave_directionA_.w -= n;
			wave_directionB_.x += n;
			wave_directionB_.z += n;
			wave_directionB_.y -= n;
			wave_directionB_.w -= n;
			wave_directionC_.x += n;
			wave_directionC_.z += n;
			wave_directionC_.y -= n;
			wave_directionC_.w -= n;
			wave_directionD_.x += n;
			wave_directionD_.z += n;
			wave_directionD_.y -= n;
			wave_directionD_.w -= n;
		}
		else if (e_wave_direction_ == eWaveDirection::LEFT_BACK) {
			float n = 0.5f;
			wave_directionA_.x += n;
			wave_directionA_.z += n;
			wave_directionA_.y += n;
			wave_directionA_.w += n;
			wave_directionB_.x += n;
			wave_directionB_.z += n;
			wave_directionB_.y += n;
			wave_directionB_.w += n;
			wave_directionC_.x += n;
			wave_directionC_.z += n;
			wave_directionC_.y += n;
			wave_directionC_.w += n;
			wave_directionD_.x += n;
			wave_directionD_.z += n;
			wave_directionD_.y += n;
			wave_directionD_.w += n;
		}
		else if (e_wave_direction_ == eWaveDirection::RIGHT_FORWARD) {
			float n = 0.5f;
			wave_directionA_.x -= n;
			wave_directionA_.z -= n;
			wave_directionA_.y -= n;
			wave_directionA_.w -= n;
			wave_directionB_.x -= n;
			wave_directionB_.z -= n;
			wave_directionB_.y -= n;
			wave_directionB_.w -= n;
			wave_directionC_.x -= n;
			wave_directionC_.z -= n;
			wave_directionC_.y -= n;
			wave_directionC_.w -= n;
			wave_directionD_.x -= n;
			wave_directionD_.z -= n;
			wave_directionD_.y -= n;
			wave_directionD_.w -= n;
		}
		else if (e_wave_direction_ == eWaveDirection::RIGHT_BACK) {
			float n = 0.5f;
			wave_directionA_.x -= n;
			wave_directionA_.z -= n;
			wave_directionA_.y += n;
			wave_directionA_.w += n;
			wave_directionB_.x -= n;
			wave_directionB_.z -= n;
			wave_directionB_.y += n;
			wave_directionB_.w += n;
			wave_directionC_.x -= n;
			wave_directionC_.z -= n;
			wave_directionC_.y += n;
			wave_directionC_.w += n;
			wave_directionD_.x -= n;
			wave_directionD_.z -= n;
			wave_directionD_.y += n;
			wave_directionD_.w += n;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::setWaveDirection(const eWaveDirection& e_wave_direction) {
		e_wave_direction_ = e_wave_direction;
		setWaveDecentralization(wave_decentralization_);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::setSizeWidth(const eSize& width) {
		e_size_width_ = width;
		size_width_ = (float)SIZE_TABLE[static_cast<uint32_t>(e_size_width_)];
		createVertexBuffer();
		resetInnerCreateTexture();
	}

	void WaterPlane::setSizeDepth(const eSize& depth) {
		e_size_depth_ = depth;
		size_depth_ = (float)SIZE_TABLE[static_cast<uint32_t>(e_size_depth_)];;
		createVertexBuffer();
		resetInnerCreateTexture();
	}

	void WaterPlane::setPatchNum(const float& path) {
		patch_num_ = path;
		patch_num_ = (float)std::clamp<uint32_t>((uint32_t)patch_num_, 1, PATCH_MAX);
		createVertexBuffer();
	}
	void WaterPlane::setDivNum(const uint32_t& div) {
		div_num_ = div;
		div_num_ = std::clamp<uint32_t>(div, 1, DIV_MAX);
		createVertexBuffer();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::createVertexBuffer() {

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA InitData;

		//------------------------------------------------------------------------------------------------------------------
		// 頂点座標を反時計回りで設定
		//WATER_MESH_VERTEX vertexs[] = {
		//	XMFLOAT3(width, 0, depth), XMFLOAT2(1.0f, 0.0f),
		//	XMFLOAT3(-width, 0.0f, depth), XMFLOAT2(0.0f, 0.0f),
		//	XMFLOAT3(width, 0.0f, -depth), XMFLOAT2(1.0f, 1.0f),
		//	XMFLOAT3(-width, 0.0f,-depth), XMFLOAT2(0.0f, 1.0f),
		//};
		//WATER_MESH_VERTEX vertexs[] = {
		//	XMFLOAT3(-width,	0, depth), XMFLOAT2(0.0f, 0.0f),
		//	XMFLOAT3(0,			0, depth), XMFLOAT2(0.5f, 0.0f),
		//	XMFLOAT3(width,		0, depth), XMFLOAT2(1.0f, 0.0f),
		//	XMFLOAT3(-width,	0, 0), XMFLOAT2(0.0f, 0.5f),
		//	XMFLOAT3(0,			0, 0), XMFLOAT2(0.5f, 0.5f),
		//	XMFLOAT3(width,		0, 0), XMFLOAT2(1.0f, 0.5f),
		//	XMFLOAT3(-width,	0, -depth), XMFLOAT2(0.0f, 1.0f),
		//	XMFLOAT3(0,			0, -depth), XMFLOAT2(0.5f, 1.0f),
		//	XMFLOAT3(width,		0, -depth), XMFLOAT2(1.0f, 1.0f)
		//};

		uint32_t v_line = div_num_ + 1;
		uint32_t vtx_num = v_line * v_line;
		WATER_PLANE_VERTEX* vertexs = new WATER_PLANE_VERTEX[vtx_num];

		float sx = -size_width_ * 0.5f;
		float sz = size_depth_ * 0.5f;
		float vw = size_width_ / div_num_;
		float vd = size_depth_ / div_num_;
		for (int z = 0; z < int(v_line); ++z) {
			for (int x = 0; x < int(v_line); ++x) {
				vertexs[(z * v_line) + x].pos = XMFLOAT3(sx + vw * x, 0, sz - (vd * z));
				vertexs[(z * v_line) + x].texel = XMFLOAT2(vw * x / size_width_, vd * z / size_depth_);
			}
		}


		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = (UINT)(sizeof(WATER_PLANE_VERTEX) * vtx_num);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexs;
		ID3D11Buffer* p_vertex_buffer = nullptr;
		hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_vertex_buffer);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Vertex Buffer \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		vertex_buffer_.Attach(p_vertex_buffer);
		delete[] vertexs;

		//------------------------------------------------------------------------------------------------------------------
		// インデックスバッファを設定
		index_num_ = div_num_ * div_num_ * 4;
		//UINT indexs[] = { 
		//	0, 1, 3, 4,
		//	1, 2, 4, 5,
		//	3, 4, 6, 7,
		//	4, 5, 7, 8
		//};


		UINT* indexs = new UINT[index_num_];
		int ic = 0;
		for (int i = 0; i < int(div_num_); ++i) {
			for (int k = 0; k < int(div_num_); ++k) {
				indexs[ic + 0] = (i * (div_num_ + 1)) + k;
				indexs[ic + 1] = indexs[ic + 0] + 1;
				indexs[ic + 2] = indexs[ic + 1] + div_num_;
				indexs[ic + 3] = indexs[ic + 2] + 1;
				ic += 4;
			}
		}

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(uint32_t) * index_num_;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = indexs;
		ID3D11Buffer* p_index_buffer = nullptr;
		hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_index_buffer);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : WaterMesh Create Index Buffer \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		index_buffer_.Attach(p_index_buffer);
		delete[] indexs;

	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::resetInnerCreateTexture() {
		Shared<dxe::AreaProjectionTexture> area_tex = std::make_shared<AreaProjectionTexture>(
			(uint32_t)size_width_, (uint32_t)size_depth_,
			tex_landing_projection_->getSrvDpiDivide(),
			tex_landing_projection_->getBlurDpiDivide(), true);
		area_tex->setProjectionMode(AreaProjectionTexture::eProjectionMode::VOLUME_Y);

		auto mesh_map = tex_landing_projection_->getMeshContainers();
		for (auto it : mesh_map) {
			area_tex->addProjectionMesh(it.second->getMesh(), it.second->getProjectionVolume(), static_cast<AreaProjectionTexture::fUseColorFlag>(it.second->getUseColorFlag()));
		}
		tex_landing_projection_ = area_tex;

		Shared<dxe::AreaProjectionTexture> cut_out_tex = std::make_shared<AreaProjectionTexture>(
			(uint32_t)size_width_, (uint32_t)size_depth_,
			AreaProjectionTexture::eDpiDivide::EQUAL,
			AreaProjectionTexture::eDpiDivide::EQUAL, false);
		cut_out_tex->setProjectionMode(AreaProjectionTexture::eProjectionMode::NEGATIVE_OVER_Y);
		cut_out_tex->setBorderPosition(tex_landing_cut_out_->getBorderPosition());

		mesh_map = tex_landing_cut_out_->getMeshContainers();
		for (auto it : mesh_map) {
			cut_out_tex->addProjectionMesh(it.second->getMesh(), it.second->getProjectionVolume(), static_cast<AreaProjectionTexture::fUseColorFlag>(it.second->getUseColorFlag()));
		}
		tex_landing_cut_out_ = cut_out_tex;
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::addLandingMesh(const Shared<dxe::Mesh>& mesh, fLandingMeshUse f_use, const float projection_volume) {
		Shared<AreaProjectionMeshContainer> mesh_ct = nullptr;

		if (static_cast<bool>( ( fLandingMeshUse::BUBLLE | fLandingMeshUse::FADE_ALPHA ) & f_use)) {
			mesh_ct = tex_landing_projection_->addProjectionMesh(mesh, projection_volume, static_cast<AreaProjectionTexture::fUseColorFlag>(0) );
		}
		if (mesh_ct) {
			if (static_cast<bool>(fLandingMeshUse::BUBLLE & f_use)) {
				mesh_ct->setUseColorFlag(static_cast<int>(AreaProjectionTexture::fUseColorFlag::RED));
			}
			if (static_cast<bool>(fLandingMeshUse::FADE_ALPHA & f_use)) {
				mesh_ct->setUseColorFlag(static_cast<int>(AreaProjectionTexture::fUseColorFlag::GREEN));
			}
			if (static_cast<bool>((fLandingMeshUse::BUBLLE | fLandingMeshUse::FADE_ALPHA) == f_use)) {
				mesh_ct->setUseColorFlag(static_cast<int>(AreaProjectionTexture::fUseColorFlag::RED | AreaProjectionTexture::fUseColorFlag::GREEN));
			}
		}

		if (static_cast<bool>(fLandingMeshUse::CUT_OUT & f_use)) {
			tex_landing_cut_out_->addProjectionMesh(mesh, 5.0f, AreaProjectionTexture::fUseColorFlag::RED);
		}
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::setLandingProjectionSrvDpiDivide(AreaProjectionTexture::eDpiDivide srv_dpi_divide) {
		tex_landing_projection_->setSrvDpiDivide(srv_dpi_divide);
	}
	void WaterPlane::setLandingProjectionBlurDpiDivide(AreaProjectionTexture::eDpiDivide blur_dpi_divide) {
		tex_landing_projection_->setBlurDpiDivide(blur_dpi_divide);
	}
	void WaterPlane::setLandingProjectionVolume(float height) {
		tex_landing_projection_->setEreaVolume(height);
	}
	void WaterPlane::setLandingProjectionColDensity(float density) {
		tex_landing_projection_->setColDensity(density);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	float WaterPlane::getLandingProjectionVolume() {
		return tex_landing_projection_->getEreaVolume();
	}
	float WaterPlane::getLandingProjectionColDensity() {
		return tex_landing_projection_->getColDensity();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	//
	//
	//
	void WaterPlane::render(const Shared<dxe::Camera>& camera) {

		float fog_start, fog_end;
		int fog_r, fog_g, fog_b;
		int fog_enable = GetFogEnable();
		int far_fade = 0;

		if (e_far_fade_ != eFarFade::NONE) {
			if (fog_enable) {
				far_fade = static_cast<int>(e_far_fade_);
			}
		}

		GetFogStartEnd(&fog_start, &fog_end);
		GetFogColor(&fog_r, &fog_g, &fog_b);

		if (use_landing_projection_tex_) {
			tex_landing_projection_->setCameraPosition(pos_);
			tex_landing_projection_->update();
		}
		if (use_landing_cut_out_tex_) {
			tex_landing_cut_out_->setCameraPosition(pos_);
			tex_landing_cut_out_->update();
		}

		progress_time_ += dxe::GetDeltaTime() * time_scale_;

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		//-------------------------------------------------------------------------------------------
		// 
		// 描画
		// 

		// プリミティブ タイプおよびデータの順序に関する情報を設定
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

		// 深度テストを無効にする
		pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_ON), 0);

		// ラスタライザステート設定
		ID3D11RasterizerState* rs = GetRasterizerState(rasterizer_state_);
		pImmediateContext->RSSetState(rs);

		// ブレンドステート設定
		pImmediateContext->OMSetBlendState(GetBlendState(blend_state_), 0, 0xffffffff);

		// ジオメトリシェーダを無効にする
		pImmediateContext->GSSetShader(NULL, NULL, 0);

		// コンピュートシェーダーを無効にする。
		pImmediateContext->CSSetShader(NULL, NULL, 0);

		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride = sizeof(WATER_PLANE_VERTEX);
		UINT offset = 0;

		ID3D11Buffer* pBuf = vertex_buffer_.Get();
		pImmediateContext->IASetVertexBuffers(0, 1, &pBuf, &stride, &offset);

		//インデックスバッファのセット
		pImmediateContext->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

		// 頂点データレイアウト設定
		ID3D11InputLayout* il = input_layout_.Get();
		pImmediateContext->IASetInputLayout(il);

		// 頂点シェーダーをセット
		ID3D11VertexShader* vs = vertex_shader_.Get();
		pImmediateContext->VSSetShader(vs, NULL, 0);

		// ハルシェーダーをセット
		ID3D11HullShader* hs = hull_shader_.Get();
		pImmediateContext->HSSetShader(hs, NULL, 0);

		// ドメインシェーダーをセット
		ID3D11DomainShader* ds = domain_shader_.Get();
		pImmediateContext->DSSetShader(ds, NULL, 0);

		// ピクセルシェーダーをセット
		ID3D11PixelShader* ps = pixel_shader_.Get();
		pImmediateContext->PSSetShader(ps, NULL, 0);


		// テクスチャーを設定する
		if (use_decoration_tex_ && tex_decoration_) {
			ID3D11ShaderResourceView* sv = tex_decoration_->getShaderResourceView().Get();
			pImmediateContext->PSSetShaderResources(0, 1, &sv);
		}

		if (use_pattern_tex_ && tex_pattern_) {
			ID3D11ShaderResourceView* nsv = tex_pattern_->getShaderResourceView().Get();
			pImmediateContext->PSSetShaderResources(1, 1, &nsv);
			pImmediateContext->DSSetShaderResources(1, 1, &nsv);
		}

		if (use_landing_projection_tex_ && use_landing_projection_tex_) {
			ID3D11ShaderResourceView* lsv = tex_landing_projection_->getBlurTexture()->getShaderResourceView().Get();
			pImmediateContext->PSSetShaderResources(2, 1, &lsv);
		}

		if (use_landing_cut_out_tex_ && gui_use_landing_cut_out_tex_) {
			ID3D11ShaderResourceView* csv = tex_landing_cut_out_->getSrvTexture()->getShaderResourceView().Get();
			pImmediateContext->PSSetShaderResources(3, 1, &csv);
		}

		if (use_cube_map_tex_ && tex_cube_map_) {
			ID3D11ShaderResourceView* dsv = tex_cube_map_->getShaderResourceView().Get();
			pImmediateContext->PSSetShaderResources(4, 1, &dsv);
		}

		// サンプラステートを設定する
		ID3D11SamplerState* st = GetSamplerState(sampler_state_);
		pImmediateContext->PSSetSamplers(0, 1, &st);


		tnl::Matrix world = tnl::Matrix::AffineTransformation(pos_, scl_, rot_);

		XMMATRIX matWorld, matView, matProj;
		matView = XMLoadFloat4x4(&camera->view_);
		matProj = XMLoadFloat4x4(&camera->proj_);
		matWorld = XMLoadFloat4x4(&world);


		WATER_PLANE_CBUFFER1 sb1;
		sb1.mat_view_ = XMMatrixTranspose(matView);
		sb1.mat_proj_ = XMMatrixTranspose(matProj);
		sb1.mat_world_ = XMMatrixTranspose(matWorld);

		sb1.sky_color_ = { TNL_DEP_V3(sky_color_), 1 };
		sb1.base_color_ = { TNL_DEP_V3(base_color_), 1 };
		sb1.shallow_color_ = { TNL_DEP_V3(shallow_color_), 1 };
		sb1.specular_ = { TNL_DEP_V3(specular_color_), 1 };
		sb1.wave_directionA_ = wave_directionA_;
		sb1.wave_directionB_ = wave_directionB_;
		sb1.wave_directionC_ = wave_directionC_;
		sb1.wave_directionD_ = wave_directionD_;
		sb1.light_direction_ = tnl::Vector3::Normalize( light_direction_ ) ;
		sb1.max_height_ = height_max_;
		sb1.cam_pos_ = camera->pos_;
		sb1.fresnel_base_alpha_ = fresnel_base_alpha_;
		sb1.fog_color_ = { float(fog_r) / 255.0f, float(fog_g) / 255.0f, float(fog_b) / 255.0f };
		sb1.base_col_str_ = base_col_str_;
		sb1.progress_time_ = progress_time_;
		sb1.shallow_col_str_ = shallow_col_str_;
		sb1.darkness_contrast_ = darkness_contrast_;
		sb1.specular_power_ = specular_power_;
		sb1.shininess_ = shininess_;
		sb1.fresnel_factor_ = fresnel_factor_;
		sb1.noize_strength_ = noize_strength_;
		sb1.base_amp_factor_ = base_amp_factor_;
		sb1.cube_amp_factor_ = cube_amp_factor_;
		sb1.frequency_factor_ = frequency_factor_;
		sb1.steepness_factor_ = steepness_factor_;
		sb1.size_width_ = size_width_;
		sb1.size_depth_ = size_depth_;
		sb1.pattern_col_factor_ = pattern_col_factor_;
		sb1.decoration_col_factor_ = decoration_col_factor_;
		sb1.landing_projection_col_factor_ = landing_projection_col_factor_;
		sb1.landing_projection_col_pow_ = landing_projection_col_pow_;
		sb1.fog_start_ = fog_start;
		sb1.fog_end_ = fog_end;

		sb1.use_pattern_tex = (use_pattern_tex_ && tex_pattern_) ? 1 : 0;
		sb1.use_decoration_tex = (use_decoration_tex_ && tex_decoration_) ? 1 : 0;
		sb1.use_landing_projection_tex_ = (use_landing_projection_tex_ && tex_landing_projection_) ? 1 : 0;
		sb1.use_landing_cut_out_tex_ = (use_landing_cut_out_tex_ && tex_landing_cut_out_) ? 1 : 0;
		sb1.use_cube_map_tex = (use_cube_map_tex_ && tex_cube_map_) ? 1 : 0;
		sb1.light_enable_ = 1;
		sb1.wave_direciton_ = static_cast<int>( e_wave_direction_ );
		sb1.gerstner_sampling_ = static_cast<int>( e_gerstner_sampling_ );
		sb1.far_fade_ = far_fade;

		ID3D11Buffer* cb1 = constant_buffer1_.Get();
		pImmediateContext->UpdateSubresource(cb1, 0, nullptr, &sb1, 0, 0);
		pImmediateContext->VSSetConstantBuffers(0, 1, &cb1);
		pImmediateContext->PSSetConstantBuffers(0, 1, &cb1);
		pImmediateContext->DSSetConstantBuffers(0, 1, &cb1);

		// シェーダに存在する変数に値を渡す
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		if (SUCCEEDED(pImmediateContext->Map(cb1, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource))) {
			memcpy(MappedResource.pData, &sb1, sizeof(WATER_PLANE_CBUFFER1));
			pImmediateContext->Unmap(cb1, 0);
		}

		WATER_MESH_CBUFFER2 sb2;
		ID3D11Buffer* cb2 = constant_buffer2_.Get();

		sb2.g_fTessFactor = patch_num_;
		sb2.g_fInsideTessFactor = patch_num_;

		pImmediateContext->UpdateSubresource(cb2, 0, nullptr, &sb2, 0, 0);

		// シェーダに存在する変数に値を渡す
		if (SUCCEEDED(pImmediateContext->Map(cb2, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource))) {
			memcpy(MappedResource.pData, &sb2, sizeof(WATER_MESH_CBUFFER2));
			pImmediateContext->Unmap(cb2, 0);
		}
		pImmediateContext->HSSetConstantBuffers(1, 1, &cb2);

		//描画
		pImmediateContext->DrawIndexed(index_num_, 0, 0);

	}


	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::saveStatus(const std::string& file_path) {
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "wb");
		if (fp) {
			int ver = 1;
			fwrite(&ver, sizeof(int), 1, fp);

			int wave_direction = static_cast<int>(e_wave_direction_);
			fwrite(&wave_direction, sizeof(int), 1, fp);

			int gerstner_sampling = static_cast<int>(e_gerstner_sampling_);
			fwrite(&gerstner_sampling, sizeof(int), 1, fp);

			int far_fade = static_cast<int>(e_far_fade_);
			fwrite(&far_fade, sizeof(int), 1, fp);

			int e_size_width = static_cast<int>(e_size_width_);
			fwrite(&e_size_width, sizeof(int), 1, fp);

			int e_size_depth = static_cast<int>(e_size_depth_);
			fwrite(&e_size_depth, sizeof(int), 1, fp);

			fwrite(&size_width_, sizeof(float), 1, fp);
			fwrite(&size_depth_, sizeof(float), 1, fp);
			fwrite(&patch_num_, sizeof(float), 1, fp);
			fwrite(&fresnel_base_alpha_, sizeof(float), 1, fp);
			fwrite(&height_max_, sizeof(float), 1, fp);
			fwrite(&progress_time_, sizeof(float), 1, fp);
			fwrite(&base_col_str_, sizeof(float), 1, fp);
			fwrite(&shallow_col_str_, sizeof(float), 1, fp);
			fwrite(&darkness_contrast_, sizeof(float), 1, fp);
			fwrite(&specular_power_, sizeof(float), 1, fp);
			fwrite(&shininess_, sizeof(float), 1, fp);
			fwrite(&fresnel_factor_, sizeof(float), 1, fp);
			fwrite(&noize_strength_, sizeof(float), 1, fp);
			fwrite(&base_amp_factor_, sizeof(float), 1, fp);
			fwrite(&cube_amp_factor_, sizeof(float), 1, fp);
			fwrite(&frequency_factor_, sizeof(float), 1, fp);
			fwrite(&steepness_factor_, sizeof(float), 1, fp);
			fwrite(&time_scale_, sizeof(float), 1, fp);
			fwrite(&pattern_col_factor_, sizeof(float), 1, fp);
			fwrite(&decoration_col_factor_, sizeof(float), 1, fp);
			fwrite(&landing_projection_col_factor_, sizeof(float), 1, fp);
			fwrite(&landing_projection_col_pow_, sizeof(float), 1, fp);
			fwrite(&wave_decentralization_, sizeof(float), 1, fp);
			fwrite(&div_num_, sizeof(uint32_t), 1, fp);
			fwrite(&index_num_, sizeof(uint32_t), 1, fp);

			int use_pattern_tex = (use_pattern_tex_ && tex_pattern_) ? 1 : 0;
			fwrite(&use_pattern_tex, sizeof(int), 1, fp);

			int use_decoration_tex = (use_decoration_tex_ && tex_decoration_) ? 1 : 0;
			fwrite(&use_decoration_tex, sizeof(int), 1, fp);

			int use_landing_projection_tex = (use_landing_projection_tex_) ? 1 : 0;
			fwrite(&use_landing_projection_tex, sizeof(int), 1, fp);

			int use_landing_cut_out_tex = (use_landing_cut_out_tex_) ? 1 : 0;
			fwrite(&use_landing_cut_out_tex, sizeof(int), 1, fp);

			int use_cube_map_tex = (use_cube_map_tex_) ? 1 : 0;
			fwrite(&use_cube_map_tex, sizeof(int), 1, fp);

			int blend_state = static_cast<int>(blend_state_);
			fwrite(&blend_state, sizeof(int), 1, fp);

			int sampler_state = static_cast<int>(sampler_state_);
			fwrite(&sampler_state, sizeof(int), 1, fp);

			int rasterizer_state = static_cast<int>(rasterizer_state_);
			fwrite(&rasterizer_state, sizeof(int), 1, fp);

			fwrite(&sky_color_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&base_color_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&shallow_color_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&specular_color_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&light_direction_, sizeof(tnl::Vector3), 1, fp);

			fwrite(&wave_directionA_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fwrite(&wave_directionB_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fwrite(&wave_directionC_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fwrite(&wave_directionD_, sizeof(DirectX::XMFLOAT4), 1, fp);

			fwrite(&pos_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&scl_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&rot_, sizeof(tnl::Quaternion), 1, fp);

			if (use_pattern_tex_) {
				char filename[256] = { 0 };
				sprintf_s(filename, "%s", tex_pattern_->getFileName().c_str());
				fwrite(filename, sizeof(filename), 1, fp);

				int data_size = tex_pattern_->getDataSize();
				fwrite(&data_size, sizeof(int), 1, fp);

				fwrite(tex_pattern_->getFileData(), data_size, 1, fp);
			}
			if (use_decoration_tex_) {
				char filename[256] = { 0 };
				sprintf_s(filename, "%s", tex_decoration_->getFileName().c_str());
				fwrite(filename, sizeof(filename), 1, fp);

				int data_size = tex_decoration_->getDataSize();
				fwrite(&data_size, sizeof(int), 1, fp);

				fwrite(tex_decoration_->getFileData(), data_size, 1, fp);
			}

			fclose(fp);
		}

	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void WaterPlane::loadStatus(const std::string& file_path) {

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {
			int ver = 0;
			fread(&ver, sizeof(int), 1, fp);

			int wave_direction = 0;
			fread(&wave_direction, sizeof(int), 1, fp);
			e_wave_direction_ = static_cast<eWaveDirection>(wave_direction);

			int gerstner_sampling = 0;
			fread(&gerstner_sampling, sizeof(int), 1, fp);
			e_gerstner_sampling_ = static_cast<eGerstnerSampling>(gerstner_sampling);

			int far_fade = 0;
			fread(&far_fade, sizeof(int), 1, fp);
			e_far_fade_ = static_cast<eFarFade>(far_fade);

			int e_size_width = 0;
			fread(&e_size_width, sizeof(int), 1, fp);
			e_size_width_ = static_cast<eSize>(e_size_width);

			int e_size_depth = 0;
			fread(&e_size_depth, sizeof(int), 1, fp);
			e_size_depth_ = static_cast<eSize>(e_size_depth);

			fread(&size_width_, sizeof(float), 1, fp);
			fread(&size_depth_, sizeof(float), 1, fp);
			fread(&patch_num_, sizeof(float), 1, fp);
			fread(&fresnel_base_alpha_, sizeof(float), 1, fp);
			fread(&height_max_, sizeof(float), 1, fp);
			fread(&progress_time_, sizeof(float), 1, fp);
			fread(&base_col_str_, sizeof(float), 1, fp);
			fread(&shallow_col_str_, sizeof(float), 1, fp);
			fread(&darkness_contrast_, sizeof(float), 1, fp);
			fread(&specular_power_, sizeof(float), 1, fp);
			fread(&shininess_, sizeof(float), 1, fp);
			fread(&fresnel_factor_, sizeof(float), 1, fp);
			fread(&noize_strength_, sizeof(float), 1, fp);
			fread(&base_amp_factor_, sizeof(float), 1, fp);
			fread(&cube_amp_factor_, sizeof(float), 1, fp);
			fread(&frequency_factor_, sizeof(float), 1, fp);
			fread(&steepness_factor_, sizeof(float), 1, fp);
			fread(&time_scale_, sizeof(float), 1, fp);
			fread(&pattern_col_factor_, sizeof(float), 1, fp);
			fread(&decoration_col_factor_, sizeof(float), 1, fp);
			fread(&landing_projection_col_factor_, sizeof(float), 1, fp);
			fread(&landing_projection_col_pow_, sizeof(float), 1, fp);
			fread(&wave_decentralization_, sizeof(float), 1, fp);
			fread(&div_num_, sizeof(uint32_t), 1, fp);
			fread(&index_num_, sizeof(uint32_t), 1, fp);

			int use_pattern_tex = (use_pattern_tex_ && tex_pattern_) ? 1 : 0;
			fread(&use_pattern_tex, sizeof(int), 1, fp);
			use_pattern_tex_ = (use_pattern_tex)? true : false ;

			int use_decoration_tex = (use_decoration_tex_ && tex_decoration_) ? 1 : 0;
			fread(&use_decoration_tex, sizeof(int), 1, fp);
			use_decoration_tex_ = (use_decoration_tex) ? true : false;

			int use_landing_projection_tex = (use_landing_projection_tex_) ? 1 : 0;
			fread(&use_landing_projection_tex, sizeof(int), 1, fp);
			use_landing_projection_tex_ = (use_landing_projection_tex) ? true : false;

			int use_landing_cut_out_tex = (use_landing_cut_out_tex_) ? 1 : 0;
			fread(&use_landing_cut_out_tex, sizeof(int), 1, fp);
			use_landing_cut_out_tex_ = (use_landing_cut_out_tex) ? true : false;

			int use_cube_map_tex = (use_cube_map_tex_) ? 1 : 0;
			fread(&use_cube_map_tex, sizeof(int), 1, fp);
			use_cube_map_tex_ = (use_cube_map_tex) ? true : false;

			int blend_state = 0 ;
			fread(&blend_state, sizeof(int), 1, fp);
			blend_state_ = static_cast<eBlendState>(blend_state);

			int sampler_state = 0;
			fread(&sampler_state, sizeof(int), 1, fp);
			sampler_state_ = static_cast<eSamplerState>(sampler_state);

			int rasterizer_state = 0;
			fread(&rasterizer_state, sizeof(int), 1, fp);
			rasterizer_state_ = static_cast<eRasterizerState>(rasterizer_state);

			fread(&sky_color_, sizeof(tnl::Vector3), 1, fp);
			fread(&base_color_, sizeof(tnl::Vector3), 1, fp);
			fread(&shallow_color_, sizeof(tnl::Vector3), 1, fp);
			fread(&specular_color_, sizeof(tnl::Vector3), 1, fp);
			fread(&light_direction_, sizeof(tnl::Vector3), 1, fp);

			fread(&wave_directionA_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fread(&wave_directionB_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fread(&wave_directionC_, sizeof(DirectX::XMFLOAT4), 1, fp);
			fread(&wave_directionD_, sizeof(DirectX::XMFLOAT4), 1, fp);

			fread(&pos_, sizeof(tnl::Vector3), 1, fp);
			fread(&scl_, sizeof(tnl::Vector3), 1, fp);
			fread(&rot_, sizeof(tnl::Quaternion), 1, fp);

			if (use_pattern_tex_) {
				char filename[256] = { 0 };
				fread(filename, sizeof(filename), 1, fp);

				int data_size = 0;
				fread(&data_size, sizeof(int), 1, fp);

				char* texture_data = new char[data_size];
				fread(texture_data, data_size, 1, fp);
				tex_pattern_ = dxe::Texture::CreateFromMemory(texture_data, data_size, filename);
				tex_pattern_->createShaderResourceView();
				delete[] texture_data;
			}

			if (use_decoration_tex_) {
				char filename[256] = { 0 };
				fread(filename, sizeof(filename), 1, fp);

				int data_size = 0;
				fread(&data_size, sizeof(int), 1, fp);

				char* texture_data = new char[data_size];
				fread(texture_data, data_size, 1, fp);
				tex_decoration_ = dxe::Texture::CreateFromMemory(texture_data, data_size, filename);
				tex_decoration_->createShaderResourceView();
				delete[] texture_data;
			}

			fclose(fp);

		}

		createVertexBuffer();
		resetInnerCreateTexture();
		setWaveDecentralization(wave_decentralization_);


		gui_use_pattern_tex_ = nullptr ;
		gui_use_decoration_tex_ = nullptr ;
		gui_use_landing_projection_tex_ = nullptr ;
		gui_use_landing_cut_out_tex_ = nullptr ;
		gui_use_cube_map_tex_ = nullptr ;
		gui_div_num_ = nullptr ;
		gui_patch_num_ = nullptr ;
		gui_sky_color_ = nullptr ;
		gui_base_color_ = nullptr ;
		gui_shallow_color_ = nullptr ;
		gui_specular_color_ = nullptr ;
		gui_light_direction_ = nullptr ;
		gui_height_max_ = nullptr ;
		gui_base_col_str_ = nullptr ;
		gui_shallow_col_str_ = nullptr ;
		gui_darkness_contrast_ = nullptr ;
		gui_specular_power_ = nullptr ;
		gui_shininess_ = nullptr ;
		gui_fresnel_factor_ = nullptr ;
		gui_fresnel_base_alpha_ = nullptr ;
		gui_noize_strength_ = nullptr ;
		gui_base_amp_factor_ = nullptr ;
		gui_cube_amp_factor_ = nullptr ;
		gui_frequency_factor_ = nullptr ;
		gui_steepness_factor_ = nullptr ;
		gui_pattern_col_factor_ = nullptr ;
		gui_decoration_col_factor_ = nullptr ;
		gui_landing_projection_col_factor_ = nullptr ;
		gui_landing_projection_col_pow_ = nullptr ;
		gui_wave_decentralization_ = nullptr ;
		gui_time_scale_ = nullptr ;
		gui_wave_direction_ = nullptr ;
		gui_far_fade_ = nullptr ;
		gui_gerstner_sampling_ = nullptr ;
		gui_size_width_ = nullptr ;
		gui_size_depth_ = nullptr ;

	}



}