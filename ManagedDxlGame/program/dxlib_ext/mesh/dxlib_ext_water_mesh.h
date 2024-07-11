#pragma once

#include "../../library/tnl_vector3.h"
#include "../../library/tnl_matrix.h"
#include "../../library/tnl_quaternion.h"

#include <string>
#include "../../library/tnl_using.h"
#include "../dxlib_ext_texture.h"
#include "../dxlib_ext_data.h"
#include "dxlib_ext_mesh_base.h"

#undef LONG_PTR
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace dxe {

	class WaterMesh : public MeshBase {
	public :
		WaterMesh(float width, float depth, uint32_t div_w, uint32_t div_d, const std::string& perlin_noize_file_path, const Shared<dxe::Texture>& texture);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// 描画
		void render(const Shared<dxe::Camera>& camera);

		TNL_PROPERTY(tnl::Vector3, Position, pos_);
		TNL_PROPERTY(tnl::Vector3, Scale, scl_);
		TNL_PROPERTY(tnl::Quaternion, Rotation, rot_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ブレンドステート設定
		TNL_PROPERTY(eBlendState, BlendState, blend_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// サンプラステート設定
		TNL_PROPERTY(eSamplerState, SamplerState, sampler_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ラスタライザステート設定
		TNL_PROPERTY(eRasterizerState, RasterizerState, rasterizer_state_);

	private :

		// 頂点定義
		typedef struct _VERTEX
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 textureCoordinate;
			float			  height_max;
		} WATER_MESH_VERTEX;

		tnl::Vector3	pos_ = { 0, 0, 0 };
		tnl::Vector3	scl_ = { 1,1,1 };
		tnl::Quaternion rot_;

		float						height_max_ = 1.0f;
		float						time_scale_ = 1.0f;
		float						progress_time_ = 0;
		uint32_t					index_num_ = 0;
		eBlendState					blend_state_ = eBlendState::NORMAL;
		eSamplerState				sampler_state_ = eSamplerState::ANISOTROPIC;
		eRasterizerState			rasterizer_state_ = eRasterizerState::CULL_NONE;

		// 頂点バッファ
		ComPtr<ID3D11Buffer>				vertex_buffer_ = nullptr;
		// インデックスバッファ
		ComPtr<ID3D11Buffer>				index_buffer_ = nullptr;
		// 頂点シェーダー
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// ピクセルシェーダー
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;

		// 頂点入力レイアウト
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// 定数バッファ
		ComPtr<ID3D11Buffer>                constant_buffer_ = nullptr;

		// シェーダーリソースビュー
		ComPtr<ID3D11Resource>			    texture_resouce_;
		ComPtr<ID3D11ShaderResourceView>	shader_resouce_view_;

	};

}
