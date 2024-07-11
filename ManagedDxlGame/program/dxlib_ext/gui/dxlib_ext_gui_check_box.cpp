#pragma once
#include "../dxlib_ext.h"
#include "dxlib_ext_gui_check_box.h"

namespace dxe {

    GuiCheckBox::GuiCheckBox(const std::function<void(bool is_check)>& input_function, bool is_default_check)
        : input_function_(input_function)
        , is_check_(is_default_check) {
    }


    //-------------------------------------------------------------------------------------------------------------------
    void GuiCheckBox::update() {
        tnl::Vector3 ms = tnl::Input::GetMousePosition();

        int tx = pos_.x;
        int ty = pos_.y;
        int bx = pos_.x + size_.y;
        int by = pos_.y + size_.y;

        is_forcus_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_forcus_ = true;
            if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::OUT_LEFT)) {
                is_check_ = !is_check_;
                input_function_(is_check_);
            }
        }

        tx = pos_.x;
        ty = pos_.y;
        bx = pos_.x + size_.x;
        by = pos_.y + size_.y;
        is_description_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_description_ = true;
            if (description_function_) {
                description_function_();
            }
        }
    }

    //-------------------------------------------------------------------------------------------------------------------
    void GuiCheckBox::draw() {

        int tx = pos_.x;
        int ty = pos_.y;
        int bx = tx + size_.x;
        int by = ty + size_.y;

        int color = (is_forcus_) ? -1 : 0x88888888;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(tx, ty, bx, by, 0, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        DrawBox(tx, ty, tx + size_.y, by, color, false);
        if (is_check_) {
            DrawCircle(tx + (size_.y / 2), ty + (size_.y / 2), 5, 0xff00ff00, false);
        }

        int label_color = (is_description_ && description_function_) ? -1 : 0x88888888;
        DrawStringEx(tx + size_.y + 5, ty + 2, label_color, "%s", label_.c_str());
    }


}

