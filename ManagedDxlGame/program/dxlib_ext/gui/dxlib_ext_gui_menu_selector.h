#pragma once
#include <functional>
#include <algorithm>
#include "../../library/tnl_vector2i.h"
#include "../../library/tnl_vector3.h"
#include "../../library/tnl_util.h"
#include "../../library/tnl_input.h"
#include "DxLib.h"

void DrawStringEx(int x, int y, int color, const char* _str, ...);
namespace dxe {

	//--------------------------------------------------------------------------------------------------------------------------
	// GuiMenuSelector
	// tips... �}�E�X����Ń��j���[���ڈꗗ����I���ł��� GUI ��񋟂��܂�
	// 
	// 
	// 
	// ==================================================================================================================
	// �� �g�p�@�T���v��
	// ==================================================================================================================
	//
	//  --------------- GameMain.cpp ----------------
	// 
	// GuiMenuSelector* gui_menu_selector = nullptr;
	//
	// void gameStart() {
	//		srand(time(0));
	//		SetBackgroundColor(32, 32, 32);
	//
	//		gui_menu_selector = new GuiMenuSelector(
	//			[&](int32_t select_index) {
	//
	//			}
	//		, "menu_A", "menu_B", "menu_C", "menu_D", "menu_E", "menu_F");
	// 
	//		gui_menu_selector->setLocation(GuiMenuSelector::eLocation::RIGHT_DOWN);
	// }
	//
	//
	// void gameMain(float delta_time) {
	//
	//		gui_menu_selector->update();
	//		gui_menu_selector->draw();
	//
	//		DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);
	//
	// }
	//

	class GuiMenuSelector {
	public:

		template<typename... tMenu>
		GuiMenuSelector(const std::function<void(int32_t)>& sel_func, tMenu... tMenus) {
			const std::string array[] = { static_cast<std::string>(tMenus)... };
			for (auto elem : array) {
				menus_.emplace_back(elem);
			}
			select_function_ = sel_func;
		}

		// ���j���[�o���ʒu
		enum class eLocation {
			RIGHT_DOWN,
			RIGHT_UP,
			LEFT_DOWN,
			LEFT_UP,
			UP,
			DOWN
		};

		void update();
		void draw();

		// ���ݒ�
		void setWidth(int32_t width) { width_ = width; }
		// ���j���[�o���ʒu�ݒ�
		void setLocation(eLocation location) { location_ = location; }
		// ���W�ݒ�
		void setPosition(const tnl::Vector2i& pos) { pos_ = pos; }
		// ���ݑI�𒆂̃��j���[�ԍ��ݒ�
		void setCurrentIndex(int32_t current_index) { 
			if (0 > current_index || menus_.size() <= current_index) return ;
			current_index_ = current_index; 
		}
		// ���j���[�̒ǉ�
		void addMenu(const std::string& menu_str) { menus_.emplace_back(menu_str); }

		TNL_PROPERTY(std::function<void()>, Description, description_function_);

	private:
		enum class eForcusStage {
			NONE
			, TAB
			, MENU
		};

		int32_t width_ = 100;
		int32_t height_ = 25;
		int32_t current_index_ = 0;
		int32_t forcus_index_ = -1;
		eLocation location_ = eLocation::RIGHT_DOWN;

		eForcusStage forcus_stage_ = eForcusStage::NONE;
		tnl::Vector2i pos_ = { 0, 0 };
		std::vector<std::string> menus_;
		std::function<void(int32_t)> select_function_;
		std::function<void()> description_function_ = nullptr;
	};

}
