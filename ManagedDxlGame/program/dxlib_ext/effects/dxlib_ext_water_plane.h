#pragma once

#include "../../library/tnl_vector3.h"
#include "../../library/tnl_matrix.h"
#include "../../library/tnl_quaternion.h"

#include <string>
#include "../../library/tnl_using.h"
#include "../dxlib_ext_texture.h"
#include "../dxlib_ext_data.h"
#include "../dxlib_ext_camera.h"
#include "dxlib_ext_erea_projection.h"

#undef LONG_PTR
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


namespace dxe {

	//-------------------------------------------------------------------------------------------------------------------------
	class WaterPlane final {
	public :

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �T�C�Y
		enum class eSize {
			S32, S64, S128, S256, S512, S1024, S2048, S4096, S8192, S_MAX
		};
		static constexpr uint32_t SIZE_TABLE[static_cast<uint32_t>(eSize::S_MAX)] = {
			32, 64, 128, 256, 512, 1024, 2048, 4096, 8192
		};

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �g�̌���
		enum class eWaveDirection {
			RANDOM
			, LEFT
			, RIGHT
			, FORWARD
			, BACK
			, LEFT_FORWARD
			, LEFT_BACK
			, RIGHT_FORWARD
			, RIGHT_BACK
		};

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �J���������ɂ��t�F�[�h�ݒ�
		enum class eFarFade {
			NONE	// �Ȃ�
			,FOG	// �t�H�O�K�p
			,ALPHA	// �t�H�O�ݒ�ŐF�t���ł͂Ȃ����߂��Ă���
		};

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �Q���X�g�i�[�g�T���v�����O��
		enum class eGerstnerSampling { D4, D8 };

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �ڒn�e�N�X�`���ւ̗p�r
		enum class fLandingMeshUse {
			TNL_ENUM_BIT_FLAG(BUBLLE)		// �A
			, TNL_ENUM_BIT_FLAG(CUT_OUT)	// ���蔲��
			, TNL_ENUM_BIT_FLAG(FADE_ALPHA) // �l�ȂǏ��X�ɓ����x���グ��
		};

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// arg1... ��
		// arg2... ���s
		// arg3... CPU �ł̕�����
		// arg4... �e�b�Z���[�V���������p�b�`��
		WaterPlane(
			eSize width
			, eSize depth
			, uint32_t div
			, uint32_t patch );

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���\�[�X����̃��[�h
		WaterPlane(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �f�X�g���N�^
		~WaterPlane();

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���W
		TNL_CALC_PROPERTY(tnl::Vector3, Position, pos_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �X�P�[��
		TNL_CALC_PROPERTY(tnl::Vector3, Scale, scl_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ��]
		TNL_PROPERTY(tnl::Quaternion, Rotation, rot_);
		void mulRotataion(const tnl::Quaternion& q) { rot_ *= q; }
		
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ��̐F
		TNL_PROPERTY(tnl::Vector3, SkyColor, sky_color_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ��{�F
		TNL_PROPERTY(tnl::Vector3, BaseColor, base_color_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �[�݂̕ω��F
		TNL_PROPERTY(tnl::Vector3, ShallowColor, shallow_color_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �X�y�L�����F
		TNL_PROPERTY(tnl::Vector3, SpecularColor, specular_color_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �e�����󂯂镽�s����
		TNL_PROPERTY(tnl::Vector3, LightDirection, light_direction_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �t���l�����˂ɂ�铧�ߗ������l
		TNL_PROPERTY(float, FresnelBaseAlpha, fresnel_base_alpha_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �g�̍����ݒ�
		TNL_PROPERTY(float, HeightMax, height_max_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ��{�F�̋���
		TNL_PROPERTY(float, BaseColorStrength, base_col_str_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �[�݂̕ω��F�̋���
		TNL_PROPERTY(float, ShallowColorStrength, shallow_col_str_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �^�C���X�P�[���ݒ�
		TNL_PROPERTY(float, TimeScale, time_scale_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �Â��R���g���X�g
		TNL_PROPERTY(float, DarknessContrast, darkness_contrast_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �X�y�L�����W��
		TNL_PROPERTY(float, SpecularPower, specular_power_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �V���C�l�X�W��
		TNL_PROPERTY(float, Shininess, shininess_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �t���l���W��
		TNL_PROPERTY(float, FresnelFactor, fresnel_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �m�C�Y�W��
		TNL_PROPERTY(float, NoizeStrength, noize_strength_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �U���W��
		TNL_PROPERTY(float, BaseAmpFactor, base_amp_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �U���W��( �L���[�u�}�b�v�p )
		TNL_PROPERTY(float, CubeAmpFactor, cube_amp_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���g�W��
		TNL_PROPERTY(float, FrequencyFactor, frequency_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���z�ɂ��F���̕ω��W��
		TNL_PROPERTY(float, SteepnessFactor, steepness_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �p�^�[���e�N�X�`���F�����W��
		TNL_PROPERTY(float, PatternColFactor, pattern_col_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �����e�N�X�`���F�����W��
		TNL_PROPERTY(float, DecorationColFactor, decoration_col_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �ڒn�e�N�X�`���F�����W��
		TNL_PROPERTY(float, LandingProjectionColFactor, landing_projection_col_factor_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �ڒn�e�N�X�`���F�����W��2
		TNL_PROPERTY(float, LandingProjectionColPow, landing_projection_col_pow_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �p�^�[���e�N�X�`���g�p
		TNL_PROPERTY(bool, UsePatternTexture, use_pattern_tex_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �����e�N�X�`���g�p
		TNL_PROPERTY(bool, UseDecorationTexture, use_decoration_tex_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �ڒn�e�N�X�`���g�p
		TNL_PROPERTY(bool, UseLandingProjectionTexture, use_landing_projection_tex_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���蔲���e�N�X�`���g�p
		TNL_PROPERTY(bool, UseLandingCutOutTexture, use_landing_cut_out_tex_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���}�b�v�e�N�X�`���g�p
		TNL_PROPERTY(bool, UseCubeMapTexture, use_cube_map_tex_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �m�C�Y�̃T���v�����O��
		TNL_PROPERTY(eGerstnerSampling, GerstnerSampling, e_gerstner_sampling_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �J���������ɂ��t�F�[�h�ݒ�
		TNL_PROPERTY(eFarFade, FarFade, e_far_fade_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �u�����h�X�e�[�g�ݒ�
		TNL_PROPERTY(eBlendState, BlendMode, blend_state_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �T���v���X�e�[�g�ݒ�
		TNL_PROPERTY(eSamplerState, SamplerState, sampler_state_);
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���X�^���C�U�X�e�[�g�ݒ�
		TNL_PROPERTY(eRasterizerState, RasterizerState, rasterizer_state_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		void loadPatternTexture(const std::string& pattern_texture_path);
		
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		void loadDecorationTexture(const std::string& decoration_texture_path);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		void setCubeMapCameraPosition(const tnl::Vector3& camera_position);
		void writeToCubeMap(const std::function< void(std::shared_ptr<dxe::Camera>&) >& render);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �`��
		void render(const Shared<dxe::Camera>& camera);

		void addLandingMesh(const Shared<dxe::Mesh>& mesh, fLandingMeshUse f_use, const float projection_volume = 5.0f);

		void setLandingProjectionVolume(float height);
		void setLandingProjectionColDensity(float density);
		void setLandingProjectionSrvDpiDivide(AreaProjectionTexture::eDpiDivide srv_dpi_divide);
		void setLandingProjectionBlurDpiDivide(AreaProjectionTexture::eDpiDivide blur_dpi_divide);
		float getLandingProjectionVolume();
		float getLandingProjectionColDensity();
		Shared<dxe::AreaProjectionTexture>	getLandingProjectionTexture() { return tex_landing_projection_; }
		Shared<dxe::AreaProjectionTexture>	getLandingCutOutTexture() { return tex_landing_cut_out_; }

		//---------------------------------------------------------------------------------------
		// GUI �R���g���[��
		void drawGuiController(const tnl::Vector2i& pos);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �g�̌���
		TNL_SET_SELF_PROPERTY(eWaveDirection, WaveDirection, e_wave_direction_);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �g�̕��U��
		TNL_SET_SELF_PROPERTY(float, WaveDecentralization, wave_decentralization_);

		TNL_SET_SELF_PROPERTY(eSize, SizeWidth, e_size_width_);
		TNL_SET_SELF_PROPERTY(eSize, SizeDepth, e_size_depth_);
		TNL_SET_SELF_PROPERTY(float, PatchNum, patch_num_);
		TNL_SET_SELF_PROPERTY(uint32_t, DivNum, div_num_);

	private :

		static constexpr uint32_t DIV_MAX = 30;
		static constexpr uint32_t PATCH_MAX = 64;

		void initialize();
		void createVertexBuffer();
		void resetInnerCreateTexture();
		void saveStatus(const std::string& file_path);
		void loadStatus(const std::string& file_path);


		eWaveDirection				e_wave_direction_ = eWaveDirection::RANDOM;
		eGerstnerSampling			e_gerstner_sampling_ = eGerstnerSampling::D8;
		eFarFade					e_far_fade_ = eFarFade::NONE;
		eSize						e_size_width_ = eSize::S1024;
		eSize						e_size_depth_ = eSize::S1024;
		float						size_width_ = 1024;
		float						size_depth_ = 1024;
		float						patch_num_ = 1.0f;
		float						fresnel_base_alpha_ = 0.75f;
		float						height_max_ = 1.0f;
		float						progress_time_ = 0;
		float						base_col_str_ = 0.5f;
		float						shallow_col_str_ = 1.36f;
		float						darkness_contrast_ = 2.0f;
		float						specular_power_ = 500;
		float						shininess_ = 0.27f;
		float						fresnel_factor_ = 0.1f;
		float						noize_strength_ = 1.6f;
		float						base_amp_factor_ = 1.0f;
		float						cube_amp_factor_ = 0.05f;
		float						frequency_factor_ = 1.0f;
		float						steepness_factor_ = 1.6f;
		float						time_scale_ = 1.0f;
		float						pattern_col_factor_ = 1.0f;
		float						decoration_col_factor_ = 0.5f;
		float						landing_projection_col_factor_ = 0.33f;
		float						landing_projection_col_pow_ = 0.6f;
		float						wave_decentralization_ = 0.25f;
		uint32_t					div_num_ = 0;
		uint32_t					index_num_ = 0;
		bool						use_pattern_tex_ = false;
		bool						use_decoration_tex_ = false;
		bool						use_landing_projection_tex_ = false; 
		bool						use_landing_cut_out_tex_ = false;
		bool						use_cube_map_tex_ = false;

		eBlendState					blend_state_ = eBlendState::ALPHA;
		eSamplerState				sampler_state_ = eSamplerState::ANISOTROPIC;
		eRasterizerState			rasterizer_state_ = eRasterizerState::CULL_NONE;
		tnl::Vector3				sky_color_ = { 0.3f, 0.3f, 0.3f };
		tnl::Vector3				base_color_ = { 0.2f, 0.3f, 0.85f };
		tnl::Vector3				shallow_color_ = { 0.1f, 0.1f, 0.1f };
		tnl::Vector3				specular_color_ = { 1, 1, 1 };
		tnl::Vector3				light_direction_ = { 0, -1, 0.7f };
		DirectX::XMFLOAT4			wave_directionA_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4			wave_directionB_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4			wave_directionC_ = { 0, 0, 0, 0 };
		DirectX::XMFLOAT4			wave_directionD_ = { 0, 0, 0, 0 };

		tnl::Vector3	pos_ = { 0, 0, 0 };
		tnl::Vector3	scl_ = { 1,1,1 };
		tnl::Quaternion rot_;

		Shared<dxe::Texture>				tex_pattern_			= nullptr ;
		Shared<dxe::Texture>				tex_decoration_			= nullptr ;
		Shared<dxe::Texture>				tex_cube_map_			= nullptr ;
		Shared<dxe::AreaProjectionTexture>	tex_landing_projection_ = nullptr ;
		Shared<dxe::AreaProjectionTexture>	tex_landing_cut_out_	= nullptr ;

		// ���_�o�b�t�@
		ComPtr<ID3D11Buffer>				vertex_buffer_ = nullptr;
		// �C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer>				index_buffer_ = nullptr;
		// ���_�V�F�[�_�[
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// �s�N�Z���V�F�[�_�[
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;
		// �n���V�F�[�_�[
		ComPtr<ID3D11HullShader>			hull_shader_ = nullptr ;
		// �h���C���V�F�[�_�[
		ComPtr<ID3D11DomainShader>			domain_shader_ = nullptr ;

		// ���_���̓��C�A�E�g
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// �萔�o�b�t�@
		ComPtr<ID3D11Buffer>                constant_buffer1_ = nullptr;
		ComPtr<ID3D11Buffer>                constant_buffer2_ = nullptr;


		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<dxe::GuiCheckBox> gui_use_pattern_tex_;
		Shared<dxe::GuiCheckBox> gui_use_decoration_tex_;
		Shared<dxe::GuiCheckBox> gui_use_landing_projection_tex_;
		Shared<dxe::GuiCheckBox> gui_use_landing_cut_out_tex_;
		Shared<dxe::GuiCheckBox> gui_use_cube_map_tex_;
		Shared<GuiInputBox> gui_div_num_;
		Shared<GuiInputBox> gui_patch_num_;
		Shared<GuiValueSlider< WaterPlane, tnl::Vector3 >> gui_sky_color_;
		Shared<GuiValueSlider< WaterPlane, tnl::Vector3 >> gui_base_color_;
		Shared<GuiValueSlider< WaterPlane, tnl::Vector3 >> gui_shallow_color_;
		Shared<GuiValueSlider< WaterPlane, tnl::Vector3 >> gui_specular_color_;
		Shared<GuiValueSlider< WaterPlane, tnl::Vector3 >> gui_light_direction_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_height_max_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_base_col_str_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_shallow_col_str_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_darkness_contrast_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_specular_power_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_shininess_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_fresnel_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_fresnel_base_alpha_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_noize_strength_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_base_amp_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_cube_amp_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_frequency_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_steepness_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_pattern_col_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_decoration_col_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_landing_projection_col_factor_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_landing_projection_col_pow_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_wave_decentralization_;
		Shared<GuiValueSlider< WaterPlane, float >> gui_time_scale_;
		Shared<GuiMenuSelector> gui_wave_direction_;
		Shared<GuiMenuSelector> gui_far_fade_;
		Shared<GuiMenuSelector> gui_gerstner_sampling_;
		Shared<GuiMenuSelector> gui_size_width_;
		Shared<GuiMenuSelector> gui_size_depth_;

		Shared<GuiDragBox> gui_drag_resouce_;
		std::string description_str_;

	};

	TNL_ENUM_CLASS_BIT_MASK_OPERATOR(WaterPlane::fLandingMeshUse);

}
