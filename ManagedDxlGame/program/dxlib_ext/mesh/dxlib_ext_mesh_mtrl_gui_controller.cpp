#include "dxlib_ext_mesh_mtrl_gui_controller.h"
#include "dxlib_ext_mesh_base.h"

namespace dxe {

	MeshMtrlGuiController::MeshMtrlGuiController(MeshBase* ref_mesh) {
		ref_mesh_ = ref_mesh;

		diff_ = Shared<GuiValueSlider< MeshBase, tnl::Vector3 >>(
				new GuiValueSlider< MeshBase, tnl::Vector3 >
				( ref_mesh
				, &MeshBase::getMtrlDiffuse
				, &MeshBase::setMtrlDiffuse
				, { 0, 30 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "diffuse" ) ) ;

		ambi_ = Shared<GuiValueSlider< MeshBase, tnl::Vector3 >>(
				new GuiValueSlider< MeshBase, tnl::Vector3 >
				(ref_mesh
				, &MeshBase::getMtrlAmbient
				, &MeshBase::setMtrlAmibent
				, { 0, 90 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "ambient"));

		emis_ = Shared<GuiValueSlider< MeshBase, tnl::Vector3 >>(
				new GuiValueSlider< MeshBase, tnl::Vector3 >
				(ref_mesh
				, &MeshBase::getMtrlEmissive
				, &MeshBase::setMtrlEmissive
				, { 0, 150 }
				, { 0, 0, 0 }
				, { 3, 3, 3 }
				, "emissive"));

		spec_ = Shared<GuiValueSlider< MeshBase, tnl::Vector3 >>(
				new GuiValueSlider< MeshBase, tnl::Vector3 >
				(ref_mesh
				, &MeshBase::getMtrlSpecular
				, &MeshBase::setMtrlSpecular
				, { 0, 210 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "specular"));

		spec_pow_ = Shared<GuiValueSlider< MeshBase, float >>(
				new GuiValueSlider< MeshBase, float >
				(ref_mesh
				, &MeshBase::getMtrlSpecPower
				, &MeshBase::setMtrlSpecPower
				, { 0, 270 }
				, 0
				, 100
				, "spec pow"));
	}

	void MeshMtrlGuiController::update() {
		diff_->update();
		ambi_->update();
		emis_->update();
		spec_->update();
		spec_pow_->update();

		tnl::Vector3 ms = tnl::Input::GetMousePosition();
		int tx = 180;
		int ty = 0;
		int bx = 330;
		int by = 20;

		is_btn_save_forcus_ = false ;
		if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
			is_btn_save_forcus_ = true;
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
				is_btn_save_select_ = true;
			}
		}
		if (is_btn_save_select_) {
			if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
				is_btn_save_select_ = false;
				ref_mesh_->saveMaterial( "material.bin" );
			}
		}

	}
	void MeshMtrlGuiController::draw() {
		diff_->draw();
		ambi_->draw();
		emis_->draw();
		spec_->draw();
		spec_pow_->draw();

		int tx = 180 ;
		int ty = 0 ;
		int bx = 330 ;
		int by = 20 ;
		int color = (is_btn_save_forcus_) ? -1 : 0x88888888;
		color = (is_btn_save_select_) ? 0xff00ff00 : color ;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, ty, bx, by, 0, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawStringEx(10, (float)ty, -1, "mesh material");
		DrawBox(tx - 5, ty, bx, by, color, false);
		DrawStringEx((float)tx, (float)ty, color, "save to file" );
	}

}
