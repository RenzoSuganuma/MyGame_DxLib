#pragma once

#include <string>
#include "../../library/tnl_using.h"
#include "../dxlib_ext_texture.h"
#include "../dxlib_ext_data.h"
#undef LONG_PTR
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace dxe {

	class Camera ;
	class Particle {
	public :

		// 再生モード
		enum class ePlayType { LOOP, ONCE, REPLAY };


		// 生成の起点モード
		enum class eOriginMode { XYZ, XY, XZ, YZ, X, Y, Z };

		// 放出タイプ
		enum class eEjectType { DIFF, CONV, ICONV };

		// 移動平面
		enum class eMoveSurface { XYZ, XY, XZ, YZ };

		// 姿勢タイプ
		enum class ePostureType { BXYZ, BY, OBY, UpYD, X, Y, Z };

		// サイズ変位モード
		enum class eDispSizeMode{ NONE, EXIST, BIGGER, SMALLER };

		// 透明度変位モード
		enum class eDispAlphaMode { NONE, EXIST, INC, DEC };

		// 歪みモード
		enum class eDistortionMode { CHAOS, ORDER };

		//---------------------------------------------------------------------------------------
		// パーティクルの描画を開始
		//static void renderBegin();

		//---------------------------------------------------------------------------------------
		// パーティクルの描画終了
		//static void renderEnd();


		//---------------------------------------------------------------------------------------
		Particle() = default;

		//---------------------------------------------------------------------------------------
		// 生成コンストラクタ
		Particle
			( const Shared<dxe::Texture>& texture
			, ePlayType play_mode = ePlayType::LOOP
			, int particle_num = 100
			, float time_limit = 10.0f);

		//---------------------------------------------------------------------------------------
		// リソースから作成
		Particle(const std::string& resouce_file_path);

		//---------------------------------------------------------------------------------------
		// クローン作製
		Shared<Particle> createClone() ;


		//---------------------------------------------------------------------------------------
		// 生成開始
		inline void start(){
			on_emitte_size_ = 1.0f;
			if (ePlayType::LOOP == play_type_) return;
			play_type_ = ePlayType::REPLAY;
		}

		//---------------------------------------------------------------------------------------
		// 停止
		inline void stop() { on_emitte_size_ = 0.0f; }

		//---------------------------------------------------------------------------------------
		// 描画
		void render( const Shared<Camera>& camera, const bool is_advance_time = true );

		//---------------------------------------------------------------------------------------
		// GUI コントローラ
		void drawGuiController(const tnl::Vector2i& pos);



		TNL_PROPERTY(eBlendState, BlendMode, blend_mode_);
		TNL_PROPERTY(tnl::Vector3, Position, pos_);
		TNL_PROPERTY(tnl::Vector3, ConvPosition, conv_position_);
		TNL_PROPERTY(tnl::Vector3, Emissive, emissive_);
		TNL_PROPERTY(tnl::Vector3, DiffDirection, diff_direction_);
		TNL_PROPERTY(tnl::Vector3, GravityDirection, gravity_direction_);
		TNL_PROPERTY(tnl::Vector3, OriginAxis, origin_axis_);
		TNL_PROPERTY(float, TimeScale, time_scale_);
		TNL_PROPERTY(float, SizeX, size_x_);
		TNL_PROPERTY(float, SizeY, size_y_);
		TNL_PROPERTY(float, Alpha, alpha_);
		TNL_PROPERTY(float, NearAlphaDistance, near_alpha_distance_);
		TNL_PROPERTY(float, StartVelocity, start_velocity_);
		TNL_PROPERTY(float, MovingDecayFactor, moving_decay_factor_);
		TNL_PROPERTY(float, DiffRandomAngleRange, diff_random_angle_range_);
		TNL_PROPERTY(float, GravityFactor, gravity_factor_);
		TNL_PROPERTY(float, WaverFactor, waver_factor_);
		TNL_PROPERTY(float, LuminanceWaveFactor, lumi_wave_factor_);
		TNL_PROPERTY(float, RotateFactor, rotate_factor_);
		TNL_PROPERTY(float, DistortionRange, distortion_range_);
		TNL_PROPERTY(float, DistortionVelocity, distortion_velocity_);
		TNL_PROPERTY(float, OriginRange, origin_range_);
		TNL_PROPERTY(bool, AdulationEmitter, is_adulation_emitter);
		float getTimeLimit() { return time_limit_; }
		int getParticleNum() { return particle_num_; }

	private :

		// 頂点定義
		typedef struct _VERTEX
		{
			DirectX::XMFLOAT3 spown_position_;  // 出現座標
			DirectX::XMFLOAT3 before_position;  // 更新前座標
			DirectX::XMFLOAT3 update_position;  // 更新座標
			DirectX::XMFLOAT3 direction_;       // 放出向き
			float move_velocity_;               // 移動速度
			float progress_time_;               // 経過時間
			float moving_progress_time_;        // 移動計算用の経過時間
			float alpha_;                       // 透明度
			float size_x_;                      // サイズ
			float size_y_;                      // サイズ
			float rand_value_;                  // 各頂点の固有乱数
			int process_counter_;               // シェーダ内での頂点に対する処理回数カウンター
			int vertex_index_;                  // 各徴点の固有インデックス
		} PTCL_VERTEX;

		void createShader();

		void saveStatus(const std::string& file_path);
		void loadStatus(const std::string& file_path);

		void refreshProcess();
		void reloadTexture();
		void createTargetIOBuffer();

		void setTimeLimit(float time_limit) { 
			if (fabs( time_limit_ - time_limit) <= FLT_EPSILON ) return;
			time_limit_ = time_limit; 
			refreshProcess();
		}
		void setParticleNum(int particle_num) {
			if (particle_num_ == particle_num) return;
			particle_num_ = particle_num;
			refreshProcess();
		}
		void setPlayType(ePlayType play_type) {
			play_type_ = play_type;
			refreshProcess();
		}

		TNL_PROPERTY(float, Scale, scale_);
		TNL_PROPERTY(float, DispAlphaFactor, disp_alpha_factor_);
		TNL_PROPERTY(float, DispSizeFactor, disp_size_factor_);
		TNL_PROPERTY(eOriginMode, OriginMode, origin_mode_);
		TNL_PROPERTY(eEjectType, EjectType, eject_type_);
		TNL_PROPERTY(eMoveSurface, MoveSurface, move_surface_);
		TNL_PROPERTY(ePostureType, PostureType, posture_type_);
		TNL_PROPERTY(eDispSizeMode, DispSizeMode, disp_size_mode_);
		TNL_PROPERTY(eDispAlphaMode, DispAlphaMode, disp_alpha_mode_);
		TNL_PROPERTY(eDistortionMode, DistortionMode, distortion_mode_);

		tnl::Vector3 pos_;
		tnl::Vector3 emissive_ = { 1, 1, 1 };
		tnl::Vector3 diff_direction_ = { 0, 1, 0 };
		tnl::Vector3 gravity_direction_ = { 0, 0, 0 };
		tnl::Vector3 conv_position_ = { 0, 0, 0 };
		tnl::Vector3 origin_axis_ = { 0, 0, 0 };

		// 粒子のサイズ
		float size_x_ = 3.0f;
		float size_y_ = 3.0f;

		// スケール
		float scale_ = 1.0f;

		// 透明度
		float alpha_ = 1.0f;

		// 透明度の変位係数
		float disp_alpha_factor_ = 1;

		// リセットまでの時間
		float time_limit_ = 10.0f;

		// タイムスケール
		float time_scale_ = 1.0f;

		// サイズの変位係数
		float disp_size_factor_ = 1.5f;

		// パーティクルがリセットされた時に設定される初速
		float start_velocity_ = 50.0f;

		// 移動量の減衰係数
		float moving_decay_factor_ = 0.0f;

		// 拡散方向からランダムな角度の最大範囲( デグリー )
		float diff_random_angle_range_ = 40.0f;

		// 重力係数
		float gravity_factor_ = 0.0f;

		// 揺らぎ係数
		float waver_factor_ = 45.0f;

		// 輝度の揺らぎの係数
		float lumi_wave_factor_ = 10.0f ;

		// 回転係数
		float rotate_factor_ = 0.0f;

		// 歪みの最大範囲
		float distortion_range_ = 0.0f;

		// 歪みの早さ
		float distortion_velocity_ = 0.0f;

		// 放出管理サイズ
		float on_emitte_size_ = 1.0f;

		// 全体で共通の乱数
		float random_value_ = 0.0f;

		// 近すぎる場合に透明度を下げる境界距離
		float near_alpha_distance_ = 0.0f;

		// 粒子を生み出す時の起点からの最大距離
		float origin_range_ = 0;

		// 粒子の数
		int particle_num_ = 0;

		// 生み出した後の粒子がエミッターを追従するか
		bool is_adulation_emitter = false;


		// 再生タイプ
		ePlayType play_type_ = ePlayType::LOOP ;
		// ブレンドモード
		eBlendState blend_mode_ = eBlendState::ADD ;
		// サンプラステート
		eSamplerState sampler_state_ = eSamplerState::ANISOTROPIC;
		// ラスタライザステート
		eRasterizerState rasterizer_state_ = eRasterizerState::CULL_NONE;
		// 生成の起点モード
		eOriginMode origin_mode_ = eOriginMode::XYZ;
		// 放出タイプ
		eEjectType eject_type_ = eEjectType::DIFF;
		// 移動平面モード
		eMoveSurface move_surface_ = eMoveSurface::XYZ;
		// 姿勢タイプ
		ePostureType posture_type_ = ePostureType::BXYZ;
		// サイズ変位モード
		eDispSizeMode disp_size_mode_ = eDispSizeMode::NONE;
		// 透明度変位モード
		eDispAlphaMode disp_alpha_mode_ = eDispAlphaMode::NONE;
		// 歪みモード
		eDistortionMode distortion_mode_ = eDistortionMode::CHAOS;

		// ジオメトリシェーダ用初回描画フラグ
		bool is_init_draw_ = true;

		// 頂点データ
		Shared<PTCL_VERTEX[]> vertexs_ = nullptr;

		// ターゲット入出力バッファー
		ComPtr<ID3D11Buffer>                target_io_buffer_[2] = { nullptr, nullptr };
		// 頂点シェーダー
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// ピクセルシェーダー
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;
		// ジオメトリシェーダー( パーティクルの座標更新用、頂点増幅用 )
		ComPtr<ID3D11GeometryShader>        geometry_shaders_[2] = { nullptr, nullptr };

		// 頂点入力レイアウト
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// 定数バッファ
		ComPtr<ID3D11Buffer>                constant_buffer_ = nullptr;

		// シェーダーリソースビュー
		Shared<dxe::Texture>				texture_ = nullptr;
		ComPtr<ID3D11ShaderResourceView>    shader_resouce_view_ = nullptr;
		ComPtr<ID3D11Resource>              texture_resouce_ = nullptr;

		// ラスタライザステート
		//ComPtr<ID3D11RasterizerState>		rasterizer_state_ = nullptr;
		// サンプラーステート
		//ComPtr<ID3D11SamplerState>          sampler_state_ = nullptr;

		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<GuiInputBox> gui_particle_num_;
		Shared<GuiInputBox> gui_time_limit_;
		Shared<GuiInputBox> gui_origin_range_;
		Shared<GuiInputBox> gui_waver_factor_;
		Shared<GuiInputBox> gui_start_velocity_;
		Shared<GuiInputBox> gui_gravity_factor_;
		Shared<GuiInputBox> gui_position_x_;
		Shared<GuiInputBox> gui_position_y_;
		Shared<GuiInputBox> gui_position_z_;
		Shared<GuiInputBox> gui_conv_position_x_;
		Shared<GuiInputBox> gui_conv_position_y_;
		Shared<GuiInputBox> gui_conv_position_z_;
		Shared<GuiValueSlider< Particle, tnl::Vector3 >> gui_emissive_;
		Shared<GuiValueSlider< Particle, tnl::Vector3 >> gui_diff_direction_;
		Shared<GuiValueSlider< Particle, tnl::Vector3 >> gui_gravity_direction_;
		Shared<GuiValueSlider< Particle, tnl::Vector3 >> gui_origin_axis_;
		Shared<GuiValueSlider< Particle, float >> gui_time_scale_;
		Shared<GuiValueSlider< Particle, float >> gui_size_x_;
		Shared<GuiValueSlider< Particle, float >> gui_size_y_;
		Shared<GuiValueSlider< Particle, float >> gui_scale_;
		Shared<GuiValueSlider< Particle, float >> gui_alpha_;
		Shared<GuiValueSlider< Particle, float >> gui_disp_alpha_factor_;
		Shared<GuiValueSlider< Particle, float >> gui_near_alpha_distance_;
		Shared<GuiValueSlider< Particle, float >> gui_existence_scale_;
		Shared<GuiValueSlider< Particle, float >> gui_moving_decay_factor_;
		Shared<GuiValueSlider< Particle, float >> gui_diff_random_angle_range_;
		Shared<GuiValueSlider< Particle, float >> gui_lumi_wave_factor_;
		Shared<GuiValueSlider< Particle, float >> gui_rotate_factor_;
		Shared<GuiValueSlider< Particle, float >> gui_distortion_range_;
		Shared<GuiValueSlider< Particle, float >> gui_distortion_velocity_;
		Shared<GuiMenuSelector> gui_play_type_;
		Shared<GuiMenuSelector> gui_blend_mode_;
		Shared<GuiMenuSelector> gui_adulation_mode_;
		Shared<GuiMenuSelector> gui_origin_mode_;
		Shared<GuiMenuSelector> gui_eject_type_;
		Shared<GuiMenuSelector> gui_move_surface_;
		Shared<GuiMenuSelector> gui_posture_type_;
		Shared<GuiMenuSelector> gui_disp_size_mode_;
		Shared<GuiMenuSelector> gui_disp_alpha_mode_;
		Shared<GuiMenuSelector> gui_distortion_mode_;
		Shared<GuiDragBox> gui_drag_resouce_;
		std::string description_str_;


		static ID3D11VertexShader* default_vs;
		static ID3D11PixelShader* default_ps;
		static ID3D11HullShader* default_hs;
		static ID3D11GeometryShader* default_gs;
		static ID3D11DomainShader* default_ds;
		static UINT32 default_sample_mask ;
		static ID3D11BlendState* default_blend_state ;
		static ID3D11RasterizerState* default_rasterize_state ;
		static ID3D11SamplerState* default_sampler_state ;
		static ID3D11ShaderResourceView* default_pixel_shader_resouce_view ;
	};

}