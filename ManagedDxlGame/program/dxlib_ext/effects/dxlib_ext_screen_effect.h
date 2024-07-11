#pragma once
#include "../../library/tnl_util.h"
#include "../../library/tnl_vector2i.h"
#include "../gui/dxlib_ext_gui_value_slider.h"
#include "../gui/dxlib_ext_gui_check_box.h"

namespace dxe {

	class ScreenEffect final {
	public:

		enum class fAdoption {
			TNL_ENUM_BIT_FLAG(BLOOM)
			, TNL_ENUM_BIT_FLAG(BLUR)
			, TNL_ENUM_BIT_FLAG(MONO)
			, TNL_ENUM_BIT_FLAG(HSB)
			, TNL_ENUM_BIT_FLAG(LEVEL)
		};

		ScreenEffect(int screen_w, int screen_h);
		~ScreenEffect();

		TNL_PROPERTY(int, BloomThreshold, bloom_threshold_);
		TNL_PROPERTY(int, BloomRatio, bloom_ratio_);
		TNL_PROPERTY(float, BloomAlpha, bloom_alpha_);
		TNL_PROPERTY(int, BlurRatio, blur_ratio_);
		TNL_PROPERTY(float, BlurAlpha, blur_alpha_);

		TNL_PROPERTY(int, MonoCb, mono_cb_);
		TNL_PROPERTY(int, MonoCr, mono_cr_);

		TNL_PROPERTY(int, HsbHue, hsb_hue_);
		TNL_PROPERTY(int, HsbSaturation, hsb_saturation_);
		TNL_PROPERTY(int, HsbBright, hsb_bright_);

		TNL_PROPERTY(int, LevelMin, level_min_);
		TNL_PROPERTY(int, LevelMax, level_max_);
		TNL_PROPERTY(int, LevelGamma, level_gamma_);
		TNL_PROPERTY(int, LevelAfterMin, level_after_min_);
		TNL_PROPERTY(int, LevelAfterMax, level_after_max_);

		TNL_PROPERTY(fAdoption, Adoption, f_adoption_);

		void renderBegin();
		void renderEnd();

		void saveStatus(const std::string& file_path);
		void loadStatus(const std::string& file_path);
		void drawGuiController(const tnl::Vector2i& pos);

	private:
		int screen_w_ = 0;
		int screen_h_ = 0;
		int scr_col_ = 0;			// ���ʂ̕`�挋�ʂ��������ރX�N���[��
		int scr_heght_bright_ = 0;	// ���ʂ̕`�挋�ʂ��獂�P�x�����𔲂��o�������ʂ��������ރX�N���[��
		int scr_down_scale_ = 0;	// ���P�x�������k���������ʂ��������ރX�N���[��
		int scr_gauss_ = 0;			// �k���摜���K�E�X�t�B���^�łڂ��������ʂ��������ރX�N���[��

		int bloom_threshold_ = 230;		// �u���[���p�P�x��臒l
		int bloom_ratio_ = 1200;		// �u���[���p�̂ڂ����x����
		float bloom_alpha_ = 1.0f;		// �u���[���t�B���^�����x

		int blur_ratio_ = 1200;			// �u���[�p�̂ڂ����x����
		float blur_alpha_ = 0.0f;		// �u���[�t�B���^�����x

		int mono_cb_ = 0;			// ���m�g�[�� YCb
		int mono_cr_ = 0;			// ���m�g�[�� YCr

		int hsb_hue_ = 0;			// HSB �F��
		int hsb_saturation_ = 0;	// HSB �ʓx
		int hsb_bright_ = 0;		// HSB �P�x

		int level_min_ = 0;			// level �␳ �ϊ����̉����l( 0 �` 255 )
		int level_max_ = 255;		// level �␳ �ϊ����̏���l( 0 �` 255 )
		int level_gamma_ = 100;		// level �␳ �K���}�l( 100 �� 1.0 ��\���A�K���}�␳�����A1 ��菬�����l�͕s�� )
		int level_after_min_ = 0;	// level �␳ �ϊ���̍Œ�l( 0 �` 255 )
		int level_after_max_ = 255;	// level �␳ �ϊ���̍ő�l( 0 �` 255 )

		fAdoption f_adoption_ = fAdoption::BLOOM ;



		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<dxe::GuiCheckBox> gui_use_bloom_;
		Shared<dxe::GuiCheckBox> gui_use_blur_;
		Shared<dxe::GuiCheckBox> gui_use_mono_;
		Shared<dxe::GuiCheckBox> gui_use_hsb_;
		Shared<dxe::GuiCheckBox> gui_use_level_;

		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_luminance_threshold_;
		Shared<dxe::GuiValueSlider< ScreenEffect, float >> gui_filter_bloom_alpha_;
		Shared<dxe::GuiValueSlider< ScreenEffect, float >> gui_filter_blur_alpha_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_bloom_ratio_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_blur_ratio_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_mono_cb_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_mono_cr_;

		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_hsb_hue_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_hsb_saturation_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_hsb_bright_;

		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_level_min_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_level_max_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_level_gamma_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_level_after_min_;
		Shared<dxe::GuiValueSlider< ScreenEffect, int >> gui_level_after_max_;

	};

	TNL_ENUM_CLASS_BIT_MASK_OPERATOR(ScreenEffect::fAdoption);

}
