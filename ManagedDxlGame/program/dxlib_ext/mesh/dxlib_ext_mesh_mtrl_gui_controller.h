#pragma once

#include "../../library/tnl_util.h"
#include "../../library/tnl_using.h"
#include "../gui/dxlib_ext_gui_value_slider.h"

namespace dxe {

	class MeshBase;
	class MeshMtrlGuiController {
	public :
		MeshMtrlGuiController(MeshBase* ref_mesh );
		void update();
		void draw();
	private :
		MeshBase* ref_mesh_ = nullptr;
		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<GuiValueSlider< MeshBase, tnl::Vector3 >> diff_;
		Shared<GuiValueSlider< MeshBase, tnl::Vector3 >> ambi_;
		Shared<GuiValueSlider< MeshBase, tnl::Vector3 >> emis_;
		Shared<GuiValueSlider< MeshBase, tnl::Vector3 >> spec_;
		Shared<GuiValueSlider< MeshBase, float >> spec_pow_;
	};

}

