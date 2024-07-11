#pragma once
#include <functional>
#include <algorithm>
#include "../../library/tnl_vector2i.h"
#include "../../library/tnl_vector3.h"
#include "../../library/tnl_util.h"
#include "../../library/tnl_input.h"
#include "DxLib.h"

void DrawStringEx(const float x, const float y, const int color, const char* _str, ...);
namespace dxe {

	//--------------------------------------------------------------------------------------------------------------------------
	// GuiValueSlider
	// tips...	int, float, tnl::Vector2i, tnl::Vector3 の getter setter 
	//			メソッドが定義されているクラスにおいて、GUI によるパラメータ調整を提供します。
	// 
	// 
	// 
	// ==================================================================================================================
	// ◇ 使用法サンプル
	// ==================================================================================================================
	//
	//  --------------- Example.h ----------------
	// 
	// class Example
	// {
	//	public :
	//		tnl::Vector3 getValue(){ return value_ ; }
	//		void setValue( const tnl::Vector3& val ){ value_ = val ; }
	//  private :
	//		tnl::Vector3 value_ ;
	// } ;
	//
	//
	//  --------------- GameMain.cpp ----------------
	// 
	// Example* p_example = nullptr  ;
	// dxe::GuiValueSlider< Example, tnl::Vector3 >* p_gui_slider = nullptr;
	// 
	// void gameStart() {
	//		p_example = new Example() ;
	//		p_gui_slider = new dxe::GuiValueSlider<Example, tnl::Vector3>
	//														( p_example				// 対象インスタンスへのポインタ
	//														, &Example::getValue	// getter メソッド
	//														, &Example::setValue	// setter メソッド
	//														, { 100, 100 }			// GUI 表示座標
	//														, { -1, -1, -1 }		// x, y, z それぞれの最小値
	//														, {  1,  1,  1 }		// x, y, z それぞれの最大値
	//														, "value" );			// 表示ラベル( 省略可 )
	// }
	// 
	// void gameMain(float delta_time){
	//		p_gui_slider->update();
	//		p_gui_slider->draw() ;
	// }
	// 
	//

	template<typename T>
	struct GuiValueSliderTraits;
	template<>
	struct GuiValueSliderTraits<int> {
		static constexpr int Value = 0;
	};
	template<>
	struct GuiValueSliderTraits<float> {
		static constexpr int Value = 1;
	};

	template<class T, class U>
	class GuiValueSlider {
	public :
		GuiValueSlider(){}
		GuiValueSlider(
			T* obj
			, const std::function<U(T*)>& getter
			, const std::function<void(T*, const U&)>& setter
			, const tnl::Vector2i pos
			, U min
			, U max
			, const std::string& label = "unknown"
			, uint32_t slider_lenght = 100)
			: accessor_(obj, getter, setter)
			, pos_(pos)
			, min_(min)
			, max_(max)
			, label_(label)
			, slider_lenght_(slider_lenght)
		{}

		void update() {
			tnl::Vector3 ms = tnl::Input::GetMousePosition() ;
			U param = accessor_.get() - min_ ;
			U min_to_max = max_ - min_ ;
			float rate = (float)param / (float)min_to_max ;
			float px = slider_lenght_ * rate ;
			tab_pos_.y = pos_.y ;
			tab_pos_.x = pos_.x + (int)px ;

			int tx = tab_pos_.x - TAB_SIZE_W2 ;
			int ty = tab_pos_.y - TAB_SIZE_H2 ;
			int bx = tab_pos_.x + TAB_SIZE_W2 ;
			int by = tab_pos_.y + TAB_SIZE_H2 ;

			is_forcus_ = false;
			if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
				is_forcus_ = true;
				if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
					is_select_ = true;
				}
			}
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
				is_select_ = false;
			}

			int num_space = 80;
			int text_space = 150;
			tx = pos_.x + slider_lenght_;
			ty = pos_.y - TAB_SIZE_H2;
			bx = pos_.x + slider_lenght_ + num_space + text_space;
			by = pos_.y + TAB_SIZE_H2;
			is_description_ = false;
			if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
				is_description_ = true;
				if (description_function_) {
					description_function_();
				}
			}

			if (!is_select_) return;

			tab_pos_.x = (int)ms.x ;
			tab_pos_.x = std::clamp<int>(tab_pos_.x, pos_.x, pos_.x + slider_lenght_);

			float new_rate = ( (float)tab_pos_.x - (float)pos_.x ) / (float)(slider_lenght_) ;
			U new_param = min_ + (U)( (float)min_to_max * new_rate ) ;
			accessor_.set(new_param);
		}

		void draw() {
			int color = (is_forcus_ || is_select_) ? -1 : 0x88888888;
			int num_space = 80;
			int text_space = 150;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
			DrawBox(pos_.x, pos_.y - TAB_SIZE_H2, pos_.x + slider_lenght_ + num_space + text_space, pos_.y + TAB_SIZE_H2, 0, true) ;
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawLine(pos_.x, pos_.y, pos_.x + slider_lenght_, pos_.y, color) ;
			DrawLine(pos_.x, pos_.y - TAB_SIZE_H2, pos_.x, pos_.y + TAB_SIZE_H2, color) ;
			DrawLine(pos_.x + slider_lenght_, pos_.y - TAB_SIZE_H2, pos_.x + slider_lenght_, pos_.y + TAB_SIZE_H2, color) ;
			DrawBox(tab_pos_.x - TAB_SIZE_W2, tab_pos_.y - TAB_SIZE_H2, tab_pos_.x + TAB_SIZE_W2, pos_.y + TAB_SIZE_H2, color, false) ;
			if constexpr (GuiValueSliderTraits<int>::Value == GuiValueSliderTraits<U>::Value) {
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + TAB_SIZE_W2 + 5, (float)pos_.y - TAB_SIZE_H2 + 3, color, "%d", accessor_.get());
			}
			if constexpr (GuiValueSliderTraits<float>::Value == GuiValueSliderTraits<U>::Value) {
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + TAB_SIZE_W2 + 5, (float)pos_.y - TAB_SIZE_H2 + 3, color, "%.3f", accessor_.get());
			}
			if (is_description_ && description_function_) color = -1;
			DrawStringEx((float)pos_.x + (float)slider_lenght_ + num_space, (float)pos_.y - TAB_SIZE_H2 + 3, color, "%s", label_.c_str());
		}

		TNL_PROPERTY(std::function<void()>, Description, description_function_);

		tnl::Vector2i getPosition() { return pos_; }

	private :
		static constexpr int TAB_SIZE_W2 = 5 ;
		static constexpr int TAB_SIZE_H2 = 10 ;
		bool is_forcus_ = false;
		bool is_select_ = false;
		bool is_description_ = false;
		U min_;
		U max_;
		uint32_t slider_lenght_;
		tnl::Vector2i pos_;
		tnl::Vector2i tab_pos_;
		tnl::FieldAccessor<T, U> accessor_;
		std::string label_;
		std::function<void()> description_function_ = nullptr;
	};


	//-----------------------------------------------------------------------------------------------------------
	// vec2i
	//-----------------------------------------------------------------------------------------------------------
	template<class T>
	class GuiValueSlider<T, tnl::Vector2i> {
	public:
		GuiValueSlider() {}
		GuiValueSlider(
			T* obj
			, const std::function<tnl::Vector2i(T*)>& getter
			, const std::function<void(T*, const tnl::Vector2i&)>& setter
			, const tnl::Vector2i& pos
			, const tnl::Vector2i& min
			, const tnl::Vector2i& max
			, const std::string& label = "unknown"
			, uint32_t slider_lenght = 100)
			: accessor_(obj, getter, setter)
			, pos_(pos)
			, min_(min)
			, max_(max)
			, label_(label)
			, slider_lenght_(slider_lenght)
		{}

		void update() {
			tnl::Vector3 ms = tnl::Input::GetMousePosition();

			int param_x = accessor_.get().x - min_.x ;
			int param_y = accessor_.get().y - min_.y ;

			int min_to_max[2];
			min_to_max[0] = max_.x - min_.x;
			min_to_max[1] = max_.y - min_.y;

			float rate_x = (float)param_x / (float)min_to_max[0];
			float rate_y = (float)param_y / (float)min_to_max[1];

			float tab1_px = slider_lenght_ * rate_x;
			float tab2_px = slider_lenght_ * rate_y;

			tab_pos_[0].y = pos_.y;
			tab_pos_[0].x = pos_.x + (int)tab1_px;
			tab_pos_[1].y = pos_.y + TAB_SIZE_H2 * 2;
			tab_pos_[1].x = pos_.x + (int)tab2_px;

			int new_param[2] = { 0 };
			new_param[0] = accessor_.get().x;
			new_param[1] = accessor_.get().y;

			int min[2] = { min_.x, min_.y } ;

			for (int i = 0; i < 2; ++i) {

				int tx = tab_pos_[i].x - TAB_SIZE_W2;
				int ty = tab_pos_[i].y - TAB_SIZE_H2;
				int bx = tab_pos_[i].x + TAB_SIZE_W2;
				int by = tab_pos_[i].y + TAB_SIZE_H2;

				is_forcus_[i] = false;
				if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
					is_forcus_[i] = true;
					if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
						is_select_[i] = true;
					}
				}
				if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
					is_select_[i] = false;
				}
				if (!is_select_[i]) continue ;

				tab_pos_[i].x = (int)ms.x;
				tab_pos_[i].x = std::clamp<int>(tab_pos_[i].x, pos_.x, pos_.x + slider_lenght_);

				float new_rate = ((float)tab_pos_[i].x - (float)pos_.x) / (float)(slider_lenght_);
				new_param[i] = min[i] + (int)((float)min_to_max[i] * new_rate);
			}

			accessor_.set({ new_param[0], new_param[1] });
		}

		void draw() {
			int num_space = 80;
			int text_space = 150;
			int param[2] = { accessor_.get().x, accessor_.get().y };
			std::string spram[2] = { ".x", ".y" };
			for (int i = 0; i < 2; ++i) {
				int color = (is_forcus_[i] || is_select_[i]) ? -1 : 0x88888888;
				tnl::Vector2i pos = pos_;
				pos.y += i * TAB_SIZE_H2 * 2 ;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
				DrawBox(pos_.x, pos.y - TAB_SIZE_H2, pos_.x + slider_lenght_ + num_space + text_space, pos.y + TAB_SIZE_H2, 0, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				DrawLine(pos_.x, pos.y, pos_.x + slider_lenght_, pos.y, color);
				DrawLine(pos_.x, pos.y - TAB_SIZE_H2, pos_.x, pos.y + TAB_SIZE_H2, color);
				DrawLine(pos_.x + slider_lenght_, pos.y - TAB_SIZE_H2, pos_.x + slider_lenght_, pos.y + TAB_SIZE_H2, color);
				DrawBox(tab_pos_[i].x - TAB_SIZE_W2, tab_pos_[i].y - TAB_SIZE_H2, tab_pos_[i].x + TAB_SIZE_W2, pos.y + TAB_SIZE_H2, color, false);
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + TAB_SIZE_W2 + 5, (float)pos.y - TAB_SIZE_H2 + 3, color, "%d", param[i] );
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + num_space, (float)pos.y - TAB_SIZE_H2 + 3, color, "%s", ( label_ + spram[i] ).c_str() );
			}
		}

		tnl::Vector2i getPosition() { return pos_; }

	private:
		static constexpr int TAB_SIZE_W2 = 5;
		static constexpr int TAB_SIZE_H2 = 10;
		bool is_forcus_[2] = { false };
		bool is_select_[2] = { false };
		tnl::Vector2i min_;
		tnl::Vector2i max_;
		uint32_t slider_lenght_;
		tnl::Vector2i pos_;
		tnl::Vector2i tab_pos_[2];
		tnl::FieldAccessor<T, tnl::Vector2i> accessor_;
		std::string label_;
	};


	//-----------------------------------------------------------------------------------------------------------
	// vec3
	//-----------------------------------------------------------------------------------------------------------
	template<class T>
	class GuiValueSlider<T, tnl::Vector3> {
	public:
		GuiValueSlider() {}
		GuiValueSlider(
			T* obj
			, const std::function<tnl::Vector3(T*)>& getter
			, const std::function<void(T*, const tnl::Vector3&)>& setter
			, const tnl::Vector2i& pos
			, const tnl::Vector3& min
			, const tnl::Vector3& max
			, const std::string& label = "unknown"
			, uint32_t slider_lenght = 100)
			: accessor_(obj, getter, setter)
			, pos_(pos)
			, min_(min)
			, max_(max)
			, label_(label)
			, slider_lenght_(slider_lenght)
		{}

		void update() {
			tnl::Vector3 ms = tnl::Input::GetMousePosition();

			float param_x = accessor_.get().x - min_.x;
			float param_y = accessor_.get().y - min_.y;
			float param_z = accessor_.get().z - min_.z;

			float min_to_max[3];
			min_to_max[0] = max_.x - min_.x;
			min_to_max[1] = max_.y - min_.y;
			min_to_max[2] = max_.z - min_.z;

			float rate_x = (float)param_x / (float)min_to_max[0];
			float rate_y = (float)param_y / (float)min_to_max[1];
			float rate_z = (float)param_z / (float)min_to_max[2];

			float tab1_px = slider_lenght_ * rate_x;
			float tab2_px = slider_lenght_ * rate_y;
			float tab3_px = slider_lenght_ * rate_z;

			tab_pos_[0].y = pos_.y;
			tab_pos_[0].x = pos_.x + (int)tab1_px;
			tab_pos_[1].y = pos_.y + TAB_SIZE_H2 * 2;
			tab_pos_[1].x = pos_.x + (int)tab2_px;
			tab_pos_[2].y = pos_.y + TAB_SIZE_H2 * 4;
			tab_pos_[2].x = pos_.x + (int)tab3_px;

			float new_param[3] = { 0 };
			new_param[0] = accessor_.get().x;
			new_param[1] = accessor_.get().y;
			new_param[2] = accessor_.get().z;

			float min[3] = { min_.x, min_.y, min_.z };
			is_description_ = false;

			for (int i = 0; i < 3; ++i) {

				int tx = tab_pos_[i].x - TAB_SIZE_W2;
				int ty = tab_pos_[i].y - TAB_SIZE_H2;
				int bx = tab_pos_[i].x + TAB_SIZE_W2;
				int by = tab_pos_[i].y + TAB_SIZE_H2;

				is_forcus_[i] = false;
				if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
					is_forcus_[i] = true;
					if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
						is_select_[i] = true;
					}
				}
				if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
					is_select_[i] = false;
				}

				int num_space = 80;
				int text_space = 150;
				tx = pos_.x + slider_lenght_;
				ty = tab_pos_[i].y - TAB_SIZE_H2 ;
				bx = pos_.x + slider_lenght_ + num_space + text_space;
				by = tab_pos_[i].y + TAB_SIZE_H2 ;
				if (ms.x >= tx && ms.x <= bx && ms.y >= ty && ms.y <= by) {
					is_description_ = true;
					if (description_function_) {
						description_function_();
					}
				}

				if (!is_select_[i]) continue;

				tab_pos_[i].x = (int)ms.x;
				tab_pos_[i].x = std::clamp<int>(tab_pos_[i].x, pos_.x, pos_.x + slider_lenght_);

				float new_rate = ((float)tab_pos_[i].x - (float)pos_.x) / (float)(slider_lenght_);
				new_param[i] = min[i] + ((float)min_to_max[i] * new_rate);
			}

			accessor_.set({ new_param[0], new_param[1], new_param[2] });
		}

		void draw() {
			int num_space = 80;
			int text_space = 150;
			float param[3] = { accessor_.get().x, accessor_.get().y, accessor_.get().z };
			std::string spram[3] = { ".x", ".y", ".z"};
			for (int i = 0; i < 3; ++i) {
				int color = (is_forcus_[i] || is_select_[i]) ? -1 : 0x88888888;
				tnl::Vector2i pos = pos_;
				pos.y += i * TAB_SIZE_H2 * 2;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
				DrawBox(pos_.x, pos.y - TAB_SIZE_H2, pos_.x + slider_lenght_ + num_space + text_space, pos.y + TAB_SIZE_H2, 0, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				DrawLine(pos_.x, pos.y, pos_.x + slider_lenght_, pos.y, color);
				DrawLine(pos_.x, pos.y - TAB_SIZE_H2, pos_.x, pos.y + TAB_SIZE_H2, color);
				DrawLine(pos_.x + slider_lenght_, pos.y - TAB_SIZE_H2, pos_.x + slider_lenght_, pos.y + TAB_SIZE_H2, color);
				DrawBox(tab_pos_[i].x - TAB_SIZE_W2, tab_pos_[i].y - TAB_SIZE_H2, tab_pos_[i].x + TAB_SIZE_W2, pos.y + TAB_SIZE_H2, color, false);
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + TAB_SIZE_W2 + 5, (float)pos.y - TAB_SIZE_H2 + 3, color, "%.3f", param[i]);
				if (is_description_ && description_function_) color = -1;
				DrawStringEx((float)pos_.x + (float)slider_lenght_ + num_space, (float)pos.y - TAB_SIZE_H2 + 3, color, "%s", (label_ + spram[i]).c_str());
			}
		}

		TNL_PROPERTY(std::function<void()>, Description, description_function_);

		tnl::Vector2i getPosition() { return pos_; }

	private:
		static constexpr int TAB_SIZE_W2 = 5;
		static constexpr int TAB_SIZE_H2 = 10;
		bool is_forcus_[3] = { false };
		bool is_select_[3] = { false };
		bool is_description_ = false;
		tnl::Vector3 min_;
		tnl::Vector3 max_;
		uint32_t slider_lenght_;
		tnl::Vector2i pos_;
		tnl::Vector2i tab_pos_[3];
		tnl::FieldAccessor<T, tnl::Vector3> accessor_;
		std::string label_;
		std::function<void()> description_function_ = nullptr;
	};


}

