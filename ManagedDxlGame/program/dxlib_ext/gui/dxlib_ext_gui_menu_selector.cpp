#include "dxlib_ext_gui_menu_selector.h"

namespace dxe {

	void GuiMenuSelector::update() {
		tnl::Vector3 ms = tnl::Input::GetMousePosition();

		int tx = pos_.x;
		int ty = pos_.y;
		int bx = pos_.x + width_;
		int by = pos_.y + height_;

		if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
			forcus_stage_ = eForcusStage::TAB;
		}

		int ltx = 0;
		int lty = 0;
		int adjusted_x = 5;
		int adjusted_y = 0;
		if (location_ == eLocation::RIGHT_DOWN) {
			ltx = pos_.x + width_;
			lty = pos_.y;
			adjusted_x = -adjusted_x;
		}
		if (location_ == eLocation::RIGHT_UP) {
			ltx = pos_.x + width_;
			lty = pos_.y - height_ * ((int32_t)menus_.size() - 1);
			adjusted_x = -adjusted_x;
		}
		if (location_ == eLocation::LEFT_DOWN) {
			ltx = pos_.x - width_;
			lty = pos_.y;
		}
		if (location_ == eLocation::LEFT_UP) {
			ltx = pos_.x - width_;
			lty = pos_.y - height_ * ((int32_t)menus_.size() - 1);
		}
		if (location_ == eLocation::UP) {
			ltx = pos_.x ;
			lty = pos_.y - height_ * (int32_t)menus_.size() ;
			adjusted_y = 5;
			lty += adjusted_y ;
		}
		if (location_ == eLocation::DOWN) {
			ltx = pos_.x;
			lty = pos_.y + height_ ;
			adjusted_y = -5;
			ty = lty + adjusted_y;
		}

		if (eForcusStage::TAB == forcus_stage_) {
			tx = ltx + adjusted_x;
			bx = tx + width_;
			by = lty + height_ * (int32_t)menus_.size();

			if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
				forcus_stage_ = eForcusStage::MENU;
			}
		}

		if (eForcusStage::TAB == forcus_stage_) {
			tx = pos_.x;
			ty = pos_.y;
			bx = pos_.x + width_;
			by = pos_.y + height_;
			if (!(ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by)) {
				forcus_stage_ = eForcusStage::NONE;
			}
		}


		if (eForcusStage::MENU == forcus_stage_) {
			tx = ltx + adjusted_x;
			ty = lty + adjusted_y;
			bx = tx + width_;
			by = lty + height_ * (int32_t)menus_.size();

			if (!(ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by)) {
				forcus_stage_ = eForcusStage::NONE;
			}
		}

		if (eForcusStage::NONE != forcus_stage_) {
			if (description_function_) {
				description_function_();
			}
		}

		forcus_index_ = -1;
		if (eForcusStage::MENU != forcus_stage_) return;

		for (int i = 0; i < menus_.size(); ++i) {
			tx = ltx + adjusted_x;
			ty = lty + adjusted_y + height_ * i;
			bx = tx + width_;
			by = lty + height_ * (i + 1);
			if ((ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by)) {
				forcus_index_ = i;
				if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
					current_index_ = i;
					select_function_(current_index_);
				}
				break;
			}
		}

	}

	void GuiMenuSelector::draw() {

		int color = (forcus_stage_ != eForcusStage::NONE) ? -1 : 0x88888888;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(pos_.x, pos_.y, pos_.x + width_, pos_.y + height_, 0, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawBox(pos_.x, pos_.y, pos_.x + width_, pos_.y + height_, color, false);

		int ltx = 0;
		int lty = 0;
		if (location_ == eLocation::RIGHT_DOWN) {
			ltx = pos_.x + width_;
			lty = pos_.y;
		}
		if (location_ == eLocation::RIGHT_UP) {
			ltx = pos_.x + width_;
			lty = pos_.y - height_ * ((int32_t)menus_.size() - 1);
		}
		if (location_ == eLocation::LEFT_DOWN) {
			ltx = pos_.x - width_;
			lty = pos_.y;
		}
		if (location_ == eLocation::LEFT_UP) {
			ltx = pos_.x - width_;
			lty = pos_.y - height_ * ((int32_t)menus_.size() - 1);
		}
		if (location_ == eLocation::UP) {
			ltx = pos_.x ;
			lty = pos_.y - height_ * (int32_t)menus_.size() ;
		}
		if (location_ == eLocation::DOWN) {
			ltx = pos_.x;
			lty = pos_.y + height_ ;
		}

		if (forcus_stage_ != eForcusStage::NONE) {
			int tx = ltx;
			int ty = lty;
			int bx = ltx + width_;
			int by = lty + height_ * (int32_t)menus_.size();

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
			DrawBox(tx, ty, bx, by, 0, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			if (-1 != forcus_index_) {
				ty = lty + height_ * forcus_index_;
				by = lty + height_ * (forcus_index_ + 1);
				DrawBox(tx, ty, bx, by, color, false);
			}

			for (int i = 0; i < menus_.size(); ++i) {
				ty = lty + height_ * i;
				DrawStringEx(tx + 5, ty + 6, -1, "%s", menus_[i].c_str());
			}
		}

		DrawStringEx(pos_.x + 5, pos_.y + 6, color, "%s", menus_[current_index_].c_str());
	}

}
