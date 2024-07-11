#pragma once
#include <functional>
#include "../../library/tnl_vector2i.h"
#include "../../library/tnl_input.h"

namespace dxe {

    class GuiInputBox {
    public:
        enum class eInputType { STRING, INT, FLOAT };

        GuiInputBox(eInputType type
            , const std::function<void(const std::string& input_string)>& input_function
            , int input_max_str_num = 64);

        TNL_PROPERTY(int, BoxWidth, box_width_);
        TNL_PROPERTY(tnl::Vector2i, Position, pos_);
        TNL_PROPERTY(tnl::Vector2i, Size, size_);
        TNL_PROPERTY(std::string, Label, label_);
        TNL_PROPERTY(std::function<void()>, Description, description_function_);

        void setBoxString(const std::string& str);

        void update();
        void draw();

    private :
        eInputType input_type_ = eInputType::STRING;
        bool is_forcus_ = false;
        bool is_description_ = false;
        int input_hdl_ = 0;
        int box_width_ = 100;
        tnl::Vector2i pos_ = { 0, 0 };
        tnl::Vector2i size_ = { 250, 20 };
        std::string label_;
        std::string input_string_;
        std::function<void(const std::string& input_string)> input_function_;
        std::function<void()> description_function_ = nullptr;
    };


}
