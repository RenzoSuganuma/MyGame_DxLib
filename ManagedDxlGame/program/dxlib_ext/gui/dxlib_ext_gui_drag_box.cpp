#include "../dxlib_ext.h"
#include "dxlib_ext_gui_drag_box.h"

namespace dxe {

    GuiDragBox::GuiDragBox(const std::function<void(const std::string& drag_data_path)>& draged_function)
        : draged_function_(draged_function) {
    }

    void GuiDragBox::update() {
        tnl::Vector3 ms = tnl::Input::GetMousePosition();

        int tx = pos_.x;
        int ty = pos_.y;
        int bx = pos_.x + width_;
        int by = pos_.y + height_;

        is_forcus_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_forcus_ = true;
            if (description_function_) description_function_();
        }

        if (!is_forcus_) return;
        std::string file_path = GetDragFilePathReadTrigger();
        if (file_path.empty()) return;
        draged_function_(file_path);
    }

    void GuiDragBox::draw() {
        int color = (is_forcus_) ? -1 : 0x88888888;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(pos_.x, pos_.y, pos_.x + width_, pos_.y + height_, 0, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        DrawBox(pos_.x, pos_.y, pos_.x + width_, pos_.y + height_, color, false);
        DrawFormatString(pos_.x + 5, pos_.y + 7, color, "%s", label_.c_str());
    }

}