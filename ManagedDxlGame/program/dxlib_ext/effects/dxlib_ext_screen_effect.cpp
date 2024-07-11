#include "DxLib.h"
#include "dxlib_ext_screen_effect.h"

namespace dxe {

	ScreenEffect::ScreenEffect(int screen_w, int screen_h) : screen_w_(screen_w), screen_h_(screen_h) {

		int down_scl_w = screen_w / 8;
		int down_scl_h = screen_h / 8;

		// �ʏ�̕`�挋�ʂ��������ރX�N���[���ƁA�t�B���^�[�̏������ʂ��������ރX�N���[���̍쐬
		scr_col_ = MakeScreen(screen_w, screen_h, FALSE);
		scr_heght_bright_ = MakeScreen(screen_w, screen_h, FALSE);
		scr_down_scale_ = MakeScreen(down_scl_w, down_scl_h, FALSE);
		scr_gauss_ = MakeScreen(down_scl_w, down_scl_h, FALSE);

	}
	ScreenEffect::~ScreenEffect() {
		DeleteGraph(scr_col_);
		DeleteGraph(scr_heght_bright_);
		DeleteGraph(scr_down_scale_);
		DeleteGraph(scr_gauss_);
	}

	void ScreenEffect::renderBegin() {
		// �ʏ�̕`�挋�ʂ��������ރX�N���[����`��Ώۂɂ���
		SetDrawScreen(scr_col_);
		SetBackgroundColor(0, 0, 0);
		ClearDrawScreen();
	}

	void ScreenEffect::renderEnd() {
		if (static_cast<bool>(f_adoption_ & fAdoption::BLOOM)) {
			// �`�挋�ʂ��獂�P�x�����݂̂𔲂��o�����摜�𓾂�
			GraphFilterBlt(scr_col_, scr_heght_bright_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, bloom_threshold_, TRUE, GetColor(0, 0, 0), 255);

			// ���P�x�������W���̂P�ɏk�������摜�𓾂�
			GraphFilterBlt(scr_heght_bright_, scr_down_scale_, DX_GRAPH_FILTER_DOWN_SCALE, 8);

			// �W���̂P�ɏk�������摜���K�E�X�t�B���^�łڂ���
			GraphFilterBlt(scr_down_scale_, scr_gauss_, DX_GRAPH_FILTER_GAUSS, 16, bloom_ratio_);

			// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɂ���( �g�債���Ƃ��Ƀh�b�g���ڂ₯��悤�ɂ��� )
			SetDrawMode(DX_DRAWMODE_BILINEAR);

			// �`��u�����h���[�h�����Z�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(bloom_alpha_ * 255.0f));

			// ���P�x�������k�����Ăڂ������摜����ʂ����ς��ɂQ��`�悷��( �Q��`�悷��̂͂�薾�邭�݂���悤�ɂ��邽�� )
			DrawExtendGraph(0, 0, screen_w_, screen_h_, scr_gauss_, FALSE);
			DrawExtendGraph(0, 0, screen_w_, screen_h_, scr_gauss_, FALSE);

			// �`��u�����h���[�h���u�����h�����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			// �`�惂�[�h���A���X�g�ɖ߂�
			SetDrawMode(DX_DRAWMODE_NEAREST);
		}


		if (static_cast<bool>(f_adoption_ & fAdoption::BLUR)) {
			// �`�挋�ʂ��W���̂P�ɏk�������摜�𓾂�
			GraphFilterBlt(scr_col_, scr_down_scale_, DX_GRAPH_FILTER_DOWN_SCALE, 8);

			// �W���̂P�ɏk�������摜���K�E�X�t�B���^�łڂ���
			GraphFilterBlt(scr_down_scale_, scr_gauss_, DX_GRAPH_FILTER_GAUSS, 16, blur_ratio_);

			// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɂ���( �g�債���Ƃ��Ƀh�b�g���ڂ₯��悤�ɂ��� )
			SetDrawMode(DX_DRAWMODE_BILINEAR);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(blur_alpha_ * 255.0f));
			DrawExtendGraph(0, 0, screen_w_, screen_h_, scr_gauss_, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			// �`�惂�[�h���A���X�g�ɖ߂�
			SetDrawMode(DX_DRAWMODE_NEAREST);
		}

		// �`��Ώۂ𗠉�ʂɂ���
		SetDrawScreen(DX_SCREEN_BACK);

		// ���m�g�[���t�B���^
		if (static_cast<bool>(f_adoption_ & fAdoption::MONO)) {
			GraphFilter(scr_col_, DX_GRAPH_FILTER_MONO, mono_cb_, mono_cr_);
		}
		// HSB�t�B���^
		if (static_cast<bool>(f_adoption_ & fAdoption::HSB)) {
			GraphFilter(scr_col_, DX_GRAPH_FILTER_HSB, 0, hsb_hue_, hsb_saturation_, hsb_bright_);
		}
		// Level�␳�t�B���^
		if (static_cast<bool>(f_adoption_ & fAdoption::LEVEL)) {
			GraphFilter(scr_col_, DX_GRAPH_FILTER_LEVEL, level_min_, level_max_, level_gamma_, level_after_min_, level_after_max_);
		}

		// �ʏ�̕`�挋�ʂ�`�悷��
		DrawGraph(0, 0, scr_col_, FALSE);

	}


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	void ScreenEffect::saveStatus(const std::string& file_path) {
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "wb");
		if (fp) {
			fwrite(&bloom_threshold_, sizeof(int), 1, fp);
			fwrite(&bloom_ratio_,	  sizeof(int), 1, fp);
			fwrite(&bloom_alpha_,	  sizeof(float), 1, fp);

			fwrite(&blur_ratio_,	  sizeof(int), 1, fp);
			fwrite(&blur_alpha_,	  sizeof(float), 1, fp);

			fwrite(&mono_cb_,		  sizeof(int), 1, fp);
			fwrite(&mono_cr_,		  sizeof(int), 1, fp);

			fwrite(&hsb_hue_,		 sizeof(int), 1, fp);
			fwrite(&hsb_saturation_, sizeof(int), 1, fp);
			fwrite(&hsb_bright_,	 sizeof(int), 1, fp);

			fwrite(&level_min_,			sizeof(int), 1, fp);
			fwrite(&level_max_,			sizeof(int), 1, fp);
			fwrite(&level_gamma_,		sizeof(int), 1, fp);
			fwrite(&level_after_min_,	sizeof(int), 1, fp);
			fwrite(&level_after_max_,	sizeof(int), 1, fp);

			fwrite(&f_adoption_, sizeof(int), 1, fp);
			fclose(fp);
		}

	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------
	void ScreenEffect::loadStatus(const std::string& file_path) {
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {

			fread(&bloom_threshold_, sizeof(int), 1, fp);
			fread(&bloom_ratio_, sizeof(int), 1, fp);
			fread(&bloom_alpha_, sizeof(float), 1, fp);

			fread(&blur_ratio_, sizeof(int), 1, fp);
			fread(&blur_alpha_, sizeof(float), 1, fp);

			fread(&mono_cb_, sizeof(int), 1, fp);
			fread(&mono_cr_, sizeof(int), 1, fp);

			fread(&hsb_hue_, sizeof(int), 1, fp);
			fread(&hsb_saturation_, sizeof(int), 1, fp);
			fread(&hsb_bright_, sizeof(int), 1, fp);

			fread(&level_min_, sizeof(int), 1, fp);
			fread(&level_max_, sizeof(int), 1, fp);
			fread(&level_gamma_, sizeof(int), 1, fp);
			fread(&level_after_min_, sizeof(int), 1, fp);
			fread(&level_after_max_, sizeof(int), 1, fp);

			fread(&f_adoption_, sizeof(int), 1, fp);

			fclose(fp);
		}

	}


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	void ScreenEffect::drawGuiController(const tnl::Vector2i& pos) {

		int x = pos.x;
		int y = pos.y;

		tnl::Vector3 ms = tnl::Input::GetMousePosition();
		int tx = x;
		int ty = y;
		int bx = tx + 330;
		int by = y += 30;

		is_btn_save_forcus_ = false;
		if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
			is_btn_save_forcus_ = true;
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
				is_btn_save_select_ = true;
			}
		}
		if (is_btn_save_select_) {
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
				is_btn_save_select_ = false;
				saveStatus("screen_effect.bin");
			}
		}

		int color = (is_btn_save_forcus_) ? -1 : 0x88888888;
		color = (is_btn_save_select_) ? 0xff00ff00 : color;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(tx, ty, bx, by, 0, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawBox(tx, ty, bx, by, color, false);
		DrawStringEx((float)tx + 5, (float)ty + 5, color, "save to file");

		if (!gui_use_bloom_) {
			gui_use_bloom_ = std::make_shared<dxe::GuiCheckBox>(
				[&](bool is_check){
					if (is_check) setAdoption(getAdoption() | fAdoption::BLOOM);
					else setAdoption(getAdoption() & ~fAdoption::BLOOM);
				}, static_cast<bool>( f_adoption_ & fAdoption::BLOOM ) );
			gui_use_bloom_->setPosition({pos.x, y});
			gui_use_bloom_->setLabel("use bloom");
		}
		if (!gui_use_blur_) {
			gui_use_blur_ = std::make_shared<dxe::GuiCheckBox>(
				[&](bool is_check) {
					if (is_check) setAdoption(getAdoption() | fAdoption::BLUR);
					else setAdoption(getAdoption() & ~fAdoption::BLUR);
				}, static_cast<bool>(f_adoption_ & fAdoption::BLUR));
			gui_use_blur_->setPosition({ pos.x, y += 20 });
			gui_use_blur_->setLabel("use blur");
		}
		if (!gui_use_mono_) {
			gui_use_mono_ = std::make_shared<dxe::GuiCheckBox>(
				[&](bool is_check) {
					if (is_check) setAdoption(getAdoption() | fAdoption::MONO);
					else setAdoption(getAdoption() & ~fAdoption::MONO);
				}, static_cast<bool>(f_adoption_ & fAdoption::MONO));
			gui_use_mono_->setPosition({ pos.x, y += 20 });
			gui_use_mono_->setLabel("use mono");
		}
		if (!gui_use_hsb_) {
			gui_use_hsb_ = std::make_shared<dxe::GuiCheckBox>(
				[&](bool is_check) {
					if (is_check) setAdoption(getAdoption() | fAdoption::HSB);
					else setAdoption(getAdoption() & ~fAdoption::HSB);
				}, static_cast<bool>(f_adoption_ & fAdoption::HSB));
			gui_use_hsb_->setPosition({ pos.x, y += 20 });
			gui_use_hsb_->setLabel("use hsb");
		}
		if (!gui_use_level_) {
			gui_use_level_ = std::make_shared<dxe::GuiCheckBox>(
				[&](bool is_check) {
					if (is_check) setAdoption(getAdoption() | fAdoption::LEVEL);
					else setAdoption(getAdoption() & ~fAdoption::LEVEL);
				}, static_cast<bool>(f_adoption_ & fAdoption::LEVEL));
			gui_use_level_->setPosition({ pos.x, y += 20 });
			gui_use_level_->setLabel("use level");
		}
		if (!gui_luminance_threshold_) {
			gui_luminance_threshold_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getBloomThreshold
					, &ScreenEffect::setBloomThreshold
					, { pos.x, y += 30 }
					, 0
					, 255
					, "bloom threshold"));
		}
		if (!gui_filter_bloom_alpha_) {
			gui_filter_bloom_alpha_ = Shared<dxe::GuiValueSlider< ScreenEffect, float >>(
				new dxe::GuiValueSlider< ScreenEffect, float >
				(this
					, &ScreenEffect::getBloomAlpha
					, &ScreenEffect::setBloomAlpha
					, { pos.x, y += 20 }
					, 0.0f
					, 1.0f
					, "bloom alpha"));
		}

		if (!gui_bloom_ratio_) {
			gui_bloom_ratio_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getBloomRatio
					, &ScreenEffect::setBloomRatio
					, { pos.x, y += 20 }
					, 0
					, 2000
					, "bloom ratio"));
		}
		if (!gui_filter_blur_alpha_) {
			gui_filter_blur_alpha_ = Shared<dxe::GuiValueSlider< ScreenEffect, float >>(
				new dxe::GuiValueSlider< ScreenEffect, float >
				(this
					, &ScreenEffect::getBlurAlpha
					, &ScreenEffect::setBlurAlpha
					, { pos.x, y += 20 }
					, 0.0f
					, 1.0f
					, "blur alpha"));
		}
		if (!gui_blur_ratio_) {
			gui_blur_ratio_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getBlurRatio
					, &ScreenEffect::setBlurRatio
					, { pos.x, y += 20 }
					, 0
					, 2000
					, "blur ratio"));
		}
		if (!gui_mono_cb_) {
			gui_mono_cb_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getMonoCb
					, &ScreenEffect::setMonoCb
					, { pos.x, y += 20 }
					, -255
					, 255
					, "mono cb"));
		}
		if (!gui_mono_cr_) {
			gui_mono_cr_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getMonoCr
					, &ScreenEffect::setMonoCr
					, { pos.x, y += 20 }
					, -255
					, 255
					, "mono cr"));
		}
		if (!gui_hsb_hue_) {
			gui_hsb_hue_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getHsbHue
					, &ScreenEffect::setHsbHue
					, { pos.x, y += 20 }
					, -180
					, 180
					, "hsb hue"));
		}
		if (!gui_hsb_saturation_) {
			gui_hsb_saturation_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getHsbSaturation
					, &ScreenEffect::setHsbSaturation
					, { pos.x, y += 20 }
					, -255
					, 255
					, "hsb saturation"));
		}
		if (!gui_hsb_bright_) {
			gui_hsb_bright_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getHsbBright
					, &ScreenEffect::setHsbBright
					, { pos.x, y += 20 }
					, -255
					, 255
					, "hsb bright"));
		}
		if (!gui_level_min_) {
			gui_level_min_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getLevelMin
					, &ScreenEffect::setLevelMin
					, { pos.x, y += 20 }
					, 0
					, 255
					, "level min"));
		}
		if (!gui_level_max_) {
			gui_level_max_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getLevelMax
					, &ScreenEffect::setLevelMax
					, { pos.x, y += 20 }
					, 0
					, 255
					, "level max"));
		}
		if (!gui_level_gamma_) {
			gui_level_gamma_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getLevelGamma
					, &ScreenEffect::setLevelGamma
					, { pos.x, y += 20 }
					, 1
					, 300
					, "level gamma"));
		}
		if (!gui_level_after_min_) {
			gui_level_after_min_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getLevelAfterMin
					, &ScreenEffect::setLevelAfterMin
					, { pos.x, y += 20 }
					, 0
					, 255
					, "level after min"));
		}
		if (!gui_level_after_max_) {
			gui_level_after_max_ = Shared<dxe::GuiValueSlider< ScreenEffect, int >>(
				new dxe::GuiValueSlider< ScreenEffect, int >
				(this
					, &ScreenEffect::getLevelAfterMax
					, &ScreenEffect::setLevelAfterMax
					, { pos.x, y += 20 }
					, 0
					, 255
					, "level after max"));
		}


		gui_use_bloom_->update();
		gui_use_blur_->update();
		gui_use_mono_->update();
		gui_use_hsb_->update();
		gui_use_level_->update();
		gui_luminance_threshold_->update();
		gui_filter_bloom_alpha_->update();
		gui_filter_blur_alpha_->update();
		gui_bloom_ratio_->update();
		gui_blur_ratio_->update();
		gui_mono_cb_->update();
		gui_mono_cr_->update();
		gui_hsb_hue_->update();
		gui_hsb_saturation_->update();
		gui_hsb_bright_->update();
		gui_level_min_->update();
		gui_level_max_->update();
		gui_level_gamma_->update();
		gui_level_after_min_->update();
		gui_level_after_max_->update();


		gui_use_bloom_->draw();
		gui_use_blur_->draw();
		gui_use_mono_->draw();
		gui_use_hsb_->draw();
		gui_use_level_->draw();
		gui_luminance_threshold_->draw();
		gui_filter_bloom_alpha_->draw();
		gui_filter_blur_alpha_->draw();
		gui_bloom_ratio_->draw();
		gui_blur_ratio_->draw();
		gui_mono_cb_->draw();
		gui_mono_cr_->draw();
		gui_hsb_hue_->draw();
		gui_hsb_saturation_->draw();
		gui_hsb_bright_->draw();
		gui_level_min_->draw();
		gui_level_max_->draw();
		gui_level_gamma_->draw();
		gui_level_after_min_->draw();
		gui_level_after_max_->draw();

	}



}