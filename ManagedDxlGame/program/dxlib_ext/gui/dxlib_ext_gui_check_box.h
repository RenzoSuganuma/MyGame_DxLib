#pragma once
#include <functional>
#include <algorithm>
#include "../../library/tnl_vector2i.h"
#include "../../library/tnl_input.h"

namespace dxe {

    class GuiCheckBox {
    public:

        GuiCheckBox(const std::function<void(bool is_check)>& input_function, bool is_default_check);

        TNL_PROPERTY(bool, Check, is_check_);
        TNL_PROPERTY(tnl::Vector2i, Position, pos_);
        TNL_PROPERTY(tnl::Vector2i, Size, size_);
        TNL_PROPERTY(std::string, Label, label_);
        TNL_PROPERTY(std::function<void()>, Description, description_function_);

        void update();
        void draw();

    private:
        bool is_check_ = true;
        bool is_forcus_ = false;
        bool is_description_ = false;
        tnl::Vector2i pos_ = { 0, 0 };
        tnl::Vector2i size_ = { 250, 20 };
        std::string label_;
        std::string input_string_;
        std::function<void(bool is_check)> input_function_;
        std::function<void()> description_function_ = nullptr;
    };

}
