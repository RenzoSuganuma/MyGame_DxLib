#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include "../library/tnl_rect.h"
#include "../library/tnl_vector3.h"

#undef LONG_PTR
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


namespace dxe {

	class Camera;
	class Texture final {
	public :
		enum class eType {
			NORMAL,
			SRV_CUBE_MAP,
			SRV_DEPTH,
			SRV_RENDER_TARGET
		};

		~Texture();

		// 幅の取得
		uint32_t getWidth() { return width_; }

		// 高さの取得
		uint32_t getHeight() { return height_; }

		// 矩形情報
		tnl::Rect& getRect() { return rect_; }

		// ファイルパスの取得
		const std::string& getFilePath() { return file_path_; }

		// ファイル名の取得
		const std::string& getFileName() { return file_name_; }

		// グラフィックハンドルの取得
		int getDxLibGraphHandle() { return graph_hdl_; }

		// データサイズの取得
		uint32_t getDataSize() { return data_size_; }

		// ファイルデータの取得
		// tips... ピクセルデータではありません
		char* getFileData() { return file_data_; }

		ComPtr<ID3D11ShaderResourceView> getShaderResourceView() { return shader_resource_view_; }

		// 生成
		// arg1... ファイルパス
		static std::shared_ptr<Texture> CreateFromFile( const std::string& file_path );
		static std::shared_ptr<Texture> CreateFromMemory( char* file_data, uint32_t data_size, const std::string& regist_key_ = "" );
		static std::shared_ptr<Texture> CreateFromColorBufferRGBA8( unsigned char* color_buffer_rgba8, uint32_t width, uint32_t height, const std::string& regist_key);
		static std::shared_ptr<Texture> CreateFromDxLibGraphHdl( int dxlib_graph_hdl );
		static std::vector<std::shared_ptr<Texture>> CreateFromFileDiv(const std::string& file_path, uint32_t frame_w, uint32_t frame_h);

		enum class eCubeMapSize{ S128, S256, S512, S1024, SMAX };
		static std::shared_ptr<Texture> CreateCubeMap( eCubeMapSize size, const tnl::Vector3& cam_pos = {0, 0, 0});


		static std::shared_ptr<Texture> CreateDepthShaderResourceView( const int screen_hdl = -1 );
		static std::shared_ptr<Texture> CreateRenderTargetShaderResourceView( const int screen_hdl = -1 );

		void setCubeMapCameraPosition(const tnl::Vector3& cam_pos);
		void writeToCubeMap( const std::function< void( std::shared_ptr<dxe::Camera>& ) >& render );

		void createShaderResourceView() ;
		void updateShaderResourceView() ;
		void drawShaderResouceView( const tnl::Vector2i& pos, const float scale = 1.0f );
		static void DestroyAllTextures();
		static void DestroyUnReferenceTextures();

	private:
		friend class Mesh;
		Texture() {}
		int screen_hdl_ = -1;
		int graph_hdl_ = 0;
		uint32_t width_ = 0;
		uint32_t height_ = 0;
		uint32_t data_size_ = 0;
		tnl::Rect rect_;
		eType e_type_ = eType::NORMAL ;
		std::string file_path_;
		std::string file_name_;
		char* file_data_ = nullptr;
		static std::unordered_map<size_t, Shared<dxe::Texture>> managed_map_;

		std::shared_ptr<dxe::Camera>		cube_map_camera_[6];
		ComPtr<ID3D11Buffer>				srv_dbg_vertex_buffer_	= nullptr;
		ComPtr<ID3D11Buffer>				srv_dbg_index_buffer_	= nullptr;
		ComPtr<ID3D11VertexShader>          srv_dbg_vertex_shader_	= nullptr;
		ComPtr<ID3D11PixelShader>           srv_dbg_pixel_shader_	= nullptr;
		ComPtr<ID3D11InputLayout>           srv_dbg_input_layout_	= nullptr;
		ComPtr<ID3D11Buffer>                srv_dbg_constant_buffer_ = nullptr;
		ComPtr<ID3D11Texture2D>				srv_texture_			= nullptr;
		ComPtr<ID3D11ShaderResourceView>	shader_resource_view_	= nullptr;
		ComPtr<ID3D11RenderTargetView>		render_target_view_		= nullptr;
		ComPtr<ID3D11DepthStencilView>		depth_stencil_view_		= nullptr;
	};

}

