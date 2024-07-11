#pragma warning(disable:4819)
#include "DxLib.h"
#include <unordered_map>
#include "../library/stb_image.h"
#include "../library/tnl_util.h"
#include "dxlib_ext_texture.h"
#include "dxlib_ext_camera.h"
#include <WICTextureLoader.h>

namespace dxe {

	std::unordered_map<size_t, Shared<dxe::Texture>> Texture::managed_map_;

	//------------------------------------------------------------------------------------------------------------------------------------
	Texture::~Texture() {
		if (graph_hdl_) DeleteGraph(graph_hdl_);
		if (file_data_) delete[] file_data_;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateFromMemory(char* file_data, uint32_t data_size, const std::string& regist_key_) {

		size_t hash = (regist_key_.empty()) ? std::hash<char*>()(file_data) : std::hash<std::string>()(regist_key_);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		tex->graph_hdl_ = CreateGraphFromMem(file_data, data_size);
		GetGraphSize(tex->graph_hdl_, (int*)(&tex->width_), (int*)(&tex->height_));
		tex->rect_ = { tex->width_, tex->height_ } ;
		if (!regist_key_.empty()) {
			tex->file_name_ = regist_key_;
			tex->file_path_ = regist_key_;
		}
		tex->data_size_ = data_size;
		tex->file_data_ = new char[tex->data_size_];
		memcpy(tex->file_data_, file_data, tex->data_size_);
		managed_map_.insert(std::make_pair(hash, tex));
		return tex;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateFromColorBufferRGBA8(unsigned char* color_buffer_rgba8, uint32_t width, uint32_t height, const std::string& regist_key) {
		size_t hash = std::hash<std::string>()(regist_key);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		uint32_t data_size = 0;
		auto buff_tga = tnl::CreateFormatTga32((unsigned char*)color_buffer_rgba8, width, height, &data_size);

		char* file_data = new char[data_size];
		memcpy(file_data, buff_tga.get(), data_size);

		tex->graph_hdl_ = CreateGraphFromMem(buff_tga.get(), data_size);
		tex->width_ = width;
		tex->height_ = height;
		tex->rect_ = { width, height };
		tex->file_data_ = file_data;
		tex->data_size_ = data_size;

		managed_map_.insert(std::make_pair(hash, tex));

		return tex;
	}


	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateFromFile(const std::string& file_path)
	{
		size_t hash = std::hash<std::string>()(file_path);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>( new Texture() );
		tex->file_path_ = file_path;
		std::tuple t = tnl::DetachmentFilePath(file_path);
		tex->file_name_ = std::get<1>(t) + "." + std::get<2>(t);

		tex->data_size_ = (uint32_t)tnl::GetFileSize(file_path.c_str());

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {
			tex->file_data_ = new char[tex->data_size_];
			fread(tex->file_data_, tex->data_size_, 1, fp);
			fclose(fp);
		}

		tex->graph_hdl_ = CreateGraphFromMem(tex->file_data_, tex->data_size_);
		GetGraphSize(tex->graph_hdl_, (int*)(&tex->width_), (int*)(&tex->height_));

		managed_map_.insert(std::make_pair( hash, tex ));

		return tex;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateFromDxLibGraphHdl(int dxlib_graph_hdl)
	{
		size_t hash = std::hash<int>()(dxlib_graph_hdl);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		tex->graph_hdl_ = dxlib_graph_hdl;
		int w, h;
		GetGraphSize(tex->graph_hdl_, &w, &h);
		tex->width_ = w;
		tex->height_ = h;
		tex->rect_ = {(uint32_t)w, (uint32_t)h};

		managed_map_.insert(std::make_pair(hash, tex));
		return tex;
	}


	//------------------------------------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Texture>> Texture::CreateFromFileDiv(const std::string& file_path, uint32_t frame_w, uint32_t frame_h) {
		std::vector<std::shared_ptr<Texture>> out;
		out.resize(frame_w * frame_h);

		auto col_tpl = tnl::LoadGraphicColorBuffer(file_path.c_str());
		if (std::get<0>(col_tpl) == nullptr) return std::vector<std::shared_ptr<Texture>>() ;

		uint32_t width = std::get<1>(col_tpl);
		uint32_t height = std::get<2>(col_tpl);
		uint32_t fr_pc_w = width / frame_w;
		uint32_t fr_pc_h = height / frame_h;

		unsigned char* root = (unsigned char*)std::get<0>(col_tpl).get();

		for (uint32_t i = 0; i < frame_h; ++i) {
			unsigned char* gp = root + ( i * width * 4 ) * fr_pc_h ;

			for (uint32_t k = 0; k < frame_w; ++k) {

				uint32_t data_size = fr_pc_w * fr_pc_h * 4;
				unsigned char* col_buff = new unsigned char[data_size];

				unsigned char* p = gp;
				unsigned char* cp = col_buff;
				for (int n = 0; n < int(fr_pc_h); ++n) {
					memcpy(cp, p, fr_pc_w * 4);
					p += width * 4;
					cp += fr_pc_w * 4;
				}
				gp += fr_pc_w * 4 ;

				std::shared_ptr<Texture> tex = CreateFromColorBufferRGBA8((unsigned char*)col_buff, fr_pc_w, fr_pc_h, file_path + std::to_string((i * frame_w) + k));
				out[( i * frame_w ) + k] = tex;
				delete[] col_buff;
			}
		}

		return out;
	}




	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateCubeMap( eCubeMapSize size, const tnl::Vector3& cam_pos ) {

		uint32_t size_tbl[static_cast<uint32_t>(eCubeMapSize::SMAX)] = { 128, 256, 512, 1024 };

		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		std::shared_ptr<Texture> cube_map = std::shared_ptr<Texture>(new Texture());
		cube_map->e_type_ = eType::SRV_CUBE_MAP;
		cube_map->width_ = (UINT)size_tbl[static_cast<uint32_t>(size)];
		cube_map->height_ = (UINT)size_tbl[static_cast<uint32_t>(size)];
		cube_map->rect_ = { cube_map->width_, cube_map->height_ };

		for (int i = 0; i < 6; ++i) {
			Shared<dxe::Camera> camera = std::make_shared<dxe::Camera>(cube_map->width_, cube_map->height_);
			cube_map->cube_map_camera_[i] = camera;
		}
		cube_map->setCubeMapCameraPosition(cam_pos);

		{

			// テクスチャリソース作成
			D3D11_TEXTURE2D_DESC desc = {};

			desc.Width = cube_map->width_ ;
			desc.Height = cube_map->height_;
			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // HDR対応にするためfloat型
			desc.ArraySize = 6;// 6枚の画像を持ったテクスチャとして作成する
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | // キューブマップ
				D3D11_RESOURCE_MISC_GENERATE_MIPS;	// Mipmap生成
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
				D3D11_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;
			desc.MipLevels = 0;	// 完全なミップマップチェーン
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			ID3D11Texture2D* resource = nullptr;
			pd3dDevice->CreateTexture2D(&desc, nullptr, &resource);
			// 正確なMipLevels取得のため、情報再取得
			resource->GetDesc(&desc);


			// RTV作成
			ID3D11RenderTargetView* rtv = nullptr;
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = desc.Format;			// Format
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY; // テクスチャ配列
			rtvDesc.Texture2DArray.ArraySize = desc.ArraySize; // 要素数
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.MipSlice = 0;
			pd3dDevice->CreateRenderTargetView(resource, &rtvDesc, &rtv);
			cube_map->render_target_view_.Attach(rtv);

			// SRV作成
			ID3D11ShaderResourceView* srv = nullptr;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = desc.MipLevels;
			pd3dDevice->CreateShaderResourceView(resource, &srvDesc, &srv);
			cube_map->shader_resource_view_.Attach(srv);

			resource->Release();
		}


		{

			D3D11_TEXTURE2D_DESC desc = {};

			desc.Width = cube_map->width_;
			desc.Height = cube_map->height_;
			desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			desc.ArraySize = 1;
			desc.MiscFlags = 0;

			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
				D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			ID3D11Texture2D* resource = nullptr;
			pd3dDevice->CreateTexture2D(&desc, nullptr, &resource);

			ID3D11DepthStencilView* dsv = nullptr;
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
			pd3dDevice->CreateDepthStencilView(resource, &dsvDesc, &dsv);
			cube_map->depth_stencil_view_.Attach(dsv);

			resource->Release();

		}

		return cube_map;

	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::setCubeMapCameraPosition(const tnl::Vector3& cam_pos) {

		if (e_type_ != eType::SRV_CUBE_MAP) return;

		for (int i = 0; i < 6; ++i) {
			cube_map_camera_[i]->pos_ = cam_pos ; 
			cube_map_camera_[i]->angle_ = tnl::ToRadian(90);
			switch (i)
			{
				// 右面(X)
			case D3D11_TEXTURECUBE_FACE_POSITIVE_X:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(1, 0, 0);
				cube_map_camera_[i]->up_ = { 0, 1, 0 };
				break;
				// 左面(-X)
			case D3D11_TEXTURECUBE_FACE_NEGATIVE_X:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(-1, 0, 0);
				cube_map_camera_[i]->up_ = { 0, 1, 0 };
				break;
				// 上面(Y)
			case D3D11_TEXTURECUBE_FACE_POSITIVE_Y:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(0, 1, 0);
				cube_map_camera_[i]->up_ = { 0, 0, -1 };
				break;
				// 下面(-Y)
			case D3D11_TEXTURECUBE_FACE_NEGATIVE_Y:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(0, -1, 0);
				cube_map_camera_[i]->up_ = { 0, 0, 1 };
				break;
				// 後面(Z)
			case D3D11_TEXTURECUBE_FACE_POSITIVE_Z:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(0, 0, 1);
				cube_map_camera_[i]->up_ = { 0, 1, 0 };
				break;
				// 前面(-Z)
			case D3D11_TEXTURECUBE_FACE_NEGATIVE_Z:
				cube_map_camera_[i]->target_ = cube_map_camera_[i]->pos_ + tnl::Vector3(0, 0, -1);
				cube_map_camera_[i]->up_ = { 0, 1, 0 };
				break;
			}
			cube_map_camera_[i]->update();
		}

	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::writeToCubeMap( const std::function< void(std::shared_ptr<dxe::Camera>& ) >& render ) {

		if (e_type_ != eType::SRV_CUBE_MAP) return;

		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		// テクスチャリソースを取得
		ID3D11Resource* cubeMapResource = nullptr;
		render_target_view_->GetResource(&cubeMapResource);

		//--------------------------------------------------
		// ２．上下左右前後の計６回描画し、CubeMapを作成する
		//--------------------------------------------------
		// RenderTarget、ZBuffer、Viewportの変更するため、
		// 現在のRenderTargetとZBufferとViewportを取得しておく
		ID3D11RenderTargetView* saveRTV[8] = {};
		ID3D11DepthStencilView* saveDSV = nullptr;
		pImmediateContext->OMGetRenderTargets(8, saveRTV, &saveDSV);
		D3D11_VIEWPORT saveVP;
		UINT numVPs = 1;
		pImmediateContext->RSGetViewports(&numVPs, &saveVP);

		// Viewportを変更
		D3D11_VIEWPORT vp = { 0, 0, (float)width_, (float)height_, 0, 1 };
		pImmediateContext->RSSetViewports(1, &vp);


		// キューブマップの各面(6枚)を描画
		for (int i = 0; i < 6; i++)
		{

			// キューブマップの各画像(上下左右前後)用のビューを作成
			ID3D11RenderTargetView* rtv = nullptr;
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;	// Format
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.ArraySize = 1;
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			rtvDesc.Texture2DArray.MipSlice = 0;
			// レンダーターゲットビュー作成
			pd3dDevice->CreateRenderTargetView(cubeMapResource, &rtvDesc, &rtv);

			// RT、Zクリア
			float color[4] = { 0, 0, 0, 1 };
			pImmediateContext->ClearRenderTargetView(rtv, color);
			pImmediateContext->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH, 1, 0);

			// RTとZを変更する
			pImmediateContext->OMSetRenderTargets(1, &rtv, depth_stencil_view_.Get());

			// 書き込み
			render( cube_map_camera_[i] );

			// ビュー解放
			rtv->Release();
		}

		// RTとZを元に戻す
		pImmediateContext->OMSetRenderTargets(8, saveRTV, saveDSV);
		for (auto&& rtv : saveRTV)
		{
			if (rtv) rtv->Release();
		}
		if (saveDSV) saveDSV->Release();

		// Viewportを元に戻す
		pImmediateContext->RSSetViewports(numVPs, &saveVP);

		// 取得したリソースの参照カウンターを減らす
		cubeMapResource->Release();

		// ミップマップの生成
		pImmediateContext->GenerateMips(shader_resource_view_.Get());

		DxLib::RefreshDxLibDirect3DSetting();
	}


	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateDepthShaderResourceView( const int screen_hdl ) {

		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();
		std::shared_ptr<Texture> new_texture = std::shared_ptr<Texture>( new Texture() );		
		new_texture->screen_hdl_ = screen_hdl;
		new_texture->e_type_ = eType::SRV_DEPTH ;

		ID3D11DepthStencilView* dsv = nullptr ;

		if (-1 == new_texture->screen_hdl_) {
			pImmediateContext->OMGetRenderTargets(0, nullptr, &dsv);
		}
		else {
			dsv = (ID3D11DepthStencilView*)GetGraphID3D11DepthStencilView(new_texture->screen_hdl_);
		}

		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D* depthTexture = nullptr ;
		ID3D11Texture2D* depthStencilTexture = nullptr;
		dsv->GetResource(reinterpret_cast<ID3D11Resource**>(&depthStencilTexture));
		depthStencilTexture->GetDesc(&desc);

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = desc.Width;
		depthTextureDesc.Height = desc.Height;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = desc.Format;

		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;
		pd3dDevice->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture);
		new_texture->srv_texture_.Attach(depthTexture);

		new_texture->width_ = desc.Width;
		new_texture->height_ = desc.Height;
		new_texture->rect_ = { desc.Width, desc.Height };

		ID3D11ShaderResourceView* depthTextureSRV = nullptr;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		pd3dDevice->CreateShaderResourceView(new_texture->srv_texture_.Get(), &srvDesc, &depthTextureSRV);
		new_texture->shader_resource_view_.Attach(depthTextureSRV);

		if (-1 == new_texture->screen_hdl_) {
			dsv->Release();
		}
		depthStencilTexture->Release();

		return new_texture ;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Texture> Texture::CreateRenderTargetShaderResourceView(const int screen_hdl) {

		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();
		std::shared_ptr<Texture> new_texture = std::shared_ptr<Texture>(new Texture());
		new_texture->screen_hdl_ = screen_hdl;
		new_texture->e_type_ = eType::SRV_RENDER_TARGET;

		ID3D11RenderTargetView* rtv = nullptr;

		if (-1 == new_texture->screen_hdl_) {
			pImmediateContext->OMGetRenderTargets(1, &rtv, nullptr);
		}
		else {
			rtv = (ID3D11RenderTargetView*)GetGraphID3D11RenderTargetView(new_texture->screen_hdl_);
		}

		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D* use_render_target_tex = nullptr;
		ID3D11Texture2D* ref_render_target_tex = nullptr;
		rtv->GetResource(reinterpret_cast<ID3D11Resource**>(&ref_render_target_tex));
		ref_render_target_tex->GetDesc(&desc);

		// テクスチャの作成
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = desc.Width;
		texDesc.Height = desc.Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = desc.Format;

		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE ;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		pd3dDevice->CreateTexture2D(&texDesc, nullptr, &use_render_target_tex);
		new_texture->srv_texture_.Attach(use_render_target_tex);

		new_texture->width_ = desc.Width;
		new_texture->height_ = desc.Height;
		new_texture->rect_ = { desc.Width, desc.Height };

		ID3D11ShaderResourceView* srv = nullptr;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		pd3dDevice->CreateShaderResourceView(new_texture->srv_texture_.Get(), &srvDesc, &srv);
		new_texture->shader_resource_view_.Attach(srv);

		if (-1 == new_texture->screen_hdl_) {
			rtv->Release();
		}
		ref_render_target_tex->Release();

		return new_texture;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::createShaderResourceView() {
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11Resource* res = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
		auto hr = DirectX::CreateWICTextureFromMemory(pd3dDevice, (uint8_t*)getFileData(), (size_t)getDataSize(), &res, &srv);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : createShaderResourceView \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		shader_resource_view_.Attach(srv);
		//res->Release();
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::updateShaderResourceView() {

		if (e_type_ != eType::SRV_DEPTH && e_type_ != eType::SRV_RENDER_TARGET) return;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		if (e_type_ == eType::SRV_DEPTH) {

			ID3D11DepthStencilView* dsv = nullptr;
			if (-1 == screen_hdl_) {
				pImmediateContext->OMGetRenderTargets(0, nullptr, &dsv);
			}
			else {
				dsv = (ID3D11DepthStencilView*)GetGraphID3D11DepthStencilView(screen_hdl_);
			}

			ID3D11Texture2D* depthStencilTexture = nullptr;
			dsv->GetResource(reinterpret_cast<ID3D11Resource**>(&depthStencilTexture));

			// 深度ステンシルバッファの内容をテクスチャにコピーする
			pImmediateContext->CopyResource(srv_texture_.Get(), depthStencilTexture);

			if (-1 == screen_hdl_) {
				dsv->Release();
			}
			depthStencilTexture->Release();
		}
		else {
			ID3D11RenderTargetView* rtv = nullptr;
			if (-1 == screen_hdl_) {
				pImmediateContext->OMGetRenderTargets(1, &rtv, nullptr);
			}
			else {
				rtv = (ID3D11RenderTargetView*)GetGraphID3D11RenderTargetView(screen_hdl_);
			}

			ID3D11Texture2D* ref_texture = nullptr;
			rtv->GetResource(reinterpret_cast<ID3D11Resource**>(&ref_texture));

			// 深度ステンシルバッファの内容をテクスチャにコピーする
			pImmediateContext->CopyResource(srv_texture_.Get(), ref_texture);

			if (-1 == screen_hdl_) {
				rtv->Release();
			}
			ref_texture->Release();
		}

		return;
	}


	//------------------------------------------------------------------------------------------------------------------------------------
	using namespace DirectX;
	struct SRV_DBG_VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texel;
	};
	typedef struct _CBUFFER
	{
		DirectX::XMMATRIX	mat_view_;              
		DirectX::XMMATRIX	mat_proj_;          
		DirectX::XMMATRIX	mat_world_;
	} SRV_DBG_CBUFFER1;

	void Texture::drawShaderResouceView(const tnl::Vector2i& pos, const float scale) {

		if (e_type_ != eType::SRV_DEPTH && e_type_ != eType::SRV_RENDER_TARGET) return ;

		int path_ary_id = (e_type_ == eType::SRV_DEPTH) ? 0 : 1;

		const LPCWSTR shader_path[2] = {
			L"shader/srv_depth_draw.hlsl\0",
			L"shader/srv_texture_draw.hlsl\0"
		};

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();
		if (!srv_dbg_vertex_buffer_) {
			ID3DBlob* p_err = nullptr;
			D3D11_BUFFER_DESC bd;
			D3D11_SUBRESOURCE_DATA InitData;

			float w2 = width_ * 0.5f;
			float h2 = height_ * 0.5f;
			SRV_DBG_VERTEX vertexs[] = {
				XMFLOAT3(-w2, h2, 0), XMFLOAT2(0.0f, 0.0f),
				XMFLOAT3( w2, h2, 0), XMFLOAT2(1.0f, 0.0f),
				XMFLOAT3(-w2, -h2, 0), XMFLOAT2(0.0f, 1.0f),
				XMFLOAT3( w2, -h2, 0), XMFLOAT2(1.0f, 1.0f)
			} ;

			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = (UINT)(sizeof(SRV_DBG_VERTEX) * 4);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = vertexs ;
			ID3D11Buffer* p_vertex_buffer = nullptr;
			hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_vertex_buffer);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Create Vertex Buffer \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}
			srv_dbg_vertex_buffer_.Attach(p_vertex_buffer);

			UINT indexs[] = { 0, 1, 2, 2, 1, 3 };
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(uint32_t) * 6;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			InitData.pSysMem = indexs;
			ID3D11Buffer* p_index_buffer = nullptr;
			hr = pd3dDevice->CreateBuffer(&bd, &InitData, &p_index_buffer);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Create Index Buffer \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}
			srv_dbg_index_buffer_.Attach(p_index_buffer);

			//------------------------------------------------------------------------------------------------------------------
			// 頂点シェーダーの作成
			D3D11_INPUT_ELEMENT_DESC layout[] = {
				  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			ID3DBlob* p_blob_vertex = nullptr;

			hr = D3DCompileFromFile(shader_path[path_ary_id], nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"VS_Main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_vertex, &p_err);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Compile Vertex Shader \n");
				OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

			ID3D11VertexShader* vertex_shader = nullptr;
			hr = pd3dDevice->CreateVertexShader(p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(), NULL, &vertex_shader);
			srv_dbg_vertex_shader_.Attach(vertex_shader);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Create Vertex Shader \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

			ID3D11InputLayout* il = nullptr;
			hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
				p_blob_vertex->GetBufferPointer(), p_blob_vertex->GetBufferSize(),
				&il);
			srv_dbg_input_layout_.Attach(il);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error :  Texture Debug SRV Draw Input Layout \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

			//-------------------------------------------------------------------------------------------------
			// ピクセルシェーダの作成
			ID3DBlob* p_blob_pixel = nullptr;
			hr = D3DCompileFromFile(shader_path[path_ary_id], nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"PS_Main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_blob_pixel, &p_err);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Compile Pixel Shader \n");
				OutputDebugStringA(static_cast<char*>(p_err->GetBufferPointer()));
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

			ID3D11PixelShader* ps = nullptr;
			hr = pd3dDevice->CreatePixelShader(p_blob_pixel->GetBufferPointer(), p_blob_pixel->GetBufferSize(), NULL, &ps);
			srv_dbg_pixel_shader_.Attach(ps);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Create Pixel Shader \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

			//-------------------------------------------------------------------------------------------------
			// シェーダ定数バッファの作成
			ID3D11Buffer* constant_buffer1 = nullptr;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.ByteWidth = sizeof(SRV_DBG_CBUFFER1);
			hr = pd3dDevice->CreateBuffer(&bd, nullptr, &constant_buffer1);
			srv_dbg_constant_buffer_.Attach(constant_buffer1);
			if (S_OK != hr) {
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				tnl::DebugTrace("Error : Texture Debug SRV Draw Constant Buffer 1 \n");
				tnl::DebugTrace("-----------------------------------------------------------------\n");
				return;
			}

		}

		// プリミティブ タイプおよびデータの順序に関する情報を設定
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pImmediateContext->OMSetDepthStencilState(GetDepthStencilState(eDepthStenclil::DEPTH_W_OFF_T_OFF), 0);

		// ラスタライザステート設定
		ID3D11RasterizerState* rs = GetRasterizerState(dxe::eRasterizerState::CULL_NONE);
		pImmediateContext->RSSetState(rs);

		// ブレンドステート設定
		pImmediateContext->OMSetBlendState(GetBlendState(dxe::eBlendState::NORMAL), 0, 0xffffffff);

		// ジオメトリシェーダを無効にする
		pImmediateContext->GSSetShader(NULL, NULL, 0);

		// コンピュートシェーダーを無効にする。
		pImmediateContext->CSSetShader(NULL, NULL, 0);

		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride = sizeof(SRV_DBG_VERTEX);
		UINT offset = 0;

		ID3D11Buffer* pBuf = srv_dbg_vertex_buffer_.Get();
		pImmediateContext->IASetVertexBuffers(0, 1, &pBuf, &stride, &offset);

		//インデックスバッファのセット
		pImmediateContext->IASetIndexBuffer(srv_dbg_index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

		// 頂点データレイアウト設定
		ID3D11InputLayout* il = srv_dbg_input_layout_.Get();
		pImmediateContext->IASetInputLayout(il);

		// 頂点シェーダーをセット
		ID3D11VertexShader* vs = srv_dbg_vertex_shader_.Get();
		pImmediateContext->VSSetShader(vs, NULL, 0);

		// ピクセルシェーダーをセット
		ID3D11PixelShader* ps = srv_dbg_pixel_shader_.Get();
		pImmediateContext->PSSetShader(ps, NULL, 0);

		// テクスチャーを設定する
		ID3D11ShaderResourceView* sv = shader_resource_view_.Get();
		pImmediateContext->PSSetShaderResources(0, 1, &sv);

		// サンプラステートを設定する
		ID3D11SamplerState* st = GetSamplerState(dxe::eSamplerState::ANISOTROPIC);
		pImmediateContext->PSSetSamplers(0, 1, &st);


		tnl::Matrix world = tnl::Matrix::AffineTransformation({(float)pos.x - DXE_WINDOW_WIDTH / 2, (float)-pos.y + DXE_WINDOW_HEIGHT / 2, 0}, {scale, scale, 1}, tnl::Quaternion());

		tnl::Matrix view = tnl::Matrix::LookAtLH({ 0, 0, -10 }, { 0, 0, 0 }, {0, 1, 0});
		tnl::Matrix proj = tnl::Matrix::OrthoLH((float)DXE_WINDOW_WIDTH, (float)DXE_WINDOW_HEIGHT, 1, 1000);

		XMMATRIX matWorld, matView, matProj;
		matView = XMLoadFloat4x4(&view);
		matProj = XMLoadFloat4x4(&proj);
		matWorld = XMLoadFloat4x4(&world);

		SRV_DBG_CBUFFER1 sb1;
		sb1.mat_view_ = XMMatrixTranspose(matView);
		sb1.mat_proj_ = XMMatrixTranspose(matProj);
		sb1.mat_world_ = XMMatrixTranspose(matWorld);

		ID3D11Buffer* cb1 = srv_dbg_constant_buffer_.Get();
		pImmediateContext->UpdateSubresource(cb1, 0, nullptr, &sb1, 0, 0);
		pImmediateContext->VSSetConstantBuffers(0, 1, &cb1);
		//pImmediateContext->PSSetConstantBuffers(0, 1, &cb1);

		// シェーダに存在する変数に値を渡す
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		if (SUCCEEDED(pImmediateContext->Map(cb1, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource))) {
			memcpy(MappedResource.pData, &sb1, sizeof(SRV_DBG_CBUFFER1));
			pImmediateContext->Unmap(cb1, 0);
		}

		//描画
		pImmediateContext->DrawIndexed(6, 0, 0);
		DxLib::RefreshDxLibDirect3DSetting();
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::DestroyAllTextures() {
		managed_map_.clear();
	}

	//------------------------------------------------------------------------------------------------------------------------------------
	void Texture::DestroyUnReferenceTextures() {
		auto it = managed_map_.begin();
		while (it != managed_map_.end()) {
			if (1 == it->second.use_count()) {
				it = managed_map_.erase(it);
				continue;
			}
			it++;
		}
	}

}

