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

		// �Đ����[�h
		enum class ePlayType { LOOP, ONCE, REPLAY };


		// �����̋N�_���[�h
		enum class eOriginMode { XYZ, XY, XZ, YZ, X, Y, Z };

		// ���o�^�C�v
		enum class eEjectType { DIFF, CONV, ICONV };

		// �ړ�����
		enum class eMoveSurface { XYZ, XY, XZ, YZ };

		// �p���^�C�v
		enum class ePostureType { BXYZ, BY, OBY, UpYD, X, Y, Z };

		// �T�C�Y�ψʃ��[�h
		enum class eDispSizeMode{ NONE, EXIST, BIGGER, SMALLER };

		// �����x�ψʃ��[�h
		enum class eDispAlphaMode { NONE, EXIST, INC, DEC };

		// �c�݃��[�h
		enum class eDistortionMode { CHAOS, ORDER };

		//---------------------------------------------------------------------------------------
		// �p�[�e�B�N���̕`����J�n
		//static void renderBegin();

		//---------------------------------------------------------------------------------------
		// �p�[�e�B�N���̕`��I��
		//static void renderEnd();


		//---------------------------------------------------------------------------------------
		Particle() = default;

		//---------------------------------------------------------------------------------------
		// �����R���X�g���N�^
		Particle
			( const Shared<dxe::Texture>& texture
			, ePlayType play_mode = ePlayType::LOOP
			, int particle_num = 100
			, float time_limit = 10.0f);

		//---------------------------------------------------------------------------------------
		// ���\�[�X����쐬
		Particle(const std::string& resouce_file_path);

		//---------------------------------------------------------------------------------------
		// �N���[���쐻
		Shared<Particle> createClone() ;


		//---------------------------------------------------------------------------------------
		// �����J�n
		inline void start(){
			on_emitte_size_ = 1.0f;
			if (ePlayType::LOOP == play_type_) return;
			play_type_ = ePlayType::REPLAY;
		}

		//---------------------------------------------------------------------------------------
		// ��~
		inline void stop() { on_emitte_size_ = 0.0f; }

		//---------------------------------------------------------------------------------------
		// �`��
		void render( const Shared<Camera>& camera, const bool is_advance_time = true );

		//---------------------------------------------------------------------------------------
		// GUI �R���g���[��
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

		// ���_��`
		typedef struct _VERTEX
		{
			DirectX::XMFLOAT3 spown_position_;  // �o�����W
			DirectX::XMFLOAT3 before_position;  // �X�V�O���W
			DirectX::XMFLOAT3 update_position;  // �X�V���W
			DirectX::XMFLOAT3 direction_;       // ���o����
			float move_velocity_;               // �ړ����x
			float progress_time_;               // �o�ߎ���
			float moving_progress_time_;        // �ړ��v�Z�p�̌o�ߎ���
			float alpha_;                       // �����x
			float size_x_;                      // �T�C�Y
			float size_y_;                      // �T�C�Y
			float rand_value_;                  // �e���_�̌ŗL����
			int process_counter_;               // �V�F�[�_���ł̒��_�ɑ΂��鏈���񐔃J�E���^�[
			int vertex_index_;                  // �e���_�̌ŗL�C���f�b�N�X
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

		// ���q�̃T�C�Y
		float size_x_ = 3.0f;
		float size_y_ = 3.0f;

		// �X�P�[��
		float scale_ = 1.0f;

		// �����x
		float alpha_ = 1.0f;

		// �����x�̕ψʌW��
		float disp_alpha_factor_ = 1;

		// ���Z�b�g�܂ł̎���
		float time_limit_ = 10.0f;

		// �^�C���X�P�[��
		float time_scale_ = 1.0f;

		// �T�C�Y�̕ψʌW��
		float disp_size_factor_ = 1.5f;

		// �p�[�e�B�N�������Z�b�g���ꂽ���ɐݒ肳��鏉��
		float start_velocity_ = 50.0f;

		// �ړ��ʂ̌����W��
		float moving_decay_factor_ = 0.0f;

		// �g�U�������烉���_���Ȋp�x�̍ő�͈�( �f�O���[ )
		float diff_random_angle_range_ = 40.0f;

		// �d�͌W��
		float gravity_factor_ = 0.0f;

		// �h�炬�W��
		float waver_factor_ = 45.0f;

		// �P�x�̗h�炬�̌W��
		float lumi_wave_factor_ = 10.0f ;

		// ��]�W��
		float rotate_factor_ = 0.0f;

		// �c�݂̍ő�͈�
		float distortion_range_ = 0.0f;

		// �c�݂̑���
		float distortion_velocity_ = 0.0f;

		// ���o�Ǘ��T�C�Y
		float on_emitte_size_ = 1.0f;

		// �S�̂ŋ��ʂ̗���
		float random_value_ = 0.0f;

		// �߂�����ꍇ�ɓ����x�������鋫�E����
		float near_alpha_distance_ = 0.0f;

		// ���q�𐶂ݏo�����̋N�_����̍ő勗��
		float origin_range_ = 0;

		// ���q�̐�
		int particle_num_ = 0;

		// ���ݏo������̗��q���G�~�b�^�[��Ǐ]���邩
		bool is_adulation_emitter = false;


		// �Đ��^�C�v
		ePlayType play_type_ = ePlayType::LOOP ;
		// �u�����h���[�h
		eBlendState blend_mode_ = eBlendState::ADD ;
		// �T���v���X�e�[�g
		eSamplerState sampler_state_ = eSamplerState::ANISOTROPIC;
		// ���X�^���C�U�X�e�[�g
		eRasterizerState rasterizer_state_ = eRasterizerState::CULL_NONE;
		// �����̋N�_���[�h
		eOriginMode origin_mode_ = eOriginMode::XYZ;
		// ���o�^�C�v
		eEjectType eject_type_ = eEjectType::DIFF;
		// �ړ����ʃ��[�h
		eMoveSurface move_surface_ = eMoveSurface::XYZ;
		// �p���^�C�v
		ePostureType posture_type_ = ePostureType::BXYZ;
		// �T�C�Y�ψʃ��[�h
		eDispSizeMode disp_size_mode_ = eDispSizeMode::NONE;
		// �����x�ψʃ��[�h
		eDispAlphaMode disp_alpha_mode_ = eDispAlphaMode::NONE;
		// �c�݃��[�h
		eDistortionMode distortion_mode_ = eDistortionMode::CHAOS;

		// �W�I���g���V�F�[�_�p����`��t���O
		bool is_init_draw_ = true;

		// ���_�f�[�^
		Shared<PTCL_VERTEX[]> vertexs_ = nullptr;

		// �^�[�Q�b�g���o�̓o�b�t�@�[
		ComPtr<ID3D11Buffer>                target_io_buffer_[2] = { nullptr, nullptr };
		// ���_�V�F�[�_�[
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// �s�N�Z���V�F�[�_�[
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;
		// �W�I���g���V�F�[�_�[( �p�[�e�B�N���̍��W�X�V�p�A���_�����p )
		ComPtr<ID3D11GeometryShader>        geometry_shaders_[2] = { nullptr, nullptr };

		// ���_���̓��C�A�E�g
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// �萔�o�b�t�@
		ComPtr<ID3D11Buffer>                constant_buffer_ = nullptr;

		// �V�F�[�_�[���\�[�X�r���[
		Shared<dxe::Texture>				texture_ = nullptr;
		ComPtr<ID3D11ShaderResourceView>    shader_resouce_view_ = nullptr;
		ComPtr<ID3D11Resource>              texture_resouce_ = nullptr;

		// ���X�^���C�U�X�e�[�g
		//ComPtr<ID3D11RasterizerState>		rasterizer_state_ = nullptr;
		// �T���v���[�X�e�[�g
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