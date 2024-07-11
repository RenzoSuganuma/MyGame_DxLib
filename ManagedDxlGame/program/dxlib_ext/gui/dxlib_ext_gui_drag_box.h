#pragma once
#include <functional>
#include <algorithm>
#include "../../library/tnl_vector2i.h"
#include "../../library/tnl_input.h"

namespace dxe {

    class GuiDragBox {
    public:

        GuiDragBox(const std::function<void(const std::string& drag_data_path)>& draged_function);

        TNL_PROPERTY(int32_t, Width, width_);
        TNL_PROPERTY(int32_t, Height, height_);
        TNL_PROPERTY(tnl::Vector2i, Position, pos_);
        TNL_PROPERTY(std::string, Label, label_);
        TNL_PROPERTY(std::function<void()>, Description, description_function_);

        void update();
        void draw();

    private:
        int32_t width_ = 100;
        int32_t height_ = 30;
        tnl::Vector2i pos_;
        std::string label_;
        bool is_forcus_ = false;
        std::function<void(const std::string& drag_data_path)> draged_function_;
        std::function<void()> description_function_ = nullptr;
    };



}