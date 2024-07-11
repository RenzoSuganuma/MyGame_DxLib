#include "../dxlib_ext.h"
#include "dxlib_ext_gui_input_box.h"


namespace dxe {

    GuiInputBox::GuiInputBox(eInputType type
        , const std::function<void(const std::string& input_string)>& input_function
        , int input_max_str_num)
        : input_type_(type)
        , input_function_(input_function) {
        if (type == eInputType::STRING) {
            input_hdl_ = MakeKeyInput(input_max_str_num, FALSE, TRUE, FALSE);
        }
        else {
            input_hdl_ = MakeKeyInput(input_max_str_num, FALSE, TRUE, TRUE);
        }
        SetKeyInputCursorBrinkTime(500);
        SetKeyInputCursorBrinkFlag(TRUE);
    }

    //-------------------------------------------------------------------------------------------------------------------
    void GuiInputBox::setBoxString(const std::string& str) {
        input_string_ = str;
        SetKeyInputString(input_string_.c_str(), input_hdl_);
    }


    //-------------------------------------------------------------------------------------------------------------------
    void GuiInputBox::update() {
        tnl::Vector3 ms = tnl::Input::GetMousePosition();

        int tx = pos_.x;
        int ty = pos_.y;
        int bx = pos_.x + box_width_;
        int by = pos_.y + size_.y;

        if (tnl::Input::IsMouseTrigger(tnl::Input::eMouseTrigger::IN_LEFT)) {
            if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
                is_forcus_ = true;
                SetActiveKeyInput(input_hdl_);
            }
            else {
                setBoxString(input_string_);
                input_function_(input_string_);
                if(GetActiveKeyInput() == input_hdl_ ) SetActiveKeyInput(-1);
                is_forcus_ = false;
            }
        }

        tx = pos_.x + box_width_;
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

        if (!is_forcus_) return;
        if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
            setBoxString(input_string_);
            input_function_(input_string_);
            SetActiveKeyInput(-1);
            is_forcus_ = false;
        }
        char buff[256] = { 0 };
        GetKeyInputString(buff, input_hdl_);
        input_string_ = buff;
    }

    //-------------------------------------------------------------------------------------------------------------------
    void GuiInputBox::draw() {

        int tx = pos_.x;
        int ty = pos_.y;
        int bx = tx + size_.x;
        int by = ty + size_.y;

        int color = (is_forcus_) ? -1 : 0x88888888;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(tx, ty, bx, by, 0, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        DrawBox(tx, ty, tx + box_width_, by, color, false);

        int label_color = (is_description_ && description_function_) ? -1 : 0x88888888;
        DrawStringEx(tx + box_width_ + 5, ty + 2, label_color, "%s", label_.c_str());
        DrawKeyInputString(tx + 5, ty + 2, input_hdl_);
        DrawStringEx(tx + 5, ty + 2, color, "%s", input_string_.c_str());
    }


}
