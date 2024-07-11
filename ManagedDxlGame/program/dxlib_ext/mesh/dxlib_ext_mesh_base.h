#pragma once
#include "../dxlib_ext_render_param.h"
#include "dxlib_ext_mesh_mtrl_gui_controller.h"

namespace dxe {

	class MeshBase {
	public :
		MeshBase() = default;
		virtual ~MeshBase(){}

		// 透明度の設定
		inline void setAlpha(const float alpha) { alpha_ = alpha; }
		inline float getAlpha() const { return alpha_; }


		// アンビエントカラー設定
		virtual void setMtrlAmibent(const tnl::Vector3& col) noexcept { render_param_.setMtrlAmbient(col); }
		inline tnl::Vector3 getMtrlAmbient() const noexcept { return render_param_.getMtrlAmbient(); }

		// ディフューズカラー設定
		virtual void setMtrlDiffuse(const tnl::Vector3& col) noexcept { render_param_.setMtrlDiffuse(col); }
		inline tnl::Vector3 getMtrlDiffuse() const noexcept { return render_param_.getMtrlDiffuse(); }

		// スペキュラカラー設定
		virtual void setMtrlSpecular(const tnl::Vector3& col) noexcept { render_param_.setMtrlSpecular(col); }
		inline tnl::Vector3 getMtrlSpecular() const noexcept { return render_param_.getMtrlSpecular(); }

		// エミッシヴカラー設定
		virtual void setMtrlEmissive(const tnl::Vector3& col) noexcept { render_param_.setMtrlEmissive(col); }
		inline tnl::Vector3 getMtrlEmissive() const noexcept { return render_param_.getMtrlEmissive(); }

		// スペキュラパワー設定
		virtual void setMtrlSpecPower(const float pow) noexcept { render_param_.setMtrlSpecPower(pow); }
		inline float getMtrlSpecPower() const noexcept { return render_param_.getMtrlSpecPower(); }

		// 一括設定
		virtual void setRenderParam(const RenderParam& param) { render_param_ = param; }

		//-----------------------------------------------------------------------------------------------------
		// GUI マテリアルコントローラ描画
		void drawGuiMaterialControlloer() {
			if (!mtrl_gui_ctrler_) {
				mtrl_gui_ctrler_ = std::make_shared<MeshMtrlGuiController>(this);
			}
			mtrl_gui_ctrler_->update();
			mtrl_gui_ctrler_->draw();
		}

		//------------------------------------------------------------------------------------------------------------------------
		// マテリアルの保存と読み込み
		void loadMaterial(const std::string& file_path);
		void saveMaterial(const std::string& file_path);

	protected :
		float						alpha_ = 1.0f;
		RenderParam					render_param_;
		std::shared_ptr<MeshMtrlGuiController> mtrl_gui_ctrler_ = nullptr;
	};

}
