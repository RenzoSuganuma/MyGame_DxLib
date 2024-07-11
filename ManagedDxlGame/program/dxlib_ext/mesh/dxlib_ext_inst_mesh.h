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
	class Mesh;
	class InstMesh;

	class InstMeshPool final : public MeshBase {
	public :
		~InstMeshPool() {}

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// メッシュプールの作成
		// arg1... 増やしたいメッシュ
		// arg2... 増やす数
		// arg3... 使用するテクスチャ配列
		static Shared<InstMeshPool> Create(const Shared<Mesh>& mesh, uint32_t instance_num, const std::vector<Shared<dxe::Texture>>* textures = nullptr) ;

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// 個別のインスタンシングメッシュの取得
		// tips... 得られたメッシュが解放されると自動的にプールに返還されます
		Shared<InstMesh> CreateInstMesh();

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// インスタンスの最大数
		uint32_t getMaxInstanceNum() { return instance_num_; }

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// 取得可能な残りのインスタンス数
		uint32_t getFreeInstanceNum() { return (uint32_t)free_instance_stack_.size(); }

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ライティング ON / OFF
		TNL_PROPERTY(bool, LightingEnable, is_lighting_enable_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ブレンドステート設定
		TNL_PROPERTY(eBlendState, BlendState, blend_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// サンプラステート設定
		TNL_PROPERTY(eSamplerState, SamplerState, sampler_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ラスタライザステート設定
		TNL_PROPERTY(eRasterizerState, RasterizerState, rasterizer_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// 描画
		void render(const Shared<dxe::Camera>& camera);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// 遮蔽物としてシャドウマップの準備
		// tips... 遮蔽物として機能しますが、内部でインスタンスの数分だけ描画が実行されるため相応のコストになります
		void reserveShadow();


	private :
		friend class InstMesh;

		class Instance final {
		public:
			~Instance();
			const tnl::Vector3& getPosition() { return pos_; }
			const tnl::Vector3& getScale() { return scl_; }
			const tnl::Quaternion& getRotation() { return rot_; }
			uint32_t getUseTextureIndex() { return use_tex_index_; }
			bool getDrawEnable() { return draw_enable_; }

			void setPosition(const tnl::Vector3& pos);
			void setScale(const tnl::Vector3& scl);
			void setRotation(const tnl::Quaternion& rot);
			void setUseTextureIndex(uint32_t index);
			void setDrawEnable(bool enable);
		private:
			friend class InstMesh;
			friend class InstMeshPool;
			Instance(const Shared<InstMeshPool>& pool);
			bool draw_enable_ = false;
			uint32_t use_tex_index_ = 0;
			tnl::Vector3 pos_;
			tnl::Vector3 scl_ = { 1,1,1 };
			tnl::Quaternion rot_;
			Weak<InstMeshPool> ref_mesh_pool_;
		};

		InstMeshPool(const Shared<Mesh>& mesh, uint32_t instance_num, const std::vector<Shared<dxe::Texture>>* textures = nullptr );
		void mapInstances(const Shared<dxe::Camera>& camera);


		TNL_PROPERTY(bool, NeedsUpdated, is_needs_updated_);
		bool						is_lighting_enable_ = true;
		bool						is_needs_updated_ = true;
		uint32_t					ref_texture_index_num_ = 0;
		uint32_t					instance_num_ = 0 ;
		uint32_t					index_num_ = 0 ;
		eBlendState					blend_state_ = eBlendState::NORMAL;
		eSamplerState				sampler_state_ = eSamplerState::ANISOTROPIC;
		eRasterizerState			rasterizer_state_ = eRasterizerState::CULL_NONE;
		Shared<Mesh>				origine_clone_;
		std::vector<Shared<Instance>>	instances_;
		std::stack<Shared<Instance>>	free_instance_stack_;

		// 頂点定義
		typedef struct _VERTEX
		{
			DirectX::XMFLOAT3 position_;
			DirectX::XMFLOAT3 normal_;
			DirectX::XMFLOAT2 tex_coord_;
		} INST_MESH_VERTEX;

		// 頂点バッファ
		ComPtr<ID3D11Buffer>				vertex_buffer_ = nullptr ;
		// インスタンスバッファ
		ComPtr<ID3D11Buffer>				instance_buffer_ = nullptr;
		// インデックスバッファ
		ComPtr<ID3D11Buffer>				index_buffer_ = nullptr ;
		// 頂点シェーダー
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// ピクセルシェーダー
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;

		// 頂点入力レイアウト
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// 定数バッファ
		ComPtr<ID3D11Buffer>                constant_buffer_ = nullptr;

		// シェーダーリソースビュー
		ComPtr<ID3D11Texture2D>			    texture_resouces_;
		ComPtr<ID3D11ShaderResourceView>	shader_resouce_views_;

	};


	class InstMesh final {
	public:

		~InstMesh();
		const tnl::Vector3& getPosition();
		const tnl::Vector3& getScale();
		const tnl::Quaternion& getRotation();
		uint32_t getUseTextureIndex();
		bool getDrawEnable();

		void setPosition(const tnl::Vector3& pos);
		void setScale(const tnl::Vector3& scl);
		void setRotation(const tnl::Quaternion& rot);
		void setUseTextureIndex(uint32_t index);
		void setDrawEnable(bool enable);

	private:
		friend class InstMeshPool;
		InstMesh() = default;
		InstMesh(Shared<InstMeshPool::Instance> instance) : ref_instance_(instance) {}
		Shared<InstMeshPool::Instance> ref_instance_ = nullptr;
	};


}